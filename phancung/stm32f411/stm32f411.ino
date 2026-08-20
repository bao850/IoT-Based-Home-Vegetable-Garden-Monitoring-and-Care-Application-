#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>   // ⭐ LCD

// ================= LCD =================
LiquidCrystal_I2C lcd(0x27, 20, 4);   // Nếu không hiện → đổi 0x3F


// ================= PIN =================
#define DHTPIN PB10
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define BULB_PIN  PB3
#define FAN_PIN   PA15
#define PUMP_PIN  PB15
#define SPRAY_PIN PB4
// ===== PIN VƯỜN 2 =====
#define FAN2_PIN   PA12
#define PUMP2_PIN  PB14

bool fan2State  = false;
bool pump2State = false;
// ===== PIN VƯỜN 3 =====
#define FAN3_PIN   PA8
#define PUMP3_PIN  PB13

bool fan3State  = false;
bool pump3State = false;
bool warnLightState = false;
bool warnHumiState  = false;
bool warnTemp1 = false;
bool warnTemp2 = false;
bool warnTemp3 = false;

bool warnSoil1 = false;
bool warnSoil2 = false;
bool warnSoil3 = false;
int waterPercent = 0;



#define LIGHT_A   PA0
//#define LIGHT_D   PA2
#define TRIG_PIN PB0
#define ECHO_PIN PA7
#define TANK_HEIGHT_CM 38.0



// ================= STATE =================

enum Mode {
  MODE_MANUAL,
  MODE_AUTO
};

Mode mode1 = MODE_MANUAL;
Mode mode2 = MODE_MANUAL;
Mode mode3 = MODE_MANUAL;

inline bool anyAuto(){
  return (mode1==MODE_AUTO) || (mode2==MODE_AUTO) || (mode3==MODE_AUTO);
}



bool bulbState = false;
bool fanState  = false;
bool pumpState = false;
bool sprayState= false;

int soil1Percent = 0; 
int soil2Percent = 0; 
int soil3Percent = 0;


// last send timer

unsigned long lastDhtSend   = 0;
unsigned long lastSoilSend = 0;
unsigned long lastSoil2Send = 0;
unsigned long lastSoil3Send = 0;



// ================= THRESHOLD (NHẬN TỪ ESP32) =================

// COMMON
int th_light_min, th_light_max;
int th_humi_min,  th_humi_max;

// VƯỜN 1
int th_t1_min, th_t1_max;
int th_s1_min, th_s1_max;

// VƯỜN 2
int th_t2_min, th_t2_max;
int th_s2_min, th_s2_max;

// VƯỜN 3
int th_t3_min, th_t3_max;
int th_s3_min, th_s3_max;




// ================= UART SEND =================
void sendState(String key, int st) {
  Serial1.println(key + ":" + String(st));
}

void sendAllStates() {
  sendState("BULB_STATE",  bulbState ? 1 : 0);
  sendState("SPRAY_STATE", sprayState? 1 : 0);

  sendState("FAN_STATE",   fanState  ? 1 : 0);
  sendState("PUMP_STATE",  pumpState ? 1 : 0);

  sendState("FAN2_STATE",  fan2State ? 1 : 0);
  sendState("PUMP2_STATE", pump2State ? 1 : 0);

  sendState("FAN3_STATE",  fan3State ? 1 : 0);
  sendState("PUMP3_STATE", pump3State ? 1 : 0);
}



// ================= DISPLAY LCD =================
void lcdDisplayAll(float t, float h, int light) {


  // ===== DÒNG 1 =====
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print((int)t);
  lcd.print((char)223); // ký hiệu độ °
  lcd.print("C    ");
  lcd.print("H:");
  lcd.print((int)h);
  lcd.print("%");

  // ===== DÒNG 2 =====
  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(light);
  lcd.print("LUX ");
    lcd.setCursor(10, 1);
  lcd.print("S1:");
  lcd.print(soil1Percent);
  lcd.print("%");

  // ===== DÒNG 3 =====
lcd.setCursor(0, 2);
lcd.print("S2:");
lcd.print(soil2Percent);
lcd.print("%");

lcd.setCursor(10, 2);
lcd.print("S3:");
lcd.print(soil3Percent);
lcd.print("%");


  // ===== DÒNG 4 =====
  lcd.setCursor(0, 3);
lcd.print("MODE:");
lcd.print(anyAuto() ? "AUTO" : "MANU");

   lcd.setCursor(10, 3);
 lcd.print("WATER:");

if (waterPercent >= 65) {
  lcd.print("FULL ");
}
else if (waterPercent >= 35) {
  lcd.print("LOW  ");
}
else {
  lcd.print("EMPT");
}

}






