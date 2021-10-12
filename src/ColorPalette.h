#include <Arduino.h>
#include <FastLED.h>

#define N_LEDS 30

//palettes
    DEFINE_GRADIENT_PALETTE( GMT_hot_gp ) {
        0,   0,  0,  0,
        95, 255,  0,  0,
        191, 255,255,  0,
        255, 255,255,255};
    DEFINE_GRADIENT_PALETTE( xkcd_bath_gp ) {
        0,   1,  1,  1,
        59,   1,  1,  1,
        157,   2, 24, 72,
        255,  33,211,255};
    DEFINE_GRADIENT_PALETTE( bhw1_06_gp ) {
        0, 184,  1,128,
    160,   1,193,182,
    219, 153,227,190,
    255, 255,255,255};
    DEFINE_GRADIENT_PALETTE( bhw2_turq_gp ) {
        0,   1, 33, 95,
    38,   1,107, 37,
    76,  42,255, 45,
    127, 255,255, 45,
    178,  42,255, 45,
    216,   1,107, 37,
    255,   1, 33, 95};
    DEFINE_GRADIENT_PALETTE( GMT_seis_gp ) {
        0,  88,  0,  0,
    28, 255,  0,  0,
    56, 255, 22,  0,
    85, 255,104,  0,
    113, 255,255,  0,
    141, 255,255,  0,
    169,  17,255,  1,
    198,   0,223, 31,
    226,   0, 19,255,
    255,   0,  0,147};

class ColorPalette{
    public:
        ColorPalette(){
            createPalettes();
        };

        void runPalette(CRGB[],int,bool);
        void autoRunPalette(CRGB[],int);

    private:
        CRGBPalette16 paletteArray[5]={GMT_hot_gp,xkcd_bath_gp,bhw1_06_gp,bhw1_06_gp,GMT_seis_gp};
        uint8_t colorIndex[N_LEDS];
        int paletteIndex;
        CRGBPalette16 currentPalette;
        void createPalettes();

};

void ColorPalette::createPalettes(){

    for(int i=0;i<N_LEDS;i++){
        colorIndex[i]=random8();
    }
    paletteIndex=0;
}

void ColorPalette::autoRunPalette(CRGB leds[],int switchTime){

    EVERY_N_SECONDS(switchTime){
        paletteIndex=(++paletteIndex)%5;
        Serial.print("Automode-palette: ");
        Serial.println(paletteIndex+1);
    } 
    
    runPalette(leds,paletteIndex,true);
}

void ColorPalette::runPalette(CRGB leds[],int index,bool autoMode){

    if(autoMode)nblendPaletteTowardPalette(currentPalette,paletteArray[index+1],10);
    else currentPalette=paletteArray[index];

    for(int i=0;i<N_LEDS;i++){
        leds[i]=ColorFromPalette(currentPalette,colorIndex[i]);
    }

    EVERY_N_MILLISECONDS(5){
        for(int i=0;i<N_LEDS;i++){
            colorIndex[i]++;
        }
    }

    FastLED.show();
}