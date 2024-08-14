#include <AccelStepper.h>
#include <vector>
using namespace std;
//const int stepsPerRev= 400;
AccelStepper mystepperLeft(1, 5, 4);
AccelStepper mystepperRight(1, 8, 7);


int SR = 0, FR = 0, FL = 0, SL = 0;  //storing the value of the infrared sensors

int cruiseR = 1000;  // modified cruise speed of right wheel to keep you centered
int cruiseL = 1000;  // modified cruise speed of left wheel to keep you centered
int cruise = 1000;   // actual cruising speed when searching

unsigned long lasttime = 0;  // for doing delays kinda. just go look.
int straightCount = 0;

void goStraight(int straightCount);  // self explanatory
void turnLeft(void);                 // self explanatory
void turnRight(void);                // self explanatory
void oneeightyRight(void);           // self explanatory but it turns in the direction of the wall it's furthest from
void oneeightyLeft(void);            // self explanatory but it turns in the direction of the wall it's furthest from
void unfuck(void);                   //this function keeps you centetred between maze walls when navigating
void stop(void);                     //this function makes you stop
void find();                         //finds next block in the pathway
void whereTo();                      //finds what direction that block is in
void turnToDirections();             //puts information into directions of brudder
void go();
void wallcheck(void);
void printXY();
bool posup();
bool lookAhead();

//int maze[5][5]={0};
int dir = 1, xpos = 0, ypos = 0, dis = 1;  //direction the mouse is initially facing incrementing by one every 90 degrees clockwise and repeating after 4.
int checkspot = 764;                       //initially set to 764 to start at 0
bool deadend = 0;
bool branch, searching = 1;
bool lookAheadVar = false;
bool posupVar = false;

int cX = 0;
int cY = 0;
int oX = 0;
int oY = 0;
int virdir = 1;        //direction going to go in, not current direction
int vals[50] = { 0 };  //{1,1,1,2,2,2,2,3,4,3,4,4,3,2,2,2,1};
int valsSize = 17;
bool turn = 0;           // true = right; false = left;
int c = 0;               // current
int f = 1;               //future
int x = 0;               //incrementing
int CountW = 0;          //counts in the where function
bool turnList[5];      //turns array
int distList[5];       //distance array
int directionsSize = 0;  // size of directions list
int moveCount = 18;      //how many moves made
int goalX = 1;
int goalY = 1;
int block = 1;  //distance counter
int countPos = 2;
bool pathFound = false;
bool goalReached = false;

int xp[20];
int yp[20];


int maze[5][5] = { 0 };
int lengthMaze = 5;



void setup() {
  // put your setup code here, to run once:
  maze[0][0] = 1;

  mystepperLeft.setMaxSpeed(cruise);
  mystepperLeft.setAcceleration(4000);
  mystepperLeft.setCurrentPosition(0);


  mystepperRight.setMaxSpeed(cruise);
  mystepperRight.setAcceleration(4000);
  mystepperRight.setCurrentPosition(0);

  pinMode(9, OUTPUT);   //this pin is M1, used to configure modes for fractional steps
  pinMode(10, OUTPUT);  //this pin is M2, used to configure modes for fractional steps

  digitalWrite(9, HIGH);  //if 9 is low and 10 is low you do full steps. 9 high 10 low is half steps. 10 high 9 low quarter steps. 9 high 10 high eight steps
  digitalWrite(10, LOW);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}


int o = 0;

void loop() {

  //SR = analogRead(A0);
  //FR = analogRead(A1);
  //FL = analogRead(A2);
  //SL = analogRead(A3);

  physicalSearch();
}

void find() {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (maze[i][j] == countPos) {
        countPos++;
        cX = i;
        cY = j;
        return;
      }
    }
  }
}

