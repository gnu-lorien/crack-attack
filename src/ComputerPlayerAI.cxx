#include "ComputerPlayerAI.h"
#include "Score.h"
#include "Garbage.h"
#include "GarbageGenerator.h"
#include "Game.h"

#define loop(v,m) for(int v = 0; v<(m); v++)
#define loopi(m) loop(i,m)

/* Begin ComputerPlayerAI */
ComputerPlayerAI::ComputerPlayerAI ()
{ 
  resetAlarm();
  last_shatter_height = 0;
  state = AI_WAITING;
  queue = new GarbageQueue();
  cout << "Creating a fucking generic" << endl;
}

int ComputerPlayerAI::baseSteps()
{
  return GC_STEPS_PER_SECOND;
}

int ComputerPlayerAI::stateSteps()
{
  if (state == AI_WAITING) {
    return (GC_CREEP_ADVANCE_VELOCITY * 5) // increase five lines
      + (GC_DYING_DELAY * 5); // five combos
  }
  if (state == AI_SHATTERING) {
    return garbageShatterDelay();
  }
}

int ComputerPlayerAI::lossHeight()
{
  return 4;
}

void ComputerPlayerAI::resetAlarm() 
{
  last_time = Game::time_step;
}

int ComputerPlayerAI::alarm()
{
  return last_time + baseSteps() + stateSteps();
}

int ComputerPlayerAI::garbageShatterDelay()
{
  int delay = GC_INITIAL_POP_DELAY + (last_shatter_height * GC_PLAY_WIDTH * GC_INTERNAL_POP_DELAY) + GC_FINAL_POP_DELAY;
  return delay;
}

GarbageQueue *ComputerPlayerAI::garbageQueue ()
{
  return queue;
}

void set_timestamp (gpointer element, gpointer time_stamp)
{
  BufferElement *e = (BufferElement *)element;
  e->time_stamp = GPOINTER_TO_INT(time_stamp);
}

GarbageQueue *ComputerPlayerAI::garbageAmount( )
{
  GarbageQueue *q = new GarbageQueue();
  size_t const size = 20;
  size_t i = 0;
  BufferElement garbage;
  int working_height = GC_SAFE_HEIGHT - 1 - garbageQueue()->height();
  int num_grays, num_normals;

  MESSAGE("Hard garbageAmount");

  if (working_height > 0) {
    num_grays = working_height % 3;
  } else {
    num_grays = 0;
  }

  num_normals = garbageQueue()->height() + working_height;

  LOG("garbageQueue height " << garbageQueue()->height());
  LOG("grays: " << num_grays << " normals: " << num_normals);
  loopi(num_grays)
    q->add(1, 6, GF_GRAY);

  int norm_div = num_normals / 3;
  int norm_mod = num_normals % 3;
  int more_gray = norm_mod / 2;
  LOG("div: " << norm_div << " mod: " << norm_mod << " gray: " << more_gray);
  if (norm_div > 0) q->add(norm_div, 6, GF_NORMAL);
  loopi(norm_mod) q->add(1, 6, GF_NORMAL);
  //loopi(more_gray) q->add(1, 6, GF_GRAY);

  shatter();
  return q;
}

void ComputerPlayerAI::shatter()
{
  MESSAGE("Resetting garbageQueue " << garbageQueue()->height());
  if (garbageQueue()->height() > 0) {
    state = AI_SHATTERING; 
    int gray_height = garbageQueue()->specialHeight();
    last_shatter_height = garbageQueue()->removeToFirstGray();
    MESSAGE(last_shatter_height << " shattered and " << garbageQueue()->height() << " remaining.");
    if (gray_height != 0) assert(garbageQueue()->height() != 0);
    /*
    int gray_height = garbageQueue()->specialHeight();
    if (gray_height > 0) {
      last_shatter_height *= gray_height + 1;
      MESSAGE("Gray mult: " << gray_height + 1);
      LOG("Gray mult: " << gray_height + 1);
    }
    */
  } else {
    state = AI_WAITING;
    last_shatter_height = 0;
  }
  garbageQueue()->reset();
}

bool ComputerPlayerAI::determineLoss()
{
  GarbageQueue *queue = garbageQueue();
  static int height = queue->height();
  int h = queue->height();
  if (h != height) {
    MESSAGE("Height change in determine loss old: " << height << " new: " << h);
    height = h;
  }
  return queue->height() > lossHeight();
}

/* End ComputerPlayerAI */

int EasyAI::baseSteps()
{
  //cout << "easy baseSteps" << endl;
  int a = ComputerPlayerAI::baseSteps() * 15;
  return a;
}

int MediumAI::baseSteps()
{
  return ComputerPlayerAI::baseSteps() * 10;
}

int MediumAI::lossHeight()
{
  return 10;
}

/* Begin HardAI */
int HardAI::baseSteps()
{
  return ComputerPlayerAI::baseSteps() * 5;
}

int HardAI::lossHeight()
{
  return 20;
}

/* End HardAI */
