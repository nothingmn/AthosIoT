#ifdef ATH_LED

#define ATHLED_PIN D0 //Led in NodeMCU at pin GPIO16 (D0) 
#define ATHLED_DEFAULT_DELAY 1000

void LED_Setup()
{
    pinMode(ATHLED_PIN, OUTPUT); //LED pin as output
}
void LED_Loop()
{
}

void LED_Blink(int onDelay) {
    digitalWrite(ATHLED_PIN, LOW); //turn the led on
    delay(onDelay); //wait for 1 sec
    digitalWrite(ATHLED_PIN, HIGH); //turn the led off
}


void LED_Blink() {
  LED_Blink(ATHLED_DEFAULT_DELAY);
}


#endif
