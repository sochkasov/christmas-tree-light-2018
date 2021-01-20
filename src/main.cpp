#include <Arduino.h>
#include "FastLED.h"
#include "AceButton.h"
#include "main.h"


using namespace ace_button;
FASTLED_USING_NAMESPACE

// For the select mode you can press the button.
//  - Fast press (press < 200ms) - select next animation
//  - Normal press (200ms < press < 600ms)
//  - Long press (600ms < press) - Light and animation on/off
//  - Double press - Select auto rotation animation mode

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif


AceButton button(BUTTON_PIN);
void handleEvent(AceButton*, uint8_t, uint8_t);

#define NUM_LEDS    (NUM_LEDS_STAR + NUM_LEDS_LINE)

CRGB ledSTAR[NUM_LEDS_STAR];
CRGB leds[NUM_LEDS];

#define star_ext CRGB::DarkRed;
#define star_mid CRGB::DarkOrange;
#define star_center CRGB::Red;

uint8_t star_ext_Hue = 0;
uint8_t star_mid_Hue = 32;
uint8_t star_center_Hue = 48;
bool star_ext_direction = false;
bool star_mid_direction = false;
bool star_center_direction = false;

CRGB temp[NUM_LEDS];
uint8_t pos;  // stores a position for color being blended in
uint8_t posR, posG, posB;  // positions of moving R,G,B dots
uint8_t hue;  // used to cycle through the rainbow
uint8_t hue2;  // used to change the moving color
boolean moving = 1;  // switch for moving/not moving

uint16_t ledsData[NUM_LEDS_LINE][4];  // array to store color data and an extra value
uint16_t pick;  // stores a temporary pixel number

#define OnAfterPowerOn = true;
bool manual_mode = false; // false - auto rotate animation, true - manual mode
bool switch_on = true; // false - light is off, true - light is on
int8_t mode_star = 1;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


void star0() {
  // --- Animation 1 --- (Star) ---------------
  const uint8_t delta = 255/NUM_LEDS_STAR/5;
  static uint8_t count;
  EVERY_N_MILLISECONDS(1) { count++; }
  fill_rainbow(ledSTAR, NUM_LEDS_STAR, count, -1*delta );

  //copy NUM_LEDS_STAR data to leds
  for (uint8_t i=0; i<NUM_LEDS_STAR; i++) { leds[i+NUM_LEDS_LINE] = ledSTAR[i]; }
}
void addGlitterForStar( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ NUM_LEDS_LINE + random16(NUM_LEDS_STAR) ] += CRGB::White;
  }
}//end star0

// Только под 11 светодиодов в звезде
void star1() 
{
  if (NUM_LEDS_STAR != 11) {
    star0(); // only 11 leds use in the star
  } else {
    leds[ NUM_LEDS_LINE ] = star_ext;
    leds[ NUM_LEDS_LINE+1 ] = star_mid;
    leds[ NUM_LEDS_LINE+2 ] = star_ext;
    leds[ NUM_LEDS_LINE+3 ] = star_mid;
    leds[ NUM_LEDS_LINE+4 ] = star_ext;
    leds[ NUM_LEDS_LINE+5 ] = star_mid;
    leds[ NUM_LEDS_LINE+6 ] = star_ext;
    leds[ NUM_LEDS_LINE+7 ] = star_mid;
    leds[ NUM_LEDS_LINE+8 ] = star_ext;
    leds[ NUM_LEDS_LINE+9 ] = star_mid;
    leds[ NUM_LEDS_LINE+10 ] = star_center;
  }

}//end star1

// Только под 11 светодиодов в звезде
void star1g() 
{
    if (NUM_LEDS_STAR != 11) {
    star0(); // only 11 leds use in the star
  } else {
    leds[ NUM_LEDS_LINE ] = star_ext;
    leds[ NUM_LEDS_LINE+1 ] = star_mid;
    leds[ NUM_LEDS_LINE+2 ] = star_ext;
    leds[ NUM_LEDS_LINE+3 ] = star_mid;
    leds[ NUM_LEDS_LINE+4 ] = star_ext;
    leds[ NUM_LEDS_LINE+5 ] = star_mid;
    leds[ NUM_LEDS_LINE+6 ] = star_ext;
    leds[ NUM_LEDS_LINE+7 ] = star_mid;
    leds[ NUM_LEDS_LINE+8 ] = star_ext;
    leds[ NUM_LEDS_LINE+9 ] = star_mid;
    leds[ NUM_LEDS_LINE+10 ] = star_center;
    addGlitterForStar(80);
  }

}//end star1g

