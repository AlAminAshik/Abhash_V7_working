#include <Arduino.h>
#include <WiFi.h>         //include wifi library
#include <NewPing.h>
#include <IRremote.hpp>
#include <WiFiManager.h>
#include <esp_task_wdt.h> // Include Task Watchdog header
#include "driver/adc.h"   //analog input driver for voltage range
#include "esp32-hal-ledc.h" //ledc driver for buzzer
#include "XT_DAC_Audio.h"
//#include "AllHeadphone.h"

//Adding audio files and library
#include "Audio_Agate_Thakun.h"
#include "Audio_Bame_Badha.h"
#include "Audio_Bame_Jan.h"
#include "Audio_Battery_20.h"
#include "Audio_Battery_50.h"
#include "Audio_Battery_Full.h"
#include "Audio_Battery_Low.h"
#include "Audio_Dane_Badha.h"
#include "Audio_Dane_Jan.h"
#include "Audio_Halka_Dane_Jan.h"
#include "Audio_Power_Off.h"
#include "Audio_Power_On.h"
#include "Audio_Thamun.h"
//#include "SoundData.h"

//initializing and assigning class to the audio array
XT_Wav_Class Agate_Thakun_Audio(Agate_Thakun);
XT_Wav_Class Bame_Badha_Audio(Bame_Badha);
XT_Wav_Class Bame_Jan_Audio(Bame_Jan);
XT_Wav_Class Battery_20_Audio(Battery_20);
XT_Wav_Class Battery_50_Audio(Battery_50);
XT_Wav_Class Battery_Full_Audio(Battery_Full);
XT_Wav_Class Battery_Low_Audio(Battery_Low);
XT_Wav_Class Dane_Badha_Audio(Dane_Badha);
XT_Wav_Class Dane_Jan_Audio(Dane_Jan);
XT_Wav_Class Halka_Dane_Audio(Halka_Dane_Jan);
XT_Wav_Class Power_Off_Audio(Power_Off);
XT_Wav_Class Power_On_Audio(Power_On);
XT_Wav_Class Thamu_Audio(Thamun);
//XT_Wav_Class Sample_audio(sample);   //initializing the sample audio data
XT_DAC_Audio_Class DacAudio(25, 0); //connected to pin10 or GPIO25 or DAC1 (create the main player class object)


long beepTimer;     //keeps record of last beep
long audioTimer;    //keeps record of last audio play
String lastSituation = "   ";

#define headphoneDetection 22
uint8_t headPhone;
int dL, dF, dR;   //stores current distance from left, front and right

//#define dds 184 // detection distance side
#define dds 100 // detection distance side
//#define ddf 184 // detection distance front
#define ddf 100 // detection distance front
//#define dds2 92 // detection distance side (Mid)
#define dds2 62 // detection distance side (Mid)
//#define ddf2 92 // detection distance front (Mid)
#define ddf2 62 // detection distance front (Mid)
//#define dds3 60 // detection distance side (Close)
#define dds3 30 // detection distance side (Close)
//#define ddf3 60 // detection distance front (Close)
#define ddf3 30 // detection distance front (Close)

#define TRIG_L 16 // pin 27
#define TRIG_F 4  // pin 26
#define TRIG_R 17 // pin 28
#define MAX_DISTANCE_SIDE dds
#define MAX_DISTANCE_FRONT ddf

// led pins
#define PEDESTRIAN_LED 19 // pedestrian led transistor connected pin 31
#define LED_INTERVAL 500 // pedestrian led blink interval
#define IR_LED_PIN 23     // pin 37

#define CH_PD_PIN 13 // pin 16 GPIO 13

// button pins
#define TOP_BUTTON_PIN 21    // pin 33, Pedestrian LED
#define PWR_BUTTON_PIN 27    // pin 12, MIDDLE_BUTTON_PIN, Power Button
#define BOTTOM_BUTTON_PIN 18 // pin 30, IR LED

#define BUZZER_PIN 26 // pin 11
#define BUZZER_CHANNEL 0
#define BATTERY_VOLT_PIN 36 // GPIO36 (VP) corresponds to ADC1_CHANNEL_0 pin 4
float voltage;
bool playedOnce;
#define MOTOR_PIN 32 // pin 8
#define MOTOR_CHANNEL 1

