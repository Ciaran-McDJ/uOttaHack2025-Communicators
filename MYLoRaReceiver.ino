// TODO - This file is copy of relay right now! Needs to be changed!!!!
#include <SPI.h>
#include <LoRa.h>

#define SCK 4 //8
#define MISO 5
#define MOSI 43
#define NSS 2 

#define RST 9
#define DIO0 6

//Specific to message format
#define HEADER_SIZE = 3

int ledPin = 8;

void setup() {
  SPI.begin(SCK, MISO, MOSI,NSS);
  LoRa.setPins(NSS, RST, DIO0);

  pinMode(ledPin, OUTPUT);

  delay(1000);

  digitalWrite(ledPin, HIGH); // flashing to signal that setup has happened
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // Keep checking if packet is received
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet, read it, then re-transmit it
    digitalWrite(ledPin, HIGH);
    Serial.print("Received packet '");
    // for (int i = 0; i < HEADER_SIZE; i++) {
    //   // do whatever with header byte at a time using (char)LoRa.read()
    // }

    // read packet into local memory

    char* message = (char*) malloc(sizeof(char)*packetSize);

    for (int i = 0; LoRa.available(); i++) {
      message[i] = (char)LoRa.read();
      // Serial.print(*next_char);
      // Serial.print((char)LoRa.read());
    }
    message[packetSize] = '\0'; //Note: this might not be neccessary, if it is I might want to get the LoRa to trasmit the Null terminator
    Serial.print(message);

    // print RSSI of packet
    //Serial.print(message);
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    
    // repeat message
    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();
    Serial.print("Relayed :)");
    digitalWrite(ledPin, LOW);
  }
}
