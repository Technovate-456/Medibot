#include <Adafruit_MLX90614.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <Wire.h>

#include <MAX30105.h>
#include <heartRate.h>
#include <spo2_algorithm.h>



#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


unsigned long lastmeasure = 0;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

MAX30105 pulsesensor;
const byte RATE_SIZE = 10;
int sortedrates[RATE_SIZE];

int ratespot = 0;
float tempo_var = 0;
long delta = 0;
long lastbeat = 0;
float bpm = 0;
int beatavg = 0;
long irvalue = 0;
long beatsum = 0;
float beat2sum = 0;
float beat2avg = 0;
int beatcount = 0;
unsigned long countertime;
bool done = false;
bool timerstarted = false;
int stabilizedbeats = 0;

const int samples = 10;
int Temp[samples];
float sum = 0;
float tempAvg = 0;


uint32_t irBuffer[100];
uint32_t redBuffer[100];
int samples2 = 100;
int32_t SPO2 = 0;
int32_t heartRate = 0;
int8_t validHeartRate = 0;
int8_t validSPO2 = 0;


void measurePulse();
void measureSPO2();
void measureTemp();

    



void setup() {
  Wire.begin(18, 19);
  Serial.begin(9600);
  mlx.begin();
  if (!pulsesensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30105 not found");
    while (1);
  }


  for (int i = 0; i < RATE_SIZE; i++) {
    sortedrates[i] = 0;
  }

  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384



  pulsesensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange );
   // put your setup code here, to run once:
}

void loop() {
 
    measurePulse();

  if (millis() - lastmeasure > 1000 && done == true){
    measureSPO2();
    delay(100);
    measureTemp();
    
 done = false;
lastmeasure = millis();
beat2sum = 0;
beatcount = 0;
stabilizedbeats = 0;
timerstarted = false;

Serial.println("Measurement Complete. Remove finger to reset.");
delay(2000);
  }
 }

   void measurePulse() {
  
  
  irvalue = pulsesensor.getIR();
  if (irvalue > 50000) {
    if (checkForBeat(irvalue) == true) {
      delta = millis() - lastbeat;
      lastbeat = millis();
    }
    bpm = 60 / (delta / 1000.0);

    if (bpm > 20 && bpm < 255) {
      sortedrates[ratespot++] = (byte)bpm;
      ratespot %= RATE_SIZE;

  
      bool swap = true;
      while (swap == true) {
        swap = false;
        for (int i = 0; i < RATE_SIZE - 1; i++) {

          if (sortedrates[i] < sortedrates[i + 1]) {
            tempo_var = sortedrates[i + 1];
            swap = true;
            sortedrates[i + 1] = sortedrates[i];
            sortedrates[i] = tempo_var;
          }
        }
      }

      beatavg = (sortedrates[4] + sortedrates[5]) / 2;
    }

    if (bpm > 40 && bpm < 180 && beatavg > 40 && beatavg < 180 && abs(beatavg - bpm) < 10) {
      stabilizedbeats++;
      if (stabilizedbeats > 12 && !timerstarted) {

        countertime = millis() + 10000;
        timerstarted = true;
      }



      if (millis() < countertime && timerstarted) {
        beat2sum += beatavg;
        beatcount++;
        Serial.print("Samples collected: ");
        Serial.print(beatcount);



      } else if (millis() > countertime && timerstarted && !done) {
          
        beat2avg = beat2sum / beatcount;


        beat2sum = 0;
        beatcount = 0;
        Serial.print(" final pulse rate: ");
        Serial.print(beat2avg);


        done = true;
      }
    }

   if (beatavg > 20)
    Serial.print("beatavg: ");
    Serial.print(beatavg);
    Serial.println();

    

  } else {
    Serial.print("irvalue: ");
    Serial.print(irvalue);
    Serial.println(" place your finger");
  }
}
   
 void measureTemp(){
  float objectTemp = mlx.readObjectTempC();
  float ambientTemp = mlx.readAmbientTempC();

  if (objectTemp - ambientTemp > 2.0){
  float sum = 0;
 for(int i = 0; i < samples; i++) {
  sum += mlx.readObjectTempC();
   delay(10);   
 }
   float tempAvg = sum / samples;
  Serial.print("Avg Temp: ");
  Serial.print(tempAvg);
  Serial.println(" C");
 } else {
  Serial.print("Place your finger for Temperature");
 }
  
 }

 void measureSPO2() {
  if (pulsesensor.getIR() > 50000){
  Serial.print("Collecting oxygen saturation...");

while (pulsesensor.available() == false) {
        pulsesensor.check();
      }

     for(int i = 0; i < samples2; i++) {
    while(pulsesensor.available() == false){
      pulsesensor.check();
    }
    irBuffer[i] = pulsesensor.getIR();
    redBuffer[i] = pulsesensor.getRed();
    pulsesensor.nextSample();

   Serial.print("irValue: ");
  Serial.print(irBuffer[i]);
  Serial.print("redvalue ");
  Serial.println(redBuffer[i]);
  }
maxim_heart_rate_and_oxygen_saturation(irBuffer, samples2, redBuffer, &SPO2, &validSPO2, &heartRate, &validHeartRate );
  // put your main code here, to run repeatedly:
  
 
if (validSPO2){

   
      Serial.print(" Final SpO2: ");
      Serial.print(SPO2);
      Serial.println(" %");
    } else {
      Serial.println("Invalid reading, try again.");
    }

  } else {
    Serial.print("Place your finger for oxygen saturation");
  }
  
  }



// put your main code here, to run repeatedly