// update OTA
bool res;
WiFiManager wm; // WiFiManager, Local intialization. Once its business is done, there is no need to keep it around

NewPing sonarL(TRIG_L, TRIG_L, MAX_DISTANCE_SIDE+30);
NewPing sonarF(TRIG_F, TRIG_F, MAX_DISTANCE_FRONT+30);
NewPing sonarR(TRIG_R, TRIG_R, MAX_DISTANCE_SIDE+30);

unsigned long previousMillisPower = 0;  // will store last time button was pressed for shutting down
unsigned long previousMillis = 0;       // will store late time button was pressed for taking voltage reading
const long interval = 10000;            //interval between voltage read

// prototypes
void getSensorData();
long dst_median(NewPing s, int max, int cnt);
void batteryLow();
void S000();
void S001();
void S010();
void S011();
void S100();
void S101();
void S110();
void S111();
void A000();
void A001();
void A010();
void A011();
void A100();
void A101();
void A110();
void A111();

// ------------- playing audio ------------- //
void Play_Agate_Thakun()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Agate_Thakun_Audio);
  while (Agate_Thakun_Audio.Playing) DacAudio.FillBuffer();
}
void Play_Bame_Badha()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Bame_Badha_Audio);
  while (Bame_Badha_Audio.Playing) DacAudio.FillBuffer();
}
void Play_Bame_Jan()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Bame_Jan_Audio);
  while (Bame_Jan_Audio.Playing) DacAudio.FillBuffer();
}
void Play_Battery_20()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Battery_20_Audio);
  while (Battery_20_Audio.Playing) DacAudio.FillBuffer();
}
void Play_Battery_50()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Battery_50_Audio);
  while (Battery_50_Audio.Playing) DacAudio.FillBuffer();
}
void Play_Battery_Full()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Battery_Full_Audio);
  while (Battery_Full_Audio.Playing) DacAudio.FillBuffer();
}
void Play_Battery_Low()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Battery_Low_Audio);
  while (Battery_Low_Audio.Playing) DacAudio.FillBuffer();
}
void Play_Dane_Badha()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Dane_Badha_Audio);
  while (Dane_Badha_Audio.Playing) DacAudio.FillBuffer();
}
void Play_Dane_Jan()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Dane_Jan_Audio);
  while (Dane_Jan_Audio.Playing) DacAudio.FillBuffer();
}
void Play_Halka_Dane_Jan()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Halka_Dane_Audio);
  while (Halka_Dane_Audio.Playing) DacAudio.FillBuffer();
}
void Play_Power_Off()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Power_Off_Audio);
  while (Power_Off_Audio.Playing) DacAudio.FillBuffer();
}
void Play_Power_On()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Power_On_Audio);
  while (Power_On_Audio.Playing) DacAudio.FillBuffer();
}
void Play_Thamun()
{
  DacAudio.FillBuffer();
  DacAudio.Play(&Thamu_Audio);
  while (Thamu_Audio.Playing) DacAudio.FillBuffer();
}
// ------------- playing audio ------------- //


