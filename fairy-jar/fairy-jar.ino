#include <CapacitiveSensor.h>
#include <FastLED.h>
#include <elapsedMillis.h>

#define DATA_PIN    4
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    8        // Change this to reflect the number of LEDs you have

int HUE = 0;            
int SATURATION = 255;          
int BRIGHTNESS = 255;   

int indexinc = 1;
elapsedMillis timeElapsed; //declare global if you don't want it reset every time loop runs
unsigned int interval = 2000;

CRGB leds[NUM_LEDS];

CRGBPalette16 gCurrentPalette;
CRGBPalette16 gTargetPalette;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

long previousMillis = 0;          

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 * Modified by Becky Stern 2013 to change the color of one RGB Neo Pixel based on touch input
 */


CapacitiveSensor   cs_4_1 = CapacitiveSensor(12,11);       
CapacitiveSensor   cs_4_2 = CapacitiveSensor(12,10);       
CapacitiveSensor   cs_4_3 = CapacitiveSensor(12,9);   


void setup()                    
{ 
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  currentPalette = RainbowStripeColors_p;
  currentBlending = LINEARBLEND;
  Serial.begin(9600);

}

int mode=0;

void loop(){
elapsedMillis timeElapsed; 
 if (timeElapsed > interval) 
  {     mode=0;
  }

 checkpress();
 
 switch (mode){
      case 0: BRIGHTNESS=175; fairylights();break;
      case 1: BRIGHTNESS=255; Red(); break;
      case 2: BRIGHTNESS=255; Purple(); break;
      case 3: BRIGHTNESS=255; Blue(); break;
      case 4: BRIGHTNESS=255; Rainbow(); break;
      case 5: BRIGHTNESS=255; Green(); break;
      case 6: BRIGHTNESS=255; Cyan(); break;
      case 7: BRIGHTNESS=255; White(); break;
 
  }

  
  
}

void checkpress() 
{
long start = millis();
    long total1 =  cs_4_1.capacitiveSensor(30);
    long total2 =  cs_4_2.capacitiveSensor(30);
    long total3 =  cs_4_3.capacitiveSensor(30);

if (total1 > 3000 && total2 > 3000 && total3 > 3000){
   mode=4;  //rainbow
  Serial.print("4");
}  
else if (total1 > 3000 && total2 > 3000){
   mode=5;   //green
  Serial.print("5");
} 

else if (total2 > 3000 && total3 > 3000){
   mode=6;   //cyan
  Serial.print("5");
}  
else if (total1 > 3000 && total3 > 3000){
   mode=7;    // white
  Serial.print("5");
}  

else if (total1 > 3000){
    mode=1; 
     Serial.print("1");
  } else if (total2 > 3000){
   mode=2; 
  Serial.print("2");
} else if (total3 > 3000){
   mode=3; 
   Serial.print("3");
} 
}

const TProgmemPalette16 FairylightsPalette_p PROGMEM =
{
  CRGB::White,
  CRGB::Gray,
  CRGB::Gold,
  CRGB::Yellow,

  CRGB::Gray,
  CRGB::Yellow,
  CRGB::Gold,
  CRGB::Yellow,

  CRGB::Gray,
  CRGB::Yellow,
  CRGB::Gold,
  CRGB::Yellow,
  
  CRGB::Gray,
  CRGB::Yellow,
  CRGB::Gold,
  CRGB::Yellow,
};


void fairylights()   //evaluate password.  Also wait 3 seconds then go back to FairyLights
{
  
  FastLED.setBrightness(BRIGHTNESS);
  currentPalette = FairylightsPalette_p;
  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; 

  FillLEDsFromPaletteColors( startIndex);
    
  FastLED.show();
  FastLED.delay(25);  

  
}
void Wait(){
 if (timeElapsed > interval) 
  {     mode=0;  
    timeElapsed=0;
  }
 
}

void Solid()
{
   fill_solid(leds, NUM_LEDS, CHSV(HUE, SATURATION, BRIGHTNESS)); 
   FastLED.show(); 
   delay(20); 
  
}

void Red()
{  
  BRIGHTNESS=255; HUE=0;  SATURATION=255;Solid();
  Wait();
}

void Purple()
{  
  BRIGHTNESS=255; HUE=190;  SATURATION=255;Solid();
  Wait();
}

void Blue()
{  
  BRIGHTNESS=255; HUE=160;  SATURATION=255;Solid();
  Wait();
}

void Cyan()
{  
  BRIGHTNESS=255; HUE=120; SATURATION=255; Solid();
  Wait();
}
void Green()
{  
  BRIGHTNESS=255; HUE=100; SATURATION=255; Solid();
  Wait();
}

void White()
{  
  BRIGHTNESS=255; SATURATION=0; Solid();
  Wait();
}



void Rainbow()
{ 
  SATURATION=255;
  FastLED.setBrightness(  BRIGHTNESS );
  currentPalette = RainbowColors_p;
  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; 

  FillLEDsFromPaletteColors( startIndex);
    
  FastLED.show();
  FastLED.delay(25);  
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = BRIGHTNESS;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 25;
  }
}
