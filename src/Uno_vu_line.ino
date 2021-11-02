// --------------------------------
// -- VU Meter - Scott's version --
// --------------------------------

#include <IRremote.h>
#include <ColorPalette.h>
#include <FastLED.h>
#include <Noise.h>

#define LEFT_OUT_PIN 6     // Left channel data out pin to LEDs [6]
#define RIGHT_OUT_PIN 5    // Right channel data out pin to LEDs [5]
#define LEFT_IN_PIN A5     // Left aux in signal [A5]
#define RIGHT_IN_PIN A4    // Right aux in signal [A4]
#define IR_DATA_IN A1      // IR signal data
#define LED_PIN 4          // IR led pin
#define N_PIXELS 30        // Number of pixels in each string [24]
#define MAX_MILLIAMPS 500  // Maximum current to draw [500]
#define COLOR_ORDER GRB    // Colour order of LED strip [GRB]
#define LED_TYPE WS2812B   // LED string type [WS2812B]
#define DC_OFFSET 26       // DC offset in aux signal [0]
#define NOISE 2            // Noise/hum/interference in aux signal [10]
#define SAMPLES 60         // Length of buffer for dynamic level adjustment [60]
#define TOP (N_PIXELS + 2) // Allow dot to go slightly off scale [(N_PIXELS + 2)]
#define PEAK_FALL 20       // Rate of peak falling dot [20]
#define N_PIXELS_HALF (N_PIXELS / 2)
#define PATTERN_TIME 10 // Seconds to show eaach pattern on auto [10]
#define STEREO false    // If true, L&R channels are independent. If false, both L&R outputs display same data from L audio channel [false]

#define NUMBER_OF_EFFECTS 11

//-----------------------------------
//---mappatura tasti telecomando
#define BUTTON_ON_OFF 0xFF02FD
#define BUTTON_AUTOMODE 0xFFF00F
#define BUTTON_SPEED_UP 0xFFE817
#define BUTTON_SPEED_DOWN 0xFFC837
#define BUTTON_EFFECT_1 0xFF30CF
#define BUTTON_EFFECT_2 0xFFB04F
#define BUTTON_EFFECT_3 0xFF708F
#define BUTTON_EFFECT_4 0xFF10EF
#define BUTTON_EFFECT_5 0xFF906F
#define BUTTON_EFFECT_6 0xFF50AF
#define BUTTON_PALETTE_1 0xFF22DD
#define BUTTON_PALETTE_2 0xFF12ED
#define BUTTON_PALETTE_3 0xFF32CD
#define BUTTON_PALETTE_4 0xFFF807
#define BUTTON_PALETTE_5 0xFFD827

uint8_t volCountLeft = 0; // Frame counter for storing past volume data
int volLeft[SAMPLES];     // Collection of prior volume samples
int lvlLeft = 0;          // Current "dampened" audio level
int minLvlAvgLeft = 0;    // For dynamic adjustment of graph low & high
int maxLvlAvgLeft = 512;

uint8_t volCountRight = 0; // Frame counter for storing past volume data
int volRight[SAMPLES];     // Collection of prior volume samples
int lvlRight = 0;          // Current "dampened" audio level
int minLvlAvgRight = 0;    // For dynamic adjustment of graph low & high
int maxLvlAvgRight = 512;

CRGB ledsLeft[N_PIXELS];
CRGB ledsRight[N_PIXELS];

uint8_t myhue = 0;

void vu6(bool is_centered, uint8_t channel);
void vu7(bool show_background);
void fire();
void sinelon();
void rainbow(uint8_t rate);
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay, CRGB leds[]);

// --------------------
// ---  Stuff ---
// --------------------
uint8_t state = 0;
int PushCounter = 0;
bool autoMode = false;
bool deviceOn = false;
int speed = 5;
ColorPalette colorPalette = ColorPalette();
Noise noise = Noise();

//----IR------
IRrecv irrecv(IR_DATA_IN);
decode_results results;
uint32_t previousVaulue = 0;

void increaseSpeed()
{
  if (speed > 1)
    speed = --speed;
}

void decreaseSpeed()
{
  if (speed < 10)
    speed = ++speed;
}

void incrementPushCounter()
{
  PushCounter = ++PushCounter % NUMBER_OF_EFFECTS;
}

void decrementPushCounter()
{
  if (PushCounter == 0)
    PushCounter = NUMBER_OF_EFFECTS - 1;
  else
    PushCounter = --PushCounter;
}

