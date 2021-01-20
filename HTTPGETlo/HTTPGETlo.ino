#include "ESP8266.h"
#include <SoftwareSerial.h>
#define SSID        "No ASUS"
#define PASSWORD    "29887245s"
#define HOST_NAME    "www.google.com"
#define HOST_PORT   (80)
SoftwareSerial mySerial(11, 10); // 接收腳, 傳送腳
ESP8266 wifi(mySerial);

uint32_t len;
 
void setup() {
  //Init debug serial connection
  mySerial.begin(9600);
  Serial.begin(9600);
  Serial.println("Serial connection setup [DONE]");
 
  //Init WiFi
  while (true) {

    
    Serial.println("WiFi setup [START]");
    
    Serial.print("FW Version: ");
    Serial.println(wifi.getVersion());
    
    
    if (wifi.setOprToStation()) {
        Serial.print("to station ok\r\n");
    } else {
        Serial.print("to station err\r\n");
    }
    
    if (wifi.joinAP(SSID, PASSWORD)) {
      Serial.print("Join AP success\r\n");
      if (wifi.disableMUX()) {
        Serial.println("WiFi setup [DONE]");
        break;
      } else {
        Serial.println("WiFi setup [MUX ERR]");
      }
    } else {
      Serial.println("WiFi setup [AP ERR]");
    }
    //If init failed, wait 2s and try again
    delay(1000);
  }
}
 
void loop() {   
  uint8_t buffer[512] = {0};  
  char *request = "GET / HTTP/1.1\r\nHost: www.google.com.tw\r\nConnection: close\r\n\r\n";
//  char *request = "";
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    if (wifi.send((const uint8_t*)request, strlen(request))) {
      Serial.println("Send data [OK]");
    }
    else {
      Serial.println("Send data [ERR]");
    }
  }
  else {
    Serial.println("Create tcp [ERR]");
  }
  len = wifi.recv(buffer, sizeof(buffer), 10000);
  if (len > 0) {
    Serial.print("Received:[");
    for (uint32_t i = 0; i < len; i++) {
      Serial.print((char)buffer[i]);
    }
    Serial.print("]\r\n");
  }
 
  if (wifi.releaseTCP()) {
    Serial.println("Release tcp [OK]");
  } else {
    Serial.println("Release tcp [ERR]");
  }
  delay(5000);
}