void whereTo() {

  if (oX > cX) {
    virdir = 3;
  }

  if (oX < cX) {
    virdir = 1;
  }

  if (oY > cY) {
    virdir = 4;
  }

  if (oY < cY) {
    virdir = 2;
  }

  oX = cX;
  oY = cY;
  vals[CountW] = virdir;
  valsSize++;
  CountW++;
}

void turnToDirections() {       //book3

  if (vals[c] == vals[f]) {
    block++;

  } 
 else if (vals[c] == 1 && vals[f] == 4) {
    turn = false;
    turnList[x] = turn;

    distList[x] = (block * 764) - 340;  //why and how is this distance calculated
    block = 1;
    directionsSize++;
      x++;


  } else if (vals[c] == 4 && vals[f] == 1) {
    turn = true;
    turnList[x] = turn;

    distList[x] = (block * 764) - 340;
    block = 1;
    directionsSize++;
      x++;

  } else if (vals[c] != 4 && vals[c] < vals[f]) {
    turn = true;
    turnList[x] = turn;
    distList[x] = (block * 764) - 340;
    block = 1;
    directionsSize++;
      x++;

  } else if (vals[c] != 4 && vals[c] > vals[f]) {
    turn = false;
    turnList[x] = turn;

    distList[x] = (block * 764) - 340;
    block = 1;
    directionsSize++;
    x++;

  }
  c++;
  f++;
  
}

void go() {

  cruise = 1500;
  cruiseR = 1500;
  cruiseL = 1500;

  if (SR > SL) {
    oneeightyRight();
  }else if (SL > SR) {
    oneeightyLeft();
  }

  for (int i = 0; i <= directionsSize; i++) {

      goStraight(distList[i]);

      if (i < directionsSize) {
        if (turnList[i] == true) {
          turnRight();
        } 
        else if (turnList[i] == false) {
          turnLeft();
        }
      }
    }
    while (true) {

    }
  }


void goToStart() {   
  //Serial.println("i'm no longer hungry");
  if (SR > SL) {
    oneeightyRight();
  } else if (SL > SR) {
    oneeightyLeft();
  }

  for (int i = directionsSize; i >= 0; i--) {

      goStraight(distList[i]);

      if (i>0) {
        if (turnList[i-1] == true) {
          turnLeft();
        } 
        else if (turnList[i-1] == false) {
          turnRight();
        }
      }
  }
  
  pathFound = true;
}

//-----------------------------------------------

