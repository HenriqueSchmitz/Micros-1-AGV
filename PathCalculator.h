#include "UtilityFunctions.h"
#include "CharMap.h"

const char halt = 'H';
const char openSpace = ' ';
const char crossing = '+';
const char line = '-';
const char target = 'X';
const char block = 'B';
const char path = '*';

class PathCalculator {
  private:
    CharMap* areaMap;
    CharMap* pathMap;
    Point startingPositionPoint;
    Point robotPositionPoint;
    Point targetPositionPoint;

  
  public: 
  PathCalculator(CharMap* areaMap){
    this->areaMap = areaMap;
    this->pathMap = areaMap->copy();
  }

  ~PathCalculator(){

  }

  void printAreaMap(){
    this->printMap(areaMap);
  }

  void printPathMap(){
    this->printMap(pathMap);
  }

  void setStartingPosition(char startingPosition){
    this->startingPositionPoint = areaMap->findFirstCellContaining(startingPosition);
    this->robotPositionPoint = this->startingPositionPoint;
  }

  void setTargetPosition(char targetPosition){
    this->targetPositionPoint = areaMap->findFirstCellContaining(targetPosition);
    this->pathMap->setCell(targetPositionPoint, target);
  }

  void calculateRoute(){
    Point objectiveDirection = this->determineObjectiveDirection();
    String movingAxis = "row";
    while(robotPositionPoint.x != targetPositionPoint.x or robotPositionPoint.y != targetPositionPoint.y){
      if((movingAxis == "row" and objectiveDirection.y == 0)
       or(movingAxis == "column" and objectiveDirection.x == 0)){
        movingAxis = getOppositeAxis(movingAxis);
      }
      int directionForCurrentAxis = getObjectiveDirectionForAxis(objectiveDirection, movingAxis);
      int directionForOppositeAxis = getObjectiveDirectionForAxis(objectiveDirection, getOppositeAxis(movingAxis));
      if(isPathOpen(movingAxis, directionForCurrentAxis)){
        moveRobotInAxis(movingAxis, getObjectiveDirectionForAxis(objectiveDirection, movingAxis));
      }/*
      else if(directionForOppositeAxis == 0){
        if(isPathOpen(getOppositeAxis(movingAxis), 1)){
          movingAxis = getOppositeAxis(movingAxis);
          moveRobotInAxis(movingAxis, 1);
        }
        else if(isPathOpen(getOppositeAxis(movingAxis), -1)){
          movingAxis = getOppositeAxis(movingAxis);
          moveRobotInAxis(movingAxis, -1);
        }
      }*/
      else if(isPathOpen(getOppositeAxis(movingAxis), directionForOppositeAxis)){
        movingAxis = getOppositeAxis(movingAxis);
        moveRobotInAxis(movingAxis, directionForOppositeAxis);
      }
      else if(isPathOpen(movingAxis, -1 * directionForCurrentAxis)){
        moveRobotInAxis(movingAxis, -1 * directionForCurrentAxis);
        movingAxis = getOppositeAxis(movingAxis);
      }
      else{
        movingAxis = getOppositeAxis(movingAxis);
        moveRobotInAxis(movingAxis, -1 * directionForOppositeAxis);
        movingAxis = getOppositeAxis(movingAxis);
      }
      printPathMap();
    }
  }

  CharMap* getAreaMap(){
    return this->areaMap;
  }

  CharMap* getPathMap(){
    return this->pathMap;
  }

  Point getStartingPosition(){
    return this->startingPositionPoint;
  }

  Point getTargetPosition(){
    return this->targetPositionPoint;
  }

  void reportBlockOn(Point blockedCell){
    areaMap->setCell(blockedCell, block);
  }

  void recalculateFrom(Point currentPosition){
    this->startingPositionPoint = currentPosition;
    this->robotPositionPoint = currentPosition;
    pathMap = areaMap->copy();
    this->pathMap->setCell(targetPositionPoint, target);
    calculateRoute();
  }

  private:
  void printMap(CharMap* mapToPrint){
    Serial.println();
    Point cell;
    for(cell.y = 0; cell.y < mapToPrint->getNumberOfRows(); cell.y++){
      for(cell.x = 0; cell.x < mapToPrint->getNumberOfColumns(); cell.x++){
        Serial.print(mapToPrint->getCell(cell));
      }
      Serial.println();
    }
  }

  Point determineObjectiveDirection(){
    Point objectiveDirection;
    if(robotPositionPoint.y < targetPositionPoint.y){
      objectiveDirection.y = 1;
    }
    else if(robotPositionPoint.y > targetPositionPoint.y){
      objectiveDirection.y = -1;
    }
    if(robotPositionPoint.x < targetPositionPoint.x){
      objectiveDirection.x = 1;
    }
    else if(robotPositionPoint.x > targetPositionPoint.x){
      objectiveDirection.x = -1;
    }
    return objectiveDirection;
  }

  void moveRobotInAxis(String moveAxis, int moveDirection){
    if(moveAxis == "row"){
      moveRobotInRows(moveDirection);
    }
    else{
      moveRobotInColumns(moveDirection);
    }
  }

  void moveRobotInRows(int moveDirection){
    robotPositionPoint.y = robotPositionPoint.y + moveDirection;
    this->pathMap->setCell(robotPositionPoint, path);
  }

  void moveRobotInColumns(int moveDirection){
    robotPositionPoint.x = robotPositionPoint.x + moveDirection;
    this->pathMap->setCell(robotPositionPoint, path);
  }

  boolean isPathOpen(String axis, int moveDirection){
    char targetedCellContent;
    Point targetedCell = robotPositionPoint;
    if(axis == "row"){
      targetedCell.y += moveDirection;
      targetedCellContent = pathMap->getCell(targetedCell);
    }
    if(axis == "column"){
      targetedCell.x += moveDirection;
      targetedCellContent = pathMap->getCell(targetedCell);
    }
    return isCellOpenForMovement(targetedCellContent);
  }

  boolean isCellOpenForMovement(char cellContent){
    if( cellContent == crossing or cellContent == line or cellContent == target){
      return true;
    }
    return false;
  }

  String getOppositeAxis(String currentDirection){
    if(currentDirection == "row"){
      return "column";
    }
    else{
      return "row";
    }
  }

  int getObjectiveDirectionForAxis(Point objectiveDirection, String axis){
    if(axis == "row"){
      return objectiveDirection.y;
    }
    else{
      return objectiveDirection.x;
    }
  }
};
