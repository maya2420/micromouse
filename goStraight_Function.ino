#include <AccelStepper.h>

// Define the stepper motor and the pins that is connected to
AccelStepper stepperLeft(1, 8, 7);
AccelStepper stepperRight(1, 5, 4);


void goStraight() {

    stepperLeft.setCurrentPosition(0);
    stepperRight.setCurrentPosition(0);

    stepperLeft.moveTo(882);
    stepperRight.moveTo(882);

    while ((stepperLeft.currentPosition() < 882) || stepperRight.currentPosition() < 882) {

      stepperLeft.run();
      stepperRight.run();

    }
  }

void setup() {

  stepperLeft.setMaxSpeed(1000); // Set maximum speed value for the stepper
  stepperLeft.setAcceleration(500); // Set acceleration value for the stepper
  stepperLeft.setCurrentPosition(0); // Set the current position to 0 steps

  stepperRight.setMaxSpeed(1000);
  stepperRight.setAcceleration(500);
  stepperRight.setCurrentPosition(0);

  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);

}

void loop() {
//180 mm
  goStraight();

}

  