// Только под 11 светодиодов в звезде
void star2() 
{
  if (star_ext_direction){ star_ext_Hue--; } else { star_ext_Hue++; }
  if (star_mid_direction){ star_mid_Hue--; } else { star_mid_Hue++; }
  if (star_center_direction){ star_center_Hue--; } else { star_center_Hue++; }

  if (star_ext_Hue>16) { star_ext_direction=true; } else if (star_ext_Hue==0) { star_ext_direction=false; }
  if (star_mid_Hue>24) { star_mid_direction=true; } else if (star_mid_Hue==0) { star_mid_direction=false; }
  if (star_center_Hue>7) { star_center_direction=true; } else if (star_center_Hue==0) { star_center_direction=false; }

    if (NUM_LEDS_STAR != 11) {
    star0(); // only 11 leds use in the star
  } else {
    leds[ NUM_LEDS_LINE ].setHue(star_ext_Hue);
    leds[ NUM_LEDS_LINE+1 ].setHue(star_mid_Hue);
    leds[ NUM_LEDS_LINE+2 ].setHue(star_ext_Hue);
    leds[ NUM_LEDS_LINE+3 ].setHue(star_mid_Hue);
    leds[ NUM_LEDS_LINE+4 ].setHue(star_ext_Hue);
    leds[ NUM_LEDS_LINE+5 ].setHue(star_mid_Hue);
    leds[ NUM_LEDS_LINE+6 ].setHue(star_ext_Hue);
    leds[ NUM_LEDS_LINE+7 ].setHue(star_mid_Hue);
    leds[ NUM_LEDS_LINE+8 ].setHue(star_ext_Hue);
    leds[ NUM_LEDS_LINE+9 ].setHue(star_mid_Hue);
    leds[ NUM_LEDS_LINE+10 ].setHue(star_center_Hue);
  }

}//end star2

// Доработано до любого кол-ва светодиодов в звезде
void star3() 
{
  if (star_ext_direction){ star_ext_Hue--; } else { star_ext_Hue++; }
  if (star_ext_Hue>254) { star_ext_direction=true; } else if (star_ext_Hue<40) { star_ext_direction=false; }

  for (uint8_t star_led; star_led < NUM_LEDS_STAR; star_led++) {
    leds[ NUM_LEDS_LINE + star_led].setHSV(0,255,star_ext_Hue);
  }

}//end star3

void rainbow() {
  // FastLED's built-in rainbow generator
  mode_star = 4;
  fill_rainbow( leds, NUM_LEDS_LINE, gHue, 7);
}//end rainbow

void rainbow2() {
  // FastLED's built-in rainbow generator
  mode_star = 2;
  fill_rainbow( leds, NUM_LEDS_LINE, gHue, 2);
}//end rainbow2

void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS_LINE) ] += CRGB::White;
  }
}

void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  mode_star = 1;
  rainbow2();
  addGlitter(80);
}//end rainbowWithGlitter

void confetti() {
  // random colored speckles that blink in and fade smoothly
  mode_star = 1;
  fadeToBlackBy( leds, NUM_LEDS_LINE, 5);
  int pos = random16(NUM_LEDS_LINE);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  delay(30);
}//end confetti

void confetti2() {
  mode_star = 3;
  fadeToBlackBy( leds, NUM_LEDS_LINE, 10);
  int pos = random16(NUM_LEDS_LINE);
  leds[pos] += CHSV( gHue + random8(128), 200, 255);
}//end confetti2

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  mode_star = 2;
  fadeToBlackBy( leds, NUM_LEDS_LINE, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS_LINE-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}//end sinelon

void sinelon2() {
  mode_star = 3;
  fadeToBlackBy( leds, NUM_LEDS_LINE, 2);
  int pos = beatsin16( 13, 0, NUM_LEDS_LINE-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}//end sinelon2

void sinelon3() {
  mode_star = 2;
  fadeToBlackBy( leds, NUM_LEDS_LINE, 5);
  int pos = beatsin16( 3, 0, NUM_LEDS_LINE-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}//end sinelon3

void bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  mode_star = 3;
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS_LINE; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}//end bpm

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  mode_star = 2;
  fadeToBlackBy( leds, NUM_LEDS_LINE, 3);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS_LINE-1 )] |= CHSV(dothue, 200, 255);
    dothue += 50;
  }
}//end juggle

