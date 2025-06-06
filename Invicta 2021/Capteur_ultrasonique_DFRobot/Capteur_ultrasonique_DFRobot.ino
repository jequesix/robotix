#include <CrcLib.h> 

// # Editor     : roker
// # Date       : 15.11.2018

// # Product name: URM V5.0 ultrasonic sensor
// # Product SKU : SEN0304
// # Version     : 1.0

#include <Wire.h>

unsigned char txbuf[10] = {0};
unsigned char rxbuf[10] = {0};


typedef enum {

  SLAVEADDR_INDEX = 0,
  PID_INDEX,
  VERSION_INDEX ,

  DIST_H_INDEX,
  DIST_L_INDEX,

  TEMP_H_INDEX,
  TEMP_L_INDEX,

  CFG_INDEX,
  CMD_INDEX,
  REG_NUM

} regindexTypedef;

#define    MEASURE_MODE_PASSIVE    (0x00)
#define    MEASURE_RANG_500        (0x20)
#define    CMD_DISTANCE_MEASURE    (0x01)


unsigned char addr0 = 0x11;




void setup() {

  //*CapteurSonic
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600); // join i2c bus (address optional for master)
  txbuf[0] =  (MEASURE_MODE_PASSIVE | MEASURE_RANG_500);//the measurement mode is set to passive mode, measurement range is set to 500CM.
  i2cWriteBytes(addr0, CFG_INDEX , &txbuf[0], 1 );//
  delay(100);
}

void i2cWriteBytes(unsigned char addr_t, unsigned char Reg , unsigned char *pdata, unsigned char datalen )
{
  Wire.beginTransmission(addr_t); // transmit to device #8
  Wire.write(Reg);              // sends one byte

  for (uint8_t i = 0; i < datalen; i++) {
    Wire.write(*pdata);
    pdata++;
  }

  Wire.endTransmission();    // stop transmitting
}

void i2cReadBytes(unsigned char addr_t, unsigned char Reg , unsigned char Num )
{
  unsigned char i = 0;
  Wire.beginTransmission(addr_t); // transmit to device #8
  Wire.write(Reg);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  Wire.requestFrom(addr_t, Num);
  while (Wire.available())   // slave may send less than requested
  {
    rxbuf[i] = Wire.read();
    i++;
  }

}


unsigned char i = 0, x = 0;
void loop() {
  int16_t  dist, temp;
  txbuf[0] = CMD_DISTANCE_MEASURE;

  i2cWriteBytes(addr0, CMD_INDEX , &txbuf[0], 1 );//write register, send ranging command
  delay(100);

  i2cReadBytes(addr0, DIST_H_INDEX , 2 );//read distance register
  dist = ((uint16_t)rxbuf[0] << 8) + rxbuf[1];

  i2cReadBytes(addr0, TEMP_H_INDEX , 2 );//read temperature register
  temp = ((uint16_t)rxbuf[0] << 8) + rxbuf[1];

  Serial.print(dist, DEC);
  Serial.print("cm");
  Serial.print("-----");

  Serial.print((float)temp / 10, 1);
  Serial.println("°C");

}

//ordre couleurs : noir, rouge vert et bleu en sortie PWM
