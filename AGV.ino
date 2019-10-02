#include "Navigator.h"
#include "Robot.h"

PathCalculator* pathCalculator;
Navigator* mapNavigator;
char areaMap[] =
{
 ' ','E',' ','F',' ','G',' ','H',' ',
 '1','+','-','+','-','+','-','+','5',
 ' ','-',' ','-',' ','-',' ','-',' ',
 '2','+','-','+','-','+','-','+','6',
 ' ','-',' ','-',' ','-',' ','-',' ',
 '3','+','-','+','-','+','-','+','7',
 ' ','-',' ','-',' ','-',' ','-',' ',
 '4','+','-','+','-','+','-','+','8',
 ' ','A',' ','B',' ','C',' ','D',' '
};


Robot* robot;

void setup() {
  Serial.begin(9600);
  robot = new Robot();
  CharMap* charMap = new CharMap(9,9,areaMap);
  pathCalculator = new PathCalculator(charMap);
  pathCalculator->printPathMap();
  pathCalculator->setStartingPosition('A');
  pathCalculator->setTargetPosition('5');
  pathCalculator->calculateRoute();
  pathCalculator->printPathMap();
  mapNavigator = new Navigator(pathCalculator);
}

void loop() {/*
  robot->followLineUntillCrossing();
  robot->surpassCrossing();
  robot->followLineUntillCrossing();
  robot->takeRightOnCrossing();
  robot->followLineUntillCrossing();
  robot->takeLeftOnCrossing();
  robot->followLineUntillCrossing();
  stopExecution();
  */
  char robotCommand;
  boolean isSuccesful;
  int iteration = 0;
  while(true){
    robotCommand = mapNavigator->getCommandForRobot();
    switch(robotCommand){
      case followLineStraight:
          Serial.println("Follow Line");
          isSuccesful = true;
          iteration++;
          if(iteration == 5){
            isSuccesful = false;
          }
          
        //isSuccesful = robot->followLineUntillCrossing();
        if(isSuccesful){
          mapNavigator->processMovement(FRONT);
        }
        else{
          mapNavigator->reportBlock();
          pathCalculator->printPathMap();
        }
        break;
      case turnRight:
        Serial.println("Take Right");
        //robot->takeRightOnCrossing();
        mapNavigator->processMovement(RIGHT);
        break;
      case turnLeft:
        Serial.println("Take Left");
        //robot->takeLeftOnCrossing();       
        mapNavigator->processMovement(LEFT);
        break;
      case surpassCrossing:
        Serial.println("Surpass Crossing");
        //robot->surpassCrossing();
        mapNavigator->processMovement(FRONT);
        break;
      case turnBack:
        Serial.println("Turn Around");
        //robot->turnAround();
        //robot->followLineUntillCrossing();
        mapNavigator->processMovement(BACK);
        break;
      case stopOnObjective:
        Serial.println("Enter Destination");
        //robot->enterDestination();
        stopExecution();
        break;
      default:
        Serial.println("Defaultou");
        break;
    }
  }
}
