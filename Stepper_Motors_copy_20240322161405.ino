
#include <AccelStepper.h>
//const int stepsPerRev= 200;
AccelStepper mystepperLeft(1,5,4);
AccelStepper mystepperRight(1,8,7);

bool stuck = true;   //We will use this to signal when all IR sensors detect a dead-end

void goStraight(void);
void goStraight_short(void);
int SR = 0, FR = 0, FL = 0, SL = 0;

 //void turnRight(){
   // mystepperRight.setCurrentPosition(0);
   // mystepperRight.moveTo(632);
   // mystepperRight.runToPosition();

 void turnLeft(void);
 void turnLeft(void);

 //Adjustments can made via small tail-fin manuever, 
 //by individualy changing single motor accell or speed

void setup() {
  // put your setup code here, to run once:
  mystepperLeft.setMaxSpeed(2000.0);           
  mystepperLeft.setAcceleration(750);          //getting up to setMax speed
  mystepperLeft.setCurrentPosition(0);

  mystepperRight.setMaxSpeed(2000.0);
  mystepperRight.setAcceleration(750);
  mystepperRight.setCurrentPosition(0);
   
  pinMode(9,OUTPUT);                        //this pin is M1, used to configure modes for fractional steps
  pinMode(10,OUTPUT);                       //this pin is M2, used to configure modes for fractional steps
  digitalWrite(9,HIGH);                     //We use this to make half-steps  
  digitalWrite(10,LOW);                     //Must be turned off to make half-steps

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
 SR = analogRead(A0);
 FR = analogRead(A1);
 FL = analogRead(A2);
 SL = analogRead(A3);

  if(FR > 850 || FL > 850){
    goStraight();
  }
  else if(SR > 850){
    turnRight();
  }
  else if(SL > 850){
    turnLeft();
  }
  
  /*while(stuck){
  
  mystepperRight.moveTo(-632);
  .moveTo(632);
  mystepperRight.run();
  .run();
  
  }
  delay(1500);
 */ 
}

void turnLeft(void) {
  mystepperRight.setCurrentPosition(0);
  mystepperRight.moveTo(632);
  mystepperRight.runToPosition();
  go_short();
 }
void turnRight(void) {
  mystepperLeft.setCurrentPosition(0);
  mystepperLeft.moveTo(632);                
  mystepperLeft.runToPosition();
  go_short();
 }

void go_short()
  {
    mystepperLeft.setCurrentPosition(0);
    mystepperRight.setCurrentPosition(0);

    mystepperLeft.moveTo(552);
    mystepperRight.moveTo(552);

    while ((mystepperLeft.currentPosition() < 552) || mystepperRight.currentPosition() < 552) {

      mystepperLeft.run();
      mystepperRight.run();
    

    }
  }

 void goStraight(void) {

    mystepperLeft.setCurrentPosition(0);
    mystepperRight.setCurrentPosition(0);

    mystepperLeft.moveTo(882);
    mystepperRight.moveTo(882);

    while ((mystepperLeft.currentPosition() < 882) || mystepperRight.currentPosition() < 882) {

      mystepperLeft.run();
      mystepperRight.run();
    

    }
  }
