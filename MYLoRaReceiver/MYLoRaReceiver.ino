//Needs to be changed!!!!
#define LGFX_TTGO_TDISPLAY    
#define LGFX_AUTODETECT             // TTGO T-Display

#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp> 

// SENDER CODE
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

#define ledPin 32

static LGFX lcd;
static LGFX_Sprite sprite(&lcd);

const int knob1 = 39;
int knobv1 = 0;

// const int button1 = 15;
// const int button2 = 13;
const int button3 = 37;

bool buttons[] = {0,0,0};


const uint32_t black = 0x000000;
const uint32_t white = 0xFFFFFF;
const uint32_t keycolor = 0xFE7713;
const uint32_t accentcolor = 0xFFA84A;

char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
char textopts[] = "abcdefghijklmnopqrstuvwxyz>";
char numbers[] = "0123456789";

int letterindex = 0;

int MAINSTATE = 0;

String message = "";

char* receivedMessage;



int mapAnalogRange(int in, int maxout){
  float x = in/4095.0 * maxout;
  int output = (int)x;
  return output;
}


void setup() {
  SPI.begin(SCK, MISO, MOSI, NSS); // custom pins for SPI (because defaults are hidden)
  LoRa.setPins(NSS, RST, DIO0); // custom pins for LoRa (because defaults are hidden)

  lcd.init();
  lcd.setRotation(1);
  lcd.setBrightness(128);
  lcd.setColorDepth(24);
  Serial.begin(115200);

  // pinMode(button1, INPUT);
  // pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(ledPin, OUTPUT);

  sprite.createSprite(lcd.width(), lcd.height());

  if (!LoRa.begin(433E6)) {
    // Serial.println("Starting LoRa failed!");
    while (1);
  }
}




void loop() {
  //sprite.fillScreen(black);

  //START OF ADDED CODE TO LOOK AT MESSAGE
  // check if packet is received
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet, read it, then re-transmit it
    digitalWrite(ledPin, HIGH);
    Serial.print("Received packet '");
    // for (int i = 0; i < HEADER_SIZE; i++) {
    //   // do whatever with header byte at a time using (char)LoRa.read()
    // }

    // read packet into local memory

    receivedMessage = (char*) malloc(sizeof(char)*packetSize); //TODONOW - might mess up for next message if new alloc

    for (int i = 0; LoRa.available(); i++) {
      receivedMessage[i] = (char)LoRa.read();
      // Serial.print(*next_char);
      // Serial.print((char)LoRa.read());
    }
    receivedMessage[packetSize] = '\0'; //Note: this might not be neccessary, if it is I might want to get the LoRa to trasmit the Null terminator
    Serial.print(receivedMessage);

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

  }

  //END OF ADDED CODE TO LOOK AT MESSAGE


  sprite.setTextFont(4);

  sprite.setTextSize(1);
  sprite.setTextColor(keycolor, black);   // Set text color to green and padding to back

  //check inpits
  // knobv1 = analogRead(knob1); //TODONOW - is this ok here?
  // letterindex = mapAnalogRange(knobv1, 26);
  // char letter = (textopts[letterindex]);

  sprite.startWrite();
  // if (digitalRead(button1) == HIGH){ // BACK
  //   delay(1000); // TODO: this is here to avoid double inputs, but it messes with receiving messages. fix later
  //   //sprite.fillRect(20 , 110, 20, 20, white);
  //   if (sizeof(message) > 0){
  //     message.remove(sizeof(message)-1);
  //   }
  // }
  // if (digitalRead(button2) == HIGH){ // SELECT
  //   delay(1000); // TODO: this is here to avoid double inputs, but it messes with receiving messages. fix later
  //   if (letter == '>') {
  //     char* char_message = (char*) malloc(sizeof(message)); //TODO - need to dealloc when done with message! (in a few spots)
  //     strcpy(char_message, message.c_str());
  //     sendMessage(createMessage(char_message));
  //     String newstr = "";
  //     message = newstr;
  //   }
  //   else{
  //     message += letter;
  //   }
  //   drawtextbox(letter);
  // }
  if (digitalRead(button3) == HIGH){ // VIEW MSG
    delay(1000); // TODO: this is here to avoid double inputs, but it messes with receiving messages. fix later
    //sprite.fillRect(80, 110, 20, 20, white);
    lookAtMessage(); //TODONOW
    digitalWrite(ledPin, LOW);
  }
  sprite.endWrite();




  //drawopts(textopts, -letter, letter);
  //drawletterfinder(letter);
  
  //Serial.println(&(letters[letter]));

  delay(20);
  sprite.pushSprite(0,0);
}