void juggle2() {
  mode_star = 3;
  fadeToBlackBy( leds, NUM_LEDS_LINE, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS_LINE-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}//end juggle2

void fadeall() { 
  for(int i = 0; i < NUM_LEDS_LINE; i++) {
    leds[i].nscale8(250);
    }
    button.check();
}//end fadeall

void cyclon() { 
  mode_star = 1;
  static uint8_t hue = 0;
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS_LINE; i++) {
    leds[i] = 0xAAAAAA;
    FastLED.show(); 
    leds[i] = CRGB::Black;
    leds[i] = CHSV(hue++, 255, 255); FastLED.show();
    fadeall();
    delay(5);
  }

  // Now go in the other direction.  
  for(int i = (NUM_LEDS_LINE)-1; i >= 0; i--) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(15);
  }
}//end cyclon

// https://github.com/marmilicious/FastLED_examples/blob/master/blend_into_rainbow_v2.ino
void blend_into_rainbow_v2() { 
  mode_star = 4;
  EVERY_N_MILLISECONDS(60) {
      hue++;
    }

    fill_rainbow(leds, NUM_LEDS_LINE, hue, 255/NUM_LEDS_LINE/4);  // draw part of the rainbow into the strip
    fadeToBlackBy(leds, NUM_LEDS_LINE, 128);  // fade the whole rainbow down some

    EVERY_N_MILLISECONDS(150) {
      if (moving) {
        pos++;
        if (pos == NUM_LEDS_LINE) { pos = 0; }  // reset to begining
      }
    }

    EVERY_N_MILLISECONDS(5) {
      if (!moving) {
        hue2 = hue2 - 1;
      }
    }

    CRGB blendThisIn  = CHSV(hue2, 140, 255);  // colors to blend into background
    CRGB blendThisIn2 = CHSV(hue2, 170, 255);
    CRGB blendThisIn3 = CHSV(hue2, 210, 255);
    CRGB blendThisIn4 = CHSV(hue2, 255, 255);

    static uint8_t blendAmount = 128;  // amount of blend  [range: 0-255]

    nblend(leds[pos],                   blendThisIn4, blendAmount/3);
    nblend(leds[mod8(pos+1, NUM_LEDS_LINE)], blendThisIn3, blendAmount/2);
    nblend(leds[mod8(pos+2, NUM_LEDS_LINE)], blendThisIn2, blendAmount);
    nblend(leds[mod8(pos+3, NUM_LEDS_LINE)], blendThisIn,  blendAmount);
    nblend(leds[mod8(pos+4, NUM_LEDS_LINE)], blendThisIn2, blendAmount);
    nblend(leds[mod8(pos+5, NUM_LEDS_LINE)], blendThisIn3, blendAmount/2);
    nblend(leds[mod8(pos+6, NUM_LEDS_LINE)], blendThisIn4, blendAmount/3);

    posR = beatsin8(5, 0, (NUM_LEDS_LINE-1));
    posG = beatsin8(7, 0, (NUM_LEDS_LINE-1));
    posB = beatsin8(9, 0, (NUM_LEDS_LINE-1));

    CRGB tempColor = CRGB::Red;
    temp[posR] = tempColor;
    tempColor = CRGB::Green;
    nblend(temp[posG], tempColor, 128);
    tempColor = CRGB::Blue;
    nblend(temp[posB], tempColor, 128);

    EVERY_N_MILLISECONDS(2) {
      fadeToBlackBy(temp, NUM_LEDS_LINE, 1);  // create fading tail effect
    }

    for (uint8_t i = 0; i < NUM_LEDS_LINE; i++) {
      nblend(leds[i], temp[i], 128);
    }

    nblend(leds[mod8(posR, NUM_LEDS_LINE)], CRGB::Red, 90);
    nblend(leds[mod8(posR+1, NUM_LEDS_LINE)], CRGB::Red, 128);
    nblend(leds[mod8(posR+2, NUM_LEDS_LINE)], CRGB::Red, 90);

    nblend(leds[mod8(posG, NUM_LEDS_LINE)], CRGB::Green, 90);
    nblend(leds[mod8(posG+1, NUM_LEDS_LINE)], CRGB::Green, 128);
    nblend(leds[mod8(posG+2, NUM_LEDS_LINE)], CRGB::Green, 90);

    nblend(leds[mod8(posB, NUM_LEDS_LINE)], CRGB::Blue, 90);
    nblend(leds[mod8(posB+1, NUM_LEDS_LINE)], CRGB::Blue, 128);
    nblend(leds[mod8(posB+2, NUM_LEDS_LINE)], CRGB::Blue, 90);

    FastLED.show();  // update the display

    EVERY_N_SECONDS(random8(4, 16)) {
      moving = !moving;  // randomly start/stop part of the animation for fun
    }
}//end blend_into_rainbow_v2

