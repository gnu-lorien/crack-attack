#include "LevelLights.h"
#include "ComputerPlayer.h"
#include "Score.h"

using namespace std;

//#define WAIT_TIME ( GC_STEPS_PER_SECOND * 10 )

bool ComputerPlayer::lost;
ComputerPlayerAI *ComputerPlayer::ai;

void ComputerPlayer::gameStart()
{
  if (!(MetaState::mode & CM_AI))
    return;
  //queue = new GarbageQueue();

  if ((MetaState::mode & CM_AI_EASY))
    ai = new EasyAI();
  if ((MetaState::mode & CM_AI_MEDIUM))
    ai = new MediumAI();
  if ((MetaState::mode & CM_AI_HARD))
    ai = new HardAI();
  /*
  if (ai)
    ai->garbageQueue(queue);
  */
  assert(ai != NULL);
  
  lost = false;
}

static void show_element (GarbageQueueElement *e) {
#ifndef NDEBUG
  printf("Element: %p h %d w %d f %d\n",
    e,
    e->height,
    e->width,
    e->flavor);
#endif
}

int ComputerPlayer::gameFinish()
{
  return ai->determineLoss() ? GS_WON : GS_LOST;
}

void ComputerPlayer::timeStep()
{
  static bool first_time = true;
  if (!(MetaState::mode & CM_AI))
    return;
  if (!ai) {
    return;
  }
  
  // handle the lights
  LevelLights::handleAI();

  ComputerPlayerAI &localAi = *ai;
  if (first_time) {
    MESSAGE("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
    LOG("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
    first_time = false;
  }
  if (Game::time_step >= localAi.alarm()) {
    //localAi.garbageQueue(queue);
    localAi.garbageAmount()->sendToGenerator();//garbage_queue);
#ifndef NDEBUG
    cout << "init pop: " << GC_INITIAL_POP_DELAY << endl;
    cout << "steps per second: " << GC_STEPS_PER_SECOND << endl;
    cout << "Height: " << ai->garbageQueue()->height() << endl;
#endif
    //delete tmp;
    //queue->reset();
    localAi.resetAlarm();
    MESSAGE("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
    LOG("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
  }
  if(localAi.determineLoss()) {
    Game::aiPlayerLoss();
  }
}

void ComputerPlayer::addGarbage ( int height, int width, int flavor ) {
  assert(ai != NULL);
  MESSAGE("Adding garbage to queue");
  ai->garbageQueue()->add(height, width, flavor);
}

bool ComputerPlayer::checkLevelLightDying()
{
  int height = ai->garbageQueue()->height();
  int ninety = ai->lossHeight() * .9;
  //MESSAGE("ninety " << ninety);
  if (ninety == ai->lossHeight())
    ninety = ai->lossHeight() - 1;
  if (height >= ninety)
    return true;
  return false;
}

bool ComputerPlayer::checkLevelLightBlue(int n)
{
  double max = LL_NUMBER_LEVEL_LIGHTS;
  double lh = ai->lossHeight();
  //MESSAGE("lh " << lh << " max " << max);
  double partition = lh / max;
  double colorh = n * partition;
  //MESSAGE("n " << n << " partition " << partition << " colorh " << colorh << " height " << ai->garbageQueue()->height());
  if (colorh >= ai->garbageQueue()->height())
    return true;
  else
    return false;
}
