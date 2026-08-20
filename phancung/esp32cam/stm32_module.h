#pragma once
#include <Arduino.h>

void initSTM32(int rx, int tx);
void sendMode(int m);
void sendBulb(int v);
void sendFan(int v);
void sendPump(int v);
void sendSpray(int v);

void sendFan2(int st);
void sendPump2(int st);
void sendFan3(int v);
void sendPump3(int v);

void requestState();
