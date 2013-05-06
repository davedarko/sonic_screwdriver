#include "pitches.h"
#include "RCSwitch.h"
#include <IRremote.h>

RCSwitch mySwitch = RCSwitch();
IRsend irsend;
int toggle = 0;

unsigned long long OnOff = 0xc800f740cLL;
const int buttonUpPin = 2;     // the number of the pushbutton pin
const int buttonDownPin = 4;     // the number of the pushbutton pin
const int buttonEnterPin = 8;     // the number of the pushbutton pin
const int redPin =  3;      // the number of the LED pin
const int bluPin =  5;      // the number of the LED pin
const int grePin =  6;      // the number of the LED pin
const int irPin =  9;      // the number of the LED pin
const int send433 =  10;      // the number of the LED pin
const int speakerOut =  11;      // the number of the LED pin
int sensorPin = A0;            // select the input pin for the ldr
unsigned int sensorValue = 0;  // variable to store the value coming from the ldr

int melody[] = { NOTE_A5, NOTE_D6, NOTE_E6, NOTE_FS6, NOTE_E6,NOTE_D6, NOTE_E6, NOTE_FS6,NOTE_D6,NOTE_D6};
int noteDurations[] = { 8, 8, 8, 8,8,8,8,4,4,2 };

// variables will change:
int buttonUpState = 0;         // variable for reading the pushbutton status
int buttonDownState = 0;         // variable for reading the pushbutton status
int buttonEnterState = 0;         // variable for reading the pushbutton status

char * menu_loop[] = {
      "433", 
      "XBOX", 
      "THEREMIN", 
      "AUDIO", 
      "1812", 
      "ROT",
      "GRUEN",
      "BLAU",
      "MIX",
      "LUMOS",
      "ORANGE",
      "PURPLE", 
      "CYAN",
      "IR",
      "WARMWEISS",
      "UV"
};
int index = 0;
int menu_length = 0;
 int red=255;
  int blue=255;
   int green=255;
void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);
  menu_length = sizeof(menu_loop)/sizeof(char *);
  mySwitch.enableTransmit(send433);
  Serial.print(menu_length);
  pinMode(buttonUpPin, INPUT);
  pinMode(buttonDownPin, INPUT);
  pinMode(buttonEnterPin, INPUT);     
  pinMode(redPin, OUTPUT);
  pinMode(grePin, OUTPUT);
  pinMode(bluPin, OUTPUT);
}

void loop() {
// read the state of the pushbutton value:
  buttonUpState = digitalRead(buttonUpPin);
  buttonDownState = digitalRead(buttonDownPin);
  buttonEnterState = digitalRead(buttonEnterPin);

  if (buttonUpState == HIGH) {     
    if (menu_loop[index]=="ROT") {
      if (buttonDownState == HIGH) {
        if (red<254) red = (255 + red+1) % 255;
        delay(10);
      } 
      if (buttonEnterState == HIGH) {
        if(red>0) red = (255 + red-1) % 255;
        delay(10);
      } 
      analogWrite(redPin, red);
    }
    
    if (menu_loop[index]=="XBOX") {
      if (toggle == 0) {
        irsend.sendRC6(OnOff, 36);
      } else {
        irsend.sendRC6(OnOff ^ 0x8000, 36);
      }
      toggle = 1 - toggle;
    }
    
    if (menu_loop[index]=="AUDIO") {    
        sensorValue = analogRead(sensorPin);     
        analogWrite(speakerOut, sensorValue /4);
         Serial.println(sensorValue /4);
    }
    if (menu_loop[index]=="THEREMIN") {    
        sensorValue = analogRead(sensorPin);     
        tone(speakerOut, sensorValue);
    }
    if (menu_loop[index]=="433") {
      if (buttonDownState == HIGH) {
        mySwitch.switchOn("11000", "00100");
        delay(500);
      }
      if (buttonEnterState == HIGH) {
        mySwitch.switchOff("11000", "00100");
          delay(500);
      }
    }
    if (menu_loop[index]=="LUMOS") {    
        sensorValue = analogRead(sensorPin);
        analogWrite(redPin, 255 - (sensorValue / 4));
        analogWrite(grePin, 255 - (sensorValue / 4));
        analogWrite(bluPin, 255 - (sensorValue / 4));
    }
    if (menu_loop[index]=="MIX") {
        analogWrite(redPin, red);
        analogWrite(grePin, green);
        analogWrite(bluPin, blue);
    }
    if (menu_loop[index]=="IR") {
        analogWrite(irPin, 127);
    }
    if (menu_loop[index]=="GRUEN") {
      if (buttonDownState == HIGH) {
        if (green<254) green = (255 + green+1) % 255;
        delay(10);
      } 
      if (buttonEnterState == HIGH) {
        if(green>0) green = (255 + green-1) % 255;
        delay(10);
      } 
      analogWrite(grePin, green);
    }
    if (menu_loop[index]=="BLAU") {
      if (buttonDownState == HIGH) {
        if (blue<254) blue = (255 + blue+1) % 255;
        delay(10);
      } 
      if (buttonEnterState == HIGH) {
        if(blue>0) blue = (255 + blue-1) % 255;
        delay(10);
      } 
      analogWrite(bluPin, blue);
    }
    
    if (menu_loop[index]=="ORANGE") {
      analogWrite(redPin, 255);
      analogWrite(grePin, 64);
    }
    if (menu_loop[index]=="CYAN") {
      analogWrite(bluPin, 255);
      analogWrite(grePin, 255);
    }
    if (menu_loop[index]=="PURPLE") {
      analogWrite(redPin, 255);
      analogWrite(bluPin, 64);
    }
    if(menu_loop[index]=="1812") {
      analogWrite(speakerOut, 0);     
      for (int thisNote = 0; thisNote < 10; thisNote++) {
                  
        // to calculate the note duration, take one second 
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000/noteDurations[thisNote];
        tone(speakerOut, melody[thisNote],noteDuration);
                      
        // to distinguish the notes, set a minimum time between them.
        //the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(speakerOut);
      }
    }
  } 
  else {
    // turn LED off:
    digitalWrite(redPin, LOW);
    digitalWrite(grePin, LOW);
    digitalWrite(bluPin, LOW);
    digitalWrite(irPin, LOW);
    noTone(speakerOut);
    if (buttonDownState == HIGH) {
      index = (menu_length + index+1)%menu_length;
      Serial.write(menu_loop[index]);
      Serial.write("\n");
      delay(250);
    } 
    if (buttonEnterState == HIGH) {
      index = (menu_length + index-1)%menu_length;
      Serial.write(menu_loop[index]);
    Serial.write("\n");
      delay(250);
    }
  }
}
