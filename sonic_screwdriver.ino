/* 
Author: Dave Plöger
V0.1 - 06.05.2013 First GITted

TODO or wouldn't it be nice too...
# make it blink/beep in bpms as metronome

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

#include <math.h>

// digital pins or output
const int buttonUpPin    =  2;     // UP Button
const int redPin         =  3;     // ~ RED LED PIN
const int buttonDownPin  =  4;     // DOWN BUTTON
const int grePin         =  5;     // ~ GREEN LED PIN
const int bluPin         =  6;     // ~ BLUE LED PIN
const int send433        =  7;     // 433 EMITTER PIN
const int buttonEnterPin =  8;     // ENTER BUTTON
const int irPin          =  9;     // ~ IR LED PIN
const int wwPin          = 10;     // ~ WW PWM PIN
const int speakerOut     = 11;     // ~ SPEAKER PIN
const int uvPin          = 12;     // free PIN
//const int free         = 13;     // free PIN

// analog pins or input
const int fader       = A0;     // Fader
const int ldrPin      = A1;     // LDR PIN
//const int free      = A2;     // Spoiler: EMF Detector
//const int free      = A3;     // Spoiler: MIC IN
//const int free      = A4;     // SDA IIC Pin
//const int free      = A5;     // SCL IIC PIN
//const int free      = A6;     // free PIN
//const int free      = A7;     // free PIN

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
int ir    = 127;
int red   = 255;
int blue  = 100;
int green = 45;

//int melody[] = { NOTE_A5, NOTE_D6, NOTE_E6, NOTE_FS6, NOTE_E6,NOTE_D6, NOTE_E6, NOTE_FS6,NOTE_D6,NOTE_D6};
//int noteDurations[] = { 8, 8, 8, 8,8,8,8,4,4,2 };

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
      "IR",
      "WHITE",
      "UV",
      "SUPERHIGH",
      "433ANARCHY"
};
int index = 0;
int menu_length = 0;
char * state = "";

void setup() {
  // start serial port at 9600 bps
  Serial.begin(9600);
  /* too lazy to count by myself */
  menu_length = sizeof(menu_loop)/sizeof(char *);
  mySwitch.enableTransmit(send433);
  Serial.print(menu_length);
  Serial.write("\n");
  Serial.write(menu_loop[index]);
  Serial.write("\n");
  pinMode(buttonUpPin, INPUT);
  pinMode(buttonDownPin, INPUT);
  pinMode(buttonEnterPin, INPUT);     
  pinMode(redPin, OUTPUT);
  pinMode(grePin, OUTPUT);
  pinMode(bluPin, OUTPUT);
  pinMode(irPin, OUTPUT);
  pinMode(wwPin, OUTPUT);
  pinMode(uvPin, OUTPUT);
  pinMode(speakerOut, OUTPUT);
}

