#include <Adafruit_WS2801.h>

int dataPin = 9;
int clockPin = 10;
int t = 0;
int potPin = 2; 
int thermPin = 0;
int val = 0; 
byte TempMax = 30;   // Temperature at which the lamp is completely Red

Adafruit_WS2801 strip = Adafruit_WS2801(3, dataPin, clockPin);

void setup() {
  Serial.begin(9600);
  strip.begin();
  TempMax = 255/TempMax;
}

void loop() {
  //val = analogRead(potPin)/4;
  val = int(Thermistor(analogRead(thermPin)));
  Serial.print(val/TempMax);
  Serial.print(" degres -   ");
  if (digitalRead(tiltPin) == HIGH) {
    if (val > 128) {
      setColor(Color(val,255-val,0));
    } else {
      setColor(Color(val,255-val,128-val));
    }
  } else {
    setColor(Color(0,0,0));
  }
  delay(3000);
}

void setColor(uint32_t c) {
  int i;
 
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}
 
/* Helper functions */
// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte b, byte g)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

void FlashBluRed(int freq) {
  //while(t < duree){
    setColor(Color(255,255,0));
    delay(freq);
    setColor(Color(0,0,255));
    delay(freq);
    //t = t + freq + freq;
  //}
  //setColor(Color(0,0,0));
  //t = 0;
}


double Thermistor(int RawADC) {
 double Temp;
 Temp = log(10000.0*((1024.0/RawADC-1)));
//         =log(10000.0/(1024.0/RawADC-1)) // for pull-up configuration
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;            // Convert Kelvin to Celcius
 Temp = Temp * TempMax;    // Convert to 255 scale: 255 is 30 degrees
 return Temp;
}
