#pragma once
#include <Arduino.h>
#include <FirebaseESP32.h>

void initFirebase(FirebaseConfig* cfg, FirebaseAuth* auth);
int  fbGetInt(FirebaseData* fb, String path, int def=0);
void fbSetInt(FirebaseData* fb, String path, int v);
void fbSetFloat(FirebaseData* fb, String path, float v);
void fbPushWarning(FirebaseData* fb, String path,  String garden,  String type, float val, String msg, time_t t);
