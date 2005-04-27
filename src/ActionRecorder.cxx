/*
 * ActionRecorder.cxx
 * 
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
 */

#include <fstream>
#include "TextureLoader.h"

using namespace std;

#include "ActionRecorder.h"

std::vector<Action> ActionRecorder::actions;

struct offset_subtract : 
  public unary_function< 
    Action, void >
{
  offset_subtract(int b) : base(b) {}
  int base;
  void operator() (Action act) {
    act.time_step -= base;
  }
};

void ActionRecorder::gameFinish (  )
{
  char file_name[256];
  if(actions.size()==0) return;
  int base_ts = actions[0].time_step;
  for(size_t i = 0; i < actions.size(); ++i) {
    actions[i].time_step -= base_ts;
  }
  TextureLoader::buildLocalDataFileName(GC_REPLAY_FILE_NAME, file_name);
  ofstream mult(file_name);
  if(!mult.fail()) {
    size_t size = actions.size();
    for (size_t i=0; i < size; ++i) {
      mult << actions[i].name << "\n" << actions[i].time_step << endl;
    }
    mult.close();
  }
}
