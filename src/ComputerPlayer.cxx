using namespace std;

#include "ComputerPlayer.h"
#include "Score.h"
#include "Garbage.h"
#include "GarbageGenerator.h"
#include "GarbageManager.h"
#include "Clock.h"
#include "ComputerPlayerAI.h"

//#define WAIT_TIME ( GC_STEPS_PER_SECOND * 10 )

bool ComputerPlayer::lost;
GarbageQueue *ComputerPlayer::queue;
ComputerPlayerAI *ComputerPlayer::ai;

void ComputerPlayer::gameStart()
{
  queue = new GarbageQueue();

  if ((MetaState::mode & CM_AI_EASY))
    ai = new EasyAI();
  if ((MetaState::mode & CM_AI_MEDIUM))
    ai = new MediumAI();
  if ((MetaState::mode & CM_AI_HARD))
    ai = new HardAI();
  if (ai)
    ai->garbageQueue(queue);
  
  lost = false;
}

void show_element (GarbageQueueElement *e) {
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

void ComputerPlayer::determineLoss()
{
  int garbage_height = queue->height();
  if (garbage_height > GC_SAFE_HEIGHT) {
    cerr << "Game over height: " << garbage_height << endl;
    exit(0);
    /*
    lost = true;
    Game::win();
    */
  }
}

int ComputerPlayer::gameFinish()
{
  return lost ? GS_WON : GS_LOST;
}

void ComputerPlayer::timeStep()
{
  MESSAGE("Computer timeStep");
  if (!ai) {
    return;
  }
  ComputerPlayerAI &localAi = *ai;
  if (Game::time_step >= localAi.alarm()) {
    localAi.garbageQueue(queue);
    GarbageQueue *tmp = localAi.garbageAmount();//garbage_queue);
    tmp->sendToGenerator();
    cout << "init pop: " << GC_INITIAL_POP_DELAY << endl;
    cout << "steps per second: " << GC_STEPS_PER_SECOND << endl;
    cout << "Height: " << queue->height() << endl;
    //delete tmp;
    queue->reset();
    localAi.resetAlarm();
  }
  if(localAi.determineLoss()) {
    Game::aiPlayerLoss();
  }
}

/* Garbage Queue */
GarbageQueue::GarbageQueue () {
  garbage_queue = NULL;
  cached_height = -1;
}

GarbageQueue::~GarbageQueue () {
  reset();
}

void delete_elements ( gpointer e, gpointer unused )
{
  delete (GarbageQueueElement *)e;
}

void GarbageQueue::reset () {
  if (garbage_queue != NULL) {
    g_slist_foreach(garbage_queue, delete_elements, NULL);
    g_slist_free(garbage_queue);
    garbage_queue = NULL;
  }
  cached_height = -1;
}

void GarbageQueue::add ( int height, int width, int flavor)
{
  GarbageQueueElement *e = new GarbageQueueElement();
  e->active = true;
  e->height = height;
  e->width = width;
  e->flavor = flavor;
  add(e);
}

void GarbageQueue::add ( GarbageQueueElement *element )
{
  element->active = true;
  MESSAGE("Adding garbage " << element->active);
  show_element(element);
  assert(element->height <= GC_PLAY_HEIGHT);
  assert(element->width  <= GC_PLAY_WIDTH);
  garbage_queue = g_slist_append(garbage_queue, element);
  cached_height = -1;
}

int GarbageQueue::height ( )
{
  int garbage_height = 0;
  GarbageQueueElement *e = NULL;
  GSList *cur = NULL;
  for (cur = garbage_queue; cur; cur = g_slist_next(cur)) {
    e = (GarbageQueueElement *) (cur->data);
    if (!e)
      break;
    garbage_height += e->height;
  }
  cached_height = garbage_height;
  return garbage_height;
}

GSList *GarbageQueue::garbage ( )
{
  return garbage_queue;
}

GSList *GarbageQueue::specialGarbage ( )
{
  GarbageQueueElement *e = NULL;
  GSList *iter = NULL, *ret = NULL;
  for (iter = garbage_queue; iter; g_slist_next(iter)) {
    e = (GarbageQueueElement *) (iter->data);
    if (!e)
      break;
    if (GarbageManager::isSpecialFlavor(e->flavor)) {
      ret = g_slist_append(ret, e);
    }
  }
}

static void send_to_generator (gpointer e, gpointer unused) 
{
  GarbageGenerator::addToQueue((GarbageQueueElement *)e);
}

void GarbageQueue::sendToGenerator ( ) 
{
  g_slist_foreach(garbage_queue, send_to_generator, NULL);
}
