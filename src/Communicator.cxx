/*
 * Communicator.cxx
 * Daniel Nelson - 8/30/0
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
 *
 * Handles all that socket stuff.  We should be more worried about the size of
 * integers on various systems.
 */

#include <cassert>
#include <iostream>
#include <sys/types.h>
#include <cstring>

#ifndef _WIN32
#  include <unistd.h>
#  include <sys/socket.h>
#  include <sys/poll.h>
#  include <netinet/in.h>
#  include <netdb.h>
#  include <arpa/inet.h>
#else
#  include <winsock2.h>
#endif

using namespace std;

#include "Game.h"
#include "Communicator.h"
#include "Displayer.h"
#include "GarbageGenerator.h"
#include "GarbageFlavorImage.h"
#include "LevelLights.h"
#include "MetaState.h"
#include "Random.h"

int Communicator::comm_link;
int Communicator::time_step;
bool Communicator::comm_link_active;
bool Communicator::no_communication;
bool Communicator::have_communicated;
int Communicator::last_recv_sync;
int Communicator::last_own_sync;
CommunicationBuffer Communicator::send_buffer;
CommunicationBuffer Communicator::recv_buffer;
CommunicationBuffer Communicator::work_buffer;
bool Communicator::win_ties;
char Communicator::opponent_name[GC_PLAYER_NAME_LENGTH];

