int getCharArrayLength(char charArray[]){
  return sizeof(charArray)/sizeof(char);
}

void stopExecution(){
  Serial.println("Execution Stopped!!");
  while(true){
    delay(100);
  }
}
