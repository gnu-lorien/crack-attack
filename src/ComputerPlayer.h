#ifndef COMP_PLAY_H
#define COMP_PLAY_H

using namespace std;

#include <glib.h>

#include "Score.h"
#include "GarbageQueue.h"
#include "ComputerPlayerAI.h"

/*
class GarbageQueueElement;
*/
class ComputerPlayerAI;

/* static */ class ComputerPlayer {
private:
  static bool lost;
  static ComputerPlayerAI *ai;

public:
  static void gameStart (  );
  static void timeStep (   );
  static int gameFinish (    );

  static void addGarbage ( int height, int width, int flavor );

  // lights
  static bool checkLevelLightDying ( );
  static bool checkLevelLightBlue ( int block );
};

#endif
