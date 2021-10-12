#include <FastLED.h>

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay,CRGB leds[]) {  
  
  for(int i=0;i<30;i++){
      leds[i]=CRGB(0,0,0);
  }
 
  for(int i = 0; i < 30+30; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<30; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        leds[j].fadeToBlackBy(meteorTrailDecay);
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <30) && (i-j>=0) ) {
        leds[i-j]=CRGB(red, green, blue);
      }
    }
   
    FastLED.show();
    delay(SpeedDelay);
  }
}