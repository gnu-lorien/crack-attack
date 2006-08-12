/*
 * Communicator.cxx
 * Daniel Nelson - 8/30/0
 *
 * Copyright (C) 2000  Daniel Nelson
 * Copyright (C) 2004  Andrew Sayman
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
 *
 * Handles all that socket stuff.  We should be more worried about the size of
 * integers on various systems.
 */

#include <cassert>
#include <iostream>
#include <sys/types.h>
#include <cstring>

#ifndef _WIN32
#  include <sys/socket.h>
#  include <sys/poll.h>
#  include <netinet/in.h>
#  include <netdb.h>
#  include <arpa/inet.h>
#else
#  include <stdlib.h>
#  include <winsock2.h>
#  define sleep(x) Sleep(x)
#endif

#include "Game.h"
#include "Communicator.h"
#include "Displayer.h"
#include "GarbageGenerator.h"
#include "GarbageFlavorImage.h"
#include "LevelLights.h"
#include "MetaState.h"
#include "Random.h"

using namespace std;

ENetHost *Communicator::host;
ENetPeer *Communicator::peer;
int Communicator::time_step;
bool Communicator::comm_link_active;
bool Communicator::no_communication;
bool Communicator::have_communicated;
int Communicator::last_recv_sync;
int Communicator::last_own_sync;
CommunicationBuffer Communicator::send_buffer;
CommunicationBuffer Communicator::recv_buffer;
GarbageBuffer       Communicator::send_garb_buffer;
GarbageBuffer       Communicator::recv_garb_buffer;
CommunicationBuffer Communicator::work_buffer;
bool Communicator::win_ties;
char Communicator::opponent_name[GC_PLAYER_NAME_LENGTH];

static void print_ip_address( int ip )
{
  ip = ntohl(ip);
  cout << ((ip & 0xFF000000) >> 24) << ".";
  cout << ((ip & 0x00FF0000) >> 16) << ".";
  cout << ((ip & 0x0000FF00) >> 8) << ".";
  cout << (ip & 0x000000FF);
}

void Communicator::startupExchange ( char player_name[GC_PLAYER_NAME_LENGTH] )
{
  // exchange names
  MESSAGE("Exchanging names");
  commSendRel(player_name, GC_PLAYER_NAME_LENGTH);
  commGetRel(opponent_name, GC_PLAYER_NAME_LENGTH);

  // notify if we have a personal garbage flavor image
  MESSAGE("Exchanging garbage");
  uint32 flag = GarbageFlavorImage::personalGarbageFlavorImageExists();
  commSendRel(flag);

  // and send it out
  if (flag) {
    GLubyte *texture = GarbageFlavorImage::loadPersonalGarbageFlavorImage();
    commSendRel(texture, 4 * DC_GARBAGE_TEX_LENGTH * DC_GARBAGE_TEX_LENGTH
     * sizeof(GLubyte));
#ifndef _WIN32
    delete [] texture;
#else
    if (texture != null) {
      delete [] texture;
      texture = null;
    }
#endif
  }

  // check to see if they have a personal garbage flavor image
  commGetRel(flag);

  // and recv it
  if (flag) {
    GLubyte texture[4 * DC_GARBAGE_TEX_LENGTH * DC_GARBAGE_TEX_LENGTH];
    commGetRel(texture, 4 * DC_GARBAGE_TEX_LENGTH * DC_GARBAGE_TEX_LENGTH
     * sizeof(GLubyte));
    GarbageFlavorImage::handleNetworkGarbageFlavorImage(texture);
  }
}

