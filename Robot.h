#include <Servo.h>
#include <HCSR04.h>
const int rightMotorPin = 5;
const int leftMotorPin = 3;
const int rightLineSensorPin = 4;
const int leftLineSensorPin = 2;
const int ultrasonicTriggerPin = 7;
const int ultrasonicEchoPin = 8;

const double maximumDistanceFromObjectsInFront = 10;

class Robot {
  private:
    Servo rightMotor;
    Servo leftMotor;
    UltraSonicDistanceSensor* distanceSensor;

  public:
  Robot(){
    rightMotor.attach(rightMotorPin);
    leftMotor.attach(leftMotorPin);
    pinMode(rightLineSensorPin, INPUT);
    pinMode(leftLineSensorPin, INPUT);
    //UltraSonicDistanceSensor ultrasonicSensor(ultrasonicTriggerPin, ultrasonicEchoPin);
    distanceSensor = new UltraSonicDistanceSensor(ultrasonicTriggerPin, ultrasonicEchoPin);
    //distanceSensor = &ultrasonicSensor;
  }

  void enterDestination(){
    moveForward();
    delay(1000);
    halt();
  }

  boolean followLineUntillCrossing(){
    while( !isCrossingDetected() ){
      if( isRightSensorOnLine() ){
        turnRight();
      }
      else if( isLeftSensorOnLine() ){
        turnLeft();
      }
      else {
        moveForward();
      }
      if(isPathBlocked()){
        halt();
        return false;
      }
    }
    halt();
    return true;
  }

  void surpassCrossing(){
    while( !isCrossingDetected() ){
      moveForward();
    }
    while( isCrossingDetected() ){
      moveForward();
    }
    delay(100);
    while( isRightSensorOnLine() or isLeftSensorOnLine() ){
      if( isRightSensorOnLine() ){
        turnRight();
      }
      else if( isLeftSensorOnLine() ){
        turnLeft();
      }
    }
    halt();
  }

  void takeRightOnCrossing(){
    surpassCrossing();
    while( !isRightSensorOnLine() ){
      turnRight();
    }
    while( isRightSensorOnLine() ){
      turnRight();
    }
    halt();
  }

  void takeLeftOnCrossing(){
    surpassCrossing();
    while( !isLeftSensorOnLine() ){
      turnLeft();
    }
    while( isLeftSensorOnLine() ){
      turnLeft();
    }
    halt();
  }

  void turnAround(){
    while( !isRightSensorOnLine() ){
      turnRight();
    }
    while( isRightSensorOnLine() ){
      turnRight();
    }
    halt();
  }

  void moveForward(int velocity = 60){
    int rightMotorOutput = map(velocity, 0, 100, 90, 180);
    int leftMotorOutput = map(velocity, 0, 100, 90, 0);
    rightMotor.write(rightMotorOutput);
    leftMotor.write(leftMotorOutput);
  }

  void moveBackward(int velocity = 60){
    int rightMotorOutput = map(velocity, 0, 100, 90, 0);
    int leftMotorOutput = map(velocity, 0, 100, 90, 180);
    rightMotor.write(rightMotorOutput);
    leftMotor.write(leftMotorOutput);
  }

  void turnLeft(int velocity = 60){
    int rightMotorOutput = map(velocity, 0, 100, 90, 180);
    int leftMotorOutput = map((velocity*1.1), 0, 100, 90, 180);
    rightMotor.write(rightMotorOutput);
    leftMotor.write(leftMotorOutput);
  }

  void turnRight(int velocity = 60){
    int rightMotorOutput = map(velocity, 0, 100, 90, 0);
    int leftMotorOutput = map((velocity*1.1), 0, 100, 90, 0);
    rightMotor.write(rightMotorOutput);
    leftMotor.write(leftMotorOutput);
  }

  void halt(){
    rightMotor.write(90);
    leftMotor.write(90);
  }

  private:
  boolean isPathBlocked(){
    double distanceFromObjectsInFrontOfRobot = distanceSensor->measureDistanceCm();
    if( distanceFromObjectsInFrontOfRobot < maximumDistanceFromObjectsInFront and distanceFromObjectsInFrontOfRobot > 0 ){
      return true;
    }
    return false;
  }

  boolean isRightSensorOnLine(){
    return digitalRead(rightLineSensorPin);
  }

  boolean isLeftSensorOnLine(){
    return digitalRead(leftLineSensorPin);
  }

  boolean isCrossingDetected(){
    if( isRightSensorOnLine() and isLeftSensorOnLine() ){
      return true;
    }
    return false;
  }
  
};
