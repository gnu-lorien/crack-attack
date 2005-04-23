/*
 * Sound.h
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

#ifndef SOUND_H
#define SOUND_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <string>
#include <vector>
#include <map>

// const char GC_SOUND_FILENAME_LENGTH      = 128;
const char GC_SOUND_COUNTDOWN[]          = "countdown.wav";
const char GC_SOUND_BLOCK_FALLEN[]       = "block_fallen.wav";
const char GC_SOUND_BLOCK_AWAKING[]      = "block_awaking.wav";
const char GC_SOUND_BLOCK_DYING[]        = "block_dying.wav";
const char GC_SOUND_GARBAGE_FALLEN[]     = "garbage_fallen.wav";
const char GC_SOUND_GARBAGE_SHATTERING[] = "garbage_shattering.wav";

class Sound {
	public:
		static void Sound::initialize ( void );
		static void Sound::play( const char *file, int vol );
		static void Sound::cleanup ( void );
		static int  Sound::audio_available( void );
	private:
};

#endif /* SOUND_H */
