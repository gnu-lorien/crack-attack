/* vim: set et ts=2 sw=2: 
 * Communicator.h
 * Daniel Nelson - 8/24/0
 *
 * Copyright (C) 2000  Daniel Nelson
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Daniel Nelson - aluminumangel.org
 * 174 W. 18th Ave.
 * Columbus, OH  43210
 */

#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

//#include <sys/types.h>

//#include "Game.h"
#include "enet/enet.h"

// default communication port
#define CO_DEFAULT_PORT                          (8080)

// seconds before server time out due to no connection
#define CO_SERVER_TIME_OUT                       (30)

// time steps between communication exchange; must be power of 2
#define CO_COMMUNICATION_PERIOD                  (32)

// test integer
#define CO_TEST_INT                              (47)

// protocol version number
#define CO_VERSION                               "1.1.15"
#define CO_VERSION_MAX_LENGTH                    (10)

// channel numbers
enum { CO_CHANNEL_REL = 0, CO_CHANNEL_GARBAGE, CO_CHANNEL_STATUS,
  CO_CHANNEL_BOARD,  NUM_CHANNELS };

class BufferElement {
public:
  uint32 time_stamp;
  uint32 height;
  uint32 width;
  uint32 flavor;

  BufferElement (): time_stamp(0), height(0), width(0), flavor(0) { };
};

class GarbageBuffer {
public:
  BufferElement garbage[GC_GARBAGE_QUEUE_SIZE];
  uint32 count;
};

class CommunicationBuffer {
public:
  uint32 level_lights;
  uint32 game_state;
  uint32 loss_time_stamp;
  uint32 sync;

  inline void print() const {
    /*
    cout << "lights: " << level_lights << " state: " 
      << game_state << " stamp: " << loss_time_stamp
      << " sync: " << sync << endl;
    */
  }
};

