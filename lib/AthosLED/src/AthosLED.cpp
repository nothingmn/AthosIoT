//AthosLED.cpp
#include "AthosLED.h"
#include <Arduino.h>

#define ATHLED_PIN LED_BUILTIN //Led in NodeMCU at pin GPIO16 (D0) 
#define ATHLED_DEFAULT_DELAY 50
#define ATHLED_DEFAULT_COUNT 5

void AthosLED::LED_Setup(void) {
    pinMode(ATHLED_PIN, OUTPUT); //LED pin as output
}
void AthosLED::LED_Loop(void) {
}

void AthosLED::LED_ON(void) {
    digitalWrite(ATHLED_PIN, LOW); //turn the led on
}

void AthosLED::LED_OFF(void) {
    digitalWrite(ATHLED_PIN, HIGH); //turn the led on
}

void AthosLED::LED_Blink(int blinkCount, int blinkDelay) {

  for(int x=0;x<blinkCount;x++) {
    digitalWrite(ATHLED_PIN, LOW); //turn the led on
    delay(blinkDelay); //wait for 1 sec
    digitalWrite(ATHLED_PIN, HIGH); //turn the led off
    delay(blinkDelay); //wait for 1 sec
  }
}


void AthosLED::LED_Blink(int blinkCount) {
  LED_Blink(blinkCount, ATHLED_DEFAULT_DELAY);
}

void AthosLED::LED_Blink(void) {
  LED_Blink(ATHLED_DEFAULT_COUNT, ATHLED_DEFAULT_DELAY);
}


/*
  Constructor
*/
AthosLED::AthosLED()
{
}

