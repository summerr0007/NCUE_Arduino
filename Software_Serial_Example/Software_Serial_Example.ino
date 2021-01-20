/*
  Software serial multple serial test

  Receives from the hardware serial, sends to software serial.
  Receives from software serial, sends to hardware serial.

  The circuit:
   RX is digital pin 2 (connect to TX of other device)
   TX is digital pin 3 (connect to RX of other device)

  Note:
  Not all pins on the Mega and Mega 2560 support change interrupts,
  so only the following can be used for RX:
  10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

  Not all pins on the Leonardo support change interrupts,
  so only the following can be used for RX:
  8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

  created back in the mists of time
  modified 25 May 2012
  by Tom Igoe
  based on Mikal Hart's example

  This example code is in the public domain.

*/
#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10); // RX, TX
//char espBuffer[1024] = {0};
//
//int readCount = 0;
//long startTime = millis();

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }


  Serial.print("Goodnight moon!\r\n");

  mySerial.begin(9600);
  mySerial.print("AT+GMR\r\n");
}

void loop() // run over and over
{


//  if (mySerial.available()) {
//
//
//    while (millis() - startTime < 5000) { // Run for at least 5 seconds
//      // Check to make sure we don't exceed espBuffer's boundaries
//      if (mySerial.available() > readCount + sizeof espBuffer - 1)
//        break;
//      readCount += mySerial.readBytes(espBuffer + readCount, mySerial.available());
//    }
//
//
//    Serial.println(espBuffer);
//
//  }


//
    if (mySerial.available()){
//      Serial.print(mySerial.readString());
        Serial.write(mySerial.read());
//      delay(5);
    }
    
    if (Serial.available())
      mySerial.write(Serial.read());
}
