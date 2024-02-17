#pragma once
#include <CrcLib.h>

class Binding {
  public:
    Binding(CrcUtility::BUTTON binding) : _bind(binding) {}
    
    bool wasClicked() {
      switch (CrcLib::ReadDigitalChannel(_bind)) {
        case 0:
          if (_pressed == false) {
            return false;
          } else {
            _pressed = false;
            return false;
          }
        case 1:
          if (_pressed == true) {
            return false;
          } else {
            _pressed = true;
            return true;
          }     
      }
    }

   bool isPressed() {
      return CrcLib::ReadDigitalChannel(_bind);
    }
    
  private:
    const CrcUtility::BUTTON _bind;
    bool _pressed = false; 
};
