#include <Arduino.h>
#include "ArduinoLabjack.h"
#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 178);
byte server[] = { 192, 168, 1, 214 }; // Labjack

float reg = -1;
float value;

EthernetClient client;
ModbusTCPClient modbusTCPClient(client);

void setup() { 
  // Required because the WIZ820io requires a reset pulse and the Teensy doesn't have a reset pin (pin 9 is used instead)
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);    // begin reset the WIZ820io
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);  // de-select WIZ820io
  // Note: delay's weren't in the original code here: https://www.pjrc.com/store/wiz820_sd_adaptor.html
  // Delays were added because of reset pin specs here: http://wiznethome.cafe24.com/wp-content/uploads/wiznethome/Network%20Module/WIZ820io/Document/WIZ820io_User_Manual_V1.0.pdf
  delay(500);
  digitalWrite(9, HIGH);   // end reset pulse
  delay(500);
  
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  LJM_Open(&client, &modbusTCPClient, server);
}

void loop() {
  LJM_eWriteAddress(&modbusTCPClient, 55120, 1, reg);
  LJM_eReadAddress(&modbusTCPClient, 55120, 1, &value);
//  Serial.println(value, HEX);
  Serial.println(value);
  delay(2000);
}
