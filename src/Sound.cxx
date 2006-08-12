/*
 * Sound.cxx
 * Miguel Ángel Vilela García - 8/29/03
 *
 * Copyright (C) 2003  Miguel Ángel Vilela García 
 * Copyright (C) 2005  See COPYRIGHT
 * Crack Attack! is the legal property of its developers, whose names 
 * are too numerous to list here.  Please refer to the COPYRIGHT file
 * distributed with this source distribution for a full listing.
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

#include "TextureLoader.h"
#include "Random.h"
#include "Sound.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>


typedef map <string,Mix_Chunk*> ChunkMap;
vector<string> sounds;
string sound_dirname;
ChunkMap chunks;

int audio_rate          = 22050;
int audio_channels      = 2;
int audio_buffers       = 1024; /* small enough buffer to get synchronized sound */
Uint16 audio_format     = AUDIO_S16; /* 16-bit stereo */
int has_audio_available = 0;

void Sound::initialize( void )
{
	SDL_Init( SDL_INIT_AUDIO );
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
		cout << "WARNING *** Unable to open audio device!" << endl;
		return; 
	}
	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	// Initialize some global variables
	has_audio_available = 1;
	// Load chunks filenames
	sounds.push_back (GC_SOUND_COUNTDOWN);
	sounds.push_back (GC_SOUND_BLOCK_FALLEN);
	sounds.push_back (GC_SOUND_BLOCK_AWAKING);
	sounds.push_back (GC_SOUND_BLOCK_DYING);
	sounds.push_back (GC_SOUND_GARBAGE_FALLEN);
	sounds.push_back (GC_SOUND_GARBAGE_SHATTERING);
	// Load chunk files to ChunkMap
	Mix_Chunk *chunk;
	string File;
	for (size_t i = 0; i < sounds.size(); i++) {
		#ifndef NDEBUG
		cout << "Loading " << sounds[i];
		#endif
		// Try to load chunk in $HOME/.crack-attack/sounds/
    char sound_dirname[256];
    TextureLoader::buildLocalDataFileName("sounds/", sound_dirname);
		File = sound_dirname + sounds[i];
		chunk = Mix_LoadWAV (File.c_str());
		if ( chunk == NULL ) { // Try to load chunk in DATA_DIRECTORY/sounds/
			char *another_dir = GC_DATA_DIRECTORY("sounds/");
			File = another_dir + sounds[i];
			chunk = Mix_LoadWAV (File.c_str());
		}
		// If chunk is NULL there is no WAV available for this sound
		if (!chunk) cout << "WARNING *** Unable to open " << sounds[i] << endl;
		chunks[sounds[i]] = chunk;
		#ifndef NDEBUG
		cout << endl;
		#endif
	}
}

void Sound::play( const char *file, int vol )
{
       if (!has_audio_available || !chunks[file])
              return;

	int channel;
	
	if (vol > 10) vol = 10;
	// string File (file);
	Mix_VolumeChunk( chunks[file], vol * MIX_MAX_VOLUME / 10 );
	channel = Mix_PlayChannel( -1, chunks[file], 0 );
}

int Sound::audio_available(  )
{
	return has_audio_available;
}

void Sound::cleanup(  )
{
       if (!has_audio_available)
              return;

	Mix_CloseAudio();
	SDL_Quit();
}
