#include <SoftwareSerial.h>

SoftwareSerial esp8266(11, 10);


void setup() {

  Serial.begin(9600);

  Serial.println("Arduino...OK");


  //
  //  esp8266.begin(115200);
  //
  //  esp8266.write("AT+UART_DEF=9600,8,1,0,0\r\n");
  //
  //  delay(1500);

  esp8266.begin(9600);

  Serial.println("ESP8266...OK");

}


void loop() {

  if (esp8266.available()) {

    Serial.write(esp8266.read());

  }

  if (Serial.available()) {

    esp8266.write(Serial.read());

  }

}
