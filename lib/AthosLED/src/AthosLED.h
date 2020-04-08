//AthosLED.h

#ifndef ATH_LED_h
#define ATH_LED_h

class AthosLED {
    public:
        AthosLED();
        void LED_Setup(void);
        void LED_Loop(void);
        void LED_ON(void);
        void LED_OFF(void);
        void LED_Blink(int blinkCount, int blinkDelay);
        void LED_Blink(int blinkCount);
        void LED_Blink(void);
        
    private:

};
#endif
