/*

  Medibot Prototype Code
  Author: Nabiha Ahmed
  Purpose: Educational & research prototype only.
  Not a certified medical device.

*/


#include <Adafruit_MLX90614.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <Wire.h>

#include <MAX30105.h>
#include <heartRate.h>
#include <spo2_algorithm.h>



#define SCREEN_WIDTH 128 // for future OLED integration
#define SCREEN_HEIGHT 64 // // OLED libraries included


unsigned long lastmeasure = 0;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();


MAX30105 pulsesensor;
const byte RATE_SIZE = 10;
int sortedrates[RATE_SIZE];

// Pulse Measurement variables
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


// Temperature measurement variables
const int samples = 10;
int Temp[samples];
float sum = 0;
float tempAvg = 0;


// Oxygen Saturation measurement variables
uint32_t irBuffer[100];
uint32_t redBuffer[100];
int samples2 = 100;
int32_t SPO2 = 0;
int32_t heartRate = 0;
int8_t validHeartRate = 0;
int8_t validSPO2 = 0;
int SPO2spot = 0; 
int bufferSPO2[5];



void measurePulse(); // median-filtered BPM with stabilization
void measureSPO2(); // noise filtered oxygen measurement
void measureTemp(); // temperature measurement

   enum CheckupState { // (state machine algorithm) integration of both the sensors(mlx90614 and max30105), pulse → SpO₂ → temperature workflow 

    WAITING_FOR_USER,
    MEASURING_PULSE,
    MEASURING_SPO2,
    MEASURING_TEMP,
    SHOW_RESULTS
  };

  CheckupState currentState = WAITING_FOR_USER;



 void setup() {
  Wire.begin(18, 19); // changed from arduino to esp32 (I2C pins SDA=18, SCL=19)...
  Serial.begin(9600);
  mlx.begin();
  if (!pulsesensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30105 not found"); 
    while (1);
  }


  for (int i = 0; i < RATE_SIZE; i++) {
    sortedrates[i] = 0;
  }

  byte ledBrightness = 60; // setup for oxygen saturation
  byte sampleAverage = 4; 
  byte ledMode = 2; 
  byte sampleRate = 100; 
  int pulseWidth = 411; 
  int adcRange = 4096; 



  pulsesensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange );
} // configuring MAX30105 for pulse + SpO2 (IR + red LEDs)

