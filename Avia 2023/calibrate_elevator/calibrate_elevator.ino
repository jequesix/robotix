#include <CrcLib.h>
#include <Encoder.h>
using namespace Crc;

int pos = 0;
Encoder enco1(CRC_ENCO_A, CRC_ENCO_B);

void setup() {
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_10);
  Serial.begin(9600);
}

void loop() {
  CrcLib::Update();
  if (CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP)) {
    pos++;
  } else if (CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN)) {
    pos--;
  }
  enco1.write(pos);
  Serial.println(pos);
}
