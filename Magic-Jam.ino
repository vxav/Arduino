#include <Adafruit_WS2801.h>   // Link to WS2801 library: https://github.com/adafruit/Adafruit-WS2801-Library/blob/master/Adafruit_WS2801.h

int dataPin = 9;     // LED strip data
int clockPin = 10;  // LED strip clock

int t = 0;

int thermPin = 0;         // Thermistor
int val = 0;                  // Temperature as 255 scale value
byte TempMax = 30;   // Temperature at which the lamp is completely Red
byte TempMin = 15;    // Temperature at which the lamp is completely Red
byte Coefficient = 0;    // Coefficient used to convert celsius temperature into a 255 scale value
int UpdateInt = 2000;   // Interval between temperature checks (in ms)

Adafruit_WS2801 strip = Adafruit_WS2801(3, dataPin, clockPin);


//////////////////////////////////
// Initialisation
//////////////////////////////////
void setup() {
  Serial.begin(9600);
  strip.begin();
  Coefficient = TempMax - TempMin;
  Coefficient = 255/Coefficient;
}


//////////////////////////////////
// Program
//////////////////////////////////
void loop() {

  val = int(Thermistor(analogRead(thermPin)));
  Temp = Temp * Coefficient;    // Convert to 255 scale: 255 is 30 degrees
   
  Serial.print(Temp);
  Serial.print(" degrees c. -   ");
	
	if (temp >= TempMin && temp <= TempMax)
		if (val > 128) {
		  setColor(Color(val,255-val,0));
		} else {
		  setColor(Color(val,255-val,128-val));
		}
	} else if (temp < TempMin) {
		setColor(Color(0,255,128));
	} else {
		setColor(Color(255,0,0));
	}

  delay(UpdateInt);
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


//////////////////////////////////
// Gets temperature
//////////////////////////////////
double Thermistor(int RawADC) {
 double Temp;
 Temp = log(10000.0*((1024.0/RawADC-1)));
//         =log(10000.0/(1024.0/RawADC-1)) // for pull-up configuration
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;            // Convert Kelvin to Celcius

 return Temp;
}
