#include "stm32_module.h"

void initSTM32(int rx, int tx){
    Serial1.begin(9600, SERIAL_8N1, rx, tx);
}

void sendMode(int m){ Serial1.println(m==1?"MODE_AUTO":"MODE_MANUAL"); }
void sendBulb(int v){ Serial1.println(v?"BULB_ON":"BULB_OFF"); }
void sendFan(int v){ Serial1.println(v?"FAN_ON":"FAN_OFF"); }
void sendPump(int v){ Serial1.println(v?"PUMP_ON":"PUMP_OFF"); }
void sendSpray(int v){ Serial1.println(v?"SPRAY_ON":"SPRAY_OFF"); }
void sendFan2(int st){
  Serial1.println(st ? "FAN2_ON" : "FAN2_OFF");
}

void sendPump2(int st){
  Serial1.println(st ? "PUMP2_ON" : "PUMP2_OFF");
}

void sendFan3(int v){
  Serial1.println(v ? "FAN3_ON" : "FAN3_OFF");
}

void sendPump3(int v){
  Serial1.println(v ? "PUMP3_ON" : "PUMP3_OFF");
}

void requestState(){
    Serial1.println("REQUEST_STATE");
}
