/*
ATmega328 Pinout

                                  +--, ,--+
             RESET--PCINT5---PC6--|1  U 28|--PC5--PCINT13--ADC5--19--A5--SCL
           0---RxD--PCINT16--PD0--|2    27|--PC4--PCINT12--ADC4--18--A4--SDA
           1---TxD--PCINT17--PD1--|3    26|--PC3--PCINT11--ADC3--17--A3
           2--INT0--PCINT18--PD2--|4    25|--PC2--PCINT10--ADC2--16--A2
OC2B--PWM--3--INT1--PCINT19--PD3--|5    24|--PC1--PCINT9---ADC1--15--A1
       T0--4---XCK--PCINT20--PD4--|6    23|--PC0--PCINT8---ADC0--14--A0
                             Vcc--|7    22|--GND
                             GND--|8    21|--AREF
       OSC1--XTAL1--PCINT6---PB6--|9    20|--AVcc
       OSC2--XTAL2--PCINT7---PB7--|10   19|--PB5--PCINT5--SCK---13
OC0B--PWM--5----T1--PCINT21--PD5--|11   18|--PB4--PCINT4--MISO--12
OC0A--PWM--6--AIN0--PCINT22--PD6--|12   17|--PB3--PCINT3--OC2A--11--PWM--MOSI
           7--AIN1--PCINT23--PD7--|13   16|--PB2--PCINT2--OC1B--10--PWM--SS
     ICP1--8--CLK0--PCINT0---PB0--|14   17|--PB1--PCINT1--OC1A---9--PWM
                                  +-------+

*/

#include <Arduino.h>
#include <SPI.h>
//define PINs
const int LCDEnabledPin   = 7;  //pin 13 on Atmega328 chip
const int LCDCmdDataPin   = 8;  //pin 14 on ATmega328 chip
const int LCDUnknownPin   = 6;  //pin 12 on ATmega328 chip
//const int SPIMOSIPin      = 11; //pin 17 on ATmega328 chip
//const int SPICLKPin       = 13; //pin 19 on ATmega328 chip
const byte DisplayInit[13]={0x75, 0x45, 0x05, 0x13, 0x91, 0x80, 0xC4, 0x81, 0x54, 0x34, 0x74, 0xF4, 0xF5};
const byte DisplayFrame1Row[6]={0x02, 0x05, 0x0D, 0x08, 0x00, 0x07};
const byte DisplayFrame2Row[5]=      {0x77, 0x8D, 0x08, 0x00, 0x07};
const byte DisplayFrame3Row[5]=      {0x77, 0x4D, 0x08, 0x00, 0x07};
const byte DisplayFrame4Row[5]=      {0x77, 0xCD, 0x08, 0x00, 0x07};
const byte DisplayFrameEnd[7]={0x77, 0xF5, 0xC7, 0x45, 0x05, 0x13, 0xC4};
byte muster=1;



void setup() {

  //some control PINs for the LCD Display
  pinMode(LCDEnabledPin, OUTPUT);
  pinMode(LCDCmdDataPin, OUTPUT);
  pinMode(LCDUnknownPin, OUTPUT);
  
  delay(100);  //delay for stable power supply


  //SPI_MasterInit();
  SPI.begin();
  // The LCD display in the Kyocera printer has ha SPI clock rate from nearly 3,5MHz.
  // I have the command sniffed as LSB first. So we must send this as LSB.
  // The clock signal pulse to negativ and the data is set after the falling edge and will read on the rising edge. This is SPI_MODE3.
  SPI.beginTransaction(SPISettings(3000000, LSBFIRST, SPI_MODE3));


  //set the unkown pin to low
  digitalWrite(LCDUnknownPin, LOW);

  //enable Display or ChipSelect
  digitalWrite(LCDEnabledPin, HIGH);


  digitalWrite(LCDCmdDataPin, LOW);   //prepare to sending commands


  //init the LCD Display  
  for (byte i = 0; i < 13; i++) {
    SPI.transfer(DisplayInit[i]);
  }
  

  delay(1);
}

void loop() {
  delay(100);  //refresh screen every 100ms
 
  //In this example we sending a 8 bit column (called "muster") over the hole screen (128 pixels in a row).
  //The display is 4 rows high (32 pixel). So we sending 4 rows with 8 bit columns.
  //The refresh rate on the Kyocera printer was 100ms.
  //In this example we increment the "muster" with eache frame. So we get a simple changing picture on the display.
  //With sending LSB first the upper pixel is bit 7 and bit 0 is the lowest pixel.


  digitalWrite(LCDCmdDataPin, LOW);   //prepare to sending commands
  for (byte i = 0; i < 6; i++) {
    //SPI_send_byte(DisplayFrame1Row[i]);
    SPI.transfer(DisplayFrame1Row[i]);
  }
  

  digitalWrite(LCDCmdDataPin, HIGH);   //prepare to sending data
  for (byte i = 0; i < 128; i++) {
    SPI.transfer(muster);
  }


  digitalWrite(LCDCmdDataPin, LOW);   //prepare to sending commands
  for (byte i = 0; i < 5; i++) {
    SPI.transfer(DisplayFrame2Row[i]);
  }


  digitalWrite(LCDCmdDataPin, HIGH);   //prepare to sending data
  for (byte i = 0; i < 128; i++) {
    SPI.transfer(muster);
  }


  digitalWrite(LCDCmdDataPin, LOW);   //prepare to sending commands
  for (byte i = 0; i < 5; i++) {
    SPI.transfer(DisplayFrame3Row[i]);
  }


  digitalWrite(LCDCmdDataPin, HIGH);   //prepare to sending data
  for (byte i = 0; i < 128; i++) {
    SPI.transfer(muster);
  }


  digitalWrite(LCDCmdDataPin, LOW);   //prepare to sending commands
  for (byte i = 0; i < 5; i++) {
    SPI.transfer(DisplayFrame4Row[i]);
  }


  digitalWrite(LCDCmdDataPin, HIGH);   //prepare to sending data
  for (byte i = 0; i < 128; i++) {
    SPI.transfer(muster);
  }


  digitalWrite(LCDCmdDataPin, LOW);   //prepare to sending commands
  for (byte i = 0; i < 7; i++) {
    SPI.transfer(DisplayFrameEnd[i]);
  }

  muster++;

}