/* static */ class Communicator {
public:
  static void initialize ( int mode, int port, char host_name[256],
   char player_name[GC_PLAYER_NAME_LENGTH] );
  static void gameStart (   );
  static void gameFinish (   );
  static void cleanUp (   );
  static void barrier (   );

  static inline void unpauseSyncCheck (   )
  {
    // latency for opponent's unpause depends on whether our next communication
    // action is to send or recv; note that this method if far from perfect and
    // often causes sporatic waits after an unpause; everything is perfectly
    // resynced by the resync system, but it would be nice to eliminate those
    // waits
    int latency = 0;
    if (time_step & CO_COMMUNICATION_PERIOD)
      latency += CO_COMMUNICATION_PERIOD;
    latency += time_step & (CO_COMMUNICATION_PERIOD - 1);
      
    if (last_recv_sync > last_own_sync + latency)
      Game::syncPause(last_recv_sync - last_own_sync - latency);
  }

  static inline void signalPaused (   )
  {
    if (!(send_buffer.game_state & GS_UNPAUSED))
      send_buffer.game_state |= GS_PAUSED;
    else
      send_buffer.game_state &= ~GS_UNPAUSED;
  }

  static inline void signalUnpaused (   )
  {
    if (!(send_buffer.game_state & GS_PAUSED))
      send_buffer.game_state |= GS_UNPAUSED;
    else
      send_buffer.game_state &= ~GS_PAUSED;
  }

  static inline void timeStepPlay (   )
  {
    time_step++;
    //if (time_step & (CO_COMMUNICATION_PERIOD - 1)) return;
    if (!no_communication)
      timeStepPlay_inline_split_();
  }

  static inline void timeStepMeta (   )
  {
    time_step++;
    //if (time_step & (CO_COMMUNICATION_PERIOD - 1)) return;
    timeStepMeta_inline_split_();
  }

  static inline bool isSendStep (   )
  {
    return time_step & CO_COMMUNICATION_PERIOD;
  }

  static inline void sendGarbage ( int height, int width, int flavor )
  {
    if (send_garb_buffer.count == GC_GARBAGE_QUEUE_SIZE) return;

    send_garb_buffer.garbage[send_garb_buffer.count].time_stamp = Game::time_step;
    send_garb_buffer.garbage[send_garb_buffer.count].height = height;
    send_garb_buffer.garbage[send_garb_buffer.count].width = width;
    send_garb_buffer.garbage[send_garb_buffer.count].flavor = flavor;
    send_garb_buffer.count++;
  }

  static inline void setLevelLightSendBit ( int mask )
  {
    send_buffer.level_lights |= mask;
  }

  static inline bool checkLevelLightRecvBit ( int mask )
  {
    return recv_buffer.level_lights & mask;
  }

  static inline void setLossTimeStep (   )
  {
    send_buffer.loss_time_stamp = (uint32) Game::time_step;
  }

  static int time_step;
  static char opponent_name[GC_PLAYER_NAME_LENGTH];

private:
  static void timeStepPlay_inline_split_ (   );
  static void timeStepMeta_inline_split_ (   );

  static void handlePlayRecv (  );
  static void handlePlayRecvGarbage ( ENetEvent event );
  static void handlePlayRecvStatus  ( ENetEvent event );
  static void handlePlayRecvBoard   ( ENetEvent event );
    
  static void handlePlaySend (  );

  static void exchangeRandomSeed (   );

  static inline void commSend ( 
      const void *message, 
      size_t size, 
      enet_uint8 channelId,
      bool reliable)
  {
    ENetPacket *packet = enet_packet_create(
        message, size, (reliable ? ENET_PACKET_FLAG_RELIABLE : 0));
    DOT((int)channelId + 10);
    if(enet_peer_send(peer, channelId, packet)!=0) {
      cerr << "Connection lost on send" << endl;
      exit(1);
    }
  }

  static inline void commSendRel ( const void *buffer, size_t size )
  {
    commSend(buffer, size, CO_CHANNEL_REL, true);
  }

  static inline void commSendRel ( uint32 value )
  {
    commSendRel(&value, sizeof(value));
  }

  static inline void commGetRel ( void *buffer, size_t size )
  {
    ENetEvent event;
    MESSAGE("Potentially waiting forever");
    while (enet_host_service(host, &event, CO_SERVER_TIME_OUT * 1000) > 0) {
      DOT(4);
      if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
        cerr << "Disconnected from host." << endl;
        exit(1);
      }
      else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
        if(event.channelID!=CO_CHANNEL_REL) {
          MESSAGE("Received out-of-order non-reliable packet on channnel " <<
            event.channelID);
          continue;
        }
        read(event.packet, buffer, size);
        enet_packet_destroy(event.packet);
        break;
      }
    }
  }

  static inline void commGetRel ( uint32 &value )
  {
    commGetRel(&value, sizeof(value));
  }

  static inline bool commGetRelNoBlock ( void *buffer, size_t size )
  {
    ENetEvent event;
    if (enet_host_service(host, &event, 0) > 0) {
      if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
        cerr << "Disconnected from host." << endl;
        exit(1);
      }
      else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
        if (event.channelID != CO_CHANNEL_REL) {
          MESSAGE("Received out-of-order non-reliable packet on channnel " <<
            event.channelID);
          return false;
        } else {
          read(event.packet, buffer, size);
          enet_packet_destroy(event.packet);
          return true;
        }
      }
    }
    return false;
  }

  static inline bool commGetRelNoBlock( uint32 &value)
  {
    return commGetRelNoBlock(&value, sizeof(value));
  }

  static inline void commSendGarbage (  )
  {
    commSendGarbage(send_garb_buffer);
  }

  static inline void commSendGarbage ( const GarbageBuffer &buffer )
  {
    for (uint32 count = 0; count < buffer.count; ++count) {
      commSend(&(buffer.garbage[count]), sizeof(BufferElement), CO_CHANNEL_GARBAGE, true);
    }
    //commSend(&buffer, sizeof(buffer), CO_CHANNEL_GARBAGE, true);
  }

  static inline void commSendStatus (  ) 
  {
    commSendStatus(send_buffer);
  }
  
  static inline void commSendStatus ( const CommunicationBuffer &buffer )
  {
    MESSAGE("Sending status");
#ifdef DEVELOPMENT
    buffer.print();
#endif
    commSend(&buffer, sizeof(buffer), CO_CHANNEL_STATUS, false);
  }

  static inline void read ( ENetPacket *p, void *buffer, size_t size)
  {
    size_t plen = p->dataLength;
    size_t ilen = std::min(plen, size);
    enet_uint8 *buf = (enet_uint8 *)buffer;
    for (size_t i=0; i < ilen; ++i) {
      buf[i] = p->data[i];
    }
  }

  static inline void fillCommunicationBuffer ( ENetPacket *packet)
  {
    read(packet, &work_buffer, sizeof(work_buffer));

    recv_buffer.level_lights = (work_buffer.level_lights);
    recv_buffer.game_state = (work_buffer.game_state);
    recv_buffer.loss_time_stamp = (work_buffer.loss_time_stamp);
    recv_buffer.sync = (work_buffer.sync);
  }

  static inline void fillGarbageBuffer ( ENetPacket *packet )
  {
    size_t len = packet->dataLength;
    size_t num_garbage = len / sizeof(BufferElement);
    recv_garb_buffer.count = num_garbage;
    for (size_t i = 0; i < num_garbage; ++i) {
      read(packet, &(recv_garb_buffer.garbage[i]), sizeof(BufferElement));
    }
  }

  static void startupExchange ( char player_name[GC_PLAYER_NAME_LENGTH] );

  static ENetHost * host;
  static ENetPeer * peer;
  static bool comm_link_active;
  static bool no_communication;
  static bool have_communicated;
  static int last_recv_sync;
  static int last_own_sync;
  static CommunicationBuffer send_buffer;
  static CommunicationBuffer recv_buffer;
  static GarbageBuffer       send_garb_buffer;
  static GarbageBuffer       recv_garb_buffer;
  static CommunicationBuffer work_buffer;

  static bool win_ties;
};

#endif