void readIR()
{

  while (!irrecv.isIdle())
    delay(1); // if not idle, wait till complete

  if (irrecv.decode(&results))
  {

    digitalWrite(LED_PIN, HIGH);
    Serial.println(results.value, HEX);

    if (results.value == 0xFFFFFFFF)
      results.value = previousVaulue;
    else
    {
      previousVaulue = results.value;

      switch (results.value)
      {

      case BUTTON_ON_OFF:
      {
        if (!deviceOn)
        {
          deviceOn = true;
          Serial.println("Device ON");

          delay(200);
        }
        else
        {
          deviceOn = false;
          Serial.println("Device OFF");
          FastLED.clear();
          FastLED.show();

          delay(200);
        }
        break;
      }

      case BUTTON_AUTOMODE:
      {
        if (autoMode)
        {
          autoMode = false;
          Serial.println("AutoMode OFF");
        }
        else
        {
          autoMode = true;
          Serial.println("AutoMode ON");
        }
        break;
      }

      case BUTTON_SPEED_UP:
      {
        increaseSpeed();
        Serial.print("Speed: ");
        Serial.println(11 - speed);
        break;
      }

      case BUTTON_SPEED_DOWN:
      {
        decreaseSpeed();
        Serial.print("Speed: ");
        Serial.println(11 - speed);
        break;
      }

      //effetti
      case BUTTON_EFFECT_1:
        PushCounter = 0;
        Serial.print("Mode: ");
        Serial.println(PushCounter + 1);
        break;

      case BUTTON_EFFECT_2:
        PushCounter = 1;
        Serial.print("Mode: ");
        Serial.println(PushCounter + 1);
        break;

      case BUTTON_EFFECT_3:
        PushCounter = 2;
        Serial.print("Mode: ");
        Serial.println(PushCounter + 1);
        break;

      case BUTTON_EFFECT_4:
        PushCounter = 3;
        Serial.print("Mode: ");
        Serial.println(PushCounter + 1);
        break;

      case BUTTON_EFFECT_5:
        PushCounter = 4;
        Serial.print("Mode: ");
        Serial.println(PushCounter + 1);
        break;

      case BUTTON_EFFECT_6:
        PushCounter = 5;
        Serial.print("Mode: ");
        Serial.println(PushCounter + 1);
        break;

      case BUTTON_PALETTE_1:
        PushCounter = 6;
        Serial.print("Mode: ");
        Serial.println(PushCounter + 1);
        break;

      case BUTTON_PALETTE_2:
        PushCounter = 7;
        Serial.print("Mode: ");
        Serial.println(PushCounter + 1);
        break;

      case BUTTON_PALETTE_3:
        PushCounter = 8;
        Serial.print("Mode: ");
        Serial.println(PushCounter + 1);
        break;

      case BUTTON_PALETTE_4:
        PushCounter = 9;
        Serial.print("Mode: ");
        Serial.println(PushCounter + 1);
        break;

      case BUTTON_PALETTE_5:
        PushCounter = 10;
        Serial.print("Mode: ");
        Serial.println(PushCounter + 1);
        break;

      default:
        break;
      }
    }

    irrecv.resume();
    digitalWrite(LED_PIN, LOW);
  }
}

