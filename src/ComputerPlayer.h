#ifndef COMP_PLAY_H
#define COMP_PLAY_H

using namespace std;

#include <glib.h>

#include "Score.h"
#include "GarbageGenerator.h"
#include "ComputerPlayerAI.h"

class GarbageQueueElement;
class ComputerPlayerAI;

class GarbageQueue {
public:
  GarbageQueue();
  ~GarbageQueue();

  void add ( int height, int width, int flavor );
  void add ( GarbageQueueElement *element );

  int height ( );

  GSList *garbage ( );
  GSList *specialGarbage ( );

  void reset();

  void sendToGenerator ( );
private:
  GSList *garbage_queue;
  int cached_height;
};

/* static */ class ComputerPlayer {
private:
  static bool lost;
  static GarbageQueue *queue;
  static ComputerPlayerAI *ai;

  static void determineLoss ( );

public:
  static void gameStart (  );
  static void timeStep (   );
  static int gameFinish (    );

  static void addGarbage ( int height, int width, int flavor ) {
    if (queue) 
      queue->add(height, width, flavor);
  }
};

#endif
