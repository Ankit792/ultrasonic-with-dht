#include "DHT.h"
#define DHT11PIN 16

DHT dht(DHT11PIN, DHT11);

// all 10 led's
const int led1 = 12;
const int led2 = 14;
const int led3 =27;
const int led4= 26;
const int led5 =25;
const int led6= 33;
const int led7 =32;
const int led8 =21;
const int led9 = 22;
const int led10 =23;

// #define motor 27
const int motor = 13;
const int trig = 5;
const int echo = 18;

int emptyTank = 0;
int fullTank = 100;

float filter[10]; 
float distance; 
long timetaken;
float percent;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode (trig, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(led1, OUTPUT);pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);pinMode(led10, OUTPUT);
}

void loop() {
  for (int trial =0; trial < 10; trial++) {
  filter[trial] = ultrasonicMeasure();
  delay(30); 
  }

  for (int i = 0; i < 9; i++) {  
    for (int j = i + 1; j < 10; j++) {
      if (filter[i] > filter[j]) {
        float swap = filter[i];
        filter[i] = filter[j];
        filter[j] = swap;
      }
    }
  }
  double sum = 0;
  for (int trial = 2; trial < 8; trial++) {
    sum += filter[trial];
  }

  distance = sum / 8;
  percent = map(distance, emptyTank, fullTank, 0, 100);
  percent = constrain(percent, 0, 100);

  if (percent<=10) {
    digitalWrite(motor, HIGH); digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);digitalWrite(led6, HIGH);digitalWrite(led7, HIGH);
    digitalWrite(led8, HIGH);digitalWrite(led9, HIGH);digitalWrite(led10, HIGH);
  } else {
    if (percent > 98) {
      digitalWrite(led10, LOW);
    } else {
      digitalWrite(led10, HIGH);
    }
    if (percent > 90) {
      digitalWrite(led9, LOW);
    } else {
      digitalWrite(led9, HIGH);
    }
    if (percent > 80) {
      digitalWrite(led8, LOW);
    } else {
      digitalWrite(led8, HIGH);
    }
    if (percent > 70) {
      digitalWrite(led7, LOW);
    } else {
      digitalWrite(led7, HIGH);
    }
    if (percent > 60) {
      digitalWrite(led6, LOW);
    } else {
      digitalWrite(led6, HIGH);
    }
    if (percent > 50) {
      digitalWrite(led5, LOW);
    } else {
      digitalWrite(led5, HIGH);
    }
    if (percent > 40) {
      digitalWrite(led4, LOW);
    } else {
      digitalWrite(led4, HIGH);
    }
    if (percent > 30) {
      digitalWrite(led3, LOW);
    } else {
      digitalWrite(led3, HIGH);
    }
    if (percent > 20) {
      digitalWrite(led2, LOW);
    } else {
      digitalWrite(led2, HIGH);
    }
    if (percent > 10) {
      digitalWrite(led1, LOW);
    } else {
      digitalWrite(led1, HIGH);
    }
  }
  if (percent > 98) {
    digitalWrite(motor, LOW); 
    digitalWrite(led10, LOW); 
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
