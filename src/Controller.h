/*
 * Controller.h
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

#ifndef CONTORLLER_H
#define CONTORLLER_H

using namespace std;

#define CC_LEFT                     (1 << 0)
#define CC_RIGHT                    (1 << 1)
#define CC_UP                       (1 << 2)
#define CC_DOWN                     (1 << 3)
#define CC_SWAP                     (1 << 4)
#define CC_ADVANCE                  (1 << 5)
#define CC_PAUSE                    (1 << 6)

/* static */ class Controller {
public:
  static void gameStart (   );
  static void keyboardPlay ( unsigned char key, int x, int y );
  static void keyboardUpPlay ( unsigned char key, int x, int y );
  static void specialPlay ( int key, int x, int y );
  static void specialUpPlay ( int key, int x, int y );
  static void keyboardMeta ( unsigned char key, int x, int y );
  static void keyboardUpMeta ( unsigned char key, int x, int y );
  static void specialMeta ( int key, int x, int y );
  static void specialUpMeta ( int key, int x, int y );
  static void entry ( int mouse_state );

  static inline int moveCommand (   )
    { return state & (CC_LEFT | CC_RIGHT | CC_UP | CC_DOWN); }

  static inline bool swapCommand (   )
    { return state & CC_SWAP; }

  static inline bool advanceCommand (   )
    { return state & CC_ADVANCE; }

  static inline bool pauseCommand (   )
    { return state & CC_PAUSE; }

private:
  static int state;
};

#endif
