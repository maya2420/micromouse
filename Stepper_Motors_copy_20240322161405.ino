
#include <AccelStepper.h>
//const int stepsPerRev= 200;
AccelStepper mystepperLeft(1,5,4);
AccelStepper mystepperRight(1,8,7);

bool stuck = true;   //We will use this to signal when all IR sensors detect a dead-end

void goStraight(void);
void go_short(void);
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
  mystepperLeft.setMaxSpeed(8000.0);           
  mystepperLeft.setAcceleration(6000);          //getting up to setMax speed
  mystepperLeft.setCurrentPosition(0);

  mystepperRight.setMaxSpeed(8000.0);
  mystepperRight.setAcceleration(6000);
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

if (FR > 950 || FL > 950) {
    goStraight();
  } else if (SR > 950) {
    turnRight();
  } else if (SL > 950) {
    turnLeft();
  } else if (FR < 950 && FL < 950 && SR < 950 && SL < 950) {
    mystepperRight.setCurrentPosition(0);
    mystepperLeft.setCurrentPosition(0);
    mystepperLeft.moveTo(480);
    mystepperRight.moveTo(-480);


    while ((mystepperLeft.distanceToGo() > 0 ) || mystepperRight.distanceToGo() > 0) {
      mystepperLeft.run();
      mystepperRight.run();
    }

    goStraight();
  }  

}

void turnLeft(void) {
  mystepperRight.setCurrentPosition(0);
  mystepperRight.moveTo(480);
  mystepperRight.runToPosition();
  go_short();
 }
void turnRight(void) {
  mystepperLeft.setCurrentPosition(0);
  mystepperLeft.moveTo(480);                
  mystepperLeft.runToPosition();
  go_short();
 }

void go_short()
{
    mystepperLeft.setCurrentPosition(0);
    mystepperRight.setCurrentPosition(0);

    mystepperLeft.moveTo(424);
    mystepperRight.moveTo(424);

  while ((mystepperLeft.currentPosition() < 424) || mystepperRight.currentPosition() < 424) {
  
  
  mystepperLeft.run();
  mystepperRight.run();
    

}
}

 void goStraight(void) {

    mystepperLeft.setCurrentPosition(0);
    mystepperRight.setCurrentPosition(0);

    mystepperLeft.moveTo(764);
    mystepperRight.moveTo(764);

    while ((mystepperLeft.currentPosition() < 764) || mystepperRight.currentPosition() < 764) {
    /*if(mystepperLeft.currentPosition() == 441 || mystepperRight.currentPosition() == 441)
    {
      if(FR > 900 || FL > 900){
        
      mystepperLeft.setCurrentPosition(0);
      mystepperRight.setCurrentPosition(0);
    }*/
    
      mystepperLeft.run();
      mystepperRight.run();

    }
 } 
