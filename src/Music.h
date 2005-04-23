/*
 * Music.h
 * Miguel Ángel Vilela García - 8/29/03
 *
 * Copyright (C) 2003  Miguel Ángel Vilela García 
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
 * Miguel Ángel Vilela García - www.miguev.net
 */

#ifndef MUSIC_H
#define MUSIC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <string>
#include <vector>

#define GC_MUSIC_FILENAME_LENGTH 128
#define GC_MUSIC_PRELUDE_TRACK   "prelude"
#define GC_MUSIC_GAME_TRACK      "game"
#define GC_MUSIC_GAMEOVER_TRACK  "gameover"
#define GC_MUSIC_YOUWIN_TRACK    "youwin"

class Music {
	public:
		static void initialize (   );
		static void play();
		static void play_prelude();
		static void play_game();
		static void play_gameover();
		static void play_youwin();
		static void pause();
		static void resume();
		static void stop();
		static void fadeout( int ms );
		static void cleanup (  );
		static void load(  );
	private:
		static void play_track( char *track );
		static void finished();
};

#endif /* MUSIC_H */