// ------------- ScenariosBuzzer ------------- //
void S000()
{
  ledcWriteTone(BUZZER_PIN, 0);
  if (millis() - beepTimer > 3000)
  {
    ledcWriteTone(BUZZER_PIN, 256);
    delay(100);
    ledcWriteTone(BUZZER_PIN, 0);
    beepTimer = millis();
  }
}
void S001()
{
  ledcWriteTone(BUZZER_PIN, 0);
  int dur = (int)constrain(map(dR, 5, dds, 50, 500), 50, 500);
  ledcWriteTone(BUZZER_PIN, 528);
  delay(dur * 2);
  ledcWriteTone(BUZZER_PIN, 0);
  delay(dur / 2);
  Serial.println(dur);
}
void S010()
{
  ledcWriteTone(BUZZER_PIN, 0);
  if (millis() - beepTimer > 1500)
  {
    // ledcWriteTone(BUZZER_PIN, 1320);
    ledcWriteTone(BUZZER_PIN, 1320);
    delay(100);
    ledcWriteTone(BUZZER_PIN, 0);
    beepTimer = millis();
  }
}
void S011()
{
  ledcWriteTone(BUZZER_PIN, 0);

  int dur = (int)constrain(map(dR, 5, dds, 50, 500), 50, 500);
  ledcWriteTone(BUZZER_PIN, 792);
  delay(dur * 2);
  ledcWriteTone(BUZZER_PIN, 0);
  delay(dur / 2);
}
void S100()
{
  ledcWriteTone(BUZZER_PIN, 0);

  int dur = (int)constrain(map(dL, 5, dds, 50, 500), 50, 500);
  // ledcWriteTone(BUZZER_PIN, 1320);
  ledcWriteTone(BUZZER_PIN, 1320);
  delay(dur / 2);
  ledcWriteTone(BUZZER_PIN, 0);
  delay(dur * 2);
}
void S101()
{
  ledcWrite(MOTOR_PIN, 0);

  /* Beep if distance is greater than 1.5 ft
    (528Hz if right distance is closer, and 1320Hz if left distance is closer)
    (Beep delay depends on whichever side has less distance)

    On (1848Hz) if both distance is less than 1.5 ft */
  if (dR < dds3 && dL < dds3)
  {
    ledcWriteTone(BUZZER_PIN, 1848);
    delay(100);
  }
  else if (dR < dL)
  {
    int dur = (int)constrain(map(dR, 5, dds, 50, 500), 50, 500);
    ledcWriteTone(BUZZER_PIN, 528);
    delay(dur * 2);
    ledcWriteTone(BUZZER_PIN, 0);
    delay(dur / 2);
  }
  else
  {
    int dur = (int)constrain(map(dL, 5, dds, 50, 500), 50, 500);
    ledcWriteTone(BUZZER_PIN, 1320);
    delay(dur / 2);
    ledcWriteTone(BUZZER_PIN, 0);
    delay(dur * 2);
  }
}
void S110()
{
  ledcWriteTone(BUZZER_PIN, 0);

  int dur = (int)constrain(map(dL, 5, dds, 50, 500), 50, 500);
  ledcWriteTone(BUZZER_PIN, 1584);
  delay(dur / 2);
  ledcWriteTone(BUZZER_PIN, 0);
  delay(dur * 2);
}
void S111()
{
  /* Beep if distance is greater than 1.5 ft
    (792Hz if right distance is closer, and 1584Hz if left distance is closer)
    (Beep delay depends on whichever side has less distance)

    On (2112Hz) if both distance is less than 1.5 ft */

  //int pow = (int)constrain(map(dF, 14, ddf, 255, 50), 50, 255);
  int pow = (int)constrain(map(dF, 14, ddf, 300, 100), 100, 300); //turn on motor with map
  ledcWrite(MOTOR_PIN, pow);

  if (dR < dds3 && dL < dds3)
  {
    ledcWriteTone(BUZZER_PIN, 2112);
    delay(100);
  }
  else if (dR < dL)
  {
    int dur = (int)constrain(map(dR, 5, dds, 50, 500), 50, 500);
    ledcWriteTone(BUZZER_PIN, 792);
    delay(dur * 2);
    ledcWriteTone(BUZZER_PIN, 0);
    delay(dur / 2);
  }
  else
  {
    int dur = (int)constrain(map(dL, 5, dds, 50, 500), 50, 500);
    ledcWriteTone(BUZZER_PIN, 1584);
    delay(dur / 2);
    ledcWriteTone(BUZZER_PIN, 0);
    delay(dur * 2);
  }
}
// ------------- ScenariosBuzzer ------------- //


