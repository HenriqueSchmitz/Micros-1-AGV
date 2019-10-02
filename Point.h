struct Point {
  int x;
  int y;

  inline bool operator==(const Point& rhs) const{
    return x == rhs.x &&
           y == rhs.y;
  }
  
  inline bool operator!=(const Point& rhs) const{
    return x != rhs.x ||
           y != rhs.y;
  }
  
};

Point cordinatesToPoint(int x, int y){
  Point point;
  point.x = x;
  point.y = y;
  return point;

  

}
