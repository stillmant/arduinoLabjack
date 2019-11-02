#include <Arduino.h>
#include "ArduinoLabjack.h"
#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 178);
byte server[] = { 192, 168, 1, 214 }; // Labjack

float STREAM_SCANRATE_HZ = 5;
float STREAM_NUM_ADDRESSES = 2;
float STREAM_SAMPLES_PER_PACKET = 5;
float STREAM_AUTO_TARGET = 0x01;
float address1 = 0;
float address2 = 2;

EthernetClient client;
ModbusTCPClient modbusTCPClient(client);
EthernetClient stream;
ModbusTCPClient modbusStream(stream);
 
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
  if (!modbusStream.begin(server, 702)) {
      Serial.println("Modbus TCP Client failed to connect!");
  } else {
      Serial.println("Modbus connected");
  } 
  LJM_eWriteAddress(&modbusTCPClient, 4002, 3, STREAM_SCANRATE_HZ);
  LJM_eWriteAddress(&modbusTCPClient, 4004, 1, STREAM_NUM_ADDRESSES);
  LJM_eWriteAddress(&modbusTCPClient, 4006, 1, STREAM_SAMPLES_PER_PACKET);
  LJM_eWriteAddress(&modbusTCPClient, 4016, 1, STREAM_AUTO_TARGET);
  LJM_eWriteAddress(&modbusTCPClient, 4100, 1, address1);
  LJM_eWriteAddress(&modbusTCPClient, 4102, 1, address2);
  LJM_eWriteAddress(&modbusTCPClient, 4990, 1, (float)1);
  delay(3000);
  LJM_eWriteAddress(&modbusTCPClient, 4990, 1, (float)0);
}

void loop() {
  
//  LJM_eReadAddress(&modbusTCPClient, 4006, 0, &value);
//  Serial.println(value, HEX);
//  Serial.println(value);
  while(stream.available()){
    Serial.println(stream.read());
  }
  Serial.println("test");
  delay(100);
}
