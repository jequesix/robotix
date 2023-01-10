class Time {
  private:
    unsigned long lastEvent;
    bool lastState = false;
    unsigned int timeSinceEvent;

  public:
  unsigned int interval;

  Time(int execInterval) {
    interval = execInterval;
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
  
  bool doubleState() {
    timeSinceEvent = millis() - lastEvent;

    if (timeSinceEvent >= interval) {
      lastEvent = millis();
      
      if (lastState == false) {
        return true;
      } else {
        return false;
      }
    }
  }
};