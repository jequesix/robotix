#include <string.h>

class NumFlash {
  private:
    unsigned long lastEvent;
    bool lastState = false;
    int elapsedTime;

  public:
    String num;
    int interval;

    NumFlash(int flashNum, int flashInterval) {
      num = flashNum;
      interval = flashInterval;
    }

    void Flash() {
      elapsedTime = millis() - lastEvent;

      if (elapsedTime >= interval) {
        Serial.print(num);

        if (lastState == false) {
            Serial.println(" - on");
            lastState = true;
          } else {
            Serial.println(" - off");
            lastState = false;
          }

        lastEvent = millis();
      }
    }
};

NumFlash num1(1, 1000);
NumFlash num2(2, 500);

void setup() {
  Serial.begin(9600);
}

void loop() {
  num1.Flash();
  num2.Flash();
}