// ================= AUTO CONTROL =================
void autoControlAll(float temp, float humi, int soil1, int soil2, int soil3, int light) {

  // =======================
  // 1) ĐÈN + PHUN CHUNG
  // =======================
  if (anyAuto()) {

 // BULB – bật khi tối
if (!bulbState && light > th_light_max) {
  bulbState = true;
  digitalWrite(BULB_PIN, HIGH);
  sendState("BULB_STATE", 1);
}

// BULB – tắt khi sáng
if (bulbState && light < th_light_min) {
  bulbState = false;
  digitalWrite(BULB_PIN, LOW);
  sendState("BULB_STATE", 0);
}


    // SPRAY
    if (!sprayState && humi < th_humi_min) {
      sprayState = true;
      digitalWrite(SPRAY_PIN, HIGH);
      sendState("SPRAY_STATE", 1);
    }
    if (sprayState && humi > th_humi_max) {
      sprayState = false;
      digitalWrite(SPRAY_PIN, LOW);
      sendState("SPRAY_STATE", 0);
    }
  }

  // =======================
  // 2) VƯỜN 1
  // =======================
  if (mode1 == MODE_AUTO) {

    if (!fanState && temp > th_t1_max) {
      fanState = true;
      digitalWrite(FAN_PIN, HIGH);
      sendState("FAN_STATE", 1);
    }
    if (fanState && temp < th_t1_min) {
      fanState = false;
      digitalWrite(FAN_PIN, LOW);
      sendState("FAN_STATE", 0);
    }

    if (!pumpState && soil1 < th_s1_min) {
      pumpState = true;
      digitalWrite(PUMP_PIN, HIGH);
      sendState("PUMP_STATE", 1);
    }
    if (pumpState && soil1 > th_s1_max) {
      pumpState = false;
      digitalWrite(PUMP_PIN, LOW);
      sendState("PUMP_STATE", 0);
    }
  }

  // =======================
  // 3) VƯỜN 2
  // =======================
  if (mode2 == MODE_AUTO) {

    if (!fan2State && temp > th_t2_max) {
      fan2State = true;
      digitalWrite(FAN2_PIN, HIGH);
      sendState("FAN2_STATE", 1);
    }
    if (fan2State && temp < th_t2_min) {
      fan2State = false;
      digitalWrite(FAN2_PIN, LOW);
      sendState("FAN2_STATE", 0);
    }

    if (!pump2State && soil2 < th_s2_min) {
      pump2State = true;
      digitalWrite(PUMP2_PIN, HIGH);
      sendState("PUMP2_STATE", 1);
    }
    if (pump2State && soil2 > th_s2_max) {
      pump2State = false;
      digitalWrite(PUMP2_PIN, LOW);
      sendState("PUMP2_STATE", 0);
    }
  }

  // =======================
  // 4) VƯỜN 3
  // =======================
  if (mode3 == MODE_AUTO) {

    if (!fan3State && temp > th_t3_max) {
      fan3State = true;
      digitalWrite(FAN3_PIN, HIGH);
      sendState("FAN3_STATE", 1);
    }
    if (fan3State && temp < th_t3_min) {
      fan3State = false;
      digitalWrite(FAN3_PIN, LOW);
      sendState("FAN3_STATE", 0);
    }

    if (!pump3State && soil3 < th_s3_min) {
      pump3State = true;
      digitalWrite(PUMP3_PIN, HIGH);
      sendState("PUMP3_STATE", 1);
    }
    if (pump3State && soil3 > th_s3_max) {
      pump3State = false;
      digitalWrite(PUMP3_PIN, LOW);
      sendState("PUMP3_STATE", 0);
    }
  }


// ===== CẢNH BÁO ÁNH SÁNG =====
bool newWarnLight = (light > th_light_max);

if (newWarnLight != warnLightState) {
  warnLightState = newWarnLight;
  Serial1.print("WARN_LIGHT:");
  Serial1.println(warnLightState ? 1 : 0);
}

// ===== CẢNH BÁO KHÔNG KHÍ =====
bool newWarnHumi = (humi < th_humi_min);
if (newWarnHumi != warnHumiState) {
  warnHumiState = newWarnHumi;
  Serial1.print("WARN_HUMI:");
  Serial1.println(warnHumiState ? 1 : 0);
}

// ===== CẢNH BÁO NHIỆT ĐỘ =====

// VƯỜN 1
if (temp > th_t1_max && !warnTemp1) {
  warnTemp1 = true;
  Serial1.println("WARN_TEMP1:1");
}
if (temp < th_t1_min && warnTemp1) {
  warnTemp1 = false;
  Serial1.println("WARN_TEMP1:0");
}

// VƯỜN 2
if (temp > th_t2_max && !warnTemp2) {
  warnTemp2 = true;
  Serial1.println("WARN_TEMP2:1");
}
if (temp < th_t2_min && warnTemp2) {
  warnTemp2 = false;
  Serial1.println("WARN_TEMP2:0");
}

// VƯỜN 3
if (temp > th_t3_max && !warnTemp3) {
  warnTemp3 = true;
  Serial1.println("WARN_TEMP3:1");
}
if (temp < th_t3_min && warnTemp3) {
  warnTemp3 = false;
  Serial1.println("WARN_TEMP3:0");
}

// ===== CẢNH BÁO ĐẤT KHÔ =====

// VƯỜN 1
if (soil1 < th_s1_min && !warnSoil1) {
  warnSoil1 = true;
  Serial1.println("WARN_SOIL1:1");
}
if (soil1 > th_s1_max && warnSoil1) {
  warnSoil1 = false;
  Serial1.println("WARN_SOIL1:0");
}

// VƯỜN 2
if (soil2 < th_s2_min && !warnSoil2) {
  warnSoil2 = true;
  Serial1.println("WARN_SOIL2:1");
}
if (soil2 > th_s2_max && warnSoil2) {
  warnSoil2 = false;
  Serial1.println("WARN_SOIL2:0");
}

// VƯỜN 3
if (soil3 < th_s3_min && !warnSoil3) {
  warnSoil3 = true;
  Serial1.println("WARN_SOIL3:1");
}
if (soil3 > th_s3_max && warnSoil3) {
  warnSoil3 = false;
  Serial1.println("WARN_SOIL3:0");
}
}