// ------------- ScenariosHeadphone ------------- //
void playAudio(String ad, int timer, String last) {
  if (millis() - audioTimer > timer || ad != lastSituation) {
    if (ad == "Agate_Thakun") {
      Play_Agate_Thakun();
    } else if (ad == "Bame_Badha") {
      Play_Bame_Badha();
    } else if (ad == "Bame_Jan") {
      Play_Bame_Jan();
    } else if (ad == "Dane_Badha") {
      Play_Dane_Badha();
    } else if (ad == "Dane_Jan") {
      Play_Dane_Jan();
    } else if (ad == "Halka_Dane_Jan") {
      Play_Halka_Dane_Jan();
    } else if (ad == "Thamun") {
      Play_Thamun();
    }
  }
    audioTimer = millis();
    lastSituation = ad;
}
void A000()
{
  ledcWriteTone(BUZZER_PIN, 0);
  playAudio("Agate_Thakun", 5000, "000");
}
void A001()
{
  ledcWriteTone(BUZZER_PIN, 0);
  if (dR > dds2)
  {
    playAudio("Agate_Thakun", 5000, "001");
  }
  else
  {
    playAudio("Dane_Badha", 1500, "001");
  }
}
void A010()
{
  ledcWriteTone(BUZZER_PIN, 0);
  playAudio("Thamun", 1500, "010");
}
void A011()
{
  ledcWriteTone(BUZZER_PIN, 0);
  if (dR > dds2)
  {
    playAudio("Dane_Badha", 1500, "011");
  }
  else
  {
    playAudio("Bame_Jan", 1500, "011");
  }
}
void A100()
{
  ledcWriteTone(BUZZER_PIN, 0);
  if (dL > dds2)
  {
    playAudio("Agate_Thakun", 5000, "100");
  }
  else
  {
    playAudio("Bame_Badha", 1500, "100");
  }
}
void A101()
{
  ledcWriteTone(BUZZER_PIN, 0);
  if (dR > dds2 && dL > dds2)
  {
    playAudio("Agate_Thakun", 5000, "101");
  }
  else if (dR > dds3)
  {
    playAudio("Bame_Badha", 1500, "101");
  }
  else if (dL > dds3)
  {
    playAudio("Dane_Badha", 1500, "101");
  }
  else if (dL <= dds3 && dR <= dds3)
  {
    playAudio("Thamun", 1000, "101");
  }
}
void A110()
{
  ledcWriteTone(BUZZER_PIN, 0);
  if (dL > dds2)
  {
    playAudio("Bame_Badha", 1500, "110");
  }
  else
  {
    playAudio("Dane_Jan", 1500, "110");
  }
}
void A111()
{
  ledcWriteTone(BUZZER_PIN, 0);
  playAudio("Thamun", 1000, "111");
}
// ------------- ScenariosHeadphone ------------- //

void getSensorData()
{
  headPhone = digitalRead(headphoneDetection);
  // if (headPhone == 0) { headPhone = 2; }
  dF = dst_median(sonarF, MAX_DISTANCE_FRONT+30, 4);  //distance front
  delay(10);
  dL = dst_median(sonarL, MAX_DISTANCE_SIDE+30, 4);   //distance left
  delay(10);
  dR = dst_median(sonarR, MAX_DISTANCE_SIDE+30, 4);   //distance right
  delay(10);
  Serial.println("Distance: Left: " + String(dL) + "cm | Front: " + String(dF) + "cm | Right: " + String(dR) + "cm" + " | Headphone: " + String(headPhone));
}


//average the distance readings
long dst_median(NewPing s, int max, int cnt)
{
  long tmr = millis();
  long dstt = 1;
  for (int i = 0; i < cnt; i++)
  {
    long d = s.ping_cm();
    // dstt += s.ping_cm();
    if (d == 0)
    {
      d = max;
    }
    dstt = dstt * d;
  }
  // dstt = dstt / cnt;
  dstt = round(pow(dstt, 1.0 / cnt));
  //Serial.println(millis() - tmr);
  return dstt;
}

//beep when battery is low
void batteryLow()
{
  if (headPhone == 0)   //if headphone not connected
  {
    ledcWriteTone(BUZZER_PIN, 1000); // 1 kHz beep
    delay(200);
    ledcWriteTone(BUZZER_PIN, 500);  // 500 Hz beep
    delay(200);
    ledcWriteTone(BUZZER_PIN, 1500); // 1.5 kHz beep
    delay(200);
    ledcWriteTone(BUZZER_PIN, 0); // Silence
    delay(10);
  }
  else
  {
    Play_Battery_Low();
  }
}

//default code loops on core 1 since core 0 used for RF communications
TaskHandle_t sonarTask; //creating a task handle
TaskHandle_t otherTask; //creating another task handle
bool coreZeroBusy = false;
bool coreOneBusy = false;