void christmasSparkles() {
  mode_star = 2;
  //"Background" color for non-sparkling pixels.
  CRGB sparkleBgColor = CHSV(50, 30, 40);  // dim white
  //CRGB sparkleBgColor = CHSV(96, 200, 30);  // dim green
  
  EVERY_N_MILLISECONDS(40){
    if( random8() < 60 ) {  // How much to sparkle!  Higher number is more.
      pick = random16(NUM_LEDS_LINE);
      if (ledsData[pick][3] == 0 ) {
        ledsData[pick][3] = 35;  // Used to tag pixel as sparkling
        uint8_t randomPick = random8(5);
        if (randomPick == 0) {
          ledsData[pick][0] = 178;   // sparkle hue (blue)
          ledsData[pick][1] = 244;  // sparkle saturation
          ledsData[pick][2] = 210;  // sparkle value
        }
        if (randomPick == 1) {
          ledsData[pick][0] = 10;  // sparkle hue (red)
          ledsData[pick][1] = 255;  // sparkle saturation
          ledsData[pick][2] = 240;  // sparkle value
        }
        if (randomPick == 2) {
          ledsData[pick][0] = 0;  // sparkle hue (white-ish)
          ledsData[pick][1] = 25;  // sparkle saturation
          ledsData[pick][2] = 255;  // sparkle value
        }
        if (randomPick == 3) {
          ledsData[pick][0] = 35;   // sparkle hue (orange)
          ledsData[pick][1] = 235;  // sparkle saturation
          ledsData[pick][2] = 245;  // sparkle value
        }
        if (randomPick == 4) {
          ledsData[pick][0] = 190;  // sparkle hue (purple)
          ledsData[pick][1] = 255;  // sparkle saturation
          ledsData[pick][2] = 238;  // sparkle value
        }
        leds[pick] = CHSV(ledsData[pick][0], ledsData[pick][1], ledsData[pick][2]);
      }
    }
    for (uint16_t i=0; i < NUM_LEDS_LINE; i++) {
      if (ledsData[i][3] == 0) {  // if not sparkling, set to "back ground" color
        leds[i] = sparkleBgColor;
      } else {
        CHSV hsv = rgb2hsv_approximate(leds[i]);  // Used to get approximate Hue
        EVERY_N_MILLISECONDS(38) { ledsData[i][0] = hsv.hue - 1; }  // slightly shift hue
        ledsData[i][2] = scale8(ledsData[i][2], 245);  // slowly darken
        leds[i] = CHSV(ledsData[i][0], ledsData[i][1], ledsData[i][2]);
        ledsData[i][3] = ledsData[i][3] - 1;  // countdown sparkle tag
      }
    }
  }
}//end christmasSparkles

void christmasSparklesRG() {
  mode_star = 2;
  //"Background" color for non-sparkling pixels.  Can be set to black for no bg color.
  // CRGB sparkleBgColor = CHSV(0, 0, 0);  // black
  CRGB sparkleBgColor = CHSV(50, 30, 30);  // dim white
 
  EVERY_N_MILLISECONDS(40){
    if( random8() < 110 ) {  // How much to sparkle!  Higher number is more.
      pick = random16(NUM_LEDS_LINE);
      if (ledsData[pick][3] == 0 ) {
        ledsData[pick][3] = 65;  // Used to tag pixel as sparkling
        uint8_t randomPick = random8(2);
        if (randomPick == 0) {
          ledsData[pick][0] = 16;  // sparkle hue (red)
          ledsData[pick][1] = 50;  // sparkle saturation
          ledsData[pick][2] = 242;  // sparkle value
        }
        if (randomPick == 1) {
          ledsData[pick][0] = 96;   // sparkle hue (green)
          ledsData[pick][1] = 50;  // sparkle saturation
          ledsData[pick][2] = 255;  // sparkle value
        }
        leds[pick] = CHSV(ledsData[pick][0], ledsData[pick][1], ledsData[pick][2]);
      }
    }
    for (uint16_t i=0; i < NUM_LEDS_LINE; i++) {
      if (ledsData[i][3] == 0) {  // if not sparkling, set to "back ground" color
        leds[i] = sparkleBgColor;
      } else {
        CHSV hsv = rgb2hsv_approximate(leds[i]);  // Used to get approximate Hue
        EVERY_N_MILLISECONDS(50) { ledsData[i][0] = hsv.hue - 1; }  // slightly shift hue
        ledsData[i][2] = scale8(ledsData[i][2], 253);  // slowly darken
        leds[i] = CHSV(ledsData[i][0], ledsData[i][1], ledsData[i][2]);
        ledsData[i][3] = ledsData[i][3] - 1;  // countdown sparkle tag
      }
    }
  }
}//end christmasSparklesRG

