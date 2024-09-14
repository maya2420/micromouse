#include <PID_v1.h>
#include <Encoder.h>

// Motor control pins
const int motorPin1 = 4; // Motor Pin FWD
const int motorPin2 = 6; // Motor Pin RVRS
const int encoderPin1A = 2; // Encoder Pin 1 PWM Forward
const int encoderPin2A = 9; // Encoder Pin 1 PWM Backward
const int encoderPin1B = 10; //Encoder PWM 2 Forward PWM
const int encoderPin2B = 8;  // Encoder PWM 2 Backward PWM

// Encoder variables
volatile long encoderTicks = 0;
unsigned long lastMillis = 0;

// Define PID variables
double setpoint = 1750; // out calculations found .0718 mm per pulse
double input, output;
double Kp = 1, Ki = 0.1, Kd = 0.2; // Adjust these parameters for your system
PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(encoderPin1A, INPUT);
  pinMode(encoderPin2A, INPUT);
  pinMode(encoderPin1B, INPUT);
  pinMode(encoderPin2B, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPin1A), countEncoderTicks, RISING);

  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(-255, 255); // Adjust this according to your motor

  
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= 100) { // Update PID every 100ms
    input = encoderTicks;
    pid.Compute();
    updateMotorSpeed(output);
    lastMillis = currentMillis;
  }
  delay(1500);
}

void countEncoderTicks() {
  encoderTicks++;
}


void updateMotorSpeed(double speed) {
  if (speed >= 0) {
    analogWrite(motorPin1, speed);
    analogWrite(motorPin2, 0);
  } else {
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, -speed);
  }
}
