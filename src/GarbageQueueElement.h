#ifndef _GQE_H_
#define _GQE_H_

typedef struct _GarbageQueueElement {
  bool active;
  int alarm;
  int height;
  int width;
  int flavor;
} GarbageQueueElement;

#endif
