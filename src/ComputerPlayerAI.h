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
  ~ComputerPlayerAI(){}

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
  ~EasyAI(){}

  virtual int lossHeight();
protected:
  virtual int baseSteps();
};

class MediumAI :public ComputerPlayerAI { 
public:
  MediumAI(){}
  ~MediumAI(){}

  virtual int lossHeight();
protected:
  virtual int baseSteps();
};

class HardAI :public ComputerPlayerAI { 
public:
  HardAI(){}
  ~HardAI(){}

  virtual int lossHeight();
protected:
  virtual int baseSteps();

};

#endif
