#ifndef _GARBAGE_QUEUE_H
#define _GARBAGE_QUEUE_H

#include "GarbageGenerator.h"

class GarbageQueueElement;

class GarbageQueue {
public:
  GarbageQueue();
  ~GarbageQueue();

  void add ( int height, int width, int flavor );
  void add ( GarbageQueueElement *element );

  int height ( );
  int specialHeight();

  void reset();

  void sendToGenerator ( );
private:
  GSList *garbage_queue;
  int cached_height;
};

#endif
