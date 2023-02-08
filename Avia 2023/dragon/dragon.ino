#include <CrcLib.h>
#include "time.h"
using namespace Crc;


// side servo variables
struct sdeSrv {
  const int pin = CRC_PWM_1;
  const ANALOG bind = ANALOG::JOYSTICK2_X;
  const int uprLmt = 180;
  const int lwrLmt = 0;
  int bindPos;
  int pos;
  int dir;
};
struct sdeSrv sde;
Time sdeTime(30);

// rotation servo variables
struct rtnSrv {
  const int pin = CRC_PWM_2;
  const ANALOG bind = ANALOG::JOYSTICK2_Y;
  const int uprLmt = 180;
  const int lwrLmt = 0;
  int bindPos;
  int pos;
  int dir; 
};
struct rtnSrv rtn;
Time rtnTime(30);

// mouth servo variables
struct mthSrv {
  const int pin = CRC_PWM_3;
  const ANALOG bind = ANALOG::JOYSTICK1_Y;
  const int uprLmt = 180;
  const int lwrLmt = 0;
  int bindPos;
  int pos;
};
struct mthSrv mth;
Time mthTime(30);

// wings motor variables
struct wngMtr {
  const int pin = CRC_PWM_4;
  const ANALOG upBind = ANALOG::GACHETTE_R;
  const ANALOG dnBind = ANALOG::GACHETTE_L;
  const int uprLmt = 180;
  const int lwrLmt = 0;
  const int speed = 50;
  int upBindPos;
  int dnBindPos;
};
struct wngMtr wng;

// mode (automatic/manual)
//const BUTTON modBind = BUTTON::COLORS_DOWN;
//bool mode;


void manCtrl() {
  sde.bindPos = CrcLib::ReadAnalogChannel(sde.bind);
  rtn.bindPos = CrcLib::ReadAnalogChannel(rtn.bind);
  mth.bindPos = CrcLib::ReadAnalogChannel(mth.bind);
  wng.upBindPos = CrcLib::ReadAnalogChannel(wng.upBind);
  wng.dnBindPos = CrcLib::ReadAnalogChannel(wng.dnBind);

  if (sdeTime.singleState()) {
    if (sde.bindPos >= 10 && sde.pos < sde.uprLmt) {
      sde.pos++;
    } else if (sde.bindPos <= -10 && sde.pos > sde.lwrLmt) {
      sde.pos--;
    }
  }

  if (rtnTime.singleState()) {
    if (rtn.bindPos >= 10 && rtn.pos < rtn.uprLmt) {
      rtn.pos++;
    } else if (rtn.bindPos <= -10 && rtn.pos > sde.lwrLmt) {
      rtn.pos--;
    }
  }

  if (mthTime.singleState()) {
    if (mth.bindPos >= 10 && mth.pos < mth.uprLmt) {
      mth.pos++;
    } else if (mth.bindPos <= -10 && mth.pos > sde.lwrLmt) {
      mth.pos--;
    }
  }

  if (wng.upBindPos < 10 && wng.dnBindPos < 10) {
    CrcLib::SetPwmOutput(wng.pin, 0);
  } else if (wng.upBindPos >= 10) {
    CrcLib::SetPwmOutput(wng.pin, wng.speed);
  } else if (wng.dnBindPos >= 10) {
    CrcLib::SetPwmOutput(wng.pin, wng.speed*(-1));
  }
}


void setup() {
  CrcLib::Initialize();

  CrcLib::InitializePwmOutput(sde.pin);
  CrcLib::InitializePwmOutput(rtn.pin);
  CrcLib::InitializePwmOutput(mth.pin);
  CrcLib::InitializePwmOutput(wng.pin);
}

void loop() {
  CrcLib::Update();

//  if (CrcLib::ReadDigitalChannel(modBind)) {
//    if (!mode) {
//      mode = true;
//    } else {
//      mode = false;
//    }
//  }

  if (CrcLib::IsCommValid()) {
    manCtrl();
  }
}
