/*
 * Attack.h
 * Daniel Nelson - 8/29/0
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
 */

#ifndef ATTACK_H
#define ATTACK_H

#include "Game.h"

using namespace std;

void run_crack_attack (int mode, int port, char *host_name, char *player_name, int width, int height);
void usage (   );
void parseCommandLine ( int argc, char **argv, int &mode, int &port,
 char *host_name, char player_name[GC_PLAYER_NAME_LENGTH] );
void setupLocalDataDirectory (   );

#endif
