#define BLYNK_TEMPLATE_ID "XXXX"
#define BLYNK_DEVICE_NAME "XXXX"
#define BLYNK_AUTH_TOKEN "XXXX"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <ir_Tcl.h>
#include <DHT.h>
char auth[] = "XXXX"; //Insert your Blynk project's authentication token here
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "XXXX"; //Insert your network's SSID here
char pass[] = "XXXX"; //Insert your network's password here

////// AC GLOBALS //////
int acState = 0;
int fanStrength = 1;
int temperature = 16;
int acMode = 1;
int acSwing = 0;
char acCommand[256];
////////////////////////
#define DHTPIN 14 (D5)
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRTcl112Ac ac(kIrLed);     // Set the GPIO used for sending messages.

//A implementar
/*
void mark (int time)
{
    int i = 0;
    int cycles = 0;
    cycles = time / 28; The carrier wave is 38kHz so each period is 26 micro seconds. After checking the output with an IR receiver, 28 seemed to be better
                          as the hardware isn't as precise as you would hope 
    if (cycles == 0){
        cycles = 1;
    }
    for (i = 0;i < cycles;i++){
        digitalWrite(outputPin, HIGH);
        delayMicroseconds(13);
        digitalWrite(outputPin, LOW);
        delayMicroseconds(13);
    }
}

void space (int time)
{
    digitalWrite(outputPin, LOW);
    if (time > 0){
        delayMicroseconds(time);
    }
}

void irRemoteSendRaw (int *codes, int len)
{
    int i;

    for (i = 0;i < len;i++){
        if (i%2 == 0){
            mark(codes[i]);
        }
        else{
            space(codes[i]);
        }
    }
}

void sendCodesTask(int state)
{
    struct airCon newAc;
    int *codes;

    codes = getCodes(&newAc,fanStrength,acMode,temperature,state,acSwing);
    irRemoteSendRaw(codes,TIMINGS_LENGTH);
}

BLYNK_WRITE(V1) //Temperature: step widget
{
    temperature = param.asInt();
    Blynk.virtualWrite(V7,temperature);
    if (acState){
        ac.setTemp(temperature);
        ac.send();
    }
}

BLYNK_WRITE(V2) //Fan: step widget
{
    fanStrength = param.asInt();
    if (fanStrength == 1){
      ac.setFan(kTcl112AcFanLow);
    }
    if (fanStrength == 2){
      ac.setFan(kTcl112AcFanMed);
    }
    if (fanStrength == 3){
      ac.setFan(kTcl112AcFanHigh);
    }
    if (fanStrength == 4){
      ac.setFan(kTcl112AcFanAuto);
    }
    if (acState){
        ac.send();
    }
}

BLYNK_WRITE(V3) //Swing: slider widget
{
    acSwing = param.asInt();
    if(acSwing == 1){
      ac.setSwingVertical(true);
    }
    if(acSwing == 0){
      ac.setSwingVertical(false);
    }
    if (acState){
        ac.send();
    }
}

BLYNK_WRITE(V4) //Mode selection: table widget
{
    char hotColor[] = "#b73838";
    char coldColor[] = "#4d9aff";
    char fanColor[] = "#808080";
    char *color = NULL;

    if (param.asInt() == 1){ //COLD
        ac.setMode(kTcl112AcCool);
        color = coldColor;
    }
    if (param.asInt() == 2){ //HOT
        ac.setMode(kTcl112AcHeat);
        color = hotColor;
    }
    if (param.asInt() == 3){ //FAN
        ac.setMode(kTcl112AcFan);
        color = fanColor;
    }
    Blynk.setProperty(V1,"color",color); // Temperature step
    Blynk.setProperty(V5,"color",color); // ON/OFF Button
    Blynk.setProperty(V7,"color",color); // Temperature gauge
    Blynk.setProperty(V6,"color","#2DC72D"); // LED - green
    Blynk.setProperty(V4,"color",color); // MODE

    if (acState){
        ac.send();
    }
}

BLYNK_WRITE(V5) // ON/OFF: button widget
{
    char ledOn[] = "255";
    char ledOff[] = "0";
    char *ledState = NULL;
    ac.send();
    acState ^= 1;
    if (acState){
        ledState = ledOff;
        ac.off();
    }
    else{
      ledState = ledOn;
        ac.on();

    }
    Blynk.virtualWrite(V6,ledState);
}
*/

//Read sensor dht11
void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V21, h);
  Blynk.virtualWrite(V22, t);
}

BLYNK_WRITE(V10){ //mode venti
  if(param.asInt()){
    ac.setMode(kTcl112AcFan);
    ac.setFan(kTcl112AcFanHigh);
    ac.setSwingVertical(kTcl112AcSwingVOn);
    ac.send();
  }
}
BLYNK_WRITE(V11){ //mode frio
  if(param.asInt()){
    ac.setMode(kTcl112AcCool);
    ac.setFan(kTcl112AcFanAuto);
    ac.setSwingVertical(kTcl112AcSwingVOn);
    ac.setTemp(24);
    ac.send();
  }
}
BLYNK_WRITE(V12){ //mode Heat
  if(param.asInt()){
    ac.setMode(kTcl112AcHeat);
    ac.setFan(kTcl112AcFanAuto);
    ac.setSwingVertical(kTcl112AcSwingVOn);
    ac.setTemp(20);
    ac.send();
  }
}
BLYNK_WRITE(V13){ //mode dry
  if(param.asInt()){
    ac.setFan(kTcl112AcDry);
    ac.setSwingVertical(kTcl112AcSwingVOn);
    ac.setFan(kTcl112AcFanAuto);
    ac.send();
  }
}
BLYNK_WRITE(V14){ //on ac
  if(param.asInt()){
    ac.on();
    ac.send();
  }
}
BLYNK_WRITE(V15){ //off ac
  if(param.asInt()){
    ac.off();
    ac.send();
  }
}

void setup(){
  ac.begin();
  Serial.begin(115200);
  delay(200);
  // Set up what we want to send. See ir_Tcl.cpp for all the options.
  Serial.println("Default state of the remote.");
  Serial.println("Setting initial state for A/C.");
  ac.off(); // Off ac
  ac.setFan(kTcl112AcFanLow);
  ac.setMode(kTcl112AcCool);
  ac.setTemp(25);
  ac.setSwingVertical(false);
  Blynk.begin(auth,ssid,pass); //add a 4th argument if you're using a local server i.e. "(auth,ssid,pass,localIp)"
  dht.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop(){
  Blynk.run();
  timer.run();
}
