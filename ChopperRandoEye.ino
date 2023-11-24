//Random Eye and LED Ladder patterns for Chopper's Dome
//Uses NeoPatterns
//Hacked by Eebel 18 April 2021
//work in progress
//Updated sketch to use NeoPatterns 3.1.1 on 7 Sep 2023
//Added Pericspope code 24 Nov 2023

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "NeoPatterns.hpp"

//#ifdef __AVR__
// #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
//#endif

#define LEDPAD //uncomment if using three NeoPixal Pads instead of an LED strip

// Which pin on the Arduino is connected to the NeoPixels?
#define EYEPIN        6 // Arduino pin to attach the Eyes
#define LADDERPIN  7 //Arduino pin to attach the LED Ladder
#define PERISCOPEPIN 8  //Periscope LED

// How many NeoPixels are attached to the Eyes?


#ifdef LEDPAD
	int EYENUM = 3;
	int eyeOne = 0;
	int eyeTwo = 1;
	int eyeThree = 2;
	//#define EYENUM 3 //if using just thre NeoPixel LEDs
#else
	int EYENUM = 8;
	int eyeOne = 1;
	int eyeTwo = 4;
	int eyeThree = 7;
//	#define EYENUM 8 // If using an LED Strip you need 8 to span the eyes
#endif



#define LADDERNUM  16//16//7//Number of Ladder LEDs

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
//Adafruit_NeoPixel pixels(EYENUM, EYEPIN, NEO_GRB + NEO_KHZ800);//Chopper's Eyes


// onComplete callback functions
void EyePatterns(NeoPatterns *aLedsPtr);
void TwoPatterns(NeoPatterns *aLedsPtr);
void PeriscopePatterns(NeoPatterns *aLedsPtr);



// construct the NeoPatterns instances
NeoPatterns neoEye = NeoPatterns(EYENUM, EYEPIN, NEO_GRB + NEO_KHZ800, &EyePatterns);
NeoPatterns ladder = NeoPatterns(LADDERNUM, LADDERPIN, NEO_GRB + NEO_KHZ800, &TwoPatterns);
NeoPatterns periscope = NeoPatterns(1, PERISCOPEPIN, NEO_GRB + NEO_KHZ800, &PeriscopePatterns);

/////Declarations
int RandoEyeVal();//Find a random eye to manipulate
void standardEyeLights(); //Steady state eye lighting
void randomEyeLights(); //Rando eye animations
void flickAAA(int flickB, int flickG, int flickR);//Flicker Routine
void powerGlitch(int flickB, int flickG, int flickR); //Power glitch animation
//void MultiPatterns(NeoPatterns *aLedsPtr);//NeoPatterns handler

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
//#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
//  clock_prescale_set(clock_div_1);
//#endif
  // END of Trinket-specific code.
  Serial.begin(115200);
  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_NEOPATTERNS));




  //ladder.begin(); // This sets the pin.
  neoEye.begin();
  ladder.begin();
  periscope.begin();


  //Initial Colors
//  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
//  pixels.setPixelColor(1, pixels.Color(90, 90, 0));
//  pixels.setPixelColor(4, pixels.Color(0, 50, 255));
//  pixels.setPixelColor(7, pixels.Color(0, 50, 255));
//  pixels.show();

  neoEye.setPixelColor(eyeOne, 90, 90, 0);
  neoEye.setPixelColor(eyeTwo, 0, 50, 255);
  neoEye.setPixelColor(eyeThree, 0, 50, 255);
  neoEye.show();

  periscope.setPixelColor(1, 0,50,200);
  periscope.show();



  /*
   * First pattern ColorWipe
   */
  Serial.println("ColorWipe");
  //ladder.ColorWipe(COLOR32(0, 0, 4), 50, REVERSE); // light Blue
  ladder.ColorWipe(COLOR32_WHITE_HALF,50);
  ladder.updateAndWaitForPatternToStop();
  delay(100);

  /*
   * Second pattern - RainbowCycle
   */
  Serial.println("RainbowCycle");
  ladder.RainbowCycle(10);
  ladder.updateAndWaitForPatternToStop();
  delay(100);

  /*
   * Third pattern - rocket
   */
  Serial.println("Rocket");
  ladder.ScannerExtended(COLOR32_WHITE_HALF, 7, 20, 0, FLAG_SCANNER_EXT_VANISH_COMPLETE);
  ladder.updateAndWaitForPatternToStop();
  delay(100);

  /*
   * Now trigger the automatic patterns
   */
  TwoPatterns(&ladder);
  EyePatterns(&neoEye);
  PeriscopePatterns(&periscope);


  Serial.println("started");
  randomSeed(1234);

  //NeoPixelBar24.begin(); // This sets the output pin.

  /*
     * Start the patterns
     */
    //LowerNeoPixelBar.ColorWipe(COLOR32_GREEN_QUARTER, 80);
    //MiddleNeoPixelBar.ScannerExtended(COLOR32_BLUE_HALF, 2, 60, 2, FLAG_SCANNER_EXT_ROCKET | FLAG_SCANNER_EXT_START_AT_BOTH_ENDS);
    //UpperNeoPixelBar.ColorWipe(COLOR32_RED_QUARTER, 80, 0, DIRECTION_DOWN);


}

