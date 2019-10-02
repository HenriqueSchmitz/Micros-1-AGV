#include "UtilityFunctions.h"
#include "MapNavigator.h"

const int maxNumberOfMapColumns = 15;
const int maxNumberOfMapRows = 15;

class MapGenerator{
  private:
    int numberOfColumns;
    int numberOfRows;
    int numberOfMapColumns;
    int numberOfMapRows;
    char* identificators;
    char areaMap[maxNumberOfMapRows][maxNumberOfMapColumns];
    
  public: 
  MapGenerator(int numberOfColumns, int numberOfRows, char identificators[]){
    this->numberOfColumns = numberOfColumns;
    this->numberOfRows = numberOfRows;
    this->identificators = identificators;
    this->generateMap();
  }

  void printMap(){
    Serial.println();
    for(int row = 0; row < numberOfMapRows; row++){
      for(int column = 0; column < numberOfMapColumns; column++){
        Serial.print(areaMap[row][column]);
      }
      Serial.println();
    }
  }

  private:
  void assessInputData(){
    int expectedNumberOfIdentificators = numberOfColumns*2 + numberOfRows*2;
    int receivedNumberOfIdentificators = getCharArrayLength(identificators);
    if(expectedNumberOfIdentificators != receivedNumberOfIdentificators){
      Serial.println("Incorrect input parameters for MapNavigator instantiation!!!");
      Serial.println("Incorrect Number of Identificators!!!");
      Serial.print("Given: ");
      Serial.print(receivedNumberOfIdentificators);
      Serial.print(", Expected: ");
      Serial.println(expectedNumberOfIdentificators);
      Serial.println();
        for(char identificator = 0; identificator<expectedNumberOfIdentificators; identificator++){
        Serial.print(identificators[identificator]);
      }
      Serial.println();
      stopExecution();
    }
    int numberOfMapColumns = numberOfColumns*2 + 1;
    if(numberOfMapColumns > maxNumberOfMapColumns){
      Serial.println("Incorrect input parameters for MapNavigator instantiation!!!");
      Serial.println("Too many columns generated!!!");
      stopExecution();
    }
    int numberOfMapRows = numberOfRows*2 + 1;
    if(numberOfMapColumns > maxNumberOfMapRows){
      Serial.println("Incorrect input parameters for MapNavigator instantiation!!!");
      Serial.println("Too many rows generated!!!");
      stopExecution();
    }
  }

  void generateMap(){
    numberOfMapColumns = numberOfColumns*2 + 1;
    numberOfMapRows = numberOfRows*2 + 1;
    for(int row = 0; row < maxNumberOfMapRows; row++){
      for(int column = 0; column < maxNumberOfMapColumns; column++){
        areaMap[row][column] = openSpace;
      }
    }
    for(int identificator = 0; identificator < numberOfColumns; identificator++){
      int column = 1 + identificator*2;
      areaMap[0][column] = identificators[identificator];
    }
    for(int identificator = 0; identificator < numberOfRows; identificator++){
      int row = 1 + identificator*2;
      areaMap[row][numberOfMapColumns - 1] = identificators[identificator + numberOfColumns];
    }
    for(int identificator = 0; identificator < numberOfColumns; identificator++){
      int column = numberOfMapColumns - 2 - identificator*2;
      areaMap[numberOfMapRows - 1][column] = identificators[identificator + numberOfColumns + numberOfRows];
    }
    for(int identificator = 0; identificator < numberOfRows; identificator++){
      int row = numberOfMapRows - 2 - identificator*2;
      areaMap[row][0] = identificators[identificator + numberOfColumns*2 + numberOfRows];
    }
  }
}
