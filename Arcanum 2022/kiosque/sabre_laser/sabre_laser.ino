#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define NUM_LEDS 60
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
  
#define BUTTON 2
byte selectedEffect = 0;

void changeEffect() {
  if (digitalRead (BUTTON) == HIGH) {
    selectedEffect++;
    EEPROM.put(0, selectedEffect);
    asm volatile ("  jmp 0");
  }
}

// *******************************
// ** Functions pour les effets**
// *******************************


void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    setPixel(i, red, green, blue);
    showStrip();
    delay(SpeedDelay);
  }
}

void colorWipeReverse(byte red, byte green, byte blue, int SpeedDelay) {

  for (uint16_t i = 60; i <= NUM_LEDS; i--) {
    setPixel(i, red, green, blue);
    showStrip();
    delay(SpeedDelay);

  }
}


// ***********************************************
// ** Functions pour FastLed/NeoPixel Functions **
// ***********************************************

// Applique le changement de couleur des DELs
void showStrip() {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif

}

// détemine la couleur de la DEL (invisible)
void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif

}

// détermine tous les LEDs à sa couleur et appliqué (visible)
void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

// ************************
// ** programme de base **
// ***********************


void setup()
{
  strip.begin();
  strip.show(); // ferme tout les DELs
  pinMode(2, INPUT_PULLUP); // pull-up pour button
  attachInterrupt (digitalPinToInterrupt (BUTTON), changeEffect, CHANGE); // pressed
  delay(50);
}


void loop() {
  EEPROM.get(0, selectedEffect);

  if (selectedEffect > 8) {
    selectedEffect = 0;
    EEPROM.put(0, 0);
  }

  switch (selectedEffect) {

    case 0  : {
        setAll(0, 0, 0);

        break;

      }

    case 1  : {

        colorWipe(255, 0, 0, 30);
        selectedEffect = 2;
        EEPROM.put(0, selectedEffect);
        break;

      }

    case 2  : {

        setAll(0xff, 0x00, 0x00);
        break;
      }

    case 3  : {

        setAll(0xff, 0x00, 0x00);
        colorWipeReverse(0x00, 0x00, 0x00, 30);
        selectedEffect = 4;
        EEPROM.put(0, selectedEffect);
        break;
      }

    case 4 : {
        setAll(0x00, 0x00, 0x00);
        break;
      }

    case 5 : {
        setAll(0, 0, 0);
        colorWipe(25, 213, 34, 30);
        selectedEffect = 6;
        EEPROM.put(0, selectedEffect);
        break;
      }

    case 6 : {
        setAll(25, 213, 34);
        break;
      }

    case 7 : {
        setAll(25, 213, 34);
        colorWipeReverse(0, 0, 0, 30);
        selectedEffect = 8;
        EEPROM.put(0, selectedEffect);
        break;
      }


    case 8 : {

        setAll(0, 0, 0);
      }



  }
}
