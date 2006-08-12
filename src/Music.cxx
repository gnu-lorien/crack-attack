/*
 * Music.cxx
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

#include "Music.h"

#include "TextureLoader.h"
#include "Random.h"
#include "Sound.h"

Mix_Music *music    = NULL;
int music_available = 0;
int keep_playing   = 0;
int current_track   = 0;

vector<string> music_filelist;
char *music_filename;
string music_dirname;
dirent *music_dirent;
DIR *music_dir;

void Music::initialize (   )
{
	if (!Sound::audio_available()) return;
  char tmp_music_dirname[256];
  TextureLoader::buildLocalDataFileName("music/", tmp_music_dirname);
  music_dirname = string( tmp_music_dirname );
	if ( (music_dir = opendir( music_dirname.c_str() ) ) == NULL ) {
    char *another_dir = GC_DATA_DIRECTORY("music/");
		music_dirname = string( another_dir );
	}
	if ( (music_dir = opendir( music_dirname.c_str() ) ) == NULL ) {
		cout << "WARNING *** Unable to open music directory!" << endl;
		return;
	}
	#ifndef NDEBUG
	cout << "Music dir: " << music_dirname.c_str() << endl;
	#endif
	while ( music_dirent = readdir ( music_dir ) ) {
		music_filename = music_dirent->d_name;
		if ( ( music_filename == string( GC_MUSIC_GAME_TRACK ) ) ) {
			music_filelist.clear();
			music_filelist.push_back( music_dirname + string( music_filename ) );
			#ifndef NDEBUG
			cout << "Added music file: " << music_filename << endl;
			#endif
			break;
		}
		if ( ( music_filename == string( "."  ) )
		  || ( music_filename == string( ".." ) )
		  || ( music_filename == string( GC_MUSIC_PRELUDE_TRACK  ) )
		  || ( music_filename == string( GC_MUSIC_GAMEOVER_TRACK ) )
		  || ( music_filename == string( GC_MUSIC_YOUWIN_TRACK   ) ) )
			continue;
		music_filelist.push_back( music_dirname + string( music_filename ) );
		#ifndef NDEBUG
		cout << "Added music file: " << music_filename << endl;
		#endif
	}
	closedir( music_dir );
	music_available = music_filelist.size();
}

void Music::play(  )
{
	if ( !music_available   ) return;
	current_track = Random::number( music_filelist.size() );
	#ifndef NDEBUG
	cout << "Playing " << music_filelist[current_track].c_str() << endl;
	#endif
	music = Mix_LoadMUS( music_filelist[current_track].c_str() );
	Mix_VolumeMusic( MIX_MAX_VOLUME / 4 );
	Mix_PlayMusic( music , 0 );
	Mix_HookMusicFinished(Music::finished);
	keep_playing = 1;
}

void Music::play_prelude(  )
{
	Music::play_track( GC_MUSIC_PRELUDE_TRACK );
}

void Music::play_game(  )
{
	Music::play_track( GC_MUSIC_GAME_TRACK );
}

void Music::play_gameover(  )
{
	Music::play_track( GC_MUSIC_GAMEOVER_TRACK );
}

void Music::play_youwin(  )
{
	Music::play_track( GC_MUSIC_YOUWIN_TRACK );
}

void Music::play_track( char *track )
{
	string Track = string( music_dirname.c_str() );
	Track.append( string( track ) );
	if ( !music_available   ) return;
	#ifndef NDEBUG
	cout << "Playing " << Track.c_str() << endl;
	#endif
	music = Mix_LoadMUS( Track.c_str() );
	Mix_VolumeMusic( MIX_MAX_VOLUME / 4 );
	Mix_PlayMusic( music , 0 );
	keep_playing = 0;
}


void Music::finished(  )
{
	Mix_HaltMusic();
	if ( keep_playing ) Music::play();
}

void Music::pause(  )
{
	if ( !Mix_PlayingMusic() ) return;
	Mix_PauseMusic();
}

void Music::resume(  )
{
	if ( !Mix_PlayingMusic() ) return;
	Mix_ResumeMusic();
}

void Music::stop(  )
{
	if ( !Mix_PlayingMusic() ) return;
	keep_playing = 0;
	Mix_HaltMusic();
}

void Music::fadeout( int ms )
{
	if ( !Mix_PlayingMusic() ) return;
	keep_playing = 0;
	Mix_FadeOutMusic(ms);
}

void Music::cleanup (  )
{
	Mix_FreeMusic( music );
	music = NULL;
}
