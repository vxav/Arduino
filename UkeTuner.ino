// the blue led isn't used because not needed to save pins (only 6 PWM pins on nano). RGB used as RG.
// PWM pins = D3, D5, D6, D9, D10, D11
// G = 392Hz
// C = 261.6Hz
// E = 329.6 Hz
// A = 440 Hz

#include "SoftPWM.h"  // Using this library to turn a regular pin to pwm.

int Gnote = 392;
int Cnote = 262;
int Enote = 323;
int Anote = 440;
int MoreOrLess = 35; // Led lights up at more or less Hz of a chord frequency

// G string LEDs
int redPinG = 2;
int greenPinG = 3;
// C string LEDs
int redPinC = 4;
int greenPinC = 5;
// E string LEDs
int redPinE = 6;
int greenPinE = 7;
// A string LEDs
int redPinA = 8;
int greenPinA = 9;

// Simulated frequency << Will need changed by whatever works with the Piezzo inch'allah
int potPin = A2; // Test purpose to simulate frequency on 0-1023 scale
int sensorValue = 0; // simulated frequency variable

int redInt = 0;
int orangeInt = 0;
int yellowInt = 0;

void setup() {
  SoftPWMBegin();
  
  pinMode(redPinG, OUTPUT);
  pinMode(greenPinG, OUTPUT);
  pinMode(redPinC, OUTPUT);
  pinMode(greenPinC, OUTPUT);
  pinMode(redPinE, OUTPUT);
  pinMode(greenPinE, OUTPUT);
  pinMode(redPinA, OUTPUT);
  pinMode(greenPinA, OUTPUT);

// (Chord-MoreOrLess) --red--> (Chord-MoreOrLess+redInt) --orange--> (Chord-MoreOrLess+orangeInt) --yellow--> (Chord-MoreOrLess+yellowInt) --green--> (Chord+MoreOrLess-yellowInt) --> ...
// Example C chord with MoreOrLess = 25 Hz
// Off < 236Hz --red--> 242Hz --Orange--> 248Hz --Yellow--> 254Hz --green--> 267Hz --Yellow--> 273 --Orange--> 279Hz --Red--> 286Hz > Off

  redInt = MoreOrLess / 4;
  orangeInt = MoreOrLess / 4 * 2;
  yellowInt = MoreOrLess / 4 * 3;
  
  Serial.begin(9600);
}

void loop()
{
  int sensorValue = analogRead(potPin) ;

  setNote(Gnote, sensorValue, redPinG, greenPinG);
  setNote(Cnote, sensorValue, redPinC, greenPinC);
  setNote(Enote, sensorValue, redPinE, greenPinE);
  setNote(Anote, sensorValue, redPinA, greenPinA);
 
  Serial.println(sensorValue);
  delay(10);
}

void setColor(int red, int green, int RLed, int GLed)
{
  SoftPWMSet(RLed, red);
  SoftPWMSet(GLed, green);
}


void setNote(int note, int sensor, int RLed, int GLed)
{
  if (sensor >= (note - MoreOrLess) && sensor <= (note + MoreOrLess)) {
    if (sensor <= (note - MoreOrLess + redInt) || sensor >= (note + MoreOrLess - redInt)) {
      setColor(255, 0, RLed, GLed);
    } else if (sensor <= (note - MoreOrLess + orangeInt) || sensor >= (note + MoreOrLess - orangeInt)) {
      setColor(255, 92, RLed, GLed);
    } else if (sensor <= (note - MoreOrLess + yellowInt) || sensor >= (note + MoreOrLess - yellowInt)) {
      setColor(200, 200, RLed, GLed);
    } else if (sensor >= (note - MoreOrLess + yellowInt) || sensor <= (note + MoreOrLess - yellowInt)) {
      setColor(0, 255, RLed, GLed);
    }
  } else {
    setColor(0, 0, RLed, GLed);
  }
}
