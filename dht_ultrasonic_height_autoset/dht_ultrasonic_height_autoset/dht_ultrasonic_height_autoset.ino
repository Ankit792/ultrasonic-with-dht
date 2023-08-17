#include "DHT.h"
#define DHT11PIN 16

DHT dht(DHT11PIN, DHT11);
const int led1 = 12;

const int motor = 13;
const int trig = 5;
const int echo = 18;

int emptyTank = 0;
int fullTank = 0;
int height = 0;

float filter[20]; 
float distance; 
long timetaken;
float percent;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(trig, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(led1, OUTPUT);

    for (int seth=0; seth<20; seth++) {
    fullTank += ultrasonicMeasure();
    delay(30); 
  }
  height = fullTank/20; 
  Serial.println(fullTank);
  Serial.println(height);
  Serial.print("-------------------");
}

void loop() {
  for (int trial =0; trial < 20; trial++) {
  filter[trial] = ultrasonicMeasure();
  delay(30); 
  }

  for (int i = 0; i < 19; i++) {  
    for (int j = i + 1; j < 20; j++) {
      if (filter[i] > filter[j]) {
        float swap = filter[i];
        filter[i] = filter[j];
        filter[j] = swap;
      }
    }
  }
  double sum = 0;
  for (int trial = 5; trial < 15; trial++) {
    sum += filter[trial];
  }

  distance = sum / 10;
  percent = map(distance, emptyTank, height, 0, 100);
  percent = constrain(percent, 0, 100);

  if (percent<=10) {
    digitalWrite(motor, HIGH); digitalWrite(led1, HIGH);
  } 
  if (percent > 98) {
    digitalWrite(motor, LOW); digitalWrite(led1, LOW); 
  }

  Serial.print("distance: ");
  Serial.print(distance);
  Serial.print(" cm and percent: ");
  Serial.println(percent);
}

float ultrasonicMeasure() {
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  float speedSound,speed_cm_us;
  
  speedSound = 331.4 + (0.606*temp) + (0.0124*hum); // in m/s
  speed_cm_us = (speedSound*100)/1000000;           // in cm/us
  
  digitalWrite (trig, LOW); 
  delayMicroseconds(2);
  digitalWrite (trig, HIGH);
  delayMicroseconds(10);
  digitalWrite (trig, LOW);
  timetaken = pulseIn(echo, HIGH); 
  distance = timetaken * (speed_cm_us/2); 
  delay(1);
  return distance;
}