// ================= HANDLE UART =================
void processCommand(String cmd) {

  if (cmd == "REQUEST_STATE") {
    sendAllStates();
    return;
  }

  // ===== MODE =====
// MODE VƯỜN 1
if(cmd == "MODE1_AUTO"){ mode1 = MODE_AUTO;  Serial1.println("MODE1:AUTO");  return; }
if(cmd == "MODE1_MANUAL"){
  mode1 = MODE_MANUAL;

  Serial1.println("MODE1:MANUAL");
  return;
}


// MODE VƯỜN 2
if(cmd == "MODE2_AUTO"){ mode2 = MODE_AUTO;  Serial1.println("MODE2:AUTO");  return; }
if(cmd == "MODE2_MANUAL"){
  mode2 = MODE_MANUAL;

  Serial1.println("MODE2:MANUAL");
  return;
}



// MODE VƯỜN 3
if(cmd == "MODE3_AUTO"){ mode3 = MODE_AUTO;  Serial1.println("MODE3:AUTO");  return; }
if(cmd == "MODE3_MANUAL"){
  mode3 = MODE_MANUAL;

  Serial1.println("MODE3:MANUAL");
  return;
}





 // ===== MANUAL CONTROL VƯỜN 1 =====
// (ĐÈN + PHUN SƯƠNG DÙNG CHUNG → KHÔNG ĐỤNG MODE)

if (cmd == "BULB_ON")  {
  bulbState = true;
  digitalWrite(BULB_PIN, HIGH);
  sendState("BULB_STATE", 1);
  return;
}

if (cmd == "BULB_OFF") {
  bulbState = false;
  digitalWrite(BULB_PIN, LOW);
  sendState("BULB_STATE", 0);
  return;
}

if (cmd == "SPRAY_ON") {
  sprayState = true;
  digitalWrite(SPRAY_PIN, HIGH);
  sendState("SPRAY_STATE", 1);
  return;
}

if (cmd == "SPRAY_OFF") {
  sprayState = false;
  digitalWrite(SPRAY_PIN, LOW);
  sendState("SPRAY_STATE", 0);
  return;
}

// FAN + PUMP VƯỜN 1 → MANUAL RIÊNG
if (cmd == "FAN_ON") {
  mode1 = MODE_MANUAL;
  fanState = true;
  digitalWrite(FAN_PIN, HIGH);
  sendState("FAN_STATE", 1);
  return;
}

if (cmd == "FAN_OFF") {
  mode1 = MODE_MANUAL;
  fanState = false;
  digitalWrite(FAN_PIN, LOW);
  sendState("FAN_STATE", 0);
  return;
}

if (cmd == "PUMP_ON") {
  mode1 = MODE_MANUAL;
  pumpState = true;
  digitalWrite(PUMP_PIN, HIGH);
  sendState("PUMP_STATE", 1);
  return;
}

if (cmd == "PUMP_OFF") {
  mode1 = MODE_MANUAL;
  pumpState = false;
  digitalWrite(PUMP_PIN, LOW);
  sendState("PUMP_STATE", 0);
  return;
}


// ===== MANUAL CONTROL VƯỜN 2 =====
if (cmd == "FAN2_ON") {
  mode2 = MODE_MANUAL;
  fan2State = true;
  digitalWrite(FAN2_PIN, HIGH);
  sendState("FAN2_STATE",1);
  return;
}

if (cmd == "FAN2_OFF") {
  mode2 = MODE_MANUAL;
  fan2State = false;
  digitalWrite(FAN2_PIN, LOW);
  sendState("FAN2_STATE",0);
  return;
}

if (cmd == "PUMP2_ON") {
  mode2 = MODE_MANUAL;
  pump2State = true;
  digitalWrite(PUMP2_PIN, HIGH);
  sendState("PUMP2_STATE",1);
  return;
}

if (cmd == "PUMP2_OFF") {
  mode2 = MODE_MANUAL;
  pump2State = false;
  digitalWrite(PUMP2_PIN, LOW);
  sendState("PUMP2_STATE",0);
  return;
}


// ===== MANUAL CONTROL VƯỜN 3 =====
if (cmd == "FAN3_ON") {
  mode3 = MODE_MANUAL;
  fan3State = true;
  digitalWrite(FAN3_PIN, HIGH);
  sendState("FAN3_STATE",1);
  return;
}

if (cmd == "FAN3_OFF") {
  mode3 = MODE_MANUAL;
  fan3State = false;
  digitalWrite(FAN3_PIN, LOW);
  sendState("FAN3_STATE",0);
  return;
}

if (cmd == "PUMP3_ON") {
  mode3 = MODE_MANUAL;
  pump3State = true;
  digitalWrite(PUMP3_PIN, HIGH);
  sendState("PUMP3_STATE",1);
  return;
}

if (cmd == "PUMP3_OFF") {
  mode3 = MODE_MANUAL;
  pump3State = false;
  digitalWrite(PUMP3_PIN, LOW);
  sendState("PUMP3_STATE",0);
  return;
}

// ===== RECEIVE THRESHOLD =====
if (cmd.startsWith("TH_LIGHT:")) {
  sscanf(cmd.c_str(), "TH_LIGHT:%d,%d", &th_light_min, &th_light_max);
  return;
}

if (cmd.startsWith("TH_HUMI:")) {
  sscanf(cmd.c_str(), "TH_HUMI:%d,%d", &th_humi_min, &th_humi_max);
  return;
}

if (cmd.startsWith("TH_T1:")) {
  sscanf(cmd.c_str(), "TH_T1:%d,%d", &th_t1_min, &th_t1_max);
  return;
}

if (cmd.startsWith("TH_S1:")) {
  sscanf(cmd.c_str(), "TH_S1:%d,%d", &th_s1_min, &th_s1_max);
  return;
}
if (cmd.startsWith("TH_T2:")) {
  sscanf(cmd.c_str(), "TH_T2:%d,%d", &th_t2_min, &th_t2_max);
  return;
}

if (cmd.startsWith("TH_S2:")) {
  sscanf(cmd.c_str(), "TH_S2:%d,%d", &th_s2_min, &th_s2_max);
  return;
}
if (cmd.startsWith("TH_T3:")) {
  sscanf(cmd.c_str(), "TH_T3:%d,%d", &th_t3_min, &th_t3_max);
  return;
}

if (cmd.startsWith("TH_S3:")) {
  sscanf(cmd.c_str(), "TH_S3:%d,%d", &th_s3_min, &th_s3_max);
  return;
}


}