void otherTaskLoop(void * pvParameters){
    for(;;){
    unsigned long currentMillis = millis();
    digitalWrite(PEDESTRIAN_LED, LOW);  //keep light off when not used
    if (currentMillis - previousMillis >= interval)   //read voltage after every 10 seconds
    {
      previousMillis = currentMillis;
  
      int sensorValue = analogRead(BATTERY_VOLT_PIN);
      voltage = sensorValue * (3.69 / 4095.0) * 2;  //convert to apprehendable voltage
  
      Serial.print("Analog Input Value: ");
      Serial.print(sensorValue);
      Serial.print("\t Voltage: ");
      Serial.print(voltage, 2);
      Serial.println("V");
      // cute.init(BUZZER_PIN);
      // cute.play(S_CONNECTION);
      if (voltage < 3.6 && voltage > 3.32)
      {
        batteryLow();
      }
    }
      
    // Power off schedule & battery read
    if (digitalRead(PWR_BUTTON_PIN) == LOW)
    { // if button is pressed when device is active
    Serial.println("power Button Pressed");
    unsigned long currentMillis = millis(); // imp
    //battery voltage voice short press
    if (playedOnce == 0) //play once
    {
      if(voltage > 4.00){
        Play_Battery_Full();
        playedOnce = 1;
      }
      else if (voltage > 3.80 and voltage <= 4.00)
      {
        Play_Battery_50();
        playedOnce = 1;
      }
      else if(voltage > 3.60 and voltage <= 3.80)
      {
        Play_Battery_20();
        playedOnce = 1;
      }
      else if(voltage <= 3.60)
      {
        Play_Battery_Low();
        playedOnce = 1;
      }
    }
    //power down long press
    if (currentMillis - previousMillisPower > 3000)
    { // if button pressed for 5 seconds
      while (digitalRead(PWR_BUTTON_PIN) == LOW)
      { // while button is kept pressed
        Serial.println("Shutting Down");
        ledcWrite(MOTOR_PIN, 400);
        Play_Power_Off();
        ledcWriteTone(BUZZER_PIN, 1000);
        delay(100);
        ledcWriteTone(BUZZER_PIN, 0);
        delay(100);
        ledcWriteTone(BUZZER_PIN, 1000);
        delay(100);
        ledcWriteTone(BUZZER_PIN, 0);
        delay(100);
        ledcWriteTone(BUZZER_PIN, 1000);
        delay(100);
        ledcWriteTone(BUZZER_PIN, 0);
        delay(100);
        ledcWrite(MOTOR_PIN, 0);
        ledcWriteTone(BUZZER_PIN, 0);
        digitalWrite(CH_PD_PIN, HIGH); // turns the mosfet off and thus device off
        while (1);
      }
    }
  }
    else
  {
    previousMillisPower = millis(); // if button is not pressed, store the current time
    playedOnce = 0;
  }
  
    // Pedestrian LED blink code
    if (digitalRead(TOP_BUTTON_PIN) == LOW)
    {
      digitalWrite(PEDESTRIAN_LED, HIGH);
      delay(LED_INTERVAL);
      digitalWrite(PEDESTRIAN_LED, LOW);
      delay(LED_INTERVAL);
    }
   
    // IR LED AC ON
    if (digitalRead(BOTTOM_BUTTON_PIN) == LOW)
    {
      // office AC on signal|||||||||||||
      uint64_t tRawData[] = {0x41006000008FC3, 0x5845002000};
      IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4600, 550, 1700, 550, 600, &tRawData[0], 104, PROTOCOL_IS_LSB_FIRST, 0, 0);
      delay(200);
      //my AC on signal|||||||||||||
      uint64_t tRawData2[]={0x56A900FF00FF00FF, 0x55AA2AD5};
      IrSender.sendPulseDistanceWidthFromArray(38, 6100, 7300, 600, 1650, 600, 550, &tRawData[0], 97, PROTOCOL_IS_LSB_FIRST, 0, 0);
      delay(200);
      //light turn on/off signal|||||||||||||
      IrSender.sendNEC(0x0, 0x40, 0);
      delay(200);
      }

    // OTA update & IR led AC OFF
    if (digitalRead(TOP_BUTTON_PIN) == LOW && digitalRead(BOTTOM_BUTTON_PIN) == LOW)
    {
      // office AC off signal|||||||||||||
      uint64_t tRawData[] = {0x41006000008FC3, 0x3845000000};
      IrSender.sendPulseDistanceWidthFromArray(38, 9000, 4550, 550, 1700, 550, 600, &tRawData[0], 104, PROTOCOL_IS_LSB_FIRST, 0, 0);
      //my AC OFF signal|||||||||||
      uint64_t tRawData2[]={0x54AB00FF00FF00FF, 0x55AA2AD5};
      IrSender.sendPulseDistanceWidthFromArray(38, 6050, 7350, 600, 1700, 600, 550, &tRawData[0], 97, PROTOCOL_IS_LSB_FIRST, 0, 0);
      yield();
      delay(5000);
      if (digitalRead(TOP_BUTTON_PIN) == LOW && digitalRead(BOTTOM_BUTTON_PIN) == LOW)
      {
        digitalWrite(PEDESTRIAN_LED, HIGH);   //keep the led on unitl update successful
        ledcWriteTone(BUZZER_PIN, 4000);
        delay(100);
        ledcWriteTone(BUZZER_PIN, 0);
        // reset using buttons or other input
        wm.resetSettings();            // wipe stored credentials for testing //these are stored by the esp library
        wm.setConfigPortalTimeout(120); // after n seconds the portal will close if no activity detected
        res = wm.autoConnect("ABHASH", "12345678");            //default password protected ap
      if (!res)
        {
          Serial.println("Failed to connect");
          // ESP.restart();
        }
        else
        {
          Serial.println("connected...yeey :)");
        }
      }
    }
    
    vTaskDelay(pdMS_TO_TICKS(100));  // Prevents watchdog timeout
  }
}





