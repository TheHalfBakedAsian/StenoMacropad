#include <BleKeyboard.h>
BleKeyboard Keyboard("MacroPad");
const uint8_t buttonPins[9]={2,3,4,5,6,7,8,9,10};

struct Macro{
  uint16_t mask;
  const char* text;
};

Macro macros[]={
  {0b000000001,"Button1 "},
  {0b000000010,"Button2 "},
  {0b000000100,"Button3 "},
  {0b000001000,"Button4 "},
  {0b000010000,"Button5 "},
  {0b000100000,"Button6 "},
  {0b001000000,"Button7 "},
  {0b010000000,"Button8 "},
  {0b100000000,"Button9 "},
  {0b000000011,"Combo B1+B2 "},
  {0b000000101,"Combo B1+B3 "},
  {0b000001001,"Combo B1+B4 "},
  {0b000010001,"Combo B1+B5 "},
  {0b000100001,"Combo B1+B6 "},
  {0b001000001,"Combo B1+B7 "},
  {0b010000001,"Combo B1+B8 "},
  {0b100000001,"Combo B1+B9 "}
};
//A table of all the codes for the input patterns where each bit is its own key and the value is its state

const int NUM_MACROS=sizeof(macros)/sizeof(macros[0]);
uint16_t lastMask=0;
unsigned long lastChange=0;
const unsigned long debounceTime=30;

uint16_t getButtonMask(){
  uint16_t mask=0;
  for(int i=0;i<9;i++){
    if(!digitalRead(buttonPins[i])){mask|=(1<<i);}
  }
  return mask;
}

void executeMacro(uint16_t mask){
  if(!Keyboard.isConnected()){return;}
  for(int i=0;i<NUM_MACROS;i++){
    if(macros[i].mask==mask){
      Keyboard.print(macros[i].text);
      Serial.print("Executed: ");
      Serial.println(macros[i].text);
      return;
    }
  } 
}

void setup(){
  Serial.begin(115200);
  Keyboard.begin();
  for(int i=0;i<9;i++){
    pinMode(buttonPins[i],INPUT_PULLUP);
  }
}

void loop(){
  uint16_t currentMask=getButtonMask();
  if(currentMask!=lastMask){
    lastChange=millis();
    lastMask=currentMask;
  }
  if((millis()-lastChange)>debounceTime){
    static uint16_t executedMask=0;
    if(currentMask!=0&&currentMask!=executedMask){
      executeMacro(currentMask);
      executedMask=currentMask;
    }
    if(currentMask==0){executedMask=0;}
  }
delay(1);
}