void turnLeft(void) {
  mystepperRight.setCurrentPosition(0);
  mystepperLeft.setCurrentPosition(0);

  dir = dir - 1;
  if (dir < 1) {
    dir = 4;
  }

  checkspot = 382;

  mystepperRight.move(540);

  while (mystepperLeft.distanceToGo() > 0 || mystepperRight.distanceToGo() > 0) {
    mystepperRight.run();
  }
  if (searching) {
  lookAhead(382, lookAheadVar);
  }
}
void turnRight(void) {

  mystepperRight.setCurrentPosition(0);
  mystepperLeft.setCurrentPosition(0);

  dir = dir + 1;
  if (dir > 4) {
    dir = 1;
  }

  checkspot = 382;

  mystepperLeft.move(540);

  while (mystepperLeft.distanceToGo() > 0 || mystepperRight.distanceToGo() > 0) {
    mystepperLeft.run();
  }
  if (searching) {
  lookAhead(382, lookAheadVar);
  }
}
void goStraight(int straightCount) {  //bookmark

  mystepperRight.setCurrentPosition(0);
  mystepperLeft.setCurrentPosition(0);

  mystepperLeft.move(straightCount);
  mystepperRight.move(straightCount);


  while (mystepperLeft.distanceToGo() > 0 || mystepperRight.distanceToGo() > 0) {
    if (lookAheadVar == true) {
      break;
    }
    // Run the stepper motors towards their set positions
    mystepperLeft.run();
    mystepperRight.run();
    SR = analogRead(A0);
    FR = analogRead(A1);
    FL = analogRead(A2);
    SL = analogRead(A3);

    unfuck();

  //  if (xpos == goalX && ypos == goalY) {
    //  goalReached = true;
    //  break;
   // }
    if ((FR < 950 || FL < 950) && searching) {
      stop(300);
      break;
    }
    if (searching) {
    posup(posupVar);
    if (posupVar == true) {
      break;
    }
    }
  }
}
void oneeightyRight(void) {

  //mystepperRight.setAcceleration(4000);
  //mystepperLeft.setAcceleration(4000);

  dir = dir + 2;
  if (dir == 5) {
    dir = 1;
  }
  if (dir == 6) {
    dir = 2;
  }

  mystepperRight.setCurrentPosition(0);
  mystepperLeft.setCurrentPosition(0);

  mystepperLeft.moveTo(540);
  mystepperRight.moveTo(-540);


  while ((mystepperLeft.distanceToGo() > 0) || mystepperRight.distanceToGo() > 0) {
    mystepperLeft.run();
    mystepperRight.run();
  }
  if (searching) {
  lookAhead(500, lookAheadVar);
  }
}
void oneeightyLeft(void) {

  //mystepperRight.setAcceleration(4000);
  //mystepperLeft.setAcceleration(4000);

  dir = dir - 2;
  if (dir == 0) {
    dir = 4;
  }
  if (dir == -1) {
    dir = 3;
  }

  mystepperRight.setCurrentPosition(0);
  mystepperLeft.setCurrentPosition(0);

  mystepperLeft.moveTo(-540);
  mystepperRight.moveTo(540);


  while ((mystepperLeft.distanceToGo() > 0) || mystepperRight.distanceToGo() > 0) {
    mystepperLeft.run();
    mystepperRight.run();
  }
  if (searching) {
  lookAhead(500, lookAheadVar);
  }
}
void unfuck(void) {
  if (millis() - lasttime > 1) {
    lasttime = millis();
    if (SL < 800) {
      cruiseL = cruise - SL + 800;
      mystepperLeft.setMaxSpeed(cruiseL);
    }
    if (SR < 800) {
      cruiseR = cruise - SR + 800;
      mystepperRight.setMaxSpeed(cruiseR);
    }
  }
}

// i updated stop to take in an integer. the integer represents how many steps until needed to stop. doing this made it possible to use for the lookAhead function as well
void stop(int stopIn) {
  mystepperLeft.move(stopIn);
  mystepperRight.move(stopIn);
  while (mystepperLeft.distanceToGo() > 0 || mystepperRight.distanceToGo() > 0) {
    // Run the stepper motors towards their set positions
    mystepperLeft.run();
    mystepperRight.run();
  }
  checkspot = 0;
  posup(posupVar);
  cruiseL = cruise;
  cruiseR = cruise;
  mystepperLeft.setMaxSpeed(cruise);
  mystepperRight.setMaxSpeed(cruise);
}

bool posup(bool posupVar) {
  if (mystepperLeft.currentPosition() >= checkspot) {
    checkspot = checkspot + 764;
    if (dir == 2) {
      ypos = ypos + 1;
    }
    if (dir == 1) {
      xpos = xpos + 1;
    }
    if (dir == 4) {
      ypos = ypos - 1;
    }
    if (dir == 3) {
      xpos = xpos - 1;
    }
    wallcheck();
    mazeup();
    checkGoal();
    lookAhead(500, lookAheadVar);
    if (lookAheadVar == true) {
      return posupVar = true;
      lookAheadVar = false;
    } else {
      return posupVar = false;
    }
  } else {
    return posupVar = false;
  }
}

void checkGoal(void) {    //zoo
                                                        
  if (xpos == goalX && ypos == goalY) {
    searching = false;
    //Serial.println(xpos);
    //Serial.println(ypos);
    for (int d = 0; d < dis-1; d++) {  // needs to be the movecount so whenever you give a block a single digit number make it movecount equal it
      find();
      whereTo();
    }
    for (int i = 0; i < dis-2; i++) {
      turnToDirections();
    }
    distList[x] = (block*764)-340;
   // delay(5000);
    goToStart();

    if (pathFound) {
      go();
    }
  }
}

