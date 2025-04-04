#include "AccelStepper.h"

#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define LedGreen 4

#define MAXSPEED 600
#define MAX_WYCHYLENIE 30.0
#define SPEED 200     // kroki/sekundę

const int QRE1113 = A0;
const int KTIR0711S = A3;
float kolor_1;
float kolor_2;
const int reg_pozycji = A1;
const int reg_progu = A2;
int wychylenie;
int prog;

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  stepper.setMaxSpeed(MAXSPEED);
  stepper.setCurrentPosition(0);

  pinMode(QRE1113, INPUT);    //normalnie NAD BIBUŁĄ ( < próg)
  pinMode(KTIR0711S, INPUT);  //normalnie NIE NAD BIBUŁĄ ( > próg)

  pinMode(reg_pozycji, INPUT);
  pinMode(reg_progu, INPUT);

  pinMode(LedGreen, OUTPUT);
  digitalWrite(LedGreen, LOW);

  Serial.begin(9600);
}

void loop() { 
  kolor_1 = analogRead(QRE1113);
  kolor_2 = analogRead(KTIR0711S);
  wychylenie = int(float(analogRead(reg_pozycji))*(MAX_WYCHYLENIE/1023.0));
  prog = analogRead(reg_progu);
  Serial.println("kolor 1: " + String(kolor_1) + "kolor 2: " + String(kolor_2) + " wychylenie: " + String(wychylenie) + " próg: " + String(prog));

  if((kolor_1 > prog) && (stepper.currentPosition() != wychylenie)){ // bibula w zlej pozycji
    digitalWrite(LedGreen, HIGH);
    while(stepper.currentPosition() < wychylenie){
    stepper.setSpeed(SPEED);
    stepper.runSpeed();
    }
    while(stepper.currentPosition() > wychylenie){
    stepper.setSpeed(-SPEED);
    stepper.runSpeed();
    }
  }else if((kolor_2 < prog) && (stepper.currentPosition() != -wychylenie)){
    digitalWrite(LedGreen, HIGH);
    while(stepper.currentPosition() < -wychylenie){
    stepper.setSpeed(SPEED);
    stepper.runSpeed();
    }
    while(stepper.currentPosition() > -wychylenie){
    stepper.setSpeed(-SPEED);
    stepper.runSpeed();
    }
  }else if((kolor_1 <= prog) && (kolor_2 >= prog) && (stepper.currentPosition() != 0)){ // bazowanie
    digitalWrite(LedGreen, LOW);
    while(stepper.currentPosition() < 0){
    stepper.setSpeed(SPEED);
    stepper.runSpeed();
    }
    while(stepper.currentPosition() > 0){
    stepper.setSpeed(-SPEED);
    stepper.runSpeed();
    }
  }
  delay(300);
}