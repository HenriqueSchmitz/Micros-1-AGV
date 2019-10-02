#include "PathCalculator.h"

const char turnLeft = 'L';
const char turnRight = 'R';
const char turnBack = 'B';
const char surpassCrossing = 'C';
const char followLineStraight = 'S';
const char turnBackOnCrossing = 'R';
const char stopOnObjective = 'H';

enum movement {
      FRONT,
      RIGHT,
      BACK,
      LEFT
    };

class Navigator{
  private:
    enum directions {
      UP = 0,
      HORIZONTALRIGHT = 1,
      DOWN = 2,
      HORIZONTALLEFT = 3
    };
    PathCalculator* pathCalculator;
    CharMap* areaMap;
    CharMap* pathMap;
    Point objectivePosition;
    Point robotPosition;
    directions robotDirection;


  public:
    Navigator(PathCalculator* pathCalculator){
      CharMap* pathCalculatorAreaMap = pathCalculator->getAreaMap();
      areaMap = pathCalculatorAreaMap->copy();
      CharMap* pathCalculatorPathMap = pathCalculator->getPathMap();
      pathMap = pathCalculatorPathMap->copy();
      robotPosition = pathCalculator->getStartingPosition();
      objectivePosition = pathCalculator->getTargetPosition();
      this->pathCalculator = pathCalculator;
      defineStartingOrientation();
    }

    ~Navigator(){

    }

    char getCommandForRobot(){
      if(robotPosition.x == objectivePosition.x and robotPosition.y == objectivePosition.y){
        return stopOnObjective;
      }
      directions nextDirectionToMove = this->getNextDirectionToMove();
      movement movementRelativeToRobot = this->getMovementRelativeToRobot(nextDirectionToMove);
      char robotCommand = getRobotCommandForCurrentPosition(movementRelativeToRobot);
      return robotCommand;
    }

    void processMovement(movement receivedMovement){
      if(receivedMovement == FRONT){
        processForwardMovement();
      }
      else{
        processTurn(receivedMovement);
        processForwardMovement();
      }
    }

    void reportBlock(){
      pathCalculator->reportBlockOn(getCellInFront());
      pathCalculator->recalculateFrom(robotPosition);
      CharMap* pathCalculatorAreaMap = pathCalculator->getAreaMap();
      areaMap = pathCalculatorAreaMap->copy();
      CharMap* pathCalculatorPathMap = pathCalculator->getPathMap();
      pathMap = pathCalculatorPathMap->copy();
    }

  private:
    void defineStartingOrientation(){
      if(robotPosition.x == 0){
        robotDirection = HORIZONTALRIGHT;
      }
      else if(robotPosition.y == 0){
        robotDirection = DOWN;
      }
      else if(robotPosition.x == (pathMap->getNumberOfColumns() - 1)){
        robotDirection = HORIZONTALLEFT;
      }
      else{
        robotDirection = UP;
      }
    }

    Point getCellInFront(){
      Point cellInFront = robotPosition;
      switch(robotDirection){
        case UP:
          cellInFront.y--;
          break;
        case DOWN:
          cellInFront.y++;
          break;
        case RIGHT:
          cellInFront.x++;
          break;
        default:
          cellInFront.x--;
          break;
      }
      return cellInFront;
    }

    directions getNextDirectionToMove(){
      if(pathMap->getCellByCoordinates(robotPosition.x, robotPosition.y - 1) == path){
        return UP;
      }
      else if(pathMap->getCellByCoordinates(robotPosition.x, robotPosition.y + 1) == path){
        return DOWN;
      }
      else if(pathMap->getCellByCoordinates(robotPosition.x + 1, robotPosition.y) == path){
        return HORIZONTALRIGHT;
      }
      else{
        return HORIZONTALLEFT;
      }
    }