void drawtextbox(char letter){
  int posy = 45;
  
  sprite.startWrite();
  sprite.fillRect(0,40,240,95, black); //refresh botthom screen
  //sprite.fillRect(50,  110, 20, 20, white);
  Serial.println(message);
  sprite.drawRect(5,posy, 240-10, 70, white);
  sprite.setTextColor(white, black);
  sprite.drawString(message, 15, posy + 7);
  sprite.endWrite();
}

void drawletterfinder(char letter){
  int posy = 11;
  char l = letter;
  
  //screen dimensions 240x135
  sprite.startWrite();
  sprite.fillRect(0,0,240,40, black); //clear upper screen
  sprite.drawRect(5,5, 240-10, 35, accentcolor);

  //draw main letter
  sprite.setTextColor(keycolor, black); 
  sprite.drawString(&l,115,posy);
  sprite.endWrite();

  // for (int i = 0; i < 5: i++){
  //   lcd.dra
  // }
}


//ADDED FUNCTION TO LOOK AT RECEIVED MESSAGE
void lookAtMessage() {
  sprite.fillScreen(black);
  sprite.drawRect(5,5, 240-10, 135-10, keycolor);
  sprite.drawString(receivedMessage, 10, 10, 2);

}


// MORE SENDER FUNCTIONS - UNUSED FOR RECEIVER (for now)
// void sendMessage(char* theMessage) {
//   Serial.println("Sending packet");

//   // send packet
//   LoRa.beginPacket();
//   LoRa.print(theMessage);
//   LoRa.endPacket();
//   Serial.println(theMessage);
// }




// char* createMessage(char* message) {
//   // char* theMessage;
//   // message.toCharArray(theMessage, sizeof(message));
//   char* mesWithHead = addSizeHeader(message);
//   return mesWithHead;
// }

#define HeaderSize 3 // This is number of decimal digits in header corresponding to size of message

// char* addSizeHeader(char* theMessage) {
//   // """This adds size of message in decimal (excluiding size of decimal chars)""" // TODO - this would probably be better in binary
//   // Serial.print("\n");
//   // Serial.print(message);
//   size_t len = strlen(theMessage); //todo try int
//   // Serial.print("debug ");
//   // Serial.print(len);
//   // Serial.print("\n");

//   static char* mesWithHead = (char*) malloc(sizeof(char)*(len+HeaderSize+1)); //+1 is so it has space for null terminator, not sure if its necessary
//   copy_string(convertTo3Chars(len), mesWithHead);
//   copy_string(theMessage, mesWithHead+3);
//   // sprintf(mes_whead, "%d", convertTo3Chars(len));
//   // Serial.print(mes_whead);

//   return mesWithHead;
// }

// char* convertTo3Chars(int num) {
//   static char char_nums[3];
//   sprintf(char_nums, "%d", num);

//   int amount_to_shift = 3 - strlen(char_nums);

//   for (int x = 0; x < amount_to_shift; x++) {
//     char_nums[2] = char_nums[1];
//     char_nums[1] = char_nums[0];
//     char_nums[0] = '0';
//   }

//   // Serial.print("\namount to shift:");
//   // Serial.print(amount_to_shift);
//   // Serial.print(" char_nums: ");
//   // Serial.print(char_nums);

//   return char_nums;
// }

void copy_string(char *src, char *dest) {
    while (*src) {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';  // Null terminate the destination string
}


