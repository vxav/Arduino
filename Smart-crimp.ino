/*
Smart-Crimp
xavier.avrillier@gmail.com
Enhance fingerboard sessions

--- 2016/08/27 ---
Version 1
 - Select program at start with button and accept with tilt switch
 - Display Program number, exercice number and repetition number
 - Cancel by pressing the button any time
 - Use of WS2801 LED strip
*/

#include <Adafruit_WS2801.h>

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 13, 4, 5, 6, 7);

int dataPin = 9;
int clockPin = 10;

Adafruit_WS2801 strip = Adafruit_WS2801(3, dataPin, clockPin);

const byte butPin = 2;
const byte tiltPin = 3;

byte program = 1;
int j = 0;
int t = 0;
int f = 1;
int k = 1;
int hang = 2000;      //temps de suspension
int rest = 1000;      //temps de repos entre chaque suspension
byte nbrep = 1;        //nombre de series de suspensions
long exorest = 12000; //temps de repos entre 2 exercises
byte nbexo = 1;        //nombre d'exercies

void setup() {
  strip.begin();
  
  Serial.begin(9600);
  lcd.begin(16, 2);
  
 // pinMode(redPin, OUTPUT);
  //pinMode(grePin, OUTPUT);
  //pinMode(bluPin, OUTPUT);

  pinMode(tiltPin, INPUT);
  pinMode(butPin, INPUT);
  
  lcd.print("Smart-Crimp"); 
  while (j<16){
   lcd.setCursor(j,1); 
   lcd.print("*");
    delay(150);
    j++;
  }
  lcd.clear();
  lcd.print("Hang longer !");
  lcd.setCursor(0,1);
  lcd.print("Crimp harder !");
  delay(3000);
}

void loop() {
  setColor(Color(10,10,10));
  start:

  ChoseProgram();

  switch (program) {
    case 1:{
      nbexo = 5;
      nbrep = 6;
      hang = 7000;
      rest = 3000;
      exorest = 150000;
    }break;
    case 2:{
      nbexo = 5;
      nbrep = 6;
      hang = 15000;
      rest = 10000;
      exorest = 240000;
    }break;
    case 3:{
      nbexo = 3;
      nbrep = 10;
      hang = 30000;
      rest = 30000;
      exorest = 300000;
    }break;
  }

  lcd.clear();
  lcd.print("Got it");
  
  while(digitalRead(tiltPin) == HIGH) {delay(1);}
  lcd.clear();
  lcd.print("Ready program:");
  lcd.print(program);
  lcd.setCursor(0, 1);
  lcd.print("Hang to start");
  
  int j = 0;
  int t = 0;
  int f = 1;
  int k = 1;
  setColor(Color(0,0,128));

  while(digitalRead(tiltPin) == LOW) {
   delay(1);
   if(digitalRead(butPin) == HIGH){goto fail;}
  }
  
  lcd.clear();
  lcd.print("Go at red !");
  
  while(j<2550) {
    j = j+10;
    setColor(Color((j/10),(j/20),128-(j/20)));
    delay(10);
    if(digitalRead(tiltPin) == LOW){goto fail;}
  }
  
  j = 0;

  while(f<=nbexo) {
    while(k<=nbrep) {
      printRep(f,k);
          setColor(Color(255,0,0));    
          while(j<hang) {  //SUSPENSION ON
            if(digitalRead(tiltPin) == HIGH) {j++;}
            delay(1);
            if(digitalRead(butPin) == HIGH){goto fail;}
          }
          j = 0;
          setColor(Color(0,255,0));
          while(j<rest) {

            if(digitalRead(tiltPin) == LOW) {j++;}
            delay(1);
            if(digitalRead(butPin) == HIGH){goto fail;}
          }
          j = 0;   //SUSPENSION OFF
      k = k + 1;
    }


    setColor(Color(0,0,255));
    while(j < (exorest - 10000)) {   
      delay(5);
      j = j+5;
      if(digitalRead(butPin) == HIGH){goto fail;}
    }
    FlashBluRed(10000,1000);
    j = 0;
    k = 1;
    f++;
  }

  for (k=0; k <= 3000; k = k+1000) {
    setColor(Color(0,0,255));
    delay(333);
    setColor(Color(255,255,255));
    delay(333);
    setColor(Color(255,0,0));
    delay(334);
  }

  goto start;
  fail:

    lcd.clear();
    lcd.print("Program");
    lcd.setCursor(0,1);
    lcd.print("canceled");
  
    for (k=0; k <= 2000; k = k+400){
    setColor(Color(255,255,255));
    delay(200);
    setColor(Color(255,0,0));
    delay(200);
  }
}






/* Choix du programme */

void ChoseProgram(){

  lcd.clear();
  lcd.print("Program "); 
  lcd.setCursor(0,1);
  lcd.print("Hang to chose"); 
  int previousBut = digitalRead(butPin);
  while(digitalRead(tiltPin) == LOW) {
    
    if(digitalRead(butPin) == HIGH){
      if (previousBut == LOW) {program++;}
      delay(50);
      previousBut = digitalRead(butPin);
    }
    /*if(program == 4){
      lcd.setCursor(9, 0);
      lcd.print("custom");
    }
    else*/ if(program == 4){ //remplacer par > et decommenter au dessus pour ajouter custom
      program=1;
      lcd.setCursor(8, 0);
      lcd.print("       ");
    }
    else{
      lcd.setCursor(8, 0);
      lcd.print(program);
    }
  }
}


/* Utility functions */
/*
void setColor(byte red, byte green, byte blue) {
  analogWrite(redPin, red);
  analogWrite(grePin, green);
  analogWrite(bluPin, blue);  
}
*/

void setColor(uint32_t c) {
  int i;
 
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}
 
/* Helper functions */
// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

void FlashBluRed(int duree, int freq) {
  while(t < duree){
    setColor(Color(255,255,0));
    delay(freq);
    setColor(Color(0,0,255));
    delay(freq);
    t = t + freq + freq;
  }
  setColor(Color(0,0,0));
  t = 0;
}

void printRep(byte x,byte y) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Program:");
  lcd.print(program);

  lcd.setCursor(0, 1);
  lcd.print("exo:");
  lcd.print(x);
  lcd.print("   reps:");
  lcd.print(y); 
}
