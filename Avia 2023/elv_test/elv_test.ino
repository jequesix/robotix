#include <CrcLib.h>
using namespace Crc;

Time time(100);

void setup() {
  CrcLib::Initialize();

}

void loop() {
  CrcLib::Update();
    if (CrcLib::IsCommValid()) {
      if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP) {
        
      }
    }
}
