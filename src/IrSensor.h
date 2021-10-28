#include <IRremote.h>
#include <Arduino.h>

//--------------------------
//  MAPPATURA TASTI TELECOMANDO
//--------------------------

struct settings
{
    bool deviceOn;
    int speed;
    int mode;
};



#define BUTTON_ON_OFF  0xFF02FD
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
#define BUTTON_PALETTE_3  0xFF32CD
#define BUTTON_PALETTE_4  0xFFF807
#define BUTTON_PALETTE_5  0xFFD827

class IrSensor{

    public:
        IrSensor(int sensorPin,int ledPin);
        settings readIR();
    private:
        settings data;
        int sensorPin;
        int ledPin;
        void nextMode();
        void previousMode();
        void increaseSpeed();
        void decreaseSpeed();
        IRrecv irrecv();
        decode_results results;
        uint32_t previousVaulue=0;      
};

IrSensor::IrSensor(int sensorPin,int ledPin){

    this->sensorPin=sensorPin;
    this->ledPin=ledPin;

    this->data.deviceOn=false;
    this->data.mode=0;
    this->data.speed=0;
}

void IrSensor::increaseSpeed(){
  if(this->data.speed>1)data.speed=--data.speed;
}

void IrSensor::decreaseSpeed(){
  if(data.speed<10)data.speed=++data.speed;
}

void IrSensor::nextMode() {
  data.mode = ++data.mode %NUMBER_OF_EFFECTS;
}

void IrSensor::previousMode(){
  if(data.mode==0)data.mode=NUMBER_OF_EFFECTS-1;
  else data.mode=--data.mode;
}

void readIR(){

  while (!irrecv.isIdle())delay(1);  // if not idle, wait till complete

  if(irrecv.decode(&results)){

    digitalWrite(LED_PIN,HIGH);
    Serial.println(results.value,HEX);

    if(results.value==0xFFFFFFFF)results.value=previousVaulue;
    else{
      previousVaulue=results.value;
      
      switch (results.value)
      {

      case BUTTON_ON_OFF:{
        if(!deviceOn){
          deviceOn=true;
          Serial.println("Device ON");
          
          delay(200);
        }
        else{
          deviceOn=false;
         Serial.println("Device OFF");
          FastLED.clear();
          FastLED.show();

          delay(200);
        }
        break;
        }

      case BUTTON_AUTOMODE:{
        if(autoMode){autoMode=false;Serial.println("AutoMode OFF");}
        else {autoMode=true;Serial.println("AutoMode ON");}
        break;
      }

      case BUTTON_SPEED_UP:{
        increaseSpeed();
        Serial.print("Speed: ");
        Serial.println(11-speed);
        break;
      }

      case BUTTON_SPEED_DOWN:{
        decreaseSpeed();
        Serial.print("Speed: ");
        Serial.println(11-speed);
        break;
      }


      //effetti
      case BUTTON_EFFECT_1:
        PushCounter=0;
        Serial.print("Mode: ");
        Serial.println(PushCounter+1);
        break;
      
      case BUTTON_EFFECT_2:
        PushCounter=1;
        Serial.print("Mode: ");
        Serial.println(PushCounter+1);
        break;
      
      case BUTTON_EFFECT_3:
        PushCounter=2;
        Serial.print("Mode: ");
        Serial.println(PushCounter+1);
        break;

      case BUTTON_EFFECT_4:
        PushCounter=3;
        Serial.print("Mode: ");
        Serial.println(PushCounter+1);
        break;

      case BUTTON_EFFECT_5:
        PushCounter=4;
        Serial.print("Mode: ");
        Serial.println(PushCounter+1);
        break;

      case BUTTON_EFFECT_6:
        PushCounter=5;
        Serial.print("Mode: ");
        Serial.println(PushCounter+1);
        break;
      
      case BUTTON_PALETTE_1:
        PushCounter=6;
        Serial.print("Mode: ");
        Serial.println(PushCounter+1);
        break;

      case BUTTON_PALETTE_2:
        PushCounter=7;
        Serial.print("Mode: ");
        Serial.println(PushCounter+1);
        break;

      case BUTTON_PALETTE_3:
        PushCounter=8;
        Serial.print("Mode: ");
        Serial.println(PushCounter+1);
        break;

      case BUTTON_PALETTE_4:
        PushCounter=9;
        Serial.print("Mode: ");
        Serial.println(PushCounter+1);
        break;

      case BUTTON_PALETTE_5:
        PushCounter=10;
        Serial.print("Mode: ");
        Serial.println(PushCounter+1);
        break;

      default:
        break;
      }
    }

    irrecv.resume();
    digitalWrite(LED_PIN,LOW);
  }
}