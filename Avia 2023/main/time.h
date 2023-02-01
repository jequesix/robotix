class Time {
  private:
    unsigned long lastEvent;
    unsigned int timeSinceEvent;
    unsigned int interval;

    unsigned int currentState = 1;
    unsigned int stateNum;
    
  public:
  

  Time(int execInterval, int stateNumber = 0) : interval(execInterval), stateNum(stateNumber) {}

  void reset() {
    lastEvent = millis();
    currentState = 1;
  }
  
  bool singleState() {
    timeSinceEvent = millis() - lastEvent;

    if (timeSinceEvent >= interval) {
      lastEvent = millis();
      return true;
    } else {
      return false;
    }
  }
  
  unsigned int cycleState() {
    if (singleState() == true) {
      currentState++;
      if (currentState > stateNum) {
        curentState = 1; }
    }

    return currentState;

  }
};