void loop() { //controlling the workflow using state machine logic 
//pulse → SpO₂ → temperature workflow 

   switch (currentState) {
      case WAITING_FOR_USER: // waits for a finger to start

       irvalue = pulsesensor.getIR();
        
        if (irvalue > 50000) { // checks if a finger was detected to start
    
          ratespot = 0; 
          stabilizedbeats = 0;
          beatavg = 0;

          currentState = MEASURING_PULSE; // allows the next step if the measurement is successful
   } else {
      Serial.println("No finger detected");
      delay(100);
   }
    break; // stops the state machine if the current step is not completed

    case MEASURING_PULSE:
    measurePulse();
    break;

    case MEASURING_SPO2:
    measureSPO2();
    break;

    case MEASURING_TEMP:
    measureTemp();
    break;
    
    case SHOW_RESULTS: // produces an organized set of data after measurements are completed
      Serial.print("Heart Rate:  "); 
      Serial.print(beat2avg); 
      Serial.println(" BPM");

      Serial.print("Oxygen Sat:  "); 
      Serial.print(SPO2); 
      Serial.println(" %");

      Serial.print("Body Temp:   "); 
      Serial.print(tempAvg); 
      Serial.println(" C");
      
      Serial.println("=============================");
      Serial.println("Checkup Complete.");
      Serial.println("Remove finger to reset system...");
      
      delay(5000); // specific delay to let user read
      
      // Reset variables for the next round 
      beat2avg = 0;
      SPO2 = 0;
      tempAvg = 0;

      currentState = WAITING_FOR_USER;

    break;

 }

}
/*
  Pulse measurement algorithm:
  1. Detect heartbeat from IR signal
  2. Compute BPM from inter-beat interval
  3. Apply median filtering (10 samples)
  4. Accept only stabilized beats
  5. Average BPM over a fixed 10s timer
*/


   void measurePulse() {
  
  
  irvalue = pulsesensor.getIR(); 
  if (irvalue > 50000) { // detects finger via IR intensity
    if (checkForBeat(irvalue) == true) { // checks for heartbeat using the checkForBeat function
      delta = millis() - lastbeat; // calculates the time between consecutive beats
      lastbeat = millis();
    }
    bpm = 60 / (delta / 1000.0); // converts beat intervals to heartbeats per minute

    if (bpm > 20 && bpm < 255) { // checks if the bpm is under the human range
      sortedrates[ratespot++] = (byte)bpm; // stores valid BPM sample for filtering
      ratespot %= RATE_SIZE; 

  
      bool swap = true; // bubble sorting for the median averaging system (to reduce noise)
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

    if (bpm > 40 && bpm < 180 && beatavg > 40 && beatavg < 180 && abs(beatavg - bpm) < 15) { // Final pulse stabilization step; Accepts BPM only if within the range (40–180); and close to instantaneous BPM (difference < 15)
      stabilizedbeats++;
     if (!timerstarted){ // beats that falls into this range
      if (stabilizedbeats > 5) { // ensures the initialization of the timer after 5 consecutive stable beats to get rid of garbage values

        countertime = millis() + 10000; // 10 seconds timer to collect filtered beats for the final pulse rate
        timerstarted = true;
      } 
      
      } else {

     

      if (millis() < countertime) {   // keeps collecting stabilized beats till 10 seconds to 
        beat2sum += beatavg;
        beatcount++;
        Serial.print("Samples collected: ");
        Serial.print(beatcount);


         } else {  // after the timer ends, it calculates the average of the stabilized beats
           if (beatcount > 0) {
           beat2avg = beat2sum / beatcount;
         

        beat2sum = 0; // resets the count for the next round
        beatcount = 0;
        Serial.print(" final pulse rate: "); // prints the one-shot final pulse rate value
        Serial.print(beat2avg);
        currentState = MEASURING_SPO2;
        timerstarted = false;
        stabilizedbeats = 0;
      
      } else {
        Serial.print("Measurement failed. Please retry..."); 
        timerstarted = false; //. resets the calculation when failed to detect stable beats
        stabilizedbeats = 0;
      

      timerstarted = false;
      stabilizedbeats = 0; 
 
}

    

  }
    
} 
    }
  if (beatavg > 20) {
    Serial.print("beatavg: ");
    Serial.print(beatavg);
    Serial.println(); // keeps printing the values for beats till the final pulse rate reading
   }
  
  } else {
    Serial.println("Please place your finger...");
    currentState = WAITING_FOR_USER; // goes back to phase 1 if finger not detected
    return;
  }
   }


   /*
  Temperature measurement algorithm:
  1. Read object and ambient temperatures from MLX90614
  2. Detect finger presence using object–ambient temperature difference
  3. Collect multiple object temperature samples
  4. Average samples to reduce sensor noise
  5. Output stabilized body temperature estimate
*/

 void measureTemp(){
  float objectTemp = mlx.readObjectTempC(); // detects human skin presence through thermal radiation
  float ambientTemp = mlx.readAmbientTempC();

  if (objectTemp - ambientTemp > 2.0){ // detects the presence of finger through 
   sum = 0;
 for(int i = 0; i < samples; i++) { // reduces noise by averaging multiple samples
  sum += mlx.readObjectTempC();
   delay(10);   
 }
    tempAvg = sum/samples;
  Serial.print("Avg Temp: ");
  Serial.print(tempAvg);
  Serial.println(" C");
  currentState = SHOW_RESULTS;

 } else {
  Serial.println("Place your finger for Temperature");
  currentState = MEASURING_TEMP;
 }
 } 


 /*
     SpO₂ measurement algorithm:
  1. Confirm finger presence using IR intensity threshold
  2. Collect IR and red light samples (100)
  3. Compute SpO₂ using Maxim’s reference algorithm
  4. Accept only valid SpO₂ values within the range
  5. Apply outlier rejection (remove min and max)
  6. Average remaining values to reduce noise caused by sudden movements
*/

 void measureSPO2() {
  if (pulsesensor.getIR() > 50000){ // detects finger via IR intensity
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
 //based on IR and red light absorption ratios 
 


if (validSPO2 && SPO2 > 90 && SPO2 < 100){ // ensures the data is in human range to avoid fluctuations

  bufferSPO2[SPO2spot++] = SPO2; // stores the accepted oxygen data for averaging
  
  if (SPO2spot == 5){ // checks if we have 5 successful values to continue the averaging method

  int maxvalue = 0;
  int minvalue = 100;
  int sumSPO2 = 0;
 
 for (int n = 0; n < 5; n++){ 

  if (bufferSPO2[n] > maxvalue) maxvalue = bufferSPO2[n]; // recognizes the extreme maximum value to eliminate from the average

  if (bufferSPO2[n] < minvalue) minvalue = bufferSPO2[n]; // recognizes the extreme minimum value to eliminate from the average

 

 sumSPO2 += bufferSPO2[n];
  } 



   
 int averageSPO2 = (sumSPO2 - maxvalue - minvalue) / 3; // eliminates the extreme values to reduce motion-related noise


  
   
      Serial.print(" Final SpO2: ");
      Serial.print(averageSPO2);
      Serial.println(" %");

      currentState = MEASURING_TEMP;
      SPO2spot = 0;
  
  }
    





    
    } else {
      Serial.println("Invalid reading, try again."); // resets the measurements if it does not fall within the range, to avoid inconsistencies
      currentState = MEASURING_SPO2;
    }
  

  
  } else {
    Serial.print("Place your finger for oxygen saturation");
    SPO2spot = 0;
  }

  }


               
                                 
                                 

 



   // put your main code here, to run repeatedly