void setup()
{
  delay(1000); // power-up safety delay

  FastLED.addLeds<LED_TYPE, LEFT_OUT_PIN, COLOR_ORDER>(ledsLeft, N_PIXELS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, RIGHT_OUT_PIN, COLOR_ORDER>(ledsRight, N_PIXELS).setCorrection(TypicalLEDStrip);
  //FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_MILLIAMPS);

  PushCounter = 0;

  analogReference(INTERNAL);
  irrecv.enableIRIn();
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.print("Starting pattern ");
  Serial.println(PushCounter);

  FastLED.clear();
  FastLED.show();

  delay(500);
}

void loop()
{

  //leggo segnale infrarossi
  readIR();

  if (deviceOn)
  {

    if (autoMode)
    {
      colorPalette.autoRunPalette(ledsLeft, PATTERN_TIME);
      copyLeftToRight();
    }
    else
    {
      // Run selected pattern
      switch (PushCounter)
      {

      case 0:
        vu6(true, 0);
        if (STEREO)
          vu6(false, 1);
        else
          copyLeftToRight();
        break;

      case 1:
        vu7(true);
        copyLeftToRight();
        break;

      case 2:
        sinelon();
        break;

      case 3:
        fire();
        break;

      case 4:
        rainbow(12 - speed);
        break;

      case 5:
        noise.runFire(ledsLeft);
        copyLeftToRight();
        break;

      case 6:
        colorPalette.runPalette(ledsLeft, 0, false);
        copyLeftToRight();
        break;

      case 7:
        colorPalette.runPalette(ledsLeft, 1, false);
        copyLeftToRight();
        break;

      case 8:
        colorPalette.runPalette(ledsLeft, 2, false);
        copyLeftToRight();
        break;

      case 9:
        colorPalette.runPalette(ledsLeft, 3, false);
        copyLeftToRight();
        break;

      case 10:
        colorPalette.runPalette(ledsLeft, 4, false);
        copyLeftToRight();
        break;
      }
    }
  }
}

// ------------------
// -- VU functions --
// ------------------


uint16_t auxReading(uint8_t channel) {

  int n = 0;
  uint16_t height = 0;

  if(channel == 0) {
    int n = analogRead(LEFT_IN_PIN); // Raw reading from left line in
    n = abs(n - 512 - DC_OFFSET); // Center on zero
    n = (n <= NOISE) ? 0 : (n - NOISE); // Remove noise/hum
    lvlLeft = ((lvlLeft * 7) + n) >> 3; // "Dampened" reading else looks twitchy (>>3 is divide by 8)
    volLeft[volCountLeft] = n; // Save sample for dynamic leveling
    volCountLeft = ++volCountLeft % SAMPLES;
    // Calculate bar height based on dynamic min/max levels (fixed point):
    height = TOP * (lvlLeft - minLvlAvgLeft) / (long)(maxLvlAvgLeft - minLvlAvgLeft);
  }
  
  else {
    int n = analogRead(RIGHT_IN_PIN); // Raw reading from mic
    n = abs(n - 512 - DC_OFFSET); // Center on zero
    n = (n <= NOISE) ? 0 : (n - NOISE); // Remove noise/hum
    lvlRight = ((lvlRight * 7) + n) >> 3; // "Dampened" reading (else looks twitchy)
    volRight[volCountRight] = n; // Save sample for dynamic leveling
    volCountRight = ++volCountRight % SAMPLES;
    // Calculate bar height based on dynamic min/max levels (fixed point):
    height = TOP * (lvlRight - minLvlAvgRight) / (long)(maxLvlAvgRight - minLvlAvgRight);
  }

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = constrain(height, 0, TOP);
  return height;
}

void copyLeftToRight() {
  for (uint8_t i = 0; i < N_PIXELS; i++) {
    ledsRight[i] = ledsLeft[i];
  }
}

/*
 * Function for dropping the peak
 */
uint8_t peakLeft, peakRight;
void dropPeak(uint8_t channel) {
  
  static uint8_t dotCountLeft, dotCountRight;
 
  if(channel == 0) {
    if(++dotCountLeft >= PEAK_FALL) { //fall rate 
      if(peakLeft > 0) peakLeft--;
      dotCountLeft = 0;
    }
  } else {
    if(++dotCountRight >= PEAK_FALL) { //fall rate 
      if(peakRight > 0) peakRight--;
      dotCountRight = 0;
    }
  }
}

/*
 * Function for averaging the sample readings
 */
void averageReadings(uint8_t channel) {

  uint16_t minLvl, maxLvl;

  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if(channel == 0) {
    minLvl = maxLvl = volLeft[0];
    for (int i = 1; i < SAMPLES; i++) {
      if (volLeft[i] < minLvl) minLvl = volLeft[i];
      else if (volLeft[i] > maxLvl) maxLvl = volLeft[i];
    }
    if ((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
    
    minLvlAvgLeft = (minLvlAvgLeft * 63 + minLvl) >> 6; // Dampen min/max levels
    maxLvlAvgLeft = (maxLvlAvgLeft * 63 + maxLvl) >> 6; // (fake rolling average)
  }

  else {
    minLvl = maxLvl = volRight[0];
    for (int i = 1; i < SAMPLES; i++) {
      if (volRight[i] < minLvl) minLvl = volRight[i];
      else if (volRight[i] > maxLvl) maxLvl = volRight[i];
    }
    if ((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
    minLvlAvgRight = (minLvlAvgRight * 63 + minLvl) >> 6; // Dampen min/max levels
    maxLvlAvgRight = (maxLvlAvgRight * 63 + maxLvl) >> 6; // (fake rolling average)
  }
}