    movement getMovementRelativeToRobot(directions directionToMove){
      if(robotDirection == directionToMove){
        return FRONT;
      }
      else if((robotDirection == UP and directionToMove == DOWN) or (robotDirection == HORIZONTALRIGHT and directionToMove == HORIZONTALLEFT)
           or (robotDirection == HORIZONTALLEFT and directionToMove == HORIZONTALRIGHT) or (robotDirection == DOWN and directionToMove == UP)){
        return BACK;
      }
      else if((robotDirection == UP and directionToMove == HORIZONTALRIGHT) or (robotDirection == HORIZONTALRIGHT and directionToMove == DOWN)
           or (robotDirection == DOWN and directionToMove == HORIZONTALLEFT) or (robotDirection == HORIZONTALLEFT and directionToMove == UP)){
        return RIGHT;
      }
      else if((robotDirection == UP and directionToMove == HORIZONTALLEFT) or (robotDirection == HORIZONTALLEFT and directionToMove == DOWN)
           or (robotDirection == DOWN and directionToMove == HORIZONTALRIGHT) or (robotDirection == HORIZONTALRIGHT and directionToMove == UP)){
        return LEFT;
      }
      return FRONT;
    }

    char getRobotCommandForCurrentPosition(movement movementRelativeToRobot){
      char currentPosition = getCellForRobotPosition();
      switch(currentPosition){
        case line:
          return getCommandForLine(movementRelativeToRobot);
          break;
        case crossing:
          return getCommandForCrossing(movementRelativeToRobot);
          break;
        default:
          return getCommandForStartingPosition(movementRelativeToRobot);
          break;
      }
    }

    char getCellForRobotPosition(){
      return this->areaMap->getCell(this->robotPosition);
    }

    char getCommandForLine(movement movementRelativeToRobot){
      switch(movementRelativeToRobot){
        case FRONT:
          return followLineStraight;
          break;
        case RIGHT:
          return turnRight;
          break;
        case BACK:
          return turnBack;
          break;
        case LEFT:
          return turnLeft;
          break;
        default:
          return followLineStraight;
          break;
      }
    }

    char getCommandForCrossing(movement movementRelativeToRobot){
      switch(movementRelativeToRobot){
        case FRONT:
          return surpassCrossing;
          break;
        case RIGHT:
          return turnRight;
          break;
        case BACK:
          return turnBack;
          break;
        case LEFT:
          return turnLeft;
          break;
        default:
          return surpassCrossing;
          break;
      }
    }

    char getCommandForStartingPosition(movement movementRelativeToRobot){
      switch(movementRelativeToRobot){
        case FRONT:
          return followLineStraight;
          break;
        case RIGHT:
          return turnRight;
          break;
        case BACK:
          return turnBackOnCrossing;
          break;
        case LEFT:
          return turnLeft;
          break;
        default:
          return followLineStraight;
          break;
      }
    }

    void processTurn(movement turn){
      if(turn == RIGHT){
        processRightTurn();
      }
      else if(turn == LEFT){
        processLeftTurn();
      }
      else{
        processBackwardTurn();
      }
    }

    void processRightTurn(){
      switch(robotDirection){
        case UP:
          robotDirection = HORIZONTALRIGHT;
          break;
        case HORIZONTALRIGHT:
          robotDirection = DOWN;
          break;
        case DOWN:
          robotDirection = HORIZONTALLEFT;
          break;
        case HORIZONTALLEFT:
          robotDirection = UP;
          break;
        default:
          break;
      }
    }

    void processLeftTurn(){
      switch(robotDirection){
        case UP:
          robotDirection = HORIZONTALLEFT;
          break;
        case HORIZONTALRIGHT:
          robotDirection = UP;
          break;
        case DOWN:
          robotDirection = HORIZONTALRIGHT;
          break;
        case HORIZONTALLEFT:
          robotDirection = DOWN;
          break;
        default:
          break;
      }
    }

    void processBackwardTurn(){
      switch(robotDirection){
        case UP:
          robotDirection = DOWN;
          break;
        case HORIZONTALRIGHT:
          robotDirection = HORIZONTALLEFT;
          break;
        case DOWN:
          robotDirection = UP;
          break;
        case HORIZONTALLEFT:
          robotDirection = HORIZONTALRIGHT;
          break;
        default:
          break;
      }
    }

    void processForwardMovement(){
      this->pathMap->setCell(robotPosition, openSpace);
      switch(robotDirection){
        case UP:
          robotPosition.y--;
          break;
        case HORIZONTALRIGHT:
          robotPosition.x++;
          break;
        case DOWN:
          robotPosition.y++;
          break;
        case HORIZONTALLEFT:
          robotPosition.x--;
          break;
        default:
          break;
      }
    }
};
