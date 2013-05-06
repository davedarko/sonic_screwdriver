/* 
Author: Dave Plöger
V0.1 - 06.05.2013 First GITted

This code is free to use. 
you can find this or an updated version here:
https://github.com/davedarko/sonic_screwdriver

This is where I got the code for the LDR from
http://www.hobbytronics.co.uk/arduino-tutorial8-nightlight

The tone tutorial helped with the 1812 part
http://arduino.cc/en/Tutorial/tone

Comes in handy to switch 433MHz stuff.
http://code.google.com/p/rc-switch/

Great IR library, the basis for tv-b-gone, when I remember correct.
http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html

future implementation for EMF Detector
http://www.instructables.com/id/Arduino-EMF-Detector/
*/

#include "pitches.h"
// http://arduino.cc/en/Tutorial/tone

#include "RCSwitch.h"
// http://code.google.com/p/rc-switch/

#include <IRremote.h>
// http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html


// digital pins or output
const int buttonUpPin    =  2;     // UP Button
const int redPin         =  3;     // RED LED PIN
const int buttonDownPin  =  4;     // DOWN BUTTON
const int bluPin         =  5;     // BLUE LED PIN
const int grePin         =  6;     // GREEN LED PIN
const int send433        =  7;     // 433 EMITTER PIN
const int buttonEnterPin =  8;     // ENTER BUTTON
const int irPin          =  9;     // IR LED PIN
//const int free         = 10;     // free PWM PIN
const int speakerOut     = 11;     // SPEAKER PIN
//const int free         = 12;     // free PIN
//const int free         = 13;     // free PIN

// analog pins or input
const int ldrPin      = A0;     // LDR PIN
//const int free      = A1;     // free PIN
//const int free      = A2;     // free PIN
//const int free      = A3;     // free PIN
//const int free      = A4;     // free PIN
//const int free      = A5;     // free PIN
//const int free      = A6;     // free PIN

// variables will change:
int buttonUpState        = 0;      // variable for reading the pushbutton status
int buttonDownState      = 0;      // variable for reading the pushbutton status
int buttonEnterState     = 0;      // variable for reading the pushbutton status
unsigned int sensorValue = 0;  // variable to store the value coming from the ldr

RCSwitch mySwitch = RCSwitch();
IRsend irsend;
int toggle = 0;
unsigned long long OnOff = 0xc800f740cLL;

// start intensity for rgb colors
int red   = 127;
int blue  = 127;
int green = 127;

int melody[] = { NOTE_A5, NOTE_D6, NOTE_E6, NOTE_FS6, NOTE_E6,NOTE_D6, NOTE_E6, NOTE_FS6,NOTE_D6,NOTE_D6};
int noteDurations[] = { 8, 8, 8, 8,8,8,8,4,4,2 };

char * menu_loop[] = {
      "433", 
      "XBOX", 
      "THEREMIN", 
      "AUDIO", 
      "1812", 
      "RED",
      "GREEN",
      "BLUE",
      "MIX",
      "LUMOS",
      "ORANGE",
      "PURPLE", 
      "CYAN",
      "IR",
      "WHITE",
      "UV"
};
int index = 0;
int menu_length = 0;

void setup() {
  // start serial port at 9600 bps
  Serial.begin(9600);
  /* too lazy to count by myself */
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
    if (menu_loop[index]=="RED") {
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
        sensorValue = analogRead(ldrPin);     
        analogWrite(speakerOut, sensorValue /4);
         Serial.println(sensorValue /4);
    }
    if (menu_loop[index]=="THEREMIN") {    
        sensorValue = analogRead(ldrPin);     
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
        sensorValue = analogRead(ldrPin);
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
    if (menu_loop[index]=="GREEN") {
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
    if (menu_loop[index]=="BLUE") {
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
      /*borrowed by arduino tone samples */
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