void loop() {
  //NeoPatterns Area
  ladder.update();
  neoEye.update();
  periscope.update();
  //Serial.println("Updated ladder:");
  delay(10);

}

void standardEyeLights(){
  neoEye.clear();
  neoEye.Color(eyeOne, 90, 90, 0);
  neoEye.Color(eyeTwo, 0, 50, 255);
  neoEye.Color(eyeThree, 0, 50, 255);
  neoEye.show();
}

//}

//void flickAAA(int flickB, int flickG, int flickR)
//{
//  // Flicker Chosen Light
//        int randoEye = random(0,3);
//        int eyeToJitter = RandoEyeVal();// 4;// randoEye;
//        Serial.print ("RandoEye: ");
//        Serial.println(randoEye);
//
//        int flickTimes = random(7, 22);
//
//        for(int i=0; i<flickTimes; i++){
//          pixels.setPixelColor(eyeToJitter, pixels.Color(flickB, flickG, flickR));
//          //Serial.print ("Flicker: ");
//          //Serial.println(eyeToJitter);
//        delay(50 );
//          pixels.show();
//          pixels.setPixelColor(eyeToJitter, pixels.Color(flickR, flickB, flickG));
//          //pixels.setPixelColor(eyeToJitter, pixels.Color(0, 0, 0));
//          delay(50);
//          pixels.show();
//          //Serial.print ("OFF: ");
//          //Serial.println(eyeToJitter);
//        }
//}

//void powerGlitch(int flickB, int flickG, int flickR){
//  //power glitches so everything sputters
//      pixels.setPixelColor(1, pixels.Color(flickR, flickB, flickG));
//      pixels.setPixelColor(4, pixels.Color(flickR, flickB, flickG));
//      pixels.setPixelColor(7, pixels.Color(flickR, flickB, flickG));
//      pixels.show();
//      delay(35);
//
//      pixels.setPixelColor(1, pixels.Color(0, 0, 0));
//      pixels.setPixelColor(4, pixels.Color(0, 0, 0));
//      pixels.setPixelColor(7, pixels.Color(0, 0, 0));
//      pixels.show();
//      delay(8);
//}

//int RandoEyeVal(){
//  //This function returns a valid random eye to operate on
//  int randoNum = random (0,2);
//  //int returnVal = 1;
//
//  switch (randoNum){
//    case 0:
//      return eyeOne;//1;
//      break;
//    case 1:
//      return eyeTwo;//4;
//      break;
//    case 2:
//      return eyeThree;//7;
//      break;
//  }
//
//}

/*
 * Simple handler for switching between 2 patterns
 */
