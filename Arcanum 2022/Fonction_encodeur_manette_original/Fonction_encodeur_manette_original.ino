#include <CrcLib.h>
#include <Encoder.h>



using namespace Crc;
int GLOBALE = 0;

#define Moteur_1 CRC_PWM_1

Encoder motor1Enco(TXD1, RXD1);

bool btnValue;

void moteur_1 (int POSITION, char V_AVANCER) {


  if ((motor1Enco.read() < POSITION) )
  {

    CrcLib::SetPwmOutput(Moteur_1, V_AVANCER);

  }
  else if  ((motor1Enco.read() > POSITION) )
  {
    int V_RECULER = -1 * V_AVANCER;
    CrcLib::SetPwmOutput(Moteur_1, V_RECULER);
  }
  else
  {
    CrcLib::SetPwmOutput(Moteur_1, 0);

    GLOBALE = POSITION;
  }

}

void setup() {


  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(Moteur_1);
  CrcLib::SetPwmOutput(Moteur_1, 0);

  Serial.begin(2000000);  //Open the serial Monitor at a 2000000 baud rate
;
  /*
    The rest of your setup code
    ...
  */
}
void loop() {


  CrcLib::Update();
 Serial.println(motor1Enco.read());




  btnValue = CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT);

  if (btnValue == HIGH) 
  
  {
    

    moteur_1 (2000, 150);


  }

 

  

  /*
    The rest of your looping code
    ...
  */
}
