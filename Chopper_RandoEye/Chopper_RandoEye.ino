#include "Arduino.h"
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include <NeoPatterns.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1
#define PIN_NEOPIXEL  7 //Ladder

//#define LADDERPIN 7// pin for Ladder LEDs

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 8 // Popular NeoPixel ring size
#define LADDERNUM 7//NUmber of Ladder LEDs

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);//Chopper's Eyes
//Adafruit_NeoPixel ladder(LADDERNUM, LADDERPIN, NEO_GRB + NEO_KHZ800);//Ladder

// onComplete callback functions
void MultiPatterns(NeoPatterns *aLedsPtr);

/////Declarations
int RandoEyeVal();//Find a random eye to manipulate
void standardEyeLights(); //Steady state eye lighting
void randomEyeLights(); //Rando eye animations
void flickAAA(int flickB, int flickG, int flickR);//Flicker Routine
void powerGlitch(int flickB, int flickG, int flickR); //Power glitch animation

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  Serial.begin(115200);

  //Initial Colors
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setPixelColor(1, pixels.Color(90, 90, 0));
  pixels.setPixelColor(4, pixels.Color(0, 50, 255));
  pixels.setPixelColor(7, pixels.Color(0, 50, 255));
  pixels.show();

  //Ladder Setup
//  ladder.begin(); //setup ladder
//  ladder.setPixelColor(0,ladder.Color(200,200,200));
//  ladder.setPixelColor(1,ladder.Color(200,200,200));
//  ladder.setPixelColor(2,ladder.Color(200,200,200));
//  ladder.setPixelColor(3,ladder.Color(200,200,200));
//  ladder.setPixelColor(4,ladder.Color(200,200,200));
//  ladder.setPixelColor(5,ladder.Color(200,200,200));
//  ladder.setPixelColor(6,ladder.Color(200,200,200));
//  ladder.show();
}

void loop() {
  //pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    //pixels.setPixelColor(i, pixels.Color(0, 255, 200));

    //pixels.show();   // Send the updated pixel colors to the hardware.
    standardEyeLights();
    int randoTime = random(1000);
    delay((DELAYVAL+randoTime)*3); // Pause before next pass through loop
    randomEyeLights();
  }
}

void standardEyeLights(){
  pixels.clear();
  pixels.setPixelColor(1, pixels.Color(90, 90, 0));
  pixels.setPixelColor(4, pixels.Color(0, 50, 255));
  pixels.setPixelColor(7, pixels.Color(0, 50, 255));
  pixels.show();
}

void randomEyeLights(){
  //pixels.clear();

//    int randoR = random(255);
//    int randoG = random(255);
//    int randoB = random(255);
    //int randLight = random(6);
    int glitchCount = random(1,7);


    for(int i=0; i<glitchCount; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    Serial.print ("GlitchCount:  ");
    Serial.print(glitchCount);
    int randLight = random(8); //cases are -1 from max
    Serial.print (" - randcount:  ");
    Serial.print(randLight);
    int randoR = random(255);
    int randoG = random(255);
    int randoB = random(255);
    int randoDelay = random(20,350);
    Serial.print (" - randoDelay:  ");
    Serial.println(randoDelay);

    switch (randLight){
      case 0:
        pixels.setPixelColor(RandoEyeVal(), pixels.Color(randoR, randoG, randoB));
        pixels.setPixelColor(RandoEyeVal(), pixels.Color(randoG, randoR, randoB));
        pixels.setPixelColor(RandoEyeVal(), pixels.Color(randoB, randoG, randoR));
        break;
      case 1:
        pixels.setPixelColor(RandoEyeVal(), pixels.Color(randoR, randoG, randoB));
        break;
      case 2:
        pixels.setPixelColor(RandoEyeVal(), pixels.Color(randoG, randoR, randoB));
        break;
      case 3:
        pixels.setPixelColor(RandoEyeVal(), pixels.Color(randoB, randoG, randoR));
        break;
      case 4:
        pixels.setPixelColor(RandoEyeVal(), pixels.Color(randoR, randoG, randoB));
        pixels.setPixelColor(RandoEyeVal(), pixels.Color(randoG, randoR, randoB));
        break;
      case 5:
        pixels.setPixelColor(RandoEyeVal(), pixels.Color(randoR, randoG, randoB));
        pixels.setPixelColor(RandoEyeVal(), pixels.Color(randoB, randoG, randoR));
        break;
       case 6:
        //jitter
          flickAAA(randoB, randoG, randoR);
        break;
      case 7:
        //power failure or something
        powerGlitch(randoB, randoG, randoR);
        break;
    }

  pixels.show();
  delay(randoDelay); // Pause before next pass through loop
  }

}

void flickAAA(int flickB, int flickG, int flickR)
{
  // Flicker Chosen Light
          int randoEye = random(0,3);
        int eyeToJitter = RandoEyeVal();// 4;// randoEye;
        Serial.print ("RandoEye: ");
        Serial.println(randoEye);

//        switch (randoEye){
//          case 0:
//            eyeToJitter = 1;
//            break;
//          case 1:
//            eyeToJitter = 4;
//            break;
//          case 2:
//            eyeToJitter = 7;
//            break;
//        }
        int flickTimes = random(7, 22);

        for(int i=0; i<flickTimes; i++){
          pixels.setPixelColor(eyeToJitter, pixels.Color(flickB, flickG, flickR));
          //Serial.print ("Flicker: ");
          //Serial.println(eyeToJitter);
        delay(50 );
          pixels.show();
          pixels.setPixelColor(eyeToJitter, pixels.Color(flickR, flickB, flickG));
          //pixels.setPixelColor(eyeToJitter, pixels.Color(0, 0, 0));
          delay(50);
          pixels.show();
          //Serial.print ("OFF: ");
          //Serial.println(eyeToJitter);
        }
}
void powerGlitch(int flickB, int flickG, int flickR){
  //power glitches so everything sputters
      pixels.setPixelColor(1, pixels.Color(flickR, flickB, flickG));
      pixels.setPixelColor(4, pixels.Color(flickR, flickB, flickG));
      pixels.setPixelColor(7, pixels.Color(flickR, flickB, flickG));
      pixels.show();
      delay(35);

      pixels.setPixelColor(1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(4, pixels.Color(0, 0, 0));
      pixels.setPixelColor(7, pixels.Color(0, 0, 0));
      pixels.show();
      delay(8);
}

int RandoEyeVal(){
  //This function returns a valid random eye to operate on
  int randoNum = random (0,2);
  //int returnVal = 1;

  switch (randoNum){
    case 0:
      return 1;
      break;
    case 1:
      return  4;
      break;
    case 2:
      return  7;
      break;
  }

}