void Communicator::startupExchange ( char player_name[GC_PLAYER_NAME_LENGTH] )
{
  // exchange names
  commSend(player_name, GC_PLAYER_NAME_LENGTH);
  commRecv(opponent_name, GC_PLAYER_NAME_LENGTH);

  // notify if we have a personal garbage flavor image
  uint32 flag = GarbageFlavorImage::personalGarbageFlavorImageExists();
  commSend(flag);

  // and send it out
  if (flag) {
    GLubyte *texture = GarbageFlavorImage::loadPersonalGarbageFlavorImage();
    commSend(texture, 4 * DC_GARBAGE_TEX_LENGTH * DC_GARBAGE_TEX_LENGTH
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
  commRecv(flag);

  // and recv it
  if (flag) {
    GLubyte texture[4 * DC_GARBAGE_TEX_LENGTH * DC_GARBAGE_TEX_LENGTH];
    commRecv(texture, 4 * DC_GARBAGE_TEX_LENGTH * DC_GARBAGE_TEX_LENGTH
     * sizeof(GLubyte));
    GarbageFlavorImage::handleNetworkGarbageFlavorImage(texture);
  }
}

void Communicator::initialize ( int mode, int port, char host_name[256],
 char player_name[GC_PLAYER_NAME_LENGTH] )
{
  comm_link_active = false;

#ifndef _WIN32
#else
  WSAData wsa_data;
  if (WSAStartup(MAKEWORD(1, 1), &wsa_data) != 0) {
    cerr << "Winsock startup failed." << endl;
    exit(1);
  }
#endif

  if (port == 0)
    port = CO_DEFAULT_PORT;

  switch (mode & (CM_SERVER | CM_CLIENT)) {
  case CM_SERVER: {
    int connection_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(connection_socket, (sockaddr *) &address, sizeof(address)) < 0) {
      cerr << "Port " << port << " is busy." << endl;
      exit(1);
    }

    cout << "Waiting for connection at port " << port << "..." << endl;
    listen(connection_socket, 1);

#ifndef _WIN32
    if (!(mode & CM_NO_TIME_OUT)) {
      pollfd ufds;
      ufds.fd = connection_socket;
      ufds.events = POLLIN;

      if (!poll(&ufds, 1, CO_SERVER_TIME_OUT * 1000)) {
        cerr << "Timed out." << endl;
        exit(1);
      }
    }
#endif

#ifndef _WIN32
    unsigned int length = sizeof(address);
#else
    int length = sizeof(address);
#endif
    comm_link = accept(connection_socket, (sockaddr *) &address, &length);
    comm_link_active = true;

    // check version id
    uint32 version_id = CO_TEST_INT;
    for (char *c = GC_VERSION; *c; c++)
      version_id += *c;
#ifdef DEVELOPMENT
    cout << "Version id:  " << version_id << endl;
#endif
    version_id = htonl(version_id);
    if (send(comm_link, (char *) &version_id, sizeof(version_id), 0) < 1) {
      cerr << "Connection failed." << endl;
      exit(1);
    }

    // server sends extremeness level
    uint32 X_level = ((mode & CM_X) ? 1 : 0);
    commSend(X_level);

    // for simplicity, server wins ties - but don't tell anyone; it's the only
    // available symmetry breaking term
    win_ties = true;

    cout << "Connection made by " << inet_ntoa(address.sin_addr) << '.' << endl;
    break;

  } case CM_CLIENT: {
    comm_link = socket(AF_INET, SOCK_STREAM, 0);
    comm_link_active = true;

    hostent *host = gethostbyname(host_name);
    if (!host) {
      cerr << "Host '" << host_name << "' not found." << endl;
      exit(1);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr = *(struct in_addr *) host->h_addr;
    address.sin_port = htons((short) port);
    if (connect(comm_link, (sockaddr *) &address, sizeof(address)) < 0) {
      cerr << "Connection failed." << endl;
      exit(1);
    }

    // check version id
    uint32 version_id = CO_TEST_INT;
    for (char *c = GC_VERSION; *c; c++)
      version_id += *c;
#ifdef DEVELOPMENT
    cout << "Version id:  " << version_id << endl;
#endif
    uint32 server_version_id;
    if (recv(comm_link, (char *) &server_version_id, sizeof(server_version_id),
     0) != sizeof(server_version_id)) {
      cerr << "Connection failed." << endl;
      exit(1);
    }
    if (ntohl(server_version_id) != version_id) {
      cerr << "Connected to incompatible version." << endl;
      exit(1);
    }

    // server sends extremeness level
    uint32 X_level;
    commRecv(X_level);
    if (X_level == 1) MetaState::mode |= CM_X;

    // for simplicity, client loses ties - but don't tell anyone
    win_ties = false;

    cout << "Connection made to " << inet_ntoa(address.sin_addr) << ':'
     << (short) port << '.' << endl;
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
    /* One solution to avoid "port is busy" after a game
       is to have the client initiate the closure, so let's
       wait a few seconds if we're the server.
       http://hea-www.harvard.edu/~fine/Tech/addrinuse.html
     */
    if (MetaState::mode & CM_SERVER)
      sleep(1);
#ifndef _WIN32
    close(comm_link);
#else
    closesocket(comm_link);
#endif
    comm_link_active = false;
  }
}

void Communicator::exchangeRandomSeed (   )
{
  uint32 seed;

  // server sends the the random seed
  if (MetaState::mode & CM_SERVER) {
    seed = Random::generateSeed();
    commSend(seed);
  } else
    commRecv(seed);

  Random::seed(seed);
}

void Communicator::gameStart (   )
{
  time_step = 1;
  no_communication = false;
  have_communicated = false;
  last_recv_sync = 0;
  last_own_sync = 0;
  send_buffer.count = 0;
  recv_buffer.count = 0;
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

void Communicator::timeStepPlay_inline_split_ (   )
{
  // recv

  if (have_communicated) {
    commRecv(recv_buffer);

    // add new garbage to the queue
    GarbageGenerator::addToQueue(recv_buffer);

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

      // if it's a concession
      if (recv_buffer.game_state & GS_CONCESSION)
        MetaState::state |= MS_CONCESSION;

      // if we also think we may have lost
      if (Game::state & GS_MAY_HAVE_LOST) {

        // pick a winner
        if (recv_buffer.loss_time_stamp < send_buffer.loss_time_stamp
         || (recv_buffer.loss_time_stamp == send_buffer.loss_time_stamp
         && win_ties))
          Game::won();

      // otherwise, we win
      } else
        Game::won();

    // if the opponent has confirmed our loss
    } else if (recv_buffer.game_state & GS_MUST_CONFIRM_LOSS) {
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
  } else
    have_communicated = true;

  // send

  // ready the level light data for sending
  LevelLights::readySendBuffer();

  // ready the game state information for sending - pause and unpause
  // information have already been set
  if (Game::state & GS_MAY_HAVE_LOST) {
    send_buffer.game_state |= GS_MAY_HAVE_LOST;
    if (MetaState::state & MS_CONCESSION)
      send_buffer.game_state |= GS_CONCESSION;
  } else if (Game::state & GS_MUST_CONFIRM_LOSS) {
    send_buffer.game_state |= GS_MUST_CONFIRM_LOSS;
    Game::state &= ~GS_MUST_CONFIRM_LOSS;
    Game::state |= GS_CONFIRMATION_HOLD;
  }

  // ready the sync for sending
  send_buffer.sync = (uint32) (time_step - Game::time_step);

  commSend(send_buffer);

  // reset send buffer
  send_buffer.count = 0;
  send_buffer.level_lights = 0;
  send_buffer.game_state = 0;
}

void Communicator::timeStepMeta_inline_split_ (   )
{
  uint32 state;

  if (have_communicated) {
    commRecv(state);

    // handle recved state data
    if (state & MS_REMOTE_KEY_WAIT)
      MetaState::remoteKeyPressed();
    else if (state & MS_READY_GAME_START)
      MetaState::remoteReady();

    if (MetaState::state & MS_GAME_PLAY) return;

  } else
    have_communicated = true;

  // ready state data for sending
  state = MetaState::state & (MS_REMOTE_KEY_WAIT | MS_READY_GAME_START);

  commSend(state);
}

void Communicator::barrier (   )
{
  uint32 c = CO_TEST_INT;

  commSend(c);
  commRecv(c);

  assert(c == CO_TEST_INT);
}
