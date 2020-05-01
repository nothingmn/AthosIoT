#ifndef ATH_NEOPIXEL_EFFECTS_
#define ATH_NEOPIXEL_EFFECTS_

#include "Arduino.h";
#include <Adafruit_NeoPixel.h>

void PixelEffect_on(Adafruit_NeoPixel &strip){
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      strip.setPixelColor(i, 255,255,255,255);
    }
    strip.show(); // Update strip with new contents
}
void PixelEffect_off(Adafruit_NeoPixel &strip){
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      strip.setPixelColor(i, 0,0,0,0);
    }
    strip.show(); // Update strip with new contents
}

void PixelEffect_dim(Adafruit_NeoPixel &strip, uint8_t b){
    strip.setBrightness(b); // Set BRIGHTNESS to about 1/5 (max = 255)
    strip.show(); // Update strip with new contents
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void PixelEffect_rainbow(Adafruit_NeoPixel &strip, int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}


void PixelEffect_rainbowFade2White(Adafruit_NeoPixel &strip, int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;firstPixelHue += 256) {

    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    strip.show();
    delay(wait);

    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }

  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
    delay(1000); // Pause 1 second
    for(int j=255; j>=0; j--) { // Ramp down 255 to 0
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
  }

  delay(500); // Pause 1/2 second
}
void PixelEffect_pulseColor(Adafruit_NeoPixel &strip, uint32_t r, uint32_t g, uint32_t b, uint8_t wait, uint8_t pulses) {
    for(int p = 0;p<pulses;p++) {
        for(int j=0; j<256; j++) { // Ramp up from 0 to 255
            // Fill entire strip with white at gamma-corrected brightness level 'j':
            strip.fill(strip.Color(r, g, b, strip.gamma8(j)));
            strip.show();
            delay(wait);
        }

        for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
            strip.fill(strip.Color(r, g, b, strip.gamma8(j)));
            strip.show();
            delay(wait);
        }
    }
}

void PixelEffect_pulseWhiteWithColor(Adafruit_NeoPixel &strip, uint32_t r, uint32_t g, uint32_t b, uint8_t wait, uint8_t pulses) {
    for(int p = 0;p<pulses;p++) {
        for(int j=0; j<256; j++) { // Ramp up from 0 to 255
            // Fill entire strip with white at gamma-corrected brightness level 'j':
            strip.fill(strip.Color(r, g, b, strip.gamma8(j)));
            strip.show();
            delay(wait);
        }

        for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
            strip.fill(strip.Color(r, g, b, strip.gamma8(j)));
            strip.show();
            delay(wait);
        }
    }
}

void PixelEffect_pulseWhite(Adafruit_NeoPixel &strip, uint8_t wait, uint8_t pulses) {
    for(int p = 0;p<pulses;p++) {
        for(int j=0; j<256; j++) { // Ramp up from 0 to 255
            // Fill entire strip with white at gamma-corrected brightness level 'j':
            strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
            strip.show();
            delay(wait);
        }

        for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
            strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
            strip.show();
            delay(wait);
        }
    }
}


void PixelEffect_whiteOverRainbow(Adafruit_NeoPixel &strip, int whiteSpeed, int whiteLength) {

  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<strip.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }

    strip.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}
// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void PixelEffect_colorWipe(Adafruit_NeoPixel &strip, uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void PixelEffect_fillColor(Adafruit_NeoPixel &strip, uint32_t r, uint32_t g, uint32_t b, uint32_t w, int wait) {
    PixelEffect_colorWipe(strip, strip.Color(r, g, b, w), wait);
}

#endif