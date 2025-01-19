#include <SPI.h>
#include <LoRa.h>
// defining SPI pins
#define SCK 25
#define MISO 27
#define MOSI 26
#define NSS 33
// defining LoRa pins
#define RST 2
#define DIO0 17

#define buttonPin 37
#define ledPin 32

int counter = 0;

void setup() {
  SPI.begin(SCK, MISO, MOSI, NSS); // custom pins for SPI (because defaults are hidden)
  LoRa.setPins(NSS, RST, DIO0); // custom pins for LoRa (because defaults are hidden)
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  delay(1000);
  Serial.begin(115200);
  while (!Serial);

  // Serial.println("LoRa Sender");
  digitalWrite(ledPin, HIGH); // flashing to signal that setup has happened
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  
  if (!LoRa.begin(433E6)) {
    // Serial.println("Starting LoRa failed!");
    while (1);
  }
  // Serial.println("End");
}

void loop() {
  if (digitalRead(buttonPin) == HIGH) {
    buttonPress();
  }
  delay(1000);
}


void buttonPress() {
  // Serial.println(test_mes);
  sendMessage(createMessage());
}

void sendMessage(char* message) {
  digitalWrite(ledPin, HIGH);  // turn the LED on (HIGH is the voltage level)

  Serial.println("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
  Serial.println(message);

  counter++;

  digitalWrite(ledPin, LOW);
}




char* createMessage() {
  // """We will change this function soon"""
  char* message = "Hello!";

  char* mesWithHead = addSizeHeader(message);

  return mesWithHead;
}

#define HeaderSize 3 // This is number of decimal digits in header corresponding to size of message

char* addSizeHeader(char* message) {
  // """This adds size of message in decimal (excluiding size of decimal chars)""" // TODO - this would probably be better in binary
  // Serial.print("\n");
  // Serial.print(message);
  size_t len = strlen(message); //todo try int
  // Serial.print("debug ");
  // Serial.print(len);
  // Serial.print("\n");

  static char* mesWithHead = (char*) malloc(sizeof(char)*(len+HeaderSize+1)); //+1 is so it has space for null terminator, not sure if its necessary
  copy_string(convertTo3Chars(len), mesWithHead);
  copy_string(message, mesWithHead+3);
  // sprintf(mes_whead, "%d", convertTo3Chars(len));
  // Serial.print(mes_whead);

  return mesWithHead;
}

char* convertTo3Chars(int num) {
  static char char_nums[3];
  sprintf(char_nums, "%d", num);

  int amount_to_shift = 3 - strlen(char_nums);

  for (int x = 0; x < amount_to_shift; x++) {
    char_nums[2] = char_nums[1];
    char_nums[1] = char_nums[0];
    char_nums[0] = '0';
  }

  // Serial.print("\namount to shift:");
  // Serial.print(amount_to_shift);
  // Serial.print(" char_nums: ");
  // Serial.print(char_nums);

  return char_nums;
}

void copy_string(char *src, char *dest) {
    while (*src) {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';  // Null terminate the destination string
}