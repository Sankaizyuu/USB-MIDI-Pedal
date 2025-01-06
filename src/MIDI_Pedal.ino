#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

#include "RotaryEncoder.h"
#include "PedalMonitor.h"

#define EXP_PIN 28

#define CLK_PIN 15
#define DT_PIN 26
#define RE_SW_PIN 27

#define SCL1_PIN 3
#define SDA1_PIN 2

#define SCL0_PIN 5
#define SDA0_PIN 4

#define PEDAL1_PIN 13
#define PEDAL2_PIN 14
#define PEDAL3_PIN 10
#define PEDAL4_PIN 9
#define PEDAL5_PIN 12
#define PEDAL6_PIN 11

#define WS2812_PIN 16

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define FONT_WIDTH 8
#define FONT_HEIGHT 6

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

#define PEDAL_NUMBER 6

#define EXP_CH 0

const int PEDAL_PIN[] = {PEDAL1_PIN, PEDAL2_PIN, PEDAL3_PIN, PEDAL4_PIN, PEDAL5_PIN, PEDAL6_PIN};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_NeoPixel pixels(1, WS2812_PIN, NEO_GRB + NEO_KHZ800);

RotaryEncoder myRE(DT_PIN, CLK_PIN, RE_SW_PIN, 1000, 100);

PedalMonitor myPedals(PEDAL_PIN, PEDAL_NUMBER);

Adafruit_USBD_MIDI usb_midi;

MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

enum MenuState{
  DEDFALUT,
  MAIN_MENU,
  ASSIGN_MENU,
  PEDAL_MENU,
  PC_MENU,
  CC_MENU,
  EXPCC_MENU,
  MENU_COUNT
};

const int menuItemCount[MENU_COUNT]{
  0,
  3,
  8,
  3,
  3,
  4,
  3
};

int PedalControlNumber[] = {1,2,3,4,5,6};

int ControlChangeValueOn = 127;
int ControlChangeValueOff = 0;


MenuState currentMenuState = MAIN_MENU;

int currentSelection = 0;

int _test_value = 0;

void onEncorderChange(){

  if(myRE.getDirection() > 0){
    currentSelection++;
  }else if(myRE.getDirection() < 0){
    currentSelection--;
  }

  if(currentSelection < 0){
    currentSelection = 0;
  }else if(currentSelection > menuItemCount[currentMenuState] - 1){
    currentSelection = menuItemCount[currentMenuState] - 1;
  }

  switch (currentMenuState) {
    case MAIN_MENU:
      drawMainMenu(currentSelection);
      break;

    case ASSIGN_MENU:
      drawAssignMenu(currentSelection);
      break;
  }
  
}

void onSwitchChange(){
  switch (currentMenuState) {
    case MAIN_MENU:
      if(currentSelection == 0){

      }else if(currentSelection == 1){
        currentMenuState = ASSIGN_MENU;
        drawAssignMenu(currentSelection);
        currentSelection = 0;
      }else if(currentSelection == 2){
        
      } 
      break;
  }
}

void onPedalChange(int switchIndex, int state) {
    MIDI.sendControlChange(PedalControlNumber[switchIndex], state ? ControlChangeValueOff : ControlChangeValueOn, 1);
}

void setup() {
  Serial.begin(115200);

  pixels.begin();

  display_init();

  myRE.attachRotaryCallback(onEncorderChange);
  myRE.attachSwitchCallback(onSwitchChange);

  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(128, 0, 0));
  pixels.show();
}

void setup1(){

  myPedals.setCallback(onPedalChange);

  usb_midi.setStringDescriptor("MIDI Pedal");

  MIDI.begin(1);

  while( !TinyUSBDevice.mounted() ) delay(1);
}

void loop() {
  myRE.checkEncoder();
  myRE.checkSwitch();
}

void loop1(){
  myPedals.update();


/*
  if(changeFlag == true){
    if(currentSwState == false){
      MIDI.sendControlChange(2, 100, 1);
    }else{
      MIDI.sendControlChange(2, 0, 1);
    }
  }
*/
}
