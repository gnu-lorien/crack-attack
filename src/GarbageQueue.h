#ifndef _GARBAGE_QUEUE_H
#define _GARBAGE_QUEUE_H

#include "GarbageQueueElement.h"

#include <vector>

using namespace std;

class GarbageQueue {
public:
  GarbageQueue();
  ~GarbageQueue();

  void add ( int height, int width, int flavor );
  void add ( GarbageQueueElement &element );

  int height ( );
  int specialHeight ( );

  void reset ( );

  int removeWithSpecials ( );

  void sendToGenerator ( );

private:

  int cached_height;
  std::vector<GarbageQueueElement> garbage_queue;

  int removeToFirst ( int flavor );
};

#endif
