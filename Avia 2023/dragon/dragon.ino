#include <CrcLib.h>
#include "time.h"
using namespace Crc;


// side servo variables
struct sdeSrv {
  const int pin = CRC_PWM_9;
  const ANALOG bind = ANALOG::JOYSTICK2_X;
  const int uprLmt = 127;
  const int lwrLmt = -128;
  int bindPos;
  int pos;
  int dir;
};
struct sdeSrv sde;
Time sdeTime(30);

// rotation servo variables
struct rtnSrv {
  const int pin = CRC_PWM_10;
  const ANALOG bind = ANALOG::JOYSTICK2_Y;
  const int uprLmt = 127;
  const int lwrLmt = -128;
  int bindPos;
  int pos;
  int dir; 
};
struct rtnSrv rtn;
Time rtnTime(30);

// mouth servo variables
struct mthSrv {
  const int pin = CRC_PWM_11;
  const ANALOG bind = ANALOG::JOYSTICK1_Y;
  const int uprLmt = 127;
  const int lwrLmt = -128;
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
  const int speed = 50;
  int upBindPos;
  int dnBindPos;
};
struct wngMtr wng;


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
    CrcLib::SetPwmOutput(sde.pin, sde.pos);
  }

  if (rtnTime.singleState()) {
    if (rtn.bindPos >= 10 && rtn.pos < rtn.uprLmt) {
      rtn.pos++;
    } else if (rtn.bindPos <= -10 && rtn.pos > sde.lwrLmt) {
      rtn.pos--;
    }
    CrcLib::SetPwmOutput(rtn.pin, rtn.pos);
  }

  if (mthTime.singleState()) {
    if (mth.bindPos >= 10 && mth.pos < mth.uprLmt) {
      mth.pos++;
    } else if (mth.bindPos <= -10 && mth.pos > sde.lwrLmt) {
      mth.pos--;
    }
    CrcLib::SetPwmOutput(mth.pin, mth.pos);
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

  CrcLib::InitializePwmOutput(sde.pin, 500, 2500);
  CrcLib::InitializePwmOutput(rtn.pin, 500, 2500);
  CrcLib::InitializePwmOutput(mth.pin, 500, 2500);
  CrcLib::InitializePwmOutput(wng.pin);

  CrcLib::SetPwmOutput(sde.pin, 0);
  CrcLib::SetPwmOutput(rtn.pin, 0);
  CrcLib::SetPwmOutput(mth.pin, 0);
}


void loop() {
  CrcLib::Update();

  if (CrcLib::IsCommValid()) {
    manCtrl();
  }
}
