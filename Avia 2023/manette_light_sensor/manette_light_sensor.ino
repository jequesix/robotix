#include <CrcLib.h>
using namespace Crc;

int buttonValue = 0;

void setup() {
  CrcLib::Initialize();
  Serial.begin(9600);
}

void loop() {
  CrcLib::Update();

  buttonValue = CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP);
  if (buttonValue == 0) {
    Serial.println("Aucun bouton");
  }
  else {
    Serial.println("Couleur haut pressé");
  }
}