// ================= DHT =================
void sendDHT() {
  if (millis() - lastDhtSend < 1000) return;
  lastDhtSend = millis();

  static float lastTemp = 0;
  static float lastHumi = 0;
  static int warmup = 0;   // ⭐ thêm dòng này

  // ===== GIẢ LẬP DHT (TẠM THỜI) =====
float t = dht.readTemperature();
float h = dht.readHumidity();
// ⭐ bỏ qua 3 lần đọc đầu hoặc giá trị rác
  if (isnan(t) || isnan(h) || warmup < 2) {
    warmup++;
    return;
  }

  if (!isnan(t)) lastTemp = t;
  if (!isnan(h)) lastHumi = h;

  int light = analogRead(LIGHT_A);

  Serial1.print("TEMP:");  Serial1.println(lastTemp);
  Serial1.print("HUMID:"); Serial1.println(lastHumi);
  Serial1.print("LIGHT:"); Serial1.println(light);

  autoControlAll(
    lastTemp,
    lastHumi,
    soil1Percent,
    soil2Percent,
    soil3Percent,
    light
  );

  lcdDisplayAll(lastTemp, lastHumi, light);
}



void sendSoil() {
  if (millis() - lastSoilSend < 1000) return;
  lastSoilSend = millis();

  int raw = analogRead(PA2);
  soil1Percent = map(raw, 4095, 0, 0, 100);
  soil1Percent = constrain(soil1Percent, 0, 100);

  Serial1.print("SOIL:");
  Serial1.println(soil1Percent);
}