void sonarTaskLoop(void * pvParameters){
  for(;;){
    getSensorData();
    ledcWrite(MOTOR_PIN, 0);       //keep motor off in normal conditions

    // 0 0 0
    if (dL > dds && dF > ddf && dR > dds) //no object detected
    {
      Serial.println("0 0 0");
      ledcWrite(MOTOR_PIN, 0);       //keep motor off
      ledcWriteTone(BUZZER_PIN, 0);       //keep buzzer off
      if (headPhone == 1)
        A000();
    }

    // 0 1 0
    else if (dL > dds && dF < ddf && dR > dds)  //if object detected at front only
    {
      Serial.println("0 1 0");
      //int pow = (int)constrain(map(dF, 14, ddf, 255, 50), 50, 255); //turn on motor with map
      int pow = (int)constrain(map(dF, 14, ddf, 300, 100), 100, 300); //turn on motor with map  //limit value between 0 and 255
      ledcWrite(MOTOR_PIN, pow);

      if (headPhone == 0) S010();
      else A010();
    }

    // 0 0 1
    else if (dL > dds && dF > ddf && dR < dds)        //if object detected from right only
    {
      Serial.println("0 0 1");
      ledcWrite(MOTOR_PIN, 0);     //keep motor off

      if (headPhone == 0) S001();
      else A001();
    }

    // 0 1 1
    else if (dL > dds && dF < ddf && dR < dds)      //if obect detected from front and right
    {
      Serial.println("0 1 1");
      //int pow = (int)constrain(map(dF, 14, ddf, 255, 50), 50, 255); //turn on motor with map
      int pow = (int)constrain(map(dF, 14, ddf, 300, 100), 100, 300);
      ledcWrite(MOTOR_PIN, pow);

      if (headPhone == 0)
        S011();
      else
        A011();
    }

    // 1 0 0
    else if (dL < dds && dF > ddf && dR > dds)    //if object detected from left only
    {
      Serial.println("1 0 0");
      ledcWrite(MOTOR_PIN, 0);  //keep motor off

      if (headPhone == 0)
        S100();
      else
        A100();
    }

    // 1 1 0
    else if (dL < dds && dF < ddf && dR > dds)    //if object detected from left and front
    {
      Serial.println("1 1 0");
      //int pow = (int)constrain(map(dF, 14, ddf, 255, 50), 50, 255); //turn on motor with map
      int pow = (int)constrain(map(dF, 14, ddf, 300, 100), 100, 300);
      ledcWrite(MOTOR_PIN, pow);

      if (headPhone == 0)
        S110();
      else
        A110();
    }

    // 1 0 1
    else if (dL < dds && dF > ddf && dR < dds)  //if object detected from left and right
    {
      Serial.println("1 0 1");
      ledcWrite(MOTOR_PIN, 0);

      if (headPhone == 0)
        S101();
      else
      {
        if (dR < dds3 && dL < dds3)
        {
          S101();
        }
        else
        {
          ledcWriteTone(BUZZER_PIN, 0);
        }
        A101();
      }
    }

    // 1 1 1
    else if (dL < dds && dF < ddf && dR < dds)
    {
      Serial.println("1 1 1");
      ledcWrite(MOTOR_PIN, 500);

      if (headPhone == 0)
        S111();
      else
      {
        if (dR < dds3 && dL < dds3)
        {
          S111();
        }
        else
        {
          ledcWriteTone(BUZZER_PIN, 0);
        }
        A111();
      }
    }

  }
}



