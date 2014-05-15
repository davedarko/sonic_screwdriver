/* 
Author: Dave Plöger
V0.1 - 06.05.2013 First GITted

TODO or wouldn't it be nice too...
# make it blink/beep in bpms as metronome
# integrate accelerator script
# integrate power saving / sleep script
# use digital 2 as interrupt for sleep

For documentation of the hardware check the link
http://hackaday.io/project/972-Sonic-Screwdriver

This code is free to use. 
you can find this or an updated version here:
https://github.com/davedarko/sonic_screwdriver

This is where I got the code for the LDR from
http://www.hobbytronics.co.uk/arduino-tutorial8-nightlight

Comes in handy to switch 433MHz stuff.
http://code.google.com/p/rc-switch/

Great IR library, the basis for tv-b-gone, when I remember correct.
http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html

future implementation for EMF Detector
http://www.instructables.com/id/Arduino-EMF-Detector/

IR lib for codes for xbox and other goodies (like tvbgone)
https://github.com/shirriff/Arduino-IRremote
http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html

*/

#include <IRremote.h>
#include <RCSwitch.h>
#include <math.h>

// ATTENTION - the pinout may change frequently
// next will probably be the up and down buttons, they will share an analog pin soon
// the ENTERBUTTON will move to pin 2 for INTERRUPT of sleep code
// 4,8,13 will be the charlie plexed menu bargraph (coming up with some fancy POV stuff)
// digital pins or output
const int buttonUpPin    =  2;     // UP Button, soon enter button
const int redPin         =  3;     // ~ RED LED PIN
const int buttonDownPin  =  4;     // DOWN BUTTON, soon charlie_led1
const int grePin         =  5;     // ~ GREEN LED PIN
const int bluPin         =  6;     // ~ BLUE LED PIN
const int send433        =  7;     // 433 EMITTER PIN
const int buttonEnterPin =  8;     // ENTER BUTTON, soon charlie_led1
const int irPin          =  9;     // ~ IR LED PIN pwm needed for IRremote library
const int wwPin          = 10;     // ~ WW PWM PIN
const int speakerOut     = 11;     // ~ SPEAKER PIN
const int uvPin          = 12;     // uvPIN
//const int free         = 13;     // free PIN, soon charlie_led1

// analog pins or input
const int fader       = A0;     // Fader
const int ldrPin      = A1;     // LDR PIN
//const int free      = A2;     // soon Button up and down
//const int free      = A3;     // Spoiler: MIC IN or EMF or BPW34 for gamma-detection
//const int i2c_sda   = A4;     // SDA IIC Pin
//const int i2c_scl   = A5;     // SCL IIC PIN
//const int free      = A6;     // free PIN, no idea
//const int send433   = A7;     // this is physically joined with D7, so not soon to be taken

int outputs[] = {
  redPin, 
  grePin,
  bluPin,
  wwPin
};

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
int ww    = 127;

char * menu_loop[] = {
      "DAZZ",

      "RED",
      "GREEN",
      "BLUE",
      "MIX",
      "LUMOS",
      "WW",
      "UV",
      "IR",

      "XBOX", 
      
      "433", 
      "433ANARCHY",
      
      "THEREMIN", 
      "AUDIO"

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
    
    // displaying basic color and setting the brightness for the mixed color
    if (state=="WW") {
      if (buttonUpState == HIGH) {
        if (ww<254) ww = (255 + ww+1) % 255;
        delay(10);
      } 
      if (buttonDownState == HIGH) {
        if(ww>0) ww = (255 + ww-1) % 255;
        delay(10);
      } 
      analogWrite(wwPin, ww);
    }
    
    if (state=="IR") {
        analogWrite(irPin, ir);
    }
    
    if (state=="UV") {
        digitalWrite(uvPin, HIGH);
    }

//  http://hackaday.com/2009/09/28/open-source-weapon-makes-you-puke/    
    if (state=="DAZZ") {
      long randNumber = random(256);
      long randIndexNumber = random(4);
    
      analogWrite(outputs[randIndexNumber], randNumber);
      
      // has to be around 10Hz == 100ms 
      // for a clomplete new color set this has to be circled 4 times,
      // so delay is 25 
      delay(25); 
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
        lights_on();
        sensorValue = analogRead(ldrPin);     
        analogWrite(speakerOut, sensorValue /4);
         Serial.println(sensorValue /4);
         lights_off();
    }
    
    if (state=="THEREMIN") {    
        lights_on();
        sensorValue = analogRead(ldrPin); 
        int thisPitch = map(sensorValue, 0 , 1024, 120, 1500);

  // play the pitch:
    // http://forum.arduino.cc/index.php/topic,106043.msg959970.html#msg959970
        toneWorkaround(speakerOut, thisPitch, 10);
        lights_off();
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
        analogWrite(redPin, 255);
        analogWrite(grePin, 255);
        analogWrite(bluPin, 255);
        analogWrite(wwPin, 255);
    }

  } 
  else {
    // turn LED off:
    lights_off();
    digitalWrite(speakerOut, LOW);
    
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
  analogWrite(wwPin, ww);
}

void lights_off() {
  digitalWrite(redPin, LOW);
  digitalWrite(grePin, LOW);
  digitalWrite(bluPin, LOW);
  digitalWrite(wwPin, LOW);
    digitalWrite(irPin, LOW);
      digitalWrite(uvPin, LOW);
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
void toneWorkaround(byte tonePin, int frequency, int duration) {
  int period = 1000000L / frequency;
  int pulse = period / 2;
  for (long i = 0; i < duration * 1000L; i += period) {
    digitalWrite(tonePin, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(tonePin, LOW);
    delayMicroseconds(pulse);
  }
}