void sendSoil2() {

  // 500ms gửi 1 lần
  if (millis() - lastSoil2Send < 1000) return;
  lastSoil2Send = millis();

  int raw = analogRead(PB1);

  // Map: đất khô = 0%, đất ướt = 100%
   soil2Percent = map(raw, 4095, 0, 0, 100);
  soil2Percent = constrain(soil2Percent, 0, 100);

  // Gửi UART cho ESP32
  Serial1.print("SOIL2:");
  Serial1.println(soil2Percent);

  // (Tương lai nếu cần tự động vườn 2 → bạn chỉ cần thêm xử lý ở đây)
}
void sendSoil3() {
  if (millis() - lastSoil3Send < 1000) return;
  lastSoil3Send = millis();

  int raw = analogRead(PA4);
   soil3Percent = map(raw, 4095, 0, 0, 100);
  soil3Percent = constrain(soil3Percent, 0, 100);

  Serial1.print("SOIL3:");
  Serial1.println(soil3Percent);
}

float readWaterDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
 if (duration < 300) return -1; // < ~5cm coi như nhiễu


  return duration * 0.034 / 2.0; // cm
}
void sendWater() {
  static unsigned long lastSend = 0;
  if (millis() - lastSend < 1500) return;
  lastSend = millis();

  float distance = readWaterDistanceCM();
  if (distance < 0) return;

  float waterLevel = TANK_HEIGHT_CM - distance;
  waterLevel = constrain(waterLevel, 0, TANK_HEIGHT_CM);

   waterPercent = (int)((waterLevel / TANK_HEIGHT_CM) * 100.0);

  Serial1.print("WATER:");
  Serial1.println(waterPercent);
}
void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial1.setTimeout(50);   // ⭐⭐ BẮT BUỘC – tránh treo readStringUntil

  Wire.setSCL(PB8);
  Wire.setSDA(PB9);
  Wire.begin();

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SMART GARDEN");
  lcd.setCursor(0,1);
  lcd.print("STM32 READY");
  delay(1000);
  lcd.clear();

  dht.begin();
    // ⭐ RESET CẢNH BÁO (QUAN TRỌNG)
  warnTemp1 = false;
  warnTemp2 = false;
  warnTemp3 = false;
 warnLightState = false;
 warnHumiState  = false;


 warnSoil1 = false;
  warnSoil2 = false;
 warnSoil3 = false;

  pinMode(BULB_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(SPRAY_PIN, OUTPUT);

  pinMode(FAN2_PIN, OUTPUT);
  pinMode(PUMP2_PIN, OUTPUT);

  pinMode(FAN3_PIN, OUTPUT);
  pinMode(PUMP3_PIN, OUTPUT);

    pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);

  digitalWrite(BULB_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(SPRAY_PIN, LOW);

  digitalWrite(FAN2_PIN, LOW);
  digitalWrite(PUMP2_PIN, LOW);

  digitalWrite(FAN3_PIN, LOW);
  digitalWrite(PUMP3_PIN, LOW);

  delay(500);
  Serial1.println("REQUEST_STATE");
  
}
void loop() {

  // Nhận lệnh từ ESP32
  if (Serial1.available()) {
    String cmd = Serial1.readStringUntil('\n');
    cmd.trim();
    if (cmd.length() > 0) {
      processCommand(cmd);
    }
  }

  // Gửi cảm biến

  sendSoil();
  sendSoil2();
  sendSoil3();
    sendDHT();
      sendWater();
}