void setup()
{
  Serial.begin(115200);
  Serial.println("Started");

  // watch dog config
  //esp_task_wdt_deinit();

  // setup buzzer
  ledcAttachChannel(BUZZER_PIN, 5000, 8, BUZZER_CHANNEL); //ledcAttach(pin, frequency, resolution, channel)
  //setup motor
  ledcAttachChannel(MOTOR_PIN, 1000, 8, MOTOR_CHANNEL);

  // pin modes of buttons
  pinMode(TOP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BOTTOM_BUTTON_PIN, INPUT_PULLUP);

  // other pin modes
  pinMode(headphoneDetection, INPUT_PULLUP);
  pinMode(PEDESTRIAN_LED, OUTPUT); // pedestrian led
  pinMode(BATTERY_VOLT_PIN, INPUT);    // Battery voltage pin
  //pinMode(MOTOR_PIN, OUTPUT);

  // Power on schedule
  pinMode(PWR_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CH_PD_PIN, OUTPUT);    // sets GPIO0 as an output which will pull the mosfet ON
  digitalWrite(CH_PD_PIN, HIGH); // keep the mosfet off
  delay(1500);                   // wait for a defined delay time the device to boot up
  digitalWrite(CH_PD_PIN, LOW);  // latches the transistor to keep device on
  Serial.println("Device is ON");
  
  // boot up sound and vibration
  if(digitalRead(headphoneDetection) == 0)  //play buzzer if heapdhone is not connected during boot
  {
    ledcWriteTone(BUZZER_PIN, 1000); // Start ton
    delay(100);
    ledcWriteTone(BUZZER_PIN, 0);
    delay(100);
    ledcWriteTone(BUZZER_PIN, 1000); // Start ton
    delay(100);
    ledcWriteTone(BUZZER_PIN, 0);
    delay(100);
    ledcWriteTone(BUZZER_PIN, 1000); // Start ton
    delay(100);
    ledcWriteTone(BUZZER_PIN, 0);
    delay(100);
    ledcWrite(MOTOR_PIN, 400);    //motor on
    delay(500);
    ledcWrite(MOTOR_PIN, 0);
  }
  else   //play voice if heapdhone is connected during boot
  {
    ledcWrite(MOTOR_PIN, 400);
    Play_Power_On();                  //play power on voice
    ledcWrite(MOTOR_PIN, 0);
  }
  IrSender.begin(IR_LED_PIN); // Start with IR_SEND_PIN -which is defined in PinDefinitionsAndMore.h- as send pin
    
  //xTaskCreatePinnedToCore(task function, name of task, stack size of task, parameter of task, priority of task, keep track of task, which core)
  //default code loops on core 1 since core 0 used for RF communications
    xTaskCreatePinnedToCore(otherTaskLoop, "otherTask", 20000, NULL, 1, &otherTask,  0);
    delay(500); 
    xTaskCreatePinnedToCore(sonarTaskLoop, "sonarTask", 20000, NULL, 1, &sonarTask,  1); 
    delay(500);
}



void loop()
{
    // core 0 loop
    vTaskDelay(pdMS_TO_TICKS(1000)); // 10 ms delay
}