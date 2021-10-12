#define NUM_LEDS 30

DEFINE_GRADIENT_PALETTE( firepal ) {    // define fire palette
   0,     0,  0,  0,   //black
  32,   255,  0,  0,  // red
  190, 255, 255, 0, //yellow
  255,   255, 255, 255  // white
};

class Noise{

    public:
        Noise(){
            firePalette=firepal;
        };
        void    runFire(CRGB[]);
        void    runRainbowNoise(CRGB[]);

    private:
        CRGBPalette16 firePalette;
        uint16_t  x;
        int scale;
        uint16_t  t;
};

void Noise::runFire(CRGB leds[]) {
  int  a = millis();
  
  for (int i = 0; i < NUM_LEDS; i++) {

    // 3D noise, x is constant, we move through time on y and z axis
    // The 60 here will change the scale of the effect, lower is smoother
    // higher is more flickery. The time value for z was in the original code
    // as that was a 2D matrix version. I've left it in here as it looks 
    // nice in 1D too!
    uint8_t noise = inoise8 (0 , i * 60 + a , a / 3);

    // Divides 255 by (NUM_LEDS - 1) and subtracts that amount away from 255 to return 
    // a decreasing number each time e.g. for NUM_LEDS = 18, difference between
    // each point is 15, so:
    // i = 0, math = 255
    // i = 1, math = 240
    // i = 2, math = 225
    // ...
    // i = NUM_LEDS, math =  0
    uint8_t math = abs8(i - (NUM_LEDS-1)) * 255 / (NUM_LEDS-1);

    // Take the noise function from earlier and subtract math from it,
    // so the further along the strip you are, the higher palette index you
    // are likely to get. This results in the base of the fire (white, yellow)
    // being at the far end of the strip
    uint8_t index = qsub8 (noise, math);

    // Set the LED color from the palette
    leds[i] = ColorFromPalette (firePalette, index, 255);    
  }  

  FastLED.show();
  delay(2);
}

void Noise::runRainbowNoise(CRGB leds[]){

    x = 0;
    t = millis() / 5;
    scale = beatsin8(10, 10, 30);
  
    for (int i = 0; i < NUM_LEDS; i++) {
        uint8_t noise = inoise8(i * scale + x, t);
        uint8_t hue = map(noise, 50, 190, 0, 255);
        leds[i] = CHSV(hue, 255, 255);
    }

    FastLED.show();
    delay(2);
}