void TwoPatterns(NeoPatterns *aLedsPtr) {
    //static int8_t sState = 7;
    static int8_t repeat = random(4,11);
    static int8_t pattCount = (8+1);//number of cases +1
    static int8_t sState = random(0,pattCount);

//#if defined(__AVR__)
//    uint32_t tRandom = random();
//#else
    uint32_t tRandom = random(__UINT32_MAX__);
//#endif
    uint8_t tDuration = random(100,300);//random(20, 120);
    uint8_t tColor1 = tRandom;
    uint8_t tColor2 = tRandom >> 8;

    switch (sState) {
    //switch (1) {
    case 0:
        // Scanner - use random mode and direction

        aLedsPtr->ScannerExtended(NeoPatterns::Wheel(tColor1), 3, tDuration, repeat,
                (tRandom & FLAG_SCANNER_EXT_CYLON) | (tRandom & FLAG_SCANNER_EXT_VANISH_COMPLETE)
                        | (tRandom & FLAG_SCANNER_EXT_START_AT_BOTH_ENDS), ((tRandom >> 8) & DIRECTION_DOWN));
        sState = random(0,pattCount);
        Serial.println("Random");
        break;

    case 1:
        // Fade - use random direction
        aLedsPtr->Fade(NeoPatterns::Wheel(tColor1), COLOR32_WHITE, repeat+12, tDuration);
        sState = random(0,pattCount);
        Serial.println("Fade");
        break;
    case 2:
        aLedsPtr->ColorWipe(COLOR32_WHITE, 50,FLAG_SCANNER_EXT_START_AT_BOTH_ENDS,DIRECTION_DOWN);
        sState = random(0,pattCount);
        Serial.println("ColorWipe");
        break;
    case 3:
        aLedsPtr->Heartbeat(COLOR32_WHITE, 50, repeat);
        sState = random(0,pattCount);
        Serial.println("Heartbeat");
        break;
    case 4:
        //aLedsPtr->RainbowCycle(50, DIRECTION_UP);
    	//aLedsPtr->ScannerExtended(COLOR32_WHITE, 7, 20, 0, FLAG_SCANNER_EXT_VANISH_COMPLETE,((tRandom >> 8) & DIRECTION_DOWN));
    	aLedsPtr->ScannerExtended(COLOR32_WHITE, 7, 20, 0, FLAG_SCANNER_EXT_VANISH_COMPLETE, DIRECTION_DOWN);

    	//aLedsPtr->updateAndWaitForPatternToStop();
        sState = random(0,pattCount);
        Serial.println("Rocket");
        break;
    case 5:
        aLedsPtr->RainbowCycle(50, DIRECTION_DOWN);
        sState = random(0,pattCount);
        Serial.println("RainbowCylceDown");
        break;
    case 6:
        aLedsPtr->Fire(30, 40, DIRECTION_DOWN); // OK Fire(30, 260)is also OK
        sState = random(0,pattCount);
        Serial.println("Fire");
        break;
     case 7:
        aLedsPtr->ScannerExtended(COLOR32_WHITE, 2, tDuration, 15,
         FLAG_SCANNER_EXT_CYLON , ((tRandom >> 8) & DIRECTION_DOWN));
        sState = random(0,pattCount);
        Serial.println("ScannerExtended");
        break;
    case 8:
        /*
         * Non blocking delay implemented as pattern :-)
         */
        aLedsPtr->Delay(10);
        sState = random(0,pattCount);
        Serial.println("Delay");
        break;

    default:
        sState = random(0,pattCount);
        Serial.println("ERROR");
        break;
    }

//    Serial.print(" ActiveLadderPattern=");
//    aLedsPtr->printPatternName(aLedsPtr->ActivePattern, &Serial);
//    Serial.print("|");
//    Serial.print(aLedsPtr->ActivePattern);
//    Serial.println();

    //sState++;
}

