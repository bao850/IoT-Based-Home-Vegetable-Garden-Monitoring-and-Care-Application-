#include "firebase_module.h"

void initFirebase(FirebaseConfig* cfg, FirebaseAuth* auth){
    Firebase.begin(cfg, auth);
    Firebase.reconnectWiFi(true);
}

int fbGetInt(FirebaseData* fb, String path, int def){
    if(Firebase.getInt(*fb, path)) return fb->intData();
    return def;
}

void fbSetInt(FirebaseData* fb, String path, int v){
    Firebase.setInt(*fb, path, v);
}

void fbSetFloat(FirebaseData* fb, String path, float v){
    Firebase.setFloat(*fb, path, v);
}

void fbPushWarning(FirebaseData* fb, String path, String garden, String type, float val, String msg, time_t t){
    FirebaseJson j;

    j.set("garden", garden);   // 🔥 TÊN VƯỜN
    j.set("type", type);         // light / temp / humi / soil
    j.set("msg", msg);           // nội dung cảnh báo
    j.set("value", val);         // giá trị đo được
    j.set("time", t);            // timestamp

    Firebase.pushJSON(*fb, path, j);
}

