#ifndef cpai_h_
#define cpai_h_

#include <glib.h>

//using namespace std;

#include "Game.h"
#include "GarbageQueue.h"
//#include "ComputerPlayer.h"

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

  virtual int lossHeight();

public:

  ComputerPlayerAI();
  ~ComputerPlayerAI(){}

  int alarm ( );
  void resetAlarm ( );
  virtual int garbageShatterDelay ( );
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
  virtual int lossHeight();
};

class HardAI :public ComputerPlayerAI { 
public:
  HardAI(){}
  ~HardAI(){}

protected:
  virtual int baseSteps();
  virtual int lossHeight();

};

#endif