void EyePatterns(NeoPatterns *aLedsPtr) {
    //static int8_t sState = 0;
    static int8_t sState = random(0,3);
//#if defined(__AVR__)
//    uint32_t tRandom = random();
//#else
    uint32_t tRandom = random(__UINT32_MAX__);
//#endif
    uint8_t tDuration = random(20, 120);
    uint8_t tColor1 = tRandom;
    uint8_t tColor2 = tRandom >> 8;

    int randoTime = random(1000);

    switch (sState) {
    case 0:
        // Scanner - use random mode and direction

        aLedsPtr->ScannerExtended(NeoPatterns::Wheel(tColor1), 4, tDuration, 1,
                (tRandom & FLAG_SCANNER_EXT_CYLON) | (tRandom & FLAG_SCANNER_EXT_VANISH_COMPLETE)
                        | (tRandom & FLAG_SCANNER_EXT_START_AT_BOTH_ENDS), ((tRandom >> 8) & DIRECTION_DOWN));
    	sState = random(1,4);
        break;

    case 1:
        // Stripes - use random direction
        aLedsPtr->Stripes(NeoPatterns::Wheel(tColor1), 5, NeoPatterns::Wheel(tColor2), 3, 2 * aLedsPtr->numPixels(), tDuration,
                (tRandom & DIRECTION_DOWN));
        sState = random(2,4);
        break;

    case 2:
          //Standard Eye Colors;
		#ifdef LEDPAD
        aLedsPtr->setPixelColor(eyeOne, aLedsPtr->Color(90, 190, 0));
        aLedsPtr->setPixelColor(eyeTwo, aLedsPtr->Color(0, 50, 255));
        aLedsPtr->setPixelColor(eyeThree, aLedsPtr->Color(0, 50, 255));

		#else
          aLedsPtr->setPixelColor(0, aLedsPtr->Color(0, 0, 0));
          aLedsPtr->setPixelColor(1, aLedsPtr->Color(90, 90, 0));//Active
          aLedsPtr->setPixelColor(2, aLedsPtr->Color(0, 0, 0));
          aLedsPtr->setPixelColor(3, aLedsPtr->Color(0, 0, 0));
          aLedsPtr->setPixelColor(4, aLedsPtr->Color(0, 50, 255));//Active
          aLedsPtr->setPixelColor(5, aLedsPtr->Color(0, 0, 0));
          aLedsPtr->setPixelColor(6, aLedsPtr->Color(0, 0, 0));
          aLedsPtr->setPixelColor(7, aLedsPtr->Color(0, 50, 255));//Active
          aLedsPtr->setPixelColor(8, aLedsPtr->Color(0, 0, 0));
		#endif
          aLedsPtr->show();
          aLedsPtr->Delay((DELAYVAL+randoTime)*3);
          sState = random(0,4);
          break;

    case 3:
        /*
         * Non blocking delay implemented as pattern :-)
         */
         //aLedsPtr->clear();
        aLedsPtr->Delay(1000);
        sState = random(0,4);// go steady//-1; // Start from beginning
        break;

    default:
        sState =2;
        Serial.println("ERROR");
        break;
    }

//    Serial.print(" ActiveEyePattern=");
//    aLedsPtr->printPatternName(aLedsPtr->ActivePattern, &Serial);
//    Serial.print("|");
//    Serial.print(aLedsPtr->ActivePattern);
//    Serial.print(" sState: ");
//    Serial.print (sState);
//    Serial.println();


//sState++;
}
void PeriscopePatterns(NeoPatterns *aLedsPtr) {
    //static int8_t sState = 0;
    static int8_t sState = random(0,3);
//#if defined(__AVR__)
//    uint32_t tRandom = random();
//#else
    uint32_t tRandom = random(__UINT32_MAX__);
//#endif
    uint8_t tDuration = random(20, 120);
    uint8_t tColor1 = tRandom;
    uint8_t tColor2 = tRandom >> 8;

    int randoTime = random(1000);

    switch (sState) {
    case 0:
        // Scanner - use random mode and direction

        aLedsPtr->ScannerExtended(NeoPatterns::Wheel(tColor1), 4, tDuration, 1,
                (tRandom & FLAG_SCANNER_EXT_CYLON) | (tRandom & FLAG_SCANNER_EXT_VANISH_COMPLETE)
                        | (tRandom & FLAG_SCANNER_EXT_START_AT_BOTH_ENDS), ((tRandom >> 8) & DIRECTION_DOWN));
    	sState = random(1,4);
        break;

    case 1:
        // Stripes - use random direction
        aLedsPtr->Stripes(NeoPatterns::Wheel(tColor1), 5, NeoPatterns::Wheel(tColor2), 3, 2 * aLedsPtr->numPixels(), tDuration,
                (tRandom & DIRECTION_DOWN));
        sState = random(2,4);
        break;

    case 2:
          //Standard Eye Colors;
		#ifdef LEDPAD
        aLedsPtr->setPixelColor(eyeOne, aLedsPtr->Color(90, 190, 0));
        aLedsPtr->setPixelColor(eyeTwo, aLedsPtr->Color(0, 50, 255));
        aLedsPtr->setPixelColor(eyeThree, aLedsPtr->Color(0, 50, 255));

		#else
          aLedsPtr->setPixelColor(0, aLedsPtr->Color(0, 0, 0));
          aLedsPtr->setPixelColor(1, aLedsPtr->Color(90, 90, 0));//Active
          aLedsPtr->setPixelColor(2, aLedsPtr->Color(0, 0, 0));
          aLedsPtr->setPixelColor(3, aLedsPtr->Color(0, 0, 0));
          aLedsPtr->setPixelColor(4, aLedsPtr->Color(0, 50, 255));//Active
          aLedsPtr->setPixelColor(5, aLedsPtr->Color(0, 0, 0));
          aLedsPtr->setPixelColor(6, aLedsPtr->Color(0, 0, 0));
          aLedsPtr->setPixelColor(7, aLedsPtr->Color(0, 50, 255));//Active
          aLedsPtr->setPixelColor(8, aLedsPtr->Color(0, 0, 0));
		#endif
          aLedsPtr->show();
          aLedsPtr->Delay((DELAYVAL+randoTime)*3);
          sState = random(0,4);
          break;

    case 3:
        /*
         * Non blocking delay implemented as pattern :-)
         */
         //aLedsPtr->clear();
        aLedsPtr->Delay(1000);
        sState = random(0,4);// go steady//-1; // Start from beginning
        break;

    default:
        sState =2;
        Serial.println("ERROR");
        break;
    }

//    Serial.print(" ActiveEyePattern=");
//    aLedsPtr->printPatternName(aLedsPtr->ActivePattern, &Serial);
//    Serial.print("|");
//    Serial.print(aLedsPtr->ActivePattern);
//    Serial.print(" sState: ");
//    Serial.print (sState);
//    Serial.println();


//sState++;
}