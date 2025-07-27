void setup() {
  Serial.begin(9600);
}

void loop() {
  int systolic = random(100, 140);
  int diastolic = random(65, 90);
   Serial.print("blood pressure: ");
   Serial.print(systolic);
   Serial.print("/");
   Serial.print(diastolic);
   Serial.print(" mmHg");
   delay(3000);
}