#ifndef ATH_LED
#define ATH_LED



#define ATHLED_PIN LED_BUILTIN //Led in NodeMCU at pin GPIO16 (D0) 
#define ATHLED_DEFAULT_DELAY 50
#define ATHLED_DEFAULT_COUNT 5

void LED_Setup()
{
    pinMode(ATHLED_PIN, OUTPUT); //LED pin as output
}
void LED_Loop()
{
}

void LED_ON() {
    digitalWrite(ATHLED_PIN, LOW); //turn the led on
}

void LED_OFF() {
    digitalWrite(ATHLED_PIN, HIGH); //turn the led on
}

void LED_Blink(int blinkCount, int blinkDelay) {

  for(int x=0;x<blinkCount;x++) {
    digitalWrite(ATHLED_PIN, LOW); //turn the led on
    delay(blinkDelay); //wait for 1 sec
    digitalWrite(ATHLED_PIN, HIGH); //turn the led off
    delay(blinkDelay); //wait for 1 sec
  }
}


void LED_Blink(int blinkCount) {
  LED_Blink(blinkCount, ATHLED_DEFAULT_DELAY);
}

void LED_Blink() {
  LED_Blink(ATHLED_DEFAULT_COUNT, ATHLED_DEFAULT_DELAY);
}

#endif