void Communicator::initialize ( int mode, int port, char host_name[256],
 char player_name[GC_PLAYER_NAME_LENGTH] )
{
  ENetAddress address;
  host = NULL;
  peer = NULL;
  comm_link_active = false;

  if (enet_initialize () != 0)
  {
    cerr << "An error occurred while initializing ENet." << endl;
    exit(1);
  }
  else
  {
    cout << "ENet properly initialized." << endl;
  }
  atexit (enet_deinitialize);

  if (port == 0)
    address.port = CO_DEFAULT_PORT;
  else
    address.port = port;

  switch (mode & (CM_SERVER | CM_CLIENT)) {
  case CM_SERVER: {
    address.host = ENET_HOST_ANY;

    host = enet_host_create(&address, 1, 0, 0);
    if (NULL == host) {
      cerr << "An error occured while trying to create the server" << endl;
      exit(1);
    }

    cout << "Waiting for connection at port " << address.port << "..." << endl;
    ENetEvent event;
    do {
      if (enet_host_service(host, &event, CO_SERVER_TIME_OUT * 1000) > 0)
      {
        switch (event.type) {
          case ENET_EVENT_TYPE_CONNECT: {
            peer = event.peer;
            cout << "Accepting connection from ";
            print_ip_address(peer->address.host);
            cout << endl;
            comm_link_active = true;
            break;
          }
          case ENET_EVENT_TYPE_RECEIVE: {
            enet_packet_destroy(event.packet);
            break;
          }
          case ENET_EVENT_TYPE_DISCONNECT: {
            break;
          }
          case ENET_EVENT_TYPE_NONE: {
            break;
          }
        }
      }
    } while((mode & CM_NO_TIME_OUT) && !comm_link_active);

    if (!comm_link_active)
    {
      cerr << "Time out." << endl;
      exit(1);
    }

    // check version id

    commSendRel(CO_VERSION, strlen(CO_VERSION) + 1);

    // server sends extremeness level
    enet_uint8 X_level = ((mode & CM_X) ? 1 : 0);
    commSendRel(&X_level, sizeof(X_level));

    // for simplicity, server wins ties - but don't tell anyone; it's the only
    // available symmetry breaking term
    win_ties = true;

    cout << "Connection made by ";
    print_ip_address(peer->address.host);
    cout << endl;
    break;

  }
  case CM_CLIENT: {
    host = enet_host_create(NULL, 1, 57600, 14400);
    if(NULL == host) {
      cerr << "Could not create ENet host." << endl;
      exit(1);
    }
    comm_link_active = true;

#ifdef DEVELOPMENT
    cout << "Hostname: " << host_name << endl;
#endif
    if (enet_address_set_host(&address, host_name)!=0) {
      cerr << "Host '" << host_name << "' not found." << endl;
      exit(1);
    }


    peer = enet_host_connect(host, &address, NUM_CHANNELS);
    if(NULL == peer) {
      cerr << "No available peers for initiating an ENet connection." << endl;
      exit(1);
    }
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    ENetEvent event;
    MESSAGE("Waiting five seconds for connection attempt to succeed");
    if (enet_host_service (host, & event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
      cout << "Connection to " << host_name << " succeeded." << endl;
    }
    else
    {
      enet_peer_reset (peer);
      cerr << "Connection to " << host_name << " failed." << endl;
      exit(1);
    }

    // check version id
    char sversion[CO_VERSION_MAX_LENGTH];
    commGetRel(sversion, CO_VERSION_MAX_LENGTH);
    if(strncmp(sversion, CO_VERSION, CO_VERSION_MAX_LENGTH)!=0) {
      cerr << "Connected to incompatible version: " << sversion << endl;
      exit(1);
    }

    // server sends extremeness level
    enet_uint8 X_level;
    commGetRel(&X_level, sizeof(X_level));
    if (X_level == 1) MetaState::mode |= CM_X;

    // for simplicity, client loses ties - but don't tell anyone
    win_ties = false;

    cout << "Connection made to " << peer->address.host << ':'
     << peer->address.port << '.' << endl;
    break;
  } }

  startupExchange(player_name);

  time_step = 1;
  no_communication = false;
  have_communicated = false;
}

void Communicator::cleanUp (   )
{
  if (comm_link_active) {
    enet_deinitialize();
    comm_link_active = false;
  }
}

void Communicator::exchangeRandomSeed (   )
{
  uint32 seed;

  // server sends the the random seed
  MESSAGE("Sending seed");
  if (MetaState::mode & CM_SERVER) {
    seed = Random::generateSeed();
    commSendRel(seed);
  } else
    commGetRel(seed);

  Random::seed(seed);
}

void Communicator::gameStart (   )
{
  time_step = 1;
  no_communication = false;
  have_communicated = false;
  last_recv_sync = 0;
  last_own_sync = 0;
  send_garb_buffer.count = 0;
  recv_garb_buffer.count = 0;
  send_buffer.level_lights = 0;
  recv_buffer.level_lights = 0;
  send_buffer.game_state = 0;

  exchangeRandomSeed();
}

void Communicator::gameFinish (   )
{
  time_step = 1;
  no_communication = false;
  have_communicated = false;
}

void Communicator::handlePlayRecv (  )
{
  ENetEvent event;
  int code;

  while((code = enet_host_service(host, &event, 0)) > 0) switch(event.type)
  {
    case ENET_EVENT_TYPE_RECEIVE:
      MESSAGE("Receiving message for channel " << (int)event.channelID << "|"
          << " len " << event.packet->dataLength << "|");
      switch(event.channelID) {
        case CO_CHANNEL_GARBAGE:
          handlePlayRecvGarbage(event);
          break;
        case CO_CHANNEL_STATUS:
          MESSAGE("Receiving status");
          handlePlayRecvStatus (event);
          break;
        case CO_CHANNEL_BOARD:
          MESSAGE("Receiving board data (unimplemented)");
          handlePlayRecvBoard  (event);
        case CO_CHANNEL_REL:
          MESSAGE("Channel reliable used! Should not be during play!");
          MESSAGE("Testing: Ignore leftovers?");
          break;
        default:
          cerr << "Networking message handled incorrectly:" << endl;
          cerr << "Invalid channel id " << (int)event.channelID << endl;
          exit(1);
      }
      enet_packet_destroy(event.packet);
      break;
    case ENET_EVENT_TYPE_DISCONNECT:
      cerr << "Disconnected from host." << endl;
      exit(1);
      break;
    case ENET_EVENT_TYPE_CONNECT:
      cerr << "Received an additional connection..." << endl;
      exit(1);
      break;
    case ENET_EVENT_TYPE_NONE:
      MESSAGE("Nothing to see here... move along.");
      break;
  }
  if (code < 0) {
    cerr << "Unknown networking failure." << endl;
    exit(1);
  }
}

void Communicator::handlePlayRecvBoard ( ENetEvent event ) 
{
  cerr << "Currently not handling board events." << endl;
  exit(1);
}

void Communicator::handlePlayRecvGarbage ( ENetEvent event )
{
  fillGarbageBuffer(event.packet);

  // add new garbage to the queue
  GarbageGenerator::addToQueue(recv_garb_buffer);
}

void Communicator::handlePlayRecvStatus ( ENetEvent event  )
{
  fillCommunicationBuffer(event.packet);

  MESSAGE("Receiving other player status");

  // handle the recved level light data
  LevelLights::handleRecvBuffer();

  // if we have been remotely paused
  if (recv_buffer.game_state & GS_PAUSED)
    Game::netSignalPause();

  // if we have been remotely unpaused
  else if (recv_buffer.game_state & GS_UNPAUSED)
    Game::netSignalUnpause();

  // store the current sync state
  last_recv_sync = recv_buffer.sync;
  last_own_sync = time_step - Game::time_step;

  // if we're out of sync with our opponent, enter a sync pause
  if (last_recv_sync > last_own_sync && (Game::state & GS_NORMAL))
    Game::syncPause(last_recv_sync - last_own_sync);

  // if our opponent thinks he may have lost
  if (recv_buffer.game_state & GS_MAY_HAVE_LOST) {
    MESSAGE("Opponent may have lost");
    // if it's a concession
    if (recv_buffer.game_state & GS_CONCESSION) {
      MetaState::state |= MS_CONCESSION;
      MESSAGE("by concession");
    }

    // if we also think we may have lost
    if (Game::state & GS_MAY_HAVE_LOST) {
      MESSAGE("We also think we may have lost");
      // pick a winner
      if (recv_buffer.loss_time_stamp < send_buffer.loss_time_stamp
       || (recv_buffer.loss_time_stamp == send_buffer.loss_time_stamp
       && win_ties)) {
        MESSAGE("Chose myself as the winner");
        Game::won();
      }

    // otherwise, we win
    } else {
      MESSAGE("I win");
      Game::won();
    }

  // if the opponent has confirmed our loss
  } else if (recv_buffer.game_state & GS_MUST_CONFIRM_LOSS) {
    MESSAGE("Opponent has confirmed my loss");
    Game::lossConfirmation();
    no_communication = true;
    return;
  }

  // if we were waiting a cycle for our opponent to recv his loss confirmation
  if (Game::state & GS_CONFIRMATION_HOLD) {
    Game::state &= ~GS_CONFIRMATION_HOLD;
    no_communication = true;
    Game::state |= GS_END_PLAY;
    return;
  }
}

void Communicator::timeStepPlay_inline_split_ (   )
{
  // recv
  if(have_communicated) {
    handlePlayRecv();
  } else
    have_communicated = true;
  if(no_communication) return;

  // send
  handlePlaySend();
}

void Communicator::handlePlaySend (   )
{
  // send
  // ready the level light data for sending
  LevelLights::readySendBuffer();

  // ready the game state information for sending - pause and unpause
  // information have already been set
  if (Game::state & GS_MAY_HAVE_LOST) {
    MESSAGE("May have lost");
    send_buffer.game_state |= GS_MAY_HAVE_LOST;
    if (MetaState::state & MS_CONCESSION) {
      MESSAGE("Conceeding");
      send_buffer.game_state |= GS_CONCESSION;
    }
  } else if (Game::state & GS_MUST_CONFIRM_LOSS) {
    MESSAGE("Must confirm loss");
    send_buffer.game_state |= GS_MUST_CONFIRM_LOSS;
    Game::state &= ~GS_MUST_CONFIRM_LOSS;
    Game::state |= GS_CONFIRMATION_HOLD;
  }

  // ready the sync for sending
  if (!(Game::state & GS_PAUSED))
    send_buffer.sync = (uint32) (time_step - Game::time_step);

#ifdef DEVELOPMENT
  send_buffer.print();
#endif

  if(send_garb_buffer.count > 0) 
    commSendGarbage();
  commSendStatus();

  // reset send buffer
  send_garb_buffer.count = 0;
  send_buffer.level_lights = 0;
  send_buffer.game_state = 0;
}

void Communicator::timeStepMeta_inline_split_ (   )
{
  uint32 state;
  bool state_available;

  if(have_communicated) {
    state_available = commGetRelNoBlock(state);
    if (state_available) {
      // handle recved state data
     if (state & MS_REMOTE_KEY_WAIT)
       MetaState::remoteKeyPressed();
     else if (state & MS_READY_GAME_START)
       MetaState::remoteReady();
    }
  } else 
    have_communicated = true;

  if (MetaState::state & MS_GAME_PLAY) return;

  // ready state data for sending
  state = MetaState::state & (MS_REMOTE_KEY_WAIT | MS_READY_GAME_START);

  commSendRel(state);
}

void Communicator::barrier (   )
{
  uint32 c = CO_TEST_INT;

  commSendRel(c);
  commGetRel(c);

  assert(c == CO_TEST_INT);
}
