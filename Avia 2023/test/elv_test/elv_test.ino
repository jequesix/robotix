#include <CrcLib.h>
#include <Encoder.h>
#include "time.h"
#include "binding.h"
using namespace Crc;

Time cptrTime(1000);
Time time(100);
Binding strtButton(BUTTON::COLORS_UP);
bool strtCptr;
bool cptr;

int pin1 = CRC_ENCO_A;
int pin2 = CRC_ENCO_B;
Encoder elv(pin1, pin2);

int pos1;
int pos2;

void setup() {
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_6);
  CrcLib::SetPwmOutput(CRC_PWM_6, 0);
  Serial.begin(9600);
}

void loop() {
  CrcLib::Update();

  if (!cptr) {
    if (strtButton.wasClicked()) {
      CrcLib::SetPwmOutput(CRC_PWM_6, 50);
      strtCptr = true;
      cptrTime.reset();
    }

    if (strtCptr && cptrTime.singleState()) {
      cptr = true;
      strtCptr = false;
      pos1 = elv.read();
      time.reset();
    }
  }

  if (cptr && time.singleState()) {
    pos2 = elv.read();
    Serial.println(pos2-pos1);
    CrcLib::SetPwmOutput(CRC_PWM_6, 20);
    cptr = false;
  }
}
