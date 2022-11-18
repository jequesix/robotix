#include <CrcLib.h>
using namespace Crc;

int pinValue;
int num;

void setup() {
  CrcLib::Initialize();
  Serial.begin(9600);

}

void loop() {
  
  pinValue = CrcLib::GetAnalogInput(A0);
  if (pinValue >= 50) {
    Serial.println("Noir");
    }
  else if (pinValue <= 20) {
    Serial.println("Claire");
    }
  else {
    Serial.println("Sombre");
    }
   

   while (num < 2000) {
    CrcLib::Update();
    num++;
   }
   num = 0;
   
}
