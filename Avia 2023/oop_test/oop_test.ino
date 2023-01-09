#include <string.h>

class Hello {
  public:
    String x;

    Hello() {
      x = "allo";
    }

    void SayHello() {
      Serial.println(x);
    }
};



void setup() {
  Serial.begin(9600);
  Hello HelloObj;
  HelloObj.SayHello();

}

void loop() {
  // put your main code here, to run repeatedly:

}
