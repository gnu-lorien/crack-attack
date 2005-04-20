/*
 * ComputerPlayerAI.h
 * Andrew Sayman - 3/27/05
 *
 * Copyright (C) 2005  Andrew Sayman
 * Copyright (C) 2005  Kevin Webb
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

#ifndef cpai_h_
#define cpai_h_

#include "GarbageQueue.h"

class GarbageQueue; 

class ComputerPlayerAI {
private:
  int last_time;
  int last_shatter_height;
  
protected:
  enum AI_STATE { AI_WAITING, AI_SHATTERING } state;
  GarbageQueue *queue;

  virtual int baseSteps();
  virtual int stateSteps();
  virtual int garbageShatterDelay ( );

  virtual void shatter();

public:

  ComputerPlayerAI();
  virtual ~ComputerPlayerAI(){}

  int alarm ( );
  void resetAlarm ( );
  GarbageQueue *garbageQueue ( );
  virtual GarbageQueue *garbageAmount ( );

  virtual bool determineLoss ( );
  virtual int lossHeight();
};

class EasyAI :public ComputerPlayerAI { 
public:
  EasyAI() { }
  virtual ~EasyAI(){}

  virtual int lossHeight();
protected:
  virtual int baseSteps();
};

class MediumAI :public ComputerPlayerAI { 
public:
  MediumAI(){}
  virtual ~MediumAI(){}

  virtual int lossHeight();
protected:
  virtual int baseSteps();
};

class HardAI :public ComputerPlayerAI { 
public:
  HardAI(){}
  virtual ~HardAI(){}

  virtual int lossHeight();
protected:
  virtual int baseSteps();

};

#endif
