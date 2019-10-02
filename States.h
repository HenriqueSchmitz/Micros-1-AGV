#include "MapNavigator.h"
#include "Robot.h"

char followLineStraight(Robot* robot){
  robot->followLineUntillCrossing();
  char nextMovement = surpassCrossing;
  return nextMovement;
}

char surpassCrossing(Robot* robot){
  robot->surpassCrossing();
  char nextMovement = followLineStraight;
  return nextMovement;
}
