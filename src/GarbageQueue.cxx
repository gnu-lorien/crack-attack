#include "GarbageQueue.h"
#include "GarbageGenerator.h"
#include "GarbageManager.h"

GarbageQueue::GarbageQueue () {
  //garbage_queue// = new vector<GarbageQueueElement();
  cached_height = -1;
}

GarbageQueue::~GarbageQueue () {
  reset();
}

void GarbageQueue::reset () {
  garbage_queue.clear();
  cached_height = -1;
}

int GarbageQueue::removeWithSpecials ()
{
  if ((*(garbage_queue.begin())).flavor == GF_GRAY) {
    return removeToFirst(GF_NORMAL);
  } else {
    return removeToFirst(GF_GRAY);
  }
#warning xtreme not supported
  return 0;
}

int GarbageQueue::removeToFirst ( int flavor )
{
  int num_removed = 0;
  assert(*(garbage_queue.begin()).flavor != flavor);
  if (garbage_queue.empty()) return 0;
  vector<GarbageQueueElement>::iterator iter;
  for (iter = garbage_queue.begin(); iter != garbage_queue.end(); ++iter) {
    if ((*iter).flavor == flavor) break;
    ++num_removed;
  }
  if (num_removed == 0) return 0;
  garbage_queue.erase(garbage_queue.begin(), iter);
  cached_height = -1;
  return num_removed;
}

void GarbageQueue::add ( int height, int width, int flavor)
{
  GarbageQueueElement e;
  e.active = true;
  e.height = height;
  e.width = width;
  e.flavor = flavor;
  add(e);
}

static void show_element (GarbageQueueElement &e) {
#ifndef NDEBUG
  printf("Element: %p h %d w %d f %d\n",
    e,
    e.height,
    e.width,
    e.flavor);
#endif
}

void GarbageQueue::add ( GarbageQueueElement &element )
{
  element.active = true;
  MESSAGE("Adding garbage " << element->active);
  show_element(element);
  assert(element->height <= GC_PLAY_HEIGHT);
  assert(element->width  <= GC_PLAY_WIDTH);
  garbage_queue.push_back(element);
  cached_height = -1;
}

int GarbageQueue::height ( )
{
  int garbage_height = 0;
  vector<GarbageQueueElement>::iterator iter;
  if (cached_height != -1) return cached_height;
  for (iter = garbage_queue.begin(); iter != garbage_queue.end(); ++iter) {
    garbage_height += (*iter).height;
  }
  cached_height = garbage_height;
  return garbage_height;
}

int GarbageQueue::specialHeight ( )
{
  int garbage_height = 0;
  vector<GarbageQueueElement>::iterator iter;
  if (cached_height != -1) return cached_height;
  for (iter = garbage_queue.begin(); iter != garbage_queue.end(); ++iter) {
    if (GarbageManager::isSpecialFlavor((*iter).flavor))
      garbage_height += (*iter).height;
  }
  return garbage_height;
}

void GarbageQueue::sendToGenerator ( ) 
{
  vector<GarbageQueueElement>::iterator iter;
  for (iter = garbage_queue.begin(); iter != garbage_queue.end(); ++iter) {
    GarbageGenerator::addToQueue(*iter);
  }
}
