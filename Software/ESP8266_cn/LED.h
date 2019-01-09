#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"

#define NUM_LEDS 1
#define DATA_PIN 2
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void colorPaletts_setup() {
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}



void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}


void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}


void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}




// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.


void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 10)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 15)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 20)  {
      SetupPurpleAndGreenPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 25)  {
      SetupTotallyRandomPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 30)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = NOBLEND;
    }
    if ( secondHand == 35)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 40)  {
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 45)  {
      currentPalette = PartyColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 50)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 55)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = LINEARBLEND;
    }
  }
}



void colorPaletts() {
  ChangePalettePeriodically();

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}



void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
  }
}

void Cylon() {
  static uint8_t hue = 0;
  // First slide the led in one direction
  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }


  // Now go in the other direction.
  for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
}


void LED_Color(int r, int g, int b) {
  fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
  FastLED.show();
}


void Brightnessset(int Brightness) {
  //fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
  FastLED.setBrightness(Brightness);
  FastLED.show();
}


int led_action;
int len_l = 0;
int action = 0;
unsigned long system_time;
bool LED_Prompt;
//bool led_color;
void LED_SHOW() {
  if (led_action == 1) {
    float breath = (exp(sin(millis() / 4000.0 * PI)) - 0.36787944) * 108.0;
    FastLED.setBrightness(breath);
    //Serial.println(breath);
    FastLED.show();
  } else if (led_action == 2) {
    float breath = (exp(sin(millis() / 2500.0 * PI)) - 0.36787944) * 108.0;
    FastLED.setBrightness(breath);
    FastLED.show();
    //Serial.println(breath);
    if (breath < 0.01) {
      //Serial.println(breath);
      LED_Color(random(255), random(255), random(255));
    }
  } else if (led_action == 3) {
    /*float breath = (exp(sin(millis() / 2500.0 * PI)) - 0.36787944) * 108.0;
      FastLED.setBrightness(breath);
      FastLED.show();
      LED_Color(random(255), random(255), random(255));*/
    colorPaletts();
  } else if (led_action == 4) {
    Cylon();
  } else if (led_action == 5) {

  } else if (led_action == 6) {
    LED_Color(255, 0, 0);
    float breath = (exp(sin(millis() / 4000.0 * PI)) - 0.36787944) * 108.0;
    FastLED.setBrightness(breath);
    FastLED.show();
  }

  if (LED_Prompt) {
    //color
    if (millis() - system_time < 1000)
      return ;
    led_action=1;
    LED_Color(0, 0, 255);
    LED_Prompt = false;
  }
  //if(ajaxData_nfc.length();
}
unsigned long CheckTime;
void  LowBatteryCheck() {
  if(led_action==6)
    return;
  if (millis() - CheckTime > 5000) {
    int  power = analogRead(A0);
    if (power < 868) {
      led_action = 6;
    }
    CheckTime=millis();
  }

}

#define LED_RUN 1
#define LED_DATA 2
#define LED_ATTACK 3
#define LED_CONNECT 4
#define LED_SIMULATE 5
#define LED_TRANSMIT 6
//#define

void LED_STATE(int STATE, bool Change = false) {
  led_action = 0;
  switch (STATE) {
    case LED_CONNECT:
      LED_Color(255, 255, 0);
      break;
    case LED_RUN:
      LED_Color(0, 0, 255);
      //Serial.println("test");
      break;
    case LED_DATA:
      LED_Color(0, 255, 0);
      break;
    case LED_ATTACK:
      LED_Color(255, 255, 255);
      break;
    case LED_SIMULATE:
      LED_Color(255, 0, 255);
      break;
    case LED_TRANSMIT:
      LED_Color(0x8B, 0, 0);
      break;
  }
  if (Change) {
    Brightnessset(50);
    system_time = millis();
    LED_Prompt = 1;
  }
}