// https://github.com/atuline/FastLED-Demos/blob/master/blur/blur.ino
void blur() {
  mode_star = 1;
  uint8_t blurAmount = dim8_raw( beatsin8(3,64, 192) );       // A sinewave at 3 Hz with values ranging from 64 to 192.
  blur1d( leds, NUM_LEDS_LINE, blurAmount);                        // Apply some blurring to whatever's already on the strip, which will eventually go black.
  
  uint8_t  i = beatsin8(  9, 0, NUM_LEDS_LINE);
  uint8_t  j = beatsin8( 7, 0, NUM_LEDS_LINE);
  uint8_t  k = beatsin8(  5, 0, NUM_LEDS_LINE);
  
  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis();  
  leds[(i+j)/2] = CHSV( ms / 29, 200, 255);
  leds[(j+k)/2] = CHSV( ms / 41, 200, 255);
  leds[(k+i)/2] = CHSV( ms / 73, 200, 255);
  leds[(k+i+j)/3] = CHSV( ms / 53, 200, 255);
  
  FastLED.show();
} //end blur()


typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { blur, blend_into_rainbow_v2, confetti2, rainbowWithGlitter, cyclon, juggle2, sinelon2, confetti, sinelon, rainbow, juggle, sinelon3, christmasSparklesRG, bpm, christmasSparkles };
// SimplePatternList gPatterns = { blur };
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  // debug
  // Serial.print(" gCurrentPatternNumber:");
  // Serial.println(gCurrentPatternNumber);
}//end nextPattern

void handleEvent(AceButton* /*button*/, uint8_t eventType, uint8_t /*buttonState*/ buttonState ) {
  switch (eventType) {
    case AceButton::kEventClicked:    // press < 200ms
      // Serial.println("button: kEventClicked");
      manual_mode = true;      //  Select next animation
      nextPattern();
      break;
    case AceButton::kEventReleased:    // 200ms < press < 600ms
      // Serial.println("button: kEventReleased");
      break;
    case AceButton::kEventLongPressed:     // 600ms < press
      // Serial.println("button: kEventLongPressed");
      switch_on = !switch_on;      // Light and animation on/off
      break;
    case AceButton::kEventDoubleClicked:    // double press
      // Serial.println("button: kEventDoubleClicked");
      manual_mode = false;      // Set auto rotation mode
      break;
  }
}//end handleEvent


void setup() {
  // button configuration
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  ButtonConfig * buttonConfig = button.getButtonConfig ();
  buttonConfig-> setEventHandler (handleEvent);
  buttonConfig-> setFeature (ButtonConfig :: kFeatureDoubleClick );
  buttonConfig-> setFeature (ButtonConfig :: kFeatureSuppressClickBeforeDoubleClick );
  buttonConfig-> setFeature (ButtonConfig :: kFeatureSuppressAfterClick );
  buttonConfig-> setFeature (ButtonConfig :: kFeatureSuppressAfterDoubleClick );
  buttonConfig-> setFeature (ButtonConfig :: kFeatureLongPress );
  buttonConfig-> setFeature (ButtonConfig :: kFeatureSuppressAfterLongPress );


  Serial.begin(115200);  //allow for output to serial monitor
  delay(1000);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  Serial.println("setup done");
  star1();
}//end setup

void loop() {
  if (switch_on) { // Light on mode
    // --- Animation 1 --- (Star) ---------------
    EVERY_N_MILLISECONDS( 80 ) { 
      // star1();
      switch (mode_star) {
        case 0:
          star0();
          break;
        case 1:
          star1(); // only 11 leds use in the star
          break;
        case 2:
          star2(); // only 11 leds use in the star
          break;
        case 3:
          star3();
          break;
        case 4:
          star1g(); // only 11 leds use in the star
          break;
        default:
          star1(); // only 11 leds use in the star
          break;
      }
      
    }

    // --- Animation 2 --- (Line) ---------------
    gPatterns[gCurrentPatternNumber]();
    FastLED.show();  
    FastLED.delay(1000/FRAMES_PER_SECOND); 
    EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
    EVERY_N_SECONDS( TIME_FOR_1_EFFECT ) { 
      if (!manual_mode) {
        nextPattern(); // change patterns periodically (in auto rotation mode)
      }
    }
  } else {
    FastLED.clear();
    FastLED.show();
    // delay(500);
  }
  button.check();
  // Serial.print("manual_mode:");
  // Serial.print(manual_mode);
  // Serial.print(" gCurrentPatternNumber:");
  // Serial.println(gCurrentPatternNumber);
  
}//end loop
