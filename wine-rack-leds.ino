#include <Adafruit_WS2801.h>   // Link to WS2801 library: https://github.com/adafruit/Adafruit-WS2801-Library

/// LED strip stuff
int dataPin = 9;     // LED strip data
int clockPin = 10;   // LED strip clock
byte numPixel = 12;   // Number of LEDs in the LED strip < THIS VARIABLE IS NOT DYNAMIC

Adafruit_WS2801 strip = Adafruit_WS2801(numPixel, dataPin, clockPin);

int red, green, blue;

/// Potentiometer stuff to change light Kelvin
int potPin = A3;
int potVal = 0;
int mapDown = 0;
int mapKelvin = 0;
// WS2801 are bad at mapping high kelvin values so I use it to reduce the reds (slightly yolo)
int coefKelvin = 0;

//////////////////////////////////
// Initialisation
//////////////////////////////////
void setup() {
  Serial.begin(9600);
  strip.begin();
}


//////////////////////////////////
// Program
//////////////////////////////////
void loop() {
  potVal = analogRead(potPin);
  // Map value down to eliminate flakiness of potentiometer
  mapDown = map(potVal, 0, 745, 0, 25);
  // Map value back up to Kelvin range 1500 to 6500)
  mapKelvin = map(mapDown, 0, 25, 1500, 6500);

  kelvinToRGB(mapKelvin, &red, &green, &blue);

  setColor(Color(red,green,blue));
  // Serial.println(mapKelvin);
  delay(250);  
}

//////////////////////////////////
// Kelvin to RGB
//////////////////////////////////
void kelvinToRGB(int kelvin, int *red, int *green, int *blue) {
    double temp = kelvin / 100.0; // Convert kelvin to double for calculations
    // Thanks ChatGPT
    // Calculate red component
    *red = 255;
    if (temp <= 66) {
        coefKelvin = 400;
    } else if (temp <= 50) {
        coefKelvin = 300;
    } else if (temp <= 40) {
        coefKelvin = 130;
    } else {
        double redCalc = 329.698727446 * pow(temp - 60, -0.1332047592);
        *red = constrain(redCalc, 0, 255);
    }
    *red = *red * ((coefKelvin - temp) / coefKelvin);


    // Calculate green component
    if (temp <= 66) {
        double greenCalc = 99.4708025861 * log(temp) - 161.1195681661;
        *green = constrain(greenCalc, 0, 255);
    } else {
        double greenCalc = 288.1221695283 * pow(temp - 60, -0.0755148492);
        *green = constrain(greenCalc, 0, 255);
    }

    // Calculate blue component
    if (temp >= 66) {
        *blue = 255;
    } else if (temp <= 19) {
        *blue = 0;
    } else {
        double blueCalc = 138.5177312231 * log(temp - 10) - 305.0447927307;
        *blue = constrain(blueCalc, 0, 255);
    }
}

//////////////////////////////////
// Control LED strip
//////////////////////////////////
void setColor(uint32_t c) {
  int i;
 
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}
 

//////////////////////////////////
// Create a 24 bit color value from R,G,B
//////////////////////////////////
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
