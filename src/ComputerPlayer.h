/*
 * ComputerPlayer.h
 * Andrew Sayman - 3/27/05
 *
 * Copyright (C) 2005  Andrew Sayman
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
 */
#ifndef COMP_PLAY_H
#define COMP_PLAY_H

#include "ComputerPlayerAI.h"

//class ComputerPlayerAI;


/* static */ class ComputerPlayer {
private:
  static bool lost;
  static bool _impact;
  static ComputerPlayerAI *ai;

  static int findTopRed (  );
  static double lightPartition ( int block );

public:
  static void gameStart (  );
  static void timeStep (   );
  static int gameFinish (    );

  static void addGarbage ( int height, int width, int flavor );

  // lights
  static bool checkLevelLightDying ( );
  static bool checkLevelLightBlue ( int block );
  static bool impact ( bool reset = false );
  static int levelLightImpact ( );
};

#endif
