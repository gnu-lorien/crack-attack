/*
 * ScoreRecordManager.cxx
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
 *
 * Handles the complexities of displaying the score record.
 */

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "MetaState.h"
#include "WinRecord.h"
#include "Score.h"
#include "Controller.h"
#include "ScoreRecordManager.h"

int ScoreRecordManager::top_rank;
int ScoreRecordManager::top_texture;
GLfloat ScoreRecordManager::offset;
GLfloat ScoreRecordManager::velocity;

bool ScoreRecordManager::spring_active;
bool ScoreRecordManager::control_active;
bool ScoreRecordManager::ignore_up;
bool ScoreRecordManager::ignore_down;

void ScoreRecordManager::initialize (   )
{
  top_rank = 0;
  top_texture = 0;
  offset = -DC_SCORE_REC_RANK_HEIGHT / 2.0f;
  velocity = DC_SCORE_REC_START_VELOCITY;

  spring_active = false;
  control_active = false;
  ignore_up = ignore_down = false;
}

void ScoreRecordManager::gameFinish_inline_split_ (   )
{
  Displayer::generateScoreRankTexture(Score::player_rank, Score::score,
   MetaState::player_name, Displayer::player_rank_texture_data);

  Displayer::rerankScoreRecord();

  glGenTextures(DC_SCORE_REC_NUMBER_DRAW, Displayer::record_textures);

  for (int n = DC_SCORE_REC_NUMBER_DRAW; n--; ) {
    glBindTexture(GL_TEXTURE_2D, Displayer::record_textures[n]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (Score::player_rank == 0)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DC_SCORE_REC_TEX_LENGTH_S,
       DC_LETTER_TEX_LENGTH, GL_FALSE, GL_RGBA, GL_UNSIGNED_BYTE,
       Displayer::player_rank_texture_data);
    else
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DC_SCORE_REC_TEX_LENGTH_S,
       DC_LETTER_TEX_LENGTH, GL_FALSE, GL_RGBA, GL_UNSIGNED_BYTE,
       Displayer::record_texture_data[0]);
  }
}

void ScoreRecordManager::timeStep_inline_split_ (   )
{
  // bounce off the edges
  if (control_active) {
    if (top_rank < DC_SCORE_REC_NUMBER_DRAW / 2 && offset < 0.0f) {
      offset = -offset;
      velocity = -DC_SCORE_REC_EDGE_BOUNCE_ELASTICITY * velocity;
      ignore_up = true;

    } else if (top_rank > GC_SCORE_REC_LENGTH - 1
     + DC_SCORE_REC_NUMBER_DRAW / 2 && offset > 0.0f) {
      offset = -offset;
      velocity = -DC_SCORE_REC_EDGE_BOUNCE_ELASTICITY * velocity;
      ignore_down = true;
    }
  }

  offset += velocity;

  // switch in a new texture when needed
  if (offset > DC_SCORE_REC_RANK_HEIGHT / 2.0f) {
    offset -= DC_SCORE_REC_RANK_HEIGHT;
    top_rank++;

    top_texture--;
    if (top_texture == -1)
      top_texture = DC_SCORE_REC_NUMBER_DRAW - 1;

    if (top_rank < GC_SCORE_REC_LENGTH) {
      glBindTexture(GL_TEXTURE_2D, Displayer::record_textures[top_texture]);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DC_SCORE_REC_TEX_LENGTH_S,
       DC_LETTER_TEX_LENGTH, GL_RGBA, GL_UNSIGNED_BYTE,
       chooseTexture(top_rank));
    }

  } else if (offset < -DC_SCORE_REC_RANK_HEIGHT / 2.0f) {
    offset += DC_SCORE_REC_RANK_HEIGHT;
    top_rank--;

    top_texture++;
    if (top_texture == DC_SCORE_REC_NUMBER_DRAW)
      top_texture = 0;

    if (top_rank >= DC_SCORE_REC_NUMBER_DRAW - 1) {
      int bottom_texture = top_texture - 1;
      if (bottom_texture == -1)
        bottom_texture = DC_SCORE_REC_NUMBER_DRAW - 1;

      glBindTexture(GL_TEXTURE_2D, Displayer::record_textures[bottom_texture]);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DC_SCORE_REC_TEX_LENGTH_S,
       DC_LETTER_TEX_LENGTH, GL_RGBA, GL_UNSIGNED_BYTE,
       chooseTexture(top_rank - (DC_SCORE_REC_NUMBER_DRAW - 1)));
    }
  }

  // score record dynamics

  GLfloat r = offset + DC_SCORE_REC_RANK_HEIGHT
   * (top_rank - Score::player_rank - DC_SCORE_REC_NUMBER_DRAW / 2);

  // spring engages
  if (!spring_active && r > 0.0f && !control_active)
    spring_active = true;

  // spring dynamics
  if (spring_active) {
    velocity += -DC_SCORE_REC_SPRING * r
     - DC_SCORE_REC_SPRING_DRAG * velocity;

    if (fabs(velocity) < DC_SCORE_REC_VELOCITY_CUTOFF
     && fabs(r) < DC_SCORE_REC_OFFSET_CUTOFF) {
      velocity = 0.0f;
      offset = 0.0f;
      spring_active = false;
    }
  }

  // user gains control
  if (!control_active && fabs(velocity) < DC_SCORE_REC_CONTROL_VELOCITY_CUTOFF
   && fabs(r) < DC_SCORE_REC_CONTROL_OFFSET_CUTOFF)
    control_active = true;

  // control dynamics
  if (control_active) {
    if ((ignore_up || ignore_down)
     && fabs(velocity) < DC_SCORE_REC_BOUNCE_VELOCITY_CUTOFF)
      ignore_up = ignore_down = false;

    if ((Controller::moveCommand() & CC_UP) && !ignore_up) {
      velocity -= DC_SCORE_REC_CONTROL_VELOCITY;
      spring_active = false;
    }
    if ((Controller::moveCommand() & CC_DOWN) && !ignore_down) {
      velocity += DC_SCORE_REC_CONTROL_VELOCITY;
      spring_active = false;
    }

    velocity += -DC_SCORE_REC_CONTROL_DRAG * velocity;
  }
}
