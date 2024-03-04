#include "IR_sensors.h"
#include "motors.h"
#include "maze.h"

bool done = false;
bool stopped = false;

int front_R = 0; 
int front_L = 0;
int side_R = 0;
int side_L = 0;

int speedf = 100;
int speedr = 0;

unsigned long int time = millis();
unsigned long startMillis;

bool FR;
bool FL;
bool SR;
bool SL;

int encoder_Count = 0;
int enable9 = 1;
int enable10 = 1;
//if changing the speed or the bool "done" then needs to be done in micromouse.ino
//the classes are only for gathering and inputing information into micromouse.ino
//swapping and changing information back in forth can cause too much traffic and problems

int dis_R();
int dis_L();
void flood_fill();
void Ka_chow();
//do we need if stop functions are already checking for obstacles
void eyes();

void turn_Right() {
  int end_Turn_Left = encoder_Count + 95;
  int end_Turn_Right = encoder_Count + 125;
  while(!stopped){
    while((encoder_Count <= end_Turn_Right || encoder_Count <= end_Turn_Left)) {
      if (encoder_Count <= end_Turn_Left) {
        analogWrite(4, 210); // FWD Left On
      }
      if (encoder_Count > end_Turn_Left) {
        analogWrite(4, 0);
      }
      if (encoder_Count <= end_Turn_Right) {
        analogWrite(7, 150); // REV Right On
      }
      if (encoder_Count > end_Turn_Right) {
        analogWrite(7, 0);
      }
      if (enable9 == 1 && digitalRead(9) == HIGH) {
        encoder_Count = encoder_Count + 1;
        enable9 = 0;
      }
      if (enable9 == 0 && digitalRead(9) == LOW) {
        enable9 = 1;
      }
      if (enable10 == 1 && digitalRead(10) == HIGH) {
        encoder_Count = encoder_Count + 1;
        enable10 = 0;
      }
      if (enable10 == 0 && digitalRead(10) == LOW) {
        enable10 = 1;
      }
    }
    stopped = true;
  }
      analogWrite(7, 0);
      analogWrite(4, 0);
      unsigned long int t = millis();
      if(delay_2(t,500)){
        //forward();
      }
      
  }

void turn_Left(){
  while(!stopped){
    int end_Turn_Left = encoder_Count + 95;
    int end_Turn_Right = encoder_Count + 125;
    while (encoder_Count <= end_Turn_Right || encoder_Count <= end_Turn_Left) {
      if (encoder_Count <= end_Turn_Left) {
        analogWrite(5, 210); // FWD Right On
      }
      if (encoder_Count > end_Turn_Left) {
        analogWrite(5, 0);
      }
      if (encoder_Count <= end_Turn_Right) {
        analogWrite(6, 150); // REV Left On
      }

      if (encoder_Count > end_Turn_Right) {
        analogWrite(6, 0);
      }
      if (enable9 == 1 && digitalRead(9) == HIGH) {
        encoder_Count = encoder_Count + 1;
        enable9 = 0;
      }
      if (enable9 == 0 && digitalRead(9) == LOW) {
        enable9 = 1;
      }
      if (enable10 == 1 && digitalRead(10) == HIGH) {
        encoder_Count = encoder_Count + 1;
        enable10 = 0;
      }
      if (enable10 == 0 && digitalRead(10) == LOW) {
        enable10 = 1;
      }
      // motors off
      analogWrite(7, 0);
      analogWrite(4, 0);
      stopped = true;
    }
  }
}
bool delay_2(unsigned long start, unsigned long how_long){
  unsigned long int current = millis(); 
  if(current-start >= how_long){
    return true;
  }
  return false;
}
void stop(){
  while(!done ){
    startMillis = millis();
    speedf = 0;
    speedr = 50;
    done  = true;
  }
    if( delay_2(startMillis, 200)){
      speedr = 0;
      stopped = true;
    }
}

void stopSide(){
  while(!done){
  startMillis = millis();
  done = true;
  }
  if( delay_2(startMillis, 300) && (SL==true || SR==true) ){ //300
    speedf = 0;
    speedr = 50;
  }
  if( delay_2(startMillis, 350) && (SL==true || SR==true)){ //350
    speedr = 0;
  }
  if(SR && delay_2(startMillis, 400)){
    turn_Right();
  }
}

void forward(){
  unsigned long sm = millis();
  if(speedf != 100){
    while(speedf < 100 && stopped){
      if(delay_2(sm, 150)){
        speedf+=20;
      }
  }
    sm = millis();
    if(delay_2(sm, 100)){
      done = false;
      stopped = false;
    }
  }
}

int move(){
  if(!FR || !FL){
    return 2;
  }
  if((SL || SR) && !stopped){
    return 3;
  }
  
  return 0;
}

IR_sensors rudder_IR;
motors rudder_motors;
//maze Labyrinth;

void setup(){
  pinMode(A0, INPUT); //input pin declared
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  pinMode(4, OUTPUT); //output pin declared
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  pinMode(9, INPUT); // Encoder Left
  pinMode(10, INPUT);  // Encoder Right
  Serial.begin(300);
}


void loop() {
  front_L = analogRead(A0); //setting values
  side_L = analogRead(A1);
  front_R = analogRead(A2);
  side_R = analogRead(A3);

  rudder_IR.check_fR(front_R); // checking if wall
  rudder_IR.check_fL(front_L);
  rudder_IR.check_sL(side_L);
  rudder_IR.check_sR(side_R);

  FR = rudder_IR.returnFR(); //set bools for functions
  FL = rudder_IR.returnFL();
  SR = rudder_IR.returnSR();
  SL = rudder_IR.returnSL();

  analogWrite(4,speedf); 
  analogWrite(5,speedf);
  analogWrite(6,speedr);
  analogWrite(7,speedr);
  
  if(!FL || !FR){
    stop();
  } 

  if( (SR && !stopped) ){
    stopSide();
  }

  if( stopped && FR && FL){
    forward();
  }
 
}

/*
int answer = move();
  switch(answer) {
    case 1:
      stopSide();
      if(SR){
        turn_Right();
      }
      break;

    case 2:
      stop();
      break;

    case 0:
      forward();
      break;

    default:
      break;
  }
  */

  
