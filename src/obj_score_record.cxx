/*
 * score_record.cxx
 * Daniel Nelson - 12/8/1
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

#include <GL/glut.h>
#include <cmath>

#ifndef _WIN32
#  include <sstream>
#else
#  include <glext.h>
#  include <strstrea.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "MetaState.h"
#include "String.h"
#include "Score.h"

GLuint Displayer::score_to_beat_texture;
GLuint Displayer::record_textures[DC_SCORE_REC_NUMBER_DRAW];
GLubyte **Displayer::record_texture_data;
GLubyte *Displayer::player_rank_texture_data;

void Displayer::copyRecordSubTexture ( GLubyte *texture, GLubyte *subtexture,
 int subtex_width, int s_location, int copy_width )
{
  for (int t = DC_LETTER_TEX_LENGTH; t--; )
    for (int s = copy_width; s--; ) {
      int ds = s_location + s;

      texture[(t * DC_SCORE_REC_TEX_LENGTH_S + ds) * 4 + 0]
       = subtexture[(t * subtex_width + s) * 4 + 0];
      texture[(t * DC_SCORE_REC_TEX_LENGTH_S + ds) * 4 + 1]
       = subtexture[(t * subtex_width + s) * 4 + 1];
      texture[(t * DC_SCORE_REC_TEX_LENGTH_S + ds) * 4 + 2]
       = subtexture[(t * subtex_width + s) * 4 + 2];
      texture[(t * DC_SCORE_REC_TEX_LENGTH_S + ds) * 4 + 3]
       = subtexture[(t * subtex_width + s) * 4 + 3];
    }
}

void Displayer::rerankScoreRecord (   )
{
  GLubyte *subtexture
   = new GLubyte[DC_LETTER_TEX_LENGTH * DC_SCORE_REC_RANK_STRING_TEX_WIDTH * 4];

  for (int n = Score::player_rank; n--; ) {
    ostringstream rank_string;
    rank_string << (GC_SCORE_REC_LENGTH - n) << ends;

    for (int i = DC_LETTER_TEX_LENGTH * DC_SCORE_REC_RANK_STRING_TEX_WIDTH * 4;
     i--; )
      subtexture[i] = 0;

    String::fillStringTexture(rank_string.str().data(), subtexture,
     DC_SCORE_REC_RANK_STRING_TEX_WIDTH, true);
    copyRecordSubTexture(record_texture_data[n], subtexture,
     DC_SCORE_REC_RANK_STRING_TEX_WIDTH, DC_SCORE_REC_RANK_TEX_MARGIN,
     DC_SCORE_REC_RANK_STRING_TEX_WIDTH);
  }

  if (subtexture != null) {
    delete [] subtexture;
    subtexture = null;
  }
}

void Displayer::generateScoreRankTexture ( int rank, int score,
 const char *name, GLubyte *texture )
{
  ostringstream score_string;
  score_string << score << ends;

  ostringstream rank_string;
  rank_string << (GC_SCORE_REC_LENGTH - rank) << ends;

  int score_width
   = String::stringWidth(score_string.str().data(), DC_SCORE_REC_TEX_LENGTH_S -
   2 * DC_SCORE_REC_RANK_TEX_MARGIN);

  int max_name_width = (DC_SCORE_REC_TEX_LENGTH_S
   - 2 * DC_SCORE_REC_RANK_TEX_MARGIN - DC_SCORE_REC_RANK_MIN_SPACE
   - DC_SCORE_REC_RANK_STRING_TEX_WIDTH) - score_width;
  int name_width = String::stringWidth(name, max_name_width);

  int subtex_width = name_width > score_width ? name_width : score_width;
  if (DC_SCORE_REC_RANK_STRING_TEX_WIDTH > subtex_width)
    subtex_width = DC_SCORE_REC_RANK_STRING_TEX_WIDTH;
  GLubyte *subtexture
   = new GLubyte[DC_LETTER_TEX_LENGTH * subtex_width * 4];

  for (int i = DC_LETTER_TEX_LENGTH * subtex_width * 4; i--; )
    subtexture[i] = 0;
  String::fillStringTexture(rank_string.str().data(), subtexture,
   DC_SCORE_REC_RANK_STRING_TEX_WIDTH, true, subtex_width);
  copyRecordSubTexture(texture, subtexture, subtex_width ,
   DC_SCORE_REC_RANK_TEX_MARGIN, DC_SCORE_REC_RANK_STRING_TEX_WIDTH);

  for (int i = DC_LETTER_TEX_LENGTH * subtex_width * 4; i--; )
    subtexture[i] = 0;
  String::fillStringTexture(name, subtexture, name_width, true, subtex_width);
  copyRecordSubTexture(texture, subtexture, subtex_width,
   DC_SCORE_REC_RANK_STRING_TEX_WIDTH + DC_SCORE_REC_RANK_TEX_MARGIN,
   name_width);

  for (int i = DC_LETTER_TEX_LENGTH * subtex_width * 4; i--; )
    subtexture[i] = 0;
  String::fillStringTexture(score_string.str().data(), subtexture, score_width, true,
   subtex_width);
  copyRecordSubTexture(texture, subtexture, subtex_width,
   (DC_SCORE_REC_TEX_LENGTH_S - DC_SCORE_REC_RANK_TEX_MARGIN)
   - score_width, score_width);

  if (subtexture != null) {
    delete [] subtexture;
    subtexture = null;
  }
}

void Displayer::generateScoreRecord (   )
{
  if (!(MetaState::mode & CM_SOLO)) return;

  // generate score to beat message

  glGenTextures(1, &score_to_beat_texture);

  glBindTexture(GL_TEXTURE_2D, score_to_beat_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  GLubyte texture[DC_SCORE_TO_BEAT_TEX_LENGTH_T]
   [DC_SCORE_TO_BEAT_TEX_LENGTH_S][3];
  for (int t = DC_SCORE_TO_BEAT_TEX_LENGTH_T; t--; )
    for (int s = DC_SCORE_TO_BEAT_TEX_LENGTH_S; s--; )
      texture[t][s][0] = texture[t][s][1] = texture[t][s][2] = 0;

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DC_SCORE_TO_BEAT_TEX_LENGTH_S,
   DC_SCORE_TO_BEAT_TEX_LENGTH_T, GL_FALSE, GL_RGB, GL_UNSIGNED_BYTE,
   texture);

  int width = String::stringWidth(DC_SCORE_TO_BEAT_MESSAGE,
   DC_SCORE_TO_BEAT_TEX_LENGTH_S);
  GLubyte *subtexture = new GLubyte[DC_LETTER_TEX_LENGTH * width * 4];
  String::fillStringTexture(DC_SCORE_TO_BEAT_MESSAGE, subtexture, width);
  glTexSubImage2D(GL_TEXTURE_2D, 0, (DC_SCORE_TO_BEAT_TEX_LENGTH_S - width)
   / 2, (30 * DC_SCORE_TO_BEAT_TEX_LENGTH_T / 100 - DC_LETTER_TEX_LENGTH) / 2,
   width, DC_LETTER_TEX_LENGTH, GL_RGBA, GL_UNSIGNED_BYTE, subtexture);
  if (subtexture != null) {
    delete [] subtexture;
    subtexture = null;
  }

  width = String::stringWidth(Score::record[GC_SCORE_REC_LENGTH - 1].name,
   DC_SCORE_TO_BEAT_TEX_LENGTH_S);
  subtexture = new GLubyte[DC_LETTER_TEX_LENGTH * width * 4];
  String::fillStringTexture(Score::record[GC_SCORE_REC_LENGTH - 1].name,
   subtexture, width);
  glTexSubImage2D(GL_TEXTURE_2D, 0, (DC_SCORE_TO_BEAT_TEX_LENGTH_S - width)
   / 2, (117 * DC_SCORE_TO_BEAT_TEX_LENGTH_T / 100 - DC_LETTER_TEX_LENGTH) / 2,
   width, DC_LETTER_TEX_LENGTH, GL_RGBA, GL_UNSIGNED_BYTE, subtexture);
  if (subtexture != null) {
    delete [] subtexture;
    subtexture = null;
  }

  ostringstream score;
  score << Score::record[GC_SCORE_REC_LENGTH - 1].score << ends;

  width = String::stringWidth(score.str().data(), DC_SCORE_TO_BEAT_TEX_LENGTH_S);
  subtexture = new GLubyte[DC_LETTER_TEX_LENGTH * width * 4];
  String::fillStringTexture(score.str().data(), subtexture, width);
  glTexSubImage2D(GL_TEXTURE_2D, 0, (DC_SCORE_TO_BEAT_TEX_LENGTH_S - width)
   / 2, (170 * DC_SCORE_TO_BEAT_TEX_LENGTH_T / 100 - DC_LETTER_TEX_LENGTH) / 2,
   width, DC_LETTER_TEX_LENGTH, GL_RGBA, GL_UNSIGNED_BYTE, subtexture);
  if (subtexture != null) {
    delete [] subtexture;
    subtexture = null;
  }

  // generate rank textures

  record_texture_data = new GLubyte *[GC_SCORE_REC_LENGTH - 1];

  for (int n = GC_SCORE_REC_LENGTH - 1; n--; ) {
    record_texture_data[n]
     = new GLubyte[DC_LETTER_TEX_LENGTH * DC_SCORE_REC_TEX_LENGTH_S * 4];
    for (int i = DC_LETTER_TEX_LENGTH * DC_SCORE_REC_TEX_LENGTH_S * 4;
     i--; )
      record_texture_data[n][i] = 0;

    generateScoreRankTexture(n + 1, Score::record[n + 1].score,
     Score::record[n + 1].name, record_texture_data[n]);
  }

  // ready space for the player's rank texture

  player_rank_texture_data
   = new GLubyte[DC_LETTER_TEX_LENGTH * DC_SCORE_REC_TEX_LENGTH_S * 4];
  for (int i = DC_LETTER_TEX_LENGTH * DC_SCORE_REC_TEX_LENGTH_S * 4; i--; )
    player_rank_texture_data[i] = 0;
}
