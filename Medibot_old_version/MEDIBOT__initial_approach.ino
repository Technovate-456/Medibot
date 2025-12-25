#include <Adafruit_MLX90614.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <Wire.h>

#include <MAX30105.h>
#include <heartRate.h>
#include <spo2_algorithm.h>



#define SCREEN_WIDTH 128 // for OLED
#define SCREEN_HEIGHT 64


unsigned long lastmeasure = 0;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();


MAX30105 pulsesensor;
const byte RATE_SIZE = 10;
int sortedrates[RATE_SIZE];

int ratespot = 0;
float temporary_var = 0;
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


void measurePulse(); // median-filtered BPM with stabilization
void measureSPO2(); // oxygen measurement
void measureTemp(); // temperature measurement

    



void setup() {
  Wire.begin(18, 19); // changed from arduino to esp32...
  Serial.begin(9600);
  mlx.begin();
  if (!pulsesensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30105 not found");
    while (1);
  }


  for (int i = 0; i < RATE_SIZE; i++) {
    sortedrates[i] = 0;
  }

  byte ledBrightness = 60; 
  byte sampleAverage = 4; 
  byte ledMode = 2; 
  byte sampleRate = 100; 
  int pulseWidth = 411; 
  int adcRange = 4096; 



  pulsesensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange );
   // put your setup code here, to run once:
} // configuring MAX30105 for pulse + SpO2 (IR + red LEDs)

void loop() { //integration of both the sensors(mlx90614 and max30105), pulse → SpO₂ → temperature workflow 
    measurePulse();

  if (millis() - lastmeasure > 1000 && done == true){ // ensures the pulse measurement is complete before collecting data for spo2
    measureSPO2();
    delay(100);
    measureTemp();
    
 done = false;// resets pulse completion flag for next turn
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
  if (irvalue > 50000) { // detects finger via IR intensity
    if (checkForBeat(irvalue) == true) { // checksfor heartbeat using the checkForBeat function
      delta = millis() - lastbeat; // calculates the time between consecutive beats
      lastbeat = millis();
    }
    bpm = 60 / (delta / 1000.0); // formula for calculating heartbeats per minute

    if (bpm > 20 && bpm < 255) { // checks if the bpm is under the human range
      sortedrates[ratespot++] = (byte)bpm; // stores valid BPM sample for filtering
      ratespot %= RATE_SIZE; 

  
      bool swap = true; // bubble sorting for the meadian averaging system (to reduce noise)
      while (swap == true) {
        swap = false;
        for (int i = 0; i < RATE_SIZE - 1; i++) {

          if (sortedrates[i] > sortedrates[i + 1]) { // sorts the data in ascending order for the median calculation, swaps the position if the next data is smaller than the previous one
            temporary_var = sortedrates[i + 1];
            swap = true;
            sortedrates[i + 1] = sortedrates[i];
            sortedrates[i] = temporary_var;
          }
        }
      }

      beatavg = (sortedrates[4] + sortedrates[5]) / 2; // middle value from 10 samples
    }

    if (bpm > 40 && bpm < 180 && beatavg > 40 && beatavg < 180 && abs(beatavg - bpm) < 15) { //(one final stabilized pulse shot) gives a stricter range of acceptable beats(between 40 and 180, and accepts if the difference is less than 15)
      stabilizedbeats++; // beats that comes into this range
      if (stabilizedbeats > 5 && !timerstarted) { // the condition to start the timer after 5 consecutive stabilizedbeats

        countertime = millis() + 10000; // 10 seconds timer to collect filtered beats for the final pulse rate
        timerstarted = true;
      }



      if (millis() < countertime && timerstarted) { // keeps collecting stabilized beats till 10 seconds
        beat2sum += beatavg;
        beatcount++;
        Serial.print("Samples collected: ");
        Serial.print(beatcount);



      } else if (millis() > countertime && timerstarted && !done) { // after the timer ends, it calculates the average of the stabilized beats
          
        beat2avg = beat2sum / beatcount;


        beat2sum = 0; // resets the count
        beatcount = 0;
        Serial.print(" final pulse rate: "); // prints the one-shot final pulse rate value
        Serial.print(beat2avg);


        done = true;
      }
    }

   if (beatavg > 20) {
    Serial.print("beatavg: ");
    Serial.print(beatavg);
    Serial.println(); // keeps printing the values for beats till the final pulse rate reading
   }
    

  } else {
    Serial.print("irvalue: ");
    Serial.print(irvalue);
    Serial.println(" place your finger");
  }
}
   
 void measureTemp(){
  float objectTemp = mlx.readObjectTempC();
  float ambientTemp = mlx.readAmbientTempC();

  if (objectTemp - ambientTemp > 2.0){ // detects human skin presence through thermal radiation
  float sum = 0;
 for(int i = 0; i < samples; i++) { // reduces noise by averaging multiple samples
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
  if (pulsesensor.getIR() > 50000){  // detects finger via IR intensity
  Serial.print("Collecting oxygen saturation..."); 

while (pulsesensor.available() == false) { // waits for new sensor sample
        pulsesensor.check();
      }

     for(int i = 0; i < samples2; i++) {
    while(pulsesensor.available() == false){
      pulsesensor.check();
    }
    irBuffer[i] = pulsesensor.getIR(); // stores ir data
    redBuffer[i] = pulsesensor.getRed(); // stores red light data
    pulsesensor.nextSample();

   Serial.print("irValue: ");
  Serial.print(irBuffer[i]);
  Serial.print("redvalue ");
  Serial.println(redBuffer[i]);
  }
maxim_heart_rate_and_oxygen_saturation(irBuffer, samples2, redBuffer, &SPO2, &validSPO2, &heartRate, &validHeartRate ); // uses the Maxim algorithm to compute spo2  
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


