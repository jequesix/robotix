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
  }
  
  bool singleState() {
    timeSinceEvent = millis() - lastEvent;

    if (timeSinceEvent >= interval) {
      reset();
      return true;
    } else {
      return false;
    }
  }
  
  unsigned int cycleState() {
    if (singleState() == true) {
      currentState += 1;
      if (currentState > stateNum) {
        curentState = 1; }

      return currentState;
    }

  }
};
