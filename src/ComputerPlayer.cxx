using namespace std;

#include "ComputerPlayer.h"
#include "Score.h"
#include "GarbageQueue.h"
#include "ComputerPlayerAI.h"

//#define WAIT_TIME ( GC_STEPS_PER_SECOND * 10 )

bool ComputerPlayer::lost;
ComputerPlayerAI *ComputerPlayer::ai;

void ComputerPlayer::gameStart()
{
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
  printf("Element: %p h %d w %d f %d\n",
    e,
    e->height,
    e->width,
    e->flavor);
}

void show_element_foreach (gpointer e, gpointer unused) 
{
  show_element((GarbageQueueElement *)e);
}

int ComputerPlayer::gameFinish()
{
  return lost ? GS_WON : GS_LOST;
}

void ComputerPlayer::timeStep()
{
  if (!ai) {
    return;
  }
  ComputerPlayerAI &localAi = *ai;
  if (Game::time_step >= localAi.alarm()) {
    //localAi.garbageQueue(queue);
    GarbageQueue *tmp = localAi.garbageAmount();//garbage_queue);
    tmp->sendToGenerator();
    cout << "init pop: " << GC_INITIAL_POP_DELAY << endl;
    cout << "steps per second: " << GC_STEPS_PER_SECOND << endl;
    cout << "Height: " << ai->garbageQueue()->height() << endl;
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
