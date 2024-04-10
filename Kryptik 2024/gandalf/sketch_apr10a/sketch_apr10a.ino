#include <CrcLib.h>

int8_t randNum[3];
unsigned long lastTime;
unsigned long lastTime2;
#define headPin CRC_PWM_10
#define eyeLeftPin CRC_PWM_5
#define eyeRightPin CRC_PWM_6

void setup() {
  CrcLib::Initialize();
  Serial.begin(2000000);
  
  CrcLib::InitializePwmOutput(headPin);
  CrcLib::InitializePwmOutput(eyeLeftPin);
  CrcLib::InitializePwmOutput(eyeRightPin);

  lastTime = 0;
  lastTime2 = 0;
}

void loop() {
  CrcLib::Update();

  if ((millis() - lastTime) >= 5000) {
    lastTime = millis();

    for (byte i=0; i<3; i++) {
      randNum[i] = random(256);
    }
    
    CrcLib::SetPwmOutput(headPin, randNum[0]);
    CrcLib::SetPwmOutput(eyeLeftPin, -128);
    CrcLib::SetPwmOutput(eyeRightPin, 0);
  }

  if (millis()-lastTime2 
}
