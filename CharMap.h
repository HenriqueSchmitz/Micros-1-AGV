#include "Point.h"

class CharMap{
  private:
    int rows;
    int columns;
    int absoluteSize;
    char* mapContents;

  public:
    CharMap(int rows, int columns, char inputContents[]){
      this->rows = rows;
      this->columns = columns;
      this->absoluteSize = rows * columns;
      mapContents = (char*)malloc(this->absoluteSize * sizeof(char));
      Point cellPosition;
      for(cellPosition.y = 0; cellPosition.y < rows; cellPosition.y++){
        for(cellPosition.x = 0; cellPosition.x < columns; cellPosition.x++){
          int absolutePosition = this->translateToAbsolutePosition(cellPosition);
          mapContents[absolutePosition] = inputContents[absolutePosition];
        }
      }
    }

    ~CharMap(){
      free(mapContents);
    }

    int getNumberOfRows(){
      return this->rows;
    }

    int getNumberOfColumns(){
      return this->columns;
    }

    Point findFirstCellContaining(char content){
      for(int cell = 0; cell < absoluteSize; cell++){
        if(mapContents[cell] == content){
          return translateToRelativePosition(cell);
        }
      }
    }

    char getCell(Point cellPosition){
      if(cellPosition.x >= columns or cellPosition.y >= rows){
        return ' ';
      }
      int absolutePosition = translateToAbsolutePosition(cellPosition);
      char content = mapContents[absolutePosition];
      return content;
    }

    char getCellByCoordinates(int x, int y){
      Point cell = cordinatesToPoint(x, y);
      return getCell(cell);
    }

    void setCell(Point cellPosition, char value){
      int absolutePosition = translateToAbsolutePosition(cellPosition);
      mapContents[absolutePosition] = value;
    }

    CharMap* copy(){
      return new CharMap(this->rows, this->columns, mapContents);
    }

  private:
    int translateToAbsolutePosition(Point relativePositon){
      return (relativePositon.y * this->columns + relativePositon.x);
    }

    Point translateToRelativePosition(int absolutePosition){
      Point relativePosition;
      relativePosition.y = absolutePosition / this->columns;
      relativePosition.x = absolutePosition % this->columns;
      return relativePosition;
    }
  
};
