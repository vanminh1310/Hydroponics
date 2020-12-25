
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 70
#define TRAIL    10

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 3
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {

  //RGB
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

}

void loop() {
  chanphao();
}




void chanphao() {

  for ( int i = 0; i <= NUM_LEDS; i++) {
    //Red, Saturation, Brightness
    //        leds[i] = CHSV( 0,     255,          255); // xanh la
    leds[i-2].setRGB(0,0,0);
    leds[i]=CRGB(0, 255 - 4 * i, 4 * i );
    // FastLED.setBrightness(2);//làm mờ bóng đèn
    FastLED.show();
    delay(100);
    fadeall() ;
    FastLED.clear();

    if (i == 10 ) {

       for(int i=50;i<NUM_LEDS;i++){
         leds[i].setRGB(0,0,255);
         FastLED.show();
        delay(20);
      }
       FastLED.clear();
      


      for(int i=50;i<NUM_LEDS;i++){
       
       leds[i].setRGB(255,255,0);
       FastLED.show();
       delay(100);
      }
       FastLED.clear();

//       mau xanh 
      for (int i = 50; i <= NUM_LEDS; i++) {
        leds[i] = CHSV( 0,     255,          255);
        FastLED.show();
        delay(100);
      }


      for (int i = 50; i < NUM_LEDS; i++) {
        leds[i].setRGB(255, 0, 0);
        FastLED.show();
        delay(20);
      }
      FastLED.clear();

      for (int i = 50; i < NUM_LEDS; i++) {
        leds[i].setRGB(0, 255, 0);
        FastLED.show();
        delay(20);
      }
      FastLED.clear();

      for (int i = 50; i < NUM_LEDS; i++) {
        leds[i] = CRGB(4 * i, 0 , 255 - 4 * i);
        FastLED.setBrightness(100 - i);
        FastLED.show();
        delay(150);
      }
      delay(1000);
       fill_solid(leds, NUM_LEDS, CRGB(145,44,20));
      FastLED.show();
       delay(1000);
     FastLED.clear();
     
       leds[50] = CRGB::HotPink;
        leds[51] = CRGB::BlueViolet;
        leds[52] = CRGB::HotPink;
        leds[53] = CRGB::YellowGreen;
        leds[54] = CRGB::HotPink;
        leds[55] = CRGB::BlueViolet;
        leds[56] = CRGB::HotPink;
        leds[57] = CRGB::YellowGreen;
        leds[58] = CRGB::HotPink;
        leds[59] = CRGB::YellowGreen;
        leds[60] = CRGB::BlueViolet;
        leds[61] = CRGB::HotPink;
        leds[62] = CRGB::HotPink;
        leds[63] = CRGB::YellowGreen;
        leds[64] = CRGB::HotPink;

        leds[65] = CRGB::HotPink;
        leds[66] = CRGB::YellowGreen;
        leds[67] = CRGB::BlueViolet;
        leds[68] = CRGB::HotPink;
        leds[69] = CRGB::HotPink;
        leds[70] = CRGB::YellowGreen;
       
      
        FastLED.show();
        delay(1000);
        FastLED.clear();
       

    }


  }

}
// fades the LED's behind the leading LED
void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(TRAIL);
  }
}

//   0 Red
//  42 Yellow
//  85 Green
// 128 Aqua
// 171 Blue
// 213 Purple
