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

typedef struct _options {
  bool no_sound;
  std::string player_name;
  std::string cube_tileset_dir;
  int port;
  int height;
  std::string host_name;
  int mode;

  _options() :
    no_sound(false),
    player_name(""),
    cube_tileset_dir(""),
    port(-1),
    height(-1),
    host_name(""),
    mode(0) { }
} ca_options;

void run_crack_attack (ca_options options);
void usage (   );
void options ( int argc, char **argv, ca_options &options);
void setupLocalDataDirectory (   );

#endif