void mazeup(void) {
  if (maze[xpos][ypos] != 0 && maze[xpos][ypos] < 100) {
    dis = maze[xpos][ypos];
    maze[xpos][ypos] = maze[xpos][ypos] + 200;
  }
  if (maze[xpos][ypos] == 0 && branch == 0 && deadend == 0) {
    dis = dis + 1;
    maze[xpos][ypos] = dis;
  }
  if (deadend == 1) {
    dis = dis + 1;
    maze[xpos][ypos] = dis + 200;
  }
  if (maze[xpos][ypos] == 0 && branch == 1) {
    dis = dis + 1;
    maze[xpos][ypos] = dis + 100;
  }
  if (maze[xpos][ypos] > 100 && maze[xpos][ypos] < 200 && branch == 0) {
    dis = maze[xpos][ypos] - 100;
    maze[xpos][ypos] = maze[xpos][ypos] - 100;
  }
  if (maze[xpos][ypos] > 100 && maze[xpos][ypos] < 200 && branch == 1) {
    dis = maze[xpos][ypos] - 100;
  }
  deadend = 0;
  branch = 0;
}
void wallcheck(void) {    //book2

  SR = analogRead(A0);
  FR = analogRead(A1);
  FL = analogRead(A2);
  SL = analogRead(A3);

  if (FR < 950 && FL < 950 && SR < 950 && SL < 950) {
    deadend = 1;
  }
  if (FR > 950 && SR > 950) {
    branch = 1;
    if (dir == 2 && (maze[xpos][ypos + 1] != 0 || maze[xpos - 1][ypos] != 0)) {
      branch = 0;
    }
    if (dir == 1 && (maze[xpos][ypos + 1] != 0 || maze[xpos + 1][ypos] != 0)) {
      branch = 0;
    }
    if (dir == 4 && (maze[xpos][ypos - 1] != 0 || maze[xpos + 1][ypos] != 0)) {
      branch = 0;
    }
    if (dir == 3 && (maze[xpos][ypos - 1] != 0 || maze[xpos - 1][ypos] != 0)) {
      branch = 0;
    }
  }
  if (FL > 950 && SL > 950) {
    branch = 1;
    if (dir == 1 && (maze[xpos][ypos - 1] != 0 || maze[xpos + 1][ypos] != 0)) {
      branch = 0;
    }
    if (dir == 2 && (maze[xpos][ypos + 1] != 0 || maze[xpos + 1][ypos] != 0)) {
      branch = 0;
    }
    if (dir == 3 && (maze[xpos][ypos + 1] != 0 || maze[xpos - 1][ypos] != 0)) {
      branch = 0;
    }
    if (dir == 4 && (maze[xpos][ypos - 1] != 0 || maze[xpos - 1][ypos] != 0)) {
      branch = 0;
    }
  }
  if (SR > 950 && SL > 950) {
    branch = 1;
    if (dir == 2 && (maze[xpos + 1][ypos] != 0 || maze[xpos - 1][ypos] != 0)) {
      branch = 0;
    }
    if (dir == 1 && (maze[xpos][ypos + 1] != 0 || maze[xpos][ypos - 1] != 0)) {
      branch = 0;
    }
    if (dir == 4 && (maze[xpos + 1][ypos] != 0 || maze[xpos - 1][ypos] != 0)) {
      branch = 0;
    }
    if (dir == 3 && (maze[xpos][ypos + 1] != 0 || maze[xpos][ypos - 1] != 0)) {
      branch = 0;
    }
  }
  if (FR> 950 && FL > 950 && SR > 950 && SL > 950) {
    branch = 1;
    if (dir == 2 && (maze[xpos+1][ypos] != 0 && maze[xpos-1][ypos] != 0 && (maze[xpos][ypos+1]) != 0)) {
      branch = 0;
    }
    if (dir == 1 && (maze[xpos+1][ypos] != 0 && maze[xpos][ypos+1] != 0 && (maze[xpos][ypos-1]) != 0)) {
      branch = 0;
    }
    if (dir == 4 && (maze[xpos+1][ypos] != 0 && maze[xpos-1][ypos] != 0 && (maze[xpos][ypos-1]) != 0)) {
      branch = 0;
    }
    if (dir == 3 && (maze[xpos-1][ypos] != 0 && maze[xpos][ypos+1] != 0 && (maze[xpos][ypos-1]) != 0)) {
      branch = 0;
    } 
  } 
}
void physicalSearch(void) {

  // option for going right, left, and straght after a stop are set to false until validation
  bool optionR = false;
  bool optionL = false;
  bool optionS = false;

  goStraight(12000);  // rudder goes straight until either a wall or lookAhead stops him

  SR = analogRead(A0);
  FR = analogRead(A1);
  FL = analogRead(A2);
  SL = analogRead(A3);

  // for each of the directions, rudder will decide if right, left, or straight are options
  if (dir == 1) {

    if (FR > 950) {  // making sure no wall is in front

      if (maze[xpos + 1][ypos] < 200) {  // when direction = 1, straight results in the y position increasing by 1
        optionS = true;
      }
    }

    if (SR > 950) {  // making sure no wall is to the right

      if (maze[xpos][ypos + 1] < 200) {  // when direction = 1, right results in the x position increasing by 1
        optionR = true;
      }
    }

    if (SL > 950) {  // making sure no wall is to the left

      if (maze[xpos][ypos - 1] < 200) {  // when direction = 1, left results in the x position decreasing by 1
        optionL = true;
      }
    }
    // if all options are open, we will prioritize unknown spaces, then straight, then right, then left
    if (optionS && optionR && optionL) {
      if (maze[xpos + 1][ypos] == 0) {  // most likely can delete bc optionS will never equal 0
        goStraight(12000);
      } else if (maze[xpos][ypos + 1] == 0) {  // checking to see if the square to the right is equal to 0
        turnRight();
      } else if (maze[xpos][ypos - 1] == 0) {  // checking to see if the square to the left is equal to 0
        turnLeft();
      } else {
        goStraight(12000);  // if no square straight, right, or left is equal to 0, go straight
      }
      // if right and left are open
    } else if (optionS && optionR) {
      if (maze[xpos + 1][ypos] == 0) {  // most likely can delete bc optionS will never equal 0
        goStraight(12000);
      } else if (maze[xpos][ypos + 1] == 0) {  // checking to see if the square to the right is equal to 0
        turnRight();
      } else {
        goStraight(12000);
      }
    } else if (optionS && optionL) {
      if (maze[xpos + 1][ypos] == 0) {  // most likely can delete bc optionS will never equal 0
        goStraight(12000);
      } else if (maze[xpos][ypos - 1] == 0) {  // checking to see if the square to the left is equal to 0
        turnLeft();
      } else {
        goStraight(12000);
      }
    } else if (optionR && optionL) {
      if (maze[xpos][ypos + 1] == 0) {  // check to see if the square to the right is equal to 0
        turnRight();
      } else if (maze[xpos][ypos - 1] == 0) {  // check to see if the square to the left is equal to 0
        turnLeft();
      } else {
        turnRight();  // if neither right or left squares are 0, turn right
      }
    } else if (optionS) {
      goStraight(12000);
    }
    // if only right is open
    else if (optionR) {
      turnRight();

      // if only left is open
    } else if (optionL) {
      turnLeft();
    }
  }
  // the rest of the directions are the same as dir = 1, just incremented appropriately
  else if (dir == 2) {

    if (FR > 950) {
      if (maze[xpos][ypos + 1] < 200) {
        optionS = true;
      }
    }

    if (SR > 950) {

      if (maze[xpos - 1][ypos] < 200) {
        optionR = true;
      }
    }

    if (SL > 950) {

      if (maze[xpos + 1][ypos] < 200) {
        optionL = true;
      }
    }
    if (optionS && optionR && optionL) {
      if (maze[xpos][ypos + 1] == 0) {  //most likely can delete bc optionS will never equal 0
        goStraight(12000);
      } else if (maze[xpos - 1][ypos] == 0) {
        turnRight();
      } else if (maze[xpos + 1][ypos] == 0) {
        turnLeft();
      } else {
        goStraight(12000);
      }
    } else if (optionS && optionR) {
      if (maze[xpos][ypos + 1] == 0) {  //most likely can delete bc optionS will never equal 0
        goStraight(12000);
      } else if (maze[xpos - 1][ypos] == 0) {
        turnRight();
      } else {
        goStraight(12000);
      }
    } else if (optionS && optionL) {
      if (maze[xpos][ypos + 1] == 0) {  //most likely can delete bc optionS will never equal 0
        goStraight(12000);
      } else if (maze[xpos + 1][ypos] == 0) {
        turnLeft();
      } else {
        goStraight(12000);
      }
    } else if (optionS) {
      goStraight(12000);
    }

    else if (optionR && optionL) {
      if (maze[xpos - 1][ypos] == 0) {
        turnRight();
      } else if (maze[xpos + 1][ypos] == 0) {
        turnLeft();
      } else {
        turnRight();
      }
    } else if (optionR) {
      turnRight();
    } else if (optionL) {
      turnLeft();
    }
  }

  else if (dir == 3) {

    if (FR > 950) {
      if (maze[xpos - 1][ypos] < 200) {
        optionS = true;
      }
    }

    if (SR > 950) {

      if (maze[xpos][ypos - 1] < 200) {
        optionR = true;
      }
    }

    if (SL > 950) {

      if (maze[xpos][ypos + 1] < 200) {
        optionL = true;
      }
    }
    if (optionS && optionR && optionL) {
      if (maze[xpos - 1][ypos] == 0) {  //most likely can delete bc optionS will never equal 0
        goStraight(12000);
      } else if (maze[xpos][ypos - 1] == 0) {
        turnRight();
      } else if (maze[xpos][ypos + 1] == 0) {
        turnLeft();
      } else {
        goStraight(12000);
      }
    } else if (optionS && optionR) {
      if (maze[xpos - 1][ypos] == 0) {  //most likely can delete bc optionS will never equal 0
        goStraight(12000);
      } else if (maze[xpos][ypos - 1] == 0) {
        turnRight();
      } else {
        goStraight(12000);
      }
    } else if (optionS && optionL) {
      if (maze[xpos - 1][ypos] == 0) {  //most likely can delete bc optionS will never equal 0
        goStraight(12000);
      } else if (maze[xpos][ypos + 1] == 0) {
        turnLeft();
      } else {
        goStraight(12000);
      }
    } else if (optionS) {
      goStraight(12000);
    }

    else if (optionR && optionL) {
      if (maze[xpos][ypos - 1] == 0) {
        turnRight();
      } else if (maze[xpos][ypos + 1] == 0) {
        turnLeft();
      } else {
        turnRight();
      }
    } else if (optionR) {
      turnRight();
    } else if (optionL) {
      turnLeft();
    }
  } else if (dir == 4) {

    if (FR > 950) {
      if (maze[xpos][ypos - 1] < 200) {
        optionS = true;
      }
    }

    if (SR > 950) {

      if (maze[xpos + 1][ypos] < 200) {
        optionR = true;
      }
    }

    if (SL > 950) {

      if (maze[xpos - 1][ypos] < 200) {
        optionL = true;
      }
    }
    if (optionS && optionR && optionL) {
      if (maze[xpos][ypos - 1] == 0) {  //most likely can delete bc optionS will never equal 0
        goStraight(12000);
      } else if (maze[xpos + 1][ypos] == 0) {
        turnRight();
      } else if (maze[xpos - 1][ypos] == 0) {
        turnLeft();
      } else {
        goStraight(12000);
      }
    } else if (optionS && optionR) {
      if (maze[xpos][ypos - 1] == 0) {  //most likely can delete bc optionS will never equal 0
        goStraight(12000);
      } else if (maze[xpos + 1][ypos] == 0) {
        turnRight();
      } else {
        goStraight(12000);
      }
    } else if (optionS && optionL) {
      if (maze[xpos][ypos - 1] == 0) {  //most likely can delete bc optionS will never equal 0
        goStraight(12000);
      } else if (maze[xpos - 1][ypos] == 0) {
        turnLeft();
      } else {
        goStraight(12000);
      }
    } else if (optionS) {
      goStraight(12000);
    }

    else if (optionR && optionL) {
      if (maze[xpos + 1][ypos] == 0) {
        turnRight();
      } else if (maze[xpos - 1][ypos] == 0) {
        turnLeft();
      } else {
        turnRight();
      }
    } else if (optionR) {
      turnRight();
    } else if (optionL) {
      turnLeft();
    }
  }
  // if neither straight, right, or left is open, rudder will do a UE (josh, can you update this to work with your decision on when to oneeightyright and oneeightyleft, please)
  if (!optionS && !optionR && !optionL) {
    if (SR > SL) {
      oneeightyRight();
    } else {
      oneeightyLeft();
    }
  }
}

