#ifndef cpai_h_
#define cpai_h_

#include <glib.h>

using namespace std;

#include "Game.h"
#include "ComputerPlayer.h"

class GarbageQueue; 

class ComputerPlayerAI {
private:
  int last_time;
  
protected:
  enum AI_STATE { AI_WAITING, AI_SHATTERING } state;
  GarbageQueue *queue;

  virtual int baseSteps();
  virtual int stateSteps();

public:

  ComputerPlayerAI(){ 
    resetAlarm();
    state = AI_WAITING;
    queue = NULL;
    cout << "Creating a fucking generic" << endl;
  }

  ~ComputerPlayerAI(){}

  int alarm ( );
  void resetAlarm ( );
  virtual int garbageShatterDelay ( );
  void garbageQueue ( GarbageQueue *queue );
  GarbageQueue *garbageQueue ( );
  virtual GarbageQueue *garbageAmount ( );
  virtual bool determineLoss ( );
};

class EasyAI :public ComputerPlayerAI { 
public:
  EasyAI() { cout << "Creating a fucking easy" << endl;}
  ~EasyAI(){}

protected:
  virtual int baseSteps();
};

class MediumAI :public ComputerPlayerAI { 
public:
  MediumAI(){}
  ~MediumAI(){}
protected:
  virtual int baseSteps();
};

class HardAI :public ComputerPlayerAI { 
public:
  HardAI(){}
  ~HardAI(){}
protected:
  virtual int baseSteps();
};

#endif
