/*
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

using namespace std;

#include <sys/types.h>

#ifndef _WIN32
#  include <unistd.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#else
#  include <winsock2.h>
#endif

#include "Game.h"

// default communication port
#define CO_DEFAULT_PORT                          (8080)

// seconds before server time out due to no connection
#define CO_SERVER_TIME_OUT                       (30)

// time steps between communication exchange; must be power of 2
#define CO_COMMUNICATION_PERIOD                  (32)

// test integer
#define CO_TEST_INT                              (47)

// protocol version number
#define CO_VERSION                               "1.1.8"

class BufferElement {
public:
  uint32 time_stamp;
  uint32 height;
  uint32 width;
  uint32 flavor;
};

class CommunicationBuffer {
public:
  BufferElement garbage[GC_GARBAGE_QUEUE_SIZE];
  uint32 count;
  uint32 level_lights;
  uint32 game_state;
  uint32 loss_time_stamp;
  uint32 sync;
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
    if (time_step & (CO_COMMUNICATION_PERIOD - 1)) return;
    if (!no_communication)
      timeStepPlay_inline_split_();
  }

  static inline void timeStepMeta (   )
  {
    time_step++;
    if (time_step & (CO_COMMUNICATION_PERIOD - 1)) return;
    timeStepMeta_inline_split_();
  }

  static inline bool isSendStep (   )
  {
    return time_step & CO_COMMUNICATION_PERIOD;
  }

  static inline void sendGarbage ( int height, int width, int flavor )
  {
    if (send_buffer.count == GC_GARBAGE_QUEUE_SIZE) return;

    send_buffer.garbage[send_buffer.count].time_stamp = Game::time_step;
    send_buffer.garbage[send_buffer.count].height = height;
    send_buffer.garbage[send_buffer.count].width = width;
    send_buffer.garbage[send_buffer.count].flavor = flavor;
    send_buffer.count++;
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

  static void exchangeRandomSeed (   );

  static inline void commSend ( const void *message, int size )
  {
    int n;
    do
      if ((n = send(comm_link, (char *) message, size, 0)) != -1) {
        message = (char *) message + n;
        size -= n;
      } else {
        cerr << "Connection lost." << endl;
        exit(1);
      }
    while (size > 0);
  }

  static inline void commRecv ( void *buffer, int size )
  {
    int n;
    do
      if ((n = recv(comm_link, (char *) buffer, size, 0)) != -1) {
        buffer = (char *) buffer + n;
        size -= n;
      } else {
        cerr << "Connection lost." << endl;
        exit(1);
      }
    while (size > 0);
  }

  static inline void commSend ( uint32 value )
  {
    value = htonl(value);
    commSend(&value, sizeof(value));
  }

  static inline void commRecv ( uint32 &value )
  {
    commRecv(&value, sizeof(value));
    value = ntohl(value);
  }

  static inline void commSend ( const CommunicationBuffer &buffer )
  {
    work_buffer.count = htonl(buffer.count);
    for (int i = buffer.count; i--; ) {
      work_buffer.garbage[i].time_stamp = htonl(buffer.garbage[i].time_stamp);
      work_buffer.garbage[i].height = htonl(buffer.garbage[i].height);
      work_buffer.garbage[i].width = htonl(buffer.garbage[i].width);
      work_buffer.garbage[i].flavor = htonl(buffer.garbage[i].flavor);
    }
    work_buffer.level_lights = htonl(buffer.level_lights);
    work_buffer.game_state = htonl(buffer.game_state);
    work_buffer.loss_time_stamp = htonl(buffer.loss_time_stamp);
    work_buffer.sync = htonl(buffer.sync);

    commSend(&work_buffer, sizeof(work_buffer));
  }

  static inline void commRecv ( CommunicationBuffer &buffer )
  {
    commRecv(&work_buffer, sizeof(work_buffer));

    buffer.count = ntohl(work_buffer.count);
    for (int i = buffer.count; i--; ) {
      buffer.garbage[i].time_stamp = ntohl(work_buffer.garbage[i].time_stamp);
      buffer.garbage[i].height = ntohl(work_buffer.garbage[i].height);
      buffer.garbage[i].width = ntohl(work_buffer.garbage[i].width);
      buffer.garbage[i].flavor = ntohl(work_buffer.garbage[i].flavor);
    }
    buffer.level_lights = ntohl(work_buffer.level_lights);
    buffer.game_state = ntohl(work_buffer.game_state);
    buffer.loss_time_stamp = ntohl(work_buffer.loss_time_stamp);
    buffer.sync = ntohl(work_buffer.sync);
  }

  static void startupExchange ( char player_name[GC_PLAYER_NAME_LENGTH] );

  static int comm_link;
  static bool comm_link_active;
  static bool no_communication;
  static bool have_communicated;
  static int last_recv_sync;
  static int last_own_sync;
  static CommunicationBuffer send_buffer;
  static CommunicationBuffer recv_buffer;
  static CommunicationBuffer work_buffer;

  static bool win_ties;
};

#endif