bool lookAhead(int stopVal, bool lookAheadVar) {           //book6
  return lookAheadVar = false; 
  // for each direction, rudder checks to see if the square in front is in between 100 and 200, and then stops if it is
  if (dir == 1) {
    if ((maze[xpos + 1][ypos] < 200) && (maze[xpos + 1][ypos] > 100)) {
      stop(stopVal);  // stop in 1 or half a unit square
      return lookAheadVar = true;
    }
    while (!lookAheadVar && (xpos + 2) >= 0 && (xpos + 2) <= lengthMaze) {
      if (maze[xpos + 2][ypos] > 200) {
        stop(stopVal);
        return lookAheadVar = true;
      }
    }
  }
  if (dir == 2) {
    if ((maze[xpos][ypos + 1] < 200) && (maze[xpos][ypos + 1] > 100)) {
      stop(stopVal);  // stop in 1 or half a unit square
      return lookAheadVar = true;
    }
    while (!lookAheadVar && (ypos + 2) >= 0 && (ypos + 2) <= lengthMaze) {
      if (maze[xpos][ypos + 2] > 200) {
        stop(stopVal);
        return lookAheadVar = true;
      }
    }
  }
  if (dir == 3) {
    if ((maze[xpos - 1][ypos] < 200) && (maze[xpos - 1][ypos] > 100)) {
      stop(stopVal);  // stop in 1 or half a unit square
      return lookAheadVar = true;
    }
    while (!lookAheadVar && (xpos - 2) >= 0 && (xpos - 2) <= lengthMaze) {
      if (maze[xpos - 2][ypos] > 200) {
        stop(stopVal);
        return lookAheadVar = true;
      }
    }
  }
  if (dir == 4) {
    if ((maze[xpos][ypos - 1] < 200) && (maze[xpos - 1][ypos] > 100)) {
      stop(stopVal);  // stop in 1 or half a unit square
      return lookAheadVar = true;
    }
    while (!lookAheadVar && (ypos - 2) >= 0 && (ypos - 2) <= lengthMaze) {
      if (maze[xpos][ypos - 2] > 200) {
        stop(stopVal);
        return lookAheadVar = true;
      }
    }
  }
}