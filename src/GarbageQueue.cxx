#include <glib.h>

#include "GarbageQueue.h"
#include "GarbageManager.h"

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

int GarbageQueue::removeToFirstGray ()
{
  int num_removed = 0;
  GarbageQueueElement *e = NULL;
  GSList *cur = NULL;
  cur = garbage_queue;
  while (cur) {
    e = (GarbageQueueElement *) (cur->data);
    if (e == NULL)
      return num_removed;
    if (e->flavor == GF_NORMAL) {
      delete e;
      garbage_queue = g_slist_delete_link(garbage_queue, cur);
      cur = garbage_queue;
      ++num_removed;
      cached_height = -1;
    } else {
      break;
    }
  }
  return num_removed;
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

static void show_element (GarbageQueueElement *e) {
  printf("Element: %p h %d w %d f %d\n",
    e,
    e->height,
    e->width,
    e->flavor);
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
  if (cached_height != -1) return cached_height;
  for (cur = garbage_queue; cur; cur = g_slist_next(cur)) {
    e = (GarbageQueueElement *) (cur->data);
    if (!e)
      break;
    garbage_height += e->height;
  }
  cached_height = garbage_height;
  return garbage_height;
}

int GarbageQueue::specialHeight ( )
{
  int garbage_height = 0;
  GarbageQueueElement *e = NULL;
  GSList *cur = NULL;
  for (cur = garbage_queue; cur; cur = g_slist_next(cur)) {
    e = (GarbageQueueElement *) (cur->data);
    if (!e)
      break;
    if (GarbageManager::isSpecialFlavor(e->flavor)) {
      garbage_height += e->height;
    }
  }
  return garbage_height;
}

static void send_to_generator (gpointer e, gpointer unused) 
{
  GarbageGenerator::addToQueue((GarbageQueueElement *)e);
}

void GarbageQueue::sendToGenerator ( ) 
{
  g_slist_foreach(garbage_queue, send_to_generator, NULL);
}