void loop() {
// read the state of the pushbutton value:
  buttonUpState = digitalRead(buttonUpPin);
  buttonDownState = digitalRead(buttonDownPin);
  buttonEnterState = digitalRead(buttonEnterPin);
  state = menu_loop[index];
  if (buttonEnterState == HIGH) {
  
    // displaying basic color and setting the brightness for the mixed color
    if (state=="RED") {
      if (buttonUpState == HIGH) {
        if (red<254) red = (255 + red+1) % 255;
        delay(10);
      } 
      if (buttonDownState == HIGH) {
        if(red>0) red = (255 + red-1) % 255;
        delay(10);
      } 
      analogWrite(redPin, red);
    }
    
    // displaying basic color and setting the brightness for the mixed color    
    if (state=="GREEN") {
      if (buttonUpState == HIGH) {
        if (green<254) green = (255 + green+1) % 255;
        delay(10);
      } 
      if (buttonDownState == HIGH) {
        if(green>0) green = (255 + green-1) % 255;
        delay(10);
      } 
      analogWrite(grePin, green);
    }
    
    // displaying basic color and setting the brightness for the mixed color
    if (state=="BLUE") {
      if (buttonUpState == HIGH) {
        if (blue<254) blue = (255 + blue+1) % 255;
        delay(10);
      } 
      if (buttonDownState == HIGH) {
        if(blue>0) blue = (255 + blue-1) % 255;
        delay(10);
      } 
      analogWrite(bluPin, blue);
    }
    
    if (state=="IR") {
        analogWrite(irPin, ir);
    }
    
    if (state=="XBOX") {
      lights_on();
      if (toggle == 0) {
        irsend.sendRC6(OnOff, 36);
      } else {
        irsend.sendRC6(OnOff ^ 0x8000, 36);
      }
      toggle = 1 - toggle;
    }
    
    if (state=="AUDIO") {    
        sensorValue = analogRead(ldrPin);     
        analogWrite(speakerOut, sensorValue /4);
         Serial.println(sensorValue /4);
    }
    
    if (state=="THEREMIN") {    
        sensorValue = analogRead(ldrPin);     
        tone(speakerOut, sensorValue);
    }
    
    if (state=="433") {
      if (buttonUpState == HIGH) {
        lights_on();
        mySwitch.switchOn("11000", "00100");
        lights_off();
        delay(250);
      }
      if (buttonDownState == HIGH) {
        lights_on();
        mySwitch.switchOff("11000", "00100");
        lights_off();
        delay(250);
      }
    }
    
    if (state=="433ANARCHY") {
      if (buttonUpState == HIGH) {
        lights_on();          
        for (int i=0;i<32;i++) {
          char remote[6];
          byte2str5(remote, i);
          mySwitch.switchOn(remote, "10000");
          mySwitch.switchOn(remote, "01000");
          mySwitch.switchOn(remote, "00100");
          mySwitch.switchOn(remote, "00010");
          mySwitch.switchOn(remote, "00001");
          mySwitch.switchOn(remote, "00000");
          lights_off();
          delay(250);
        }
      }
      if (buttonDownState == HIGH) {
        lights_on();
        for (int i=0;i<32;i++) {
          char remote[6];
          byte2str5(remote, i);
          mySwitch.switchOff(remote, "10000");
          mySwitch.switchOff(remote, "01000");
          mySwitch.switchOff(remote, "00100");
          mySwitch.switchOff(remote, "00010");
          mySwitch.switchOff(remote, "00001");
          mySwitch.switchOff(remote, "00000");
        }
        lights_off();
        delay(250);
      }
    }
    
    if (state=="MIX") {
        lights_on();
    }

    if (state=="LUMOS") {    
        sensorValue = analogRead(ldrPin);
        analogWrite(redPin, 255 - (sensorValue / 4));
        analogWrite(grePin, 255 - (sensorValue / 4));
        analogWrite(bluPin, 255 - (sensorValue / 4));
    }
    if (state=="SUPERHIGH") {
    tone(speakerOut, 8000);
    }
    if(state=="1812") {
      lights_on();
      analogWrite(speakerOut, 0);
      // borrowed by arduino tone samples 
      for (int thisNote = 0; thisNote < 10; thisNote++) {
        buttonEnterState = digitalRead(buttonEnterPin);
        if (buttonEnterState == LOW) break;
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
      lights_off();
    }
  } 
  else {
    // turn LED off:
    lights_off();
    
    noTone(speakerOut);
    if (buttonUpState == HIGH) {
      index = (menu_length + index+1)%menu_length;
      Serial.write(menu_loop[index]);
      Serial.write("\n");
      delay(250);
    } 
    if (buttonDownState == HIGH) {
      index = (menu_length + index-1)%menu_length;
      Serial.write(menu_loop[index]);
    Serial.write("\n");
      delay(250);
    }
  }
}

void lights_on() {
  analogWrite(redPin, red);
  analogWrite(grePin, green);
  analogWrite(bluPin, blue);
}

void lights_off() {
  digitalWrite(redPin, LOW);
  digitalWrite(grePin, LOW);
  digitalWrite(bluPin, LOW);
}

int pow_int (int x, int y) {
  if (y==0) return 1;
  if (y==1) return x;
  if (y>1) return x * pow_int(x,y-1);
  return 0;
}

void byte2str5 (char* localString, int conv) {
  char thearray[6] ="00000";
  int dummy = conv;
  
  for (int i = 4; i>=0; i--) {
    int powi = pow_int(2,i);
    int temp = dummy - powi;
    if( temp >= 0) {
      thearray[4-i] = '1';
      dummy=temp;
    } else {
      thearray[4-i] = '0';
    }
  }
  strcpy(localString, thearray);
}
