#include "ComputerPlayerAI.h"
#include "Score.h"
#include "Garbage.h"
#include "GarbageGenerator.h"
#include "Game.h"

/* Begin ComputerPlayerAI */
int ComputerPlayerAI::baseSteps()
{
  return GC_STEPS_PER_SECOND;
}

int ComputerPlayerAI::stateSteps()
{
  if (state == AI_WAITING) {
    return 0;
  }
  if (state == AI_SHATTERING) {
    return garbageShatterDelay();
  }
}

void ComputerPlayerAI::resetAlarm() 
{
  last_time = Game::time_step;
}

int ComputerPlayerAI::alarm()
{
  //cout << "Comp AI" << endl;
  return last_time + baseSteps() + stateSteps();
}

int ComputerPlayerAI::garbageShatterDelay()
{
  if (queue) {
    cout << "Given height on shatter calc: " << queue->height() << endl;
    return GC_INITIAL_POP_DELAY * 3 * queue->height();
  } else
    return 0;
}

void ComputerPlayerAI::garbageQueue ( GarbageQueue *q )
{
  queue = q;
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
  if (Score::score > 500) {
    for (i = 0; i < (2 % size); ++i) {
      garbage.height = 3;
      garbage.width  = 6;
      garbage.flavor = GF_NORMAL;
      q->add(garbage.height, garbage.width, garbage.flavor);
    }
    for (i = 0; i < (3 % size); ++i) {
      garbage.height = 1;
      garbage.width  = 3;
      garbage.flavor = GF_GRAY;
      q->add(garbage.height, garbage.width, garbage.flavor);
    }
    for (i = 0; i < (2 % size); ++i) {
      garbage.height = 3;
      garbage.width  = 6;
      garbage.flavor = GF_NORMAL;
      q->add(garbage.height, garbage.width, garbage.flavor);
    }
  } else 
  if (Score::score > 300) {
    for (i = 0; i < (2 % size); ++i) {
      garbage.height = 2;
      garbage.width  = 6;
      garbage.flavor = GF_NORMAL;
      q->add(garbage.height, garbage.width, garbage.flavor);
    }
    for (i = 0; i < (3 % size); ++i) {
      garbage.height = 1;
      garbage.width  = 3;
      garbage.flavor = GF_GRAY;
      q->add(garbage.height, garbage.width, garbage.flavor);
    }
  } else
  if (Score::score > 100) {
    for (i = 0; i < (3 % size); ++i) {
      garbage.height = 1;
      garbage.width  = 6;
      garbage.flavor = GF_NORMAL;
      q->add(garbage.height, garbage.width, garbage.flavor);
    }
    garbage.flavor = GF_GRAY;
    q->add(garbage.height, garbage.width, garbage.flavor);
  } else
  if (Score::score > 50) {
    for (i = 0; i < (3 % size); ++i) {
      garbage.height = 1;
      garbage.width  = 4;
      garbage.flavor = GF_NORMAL;
      q->add(garbage.height, garbage.width, garbage.flavor);
    }
  } else
  for (i = 0; i < (3 % size); ++i) {
    garbage.height = 1;
    garbage.width  = 3;
    garbage.flavor = GF_NORMAL;
    q->add(garbage.height, garbage.width, garbage.flavor);
  }
  return q;
}

bool ComputerPlayerAI::determineLoss()
{
  const unsigned int target = 4;
  MESSAGE("Begin AI check for loss");
  if (queue) {
    MESSAGE("AI queue height " << queue->height() << " target " << target);
    return queue->height() > target;
  } else {
    return true;
  }
}

/* End ComputerPlayerAI */

int EasyAI::baseSteps()
{
  //cout << "easy baseSteps" << endl;
  int a = ComputerPlayerAI::baseSteps() * 20;
  return a;
}

int MediumAI::baseSteps()
{
  return ComputerPlayerAI::baseSteps() * 10;
}

int HardAI::baseSteps()
{
  return ComputerPlayerAI::baseSteps() * 5;
}
