#include <PID_v1.h>
#include <Encoder.h>

// Motor control pins
const int motorForwardPinL = 4; // PWM pin for forward direction
const int motorReversePinL = 6; // PWM pin for reverse direction

const int motorForwardPinR = 5; // PWM pin for forward direction
const int motorReversePinR = 7; // PWM pin for reverse direction

// Encoder pins
const int encoderPinA1 = 2;
const int encoderPinB1 = 9;
const int encoderPinA2 = 10;
const int encoderPinB2 = 8;

// Encoder setup
Encoder myEncoder1(encoderPinA1, encoderPinB1);
Encoder myEncoder2(encoderPinA2, encoderPinB2);

// PID variables
double Setpoint1, Input1, Output1, Output1Re;

double Setpoint2, Input2, Output2, Output2Re;

// Specify the links and initial tuning parameters
double Kp1 = 0.1, Ki1 = 0.0, Kd1 = 0.004;
PID myPID1(&Input1, &Output1, &Setpoint1, Kp1, Ki1, Kd1, DIRECT);

double Kp2 = 0.1, Ki2 = 0.0, Kd2 = 0.004;
PID myPID2(&Input2, &Output2, &Setpoint2, Kp2, Ki2, Kd2, DIRECT);

void setup() {
  pinMode(motorForwardPinL, OUTPUT);
  pinMode(motorReversePinL, OUTPUT);

  pinMode(motorForwardPinR, OUTPUT);
  pinMode(motorReversePinR, OUTPUT);

  // Initialize the PID
  Setpoint1 = 0; // Desired position
  myPID1.SetMode(AUTOMATIC);
  myPID1.SetOutputLimits(-255, 255); // Set output limits to match PWM signal range

  Setpoint2 = 0; // Desired position
  myPID2.SetMode(AUTOMATIC);
  myPID2.SetOutputLimits(-255, 255);
}

void turnRight(void);

void turnLeft(void);

void loop() {
  
turnLeft();

  // Optional: Add a delay or other logic as needed
}

void turnRight(void){
  Setpoint1 = 1755;
Input1 = myEncoder1.read();
  myPID1.Compute();

  // Adjust motor speed and direction based on PID output
  if (Output1 >= 0) {
    Output1Re = map(Output1, 0, 255, 55, 255);
    analogWrite(motorForwardPinL, Output1Re);
    analogWrite(motorReversePinL, 0);
  } else if(Output1 < 0) {
    Output1Re = map(Output1, 0, -255, -55, -255);
    analogWrite(motorForwardPinL, 0);
    analogWrite(motorReversePinL, -Output1Re); // Reverse output for reverse movement
  }
}

void turnLeft(void){
  Setpoint2 = 1755;
Input2 = myEncoder2.read();
  myPID2.Compute();

  // Adjust motor speed and direction based on PID output
  if (Output2 >= 0) {
    Output2Re = map(Output2, 0, 255, 55, 255);
    analogWrite(motorForwardPinR, Output2Re);
    analogWrite(motorReversePinR, 0);
  } else if(Output2 < 0) {
    Output2Re = map(Output2, 0, -255, -50, -255);
    analogWrite(motorForwardPinR, 0);
    analogWrite(motorReversePinR, -Output2Re); // Reverse output for reverse movement
  }
}