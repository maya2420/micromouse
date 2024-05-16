#include <AccelStepper.h>
#include <vector>
using namespace std;
//const int stepsPerRev= 400;
AccelStepper mystepperLeft(1, 5, 4);
AccelStepper mystepperRight(1, 8, 7);


int SR = 0, FR = 0, FL = 0, SL = 0; //storing the value of the infrared sensors

int cruiseR = 1000;  // modified cruise speed of right wheel to keep you centered
int cruiseL = 1000;  // modified cruise speed of left wheel to keep you centered
int cruise = 1000; // actual cruising speed when searching

unsigned long lasttime = 0; // for doing delays kinda. just go look.
int straightCount = 0;

void goStraight(int straightCount); // self explanatory
void turnLeft(void);  // self explanatory
void turnRight(void);  // self explanatory
void oneeightyRight(void); // self explanatory but it turns in the direction of the wall it's furthest from
void oneeightyLeft(void); // self explanatory but it turns in the direction of the wall it's furthest from
void unfuck(void); //this function keeps you centetred between maze walls when navigating
void stop(void); //this function makes you stop
void find(); //finds next block in the pathway
void whereTo(); //finds what direction that block is in
void turnToDirections(); //puts information into directions of brudder
void go();
void wallcheck(void);

int maze[5][5] = {0};
int dir = 1,xpos = 0, ypos = 0, dis = 0; //direction the mouse is initially facing incrementing by one every 90 degrees clockwise and repeating after 4.
int checkspot = 0; //initially set to zero to check starting position
bool deadend;
bool branch, searching = 1;

int cX = 0;
int cY = 0;
int oX = 0;
int oY = 0;
int virdir = 1; //direction going to go in, not current direction
int vals[50] = {0}; //{1,1,1,2,2,2,2,3,4,3,4,4,3,2,2,2,1};
int valsSize = 17;
bool turn = 0; // true = right; false = left;
int c = 0; // current
int f = 1; //future
int x = 0; //incrementing
int CountW = 0; //counts in the where function
bool turnList[100]; //turns array
int distList[100]; //distance array
int directionsSize = 0; // size of directions list
int moveCount = 18; //how many moves made
int goalX = 0;
int goalY = 0;
int block = 1; //distance counter
int countPos = 2;
bool pathFound = false;


int matrix[5][5] = { {1,2,3,4,0},
                     {14,13,0,5,0},
                     {15,12,0,6,0},
                     {16,11,10,7,0},
                     {17,18,9,8,0} };

void setup() {
  // put your setup code here, to run once:
  mystepperLeft.setMaxSpeed(cruise);
  mystepperLeft.setAcceleration(4000);
  mystepperLeft.setCurrentPosition(0);


  mystepperRight.setMaxSpeed(cruise);
  mystepperRight.setAcceleration(4000);
  mystepperRight.setCurrentPosition(0);

  pinMode(9, OUTPUT);     //this pin is M1, used to configure modes for fractional steps
  pinMode(10, OUTPUT);    //this pin is M2, used to configure modes for fractional steps

  digitalWrite(9, HIGH);  //if 9 is low and 10 is low you do full steps. 9 high 10 low is half steps. 10 high 9 low quarter steps. 9 high 10 high eight steps
  digitalWrite(10, LOW);  

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}




void loop() {
  
  SR = analogRead(A0);
  FR = analogRead(A1);
  FL = analogRead(A2);
  SL = analogRead(A3);
/*
  if (FR > 950 || FL > 950) {
    //goStraight(); //are there two functions or does it need to be edited to goStraight(distance);
  } else if (SR > 950) {
    turnRight();
  } else if (SL > 950) {
    turnLeft();
  } else if (FR < 950 && FL < 950 && SR < 950 && SL < 950) {
    if (SR > SL) {
      oneeightyRight();
    }
    if (SL > SR) {
      oneeightyLeft();
    }
  }

    //movecount set to 13 rn
  //if(goalX == xpos && goalY == ypos){
    for (int x = 0; x < moveCount; x++) { // needs to be the movecount so whenever you give a block a single digit number make it movecount equal it
      find(); //its set to matrix rn, change to maze if using maze
      whereTo();
   }  

    for (int i = 0; i < valsSize; i++) {
      turnToDirections();
    }
    pathFound = true;
  //}

  //if(xpos == 0 && ypos == 0 && pathFound){
    go();
 // }*/

 for (int d = 0; d < 18; d++) { // needs to be the movecount so whenever you give a block a single digit number make it movecount equal it
    find();
    whereTo();
} 
 
for (int i = 0; i < 17; i++) {
    turnToDirections();
}
go();

}

//-----------------------------------------------------

void find(){
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == countPos) {
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
        virdir = 2;
    }

    if (oY < cY) {
        virdir = 4;
    }

    oX = cX;
    oY = cY;
    vals[CountW] = virdir;
    valsSize++;
    CountW++;

}

void turnToDirections(){
    
    if (vals[c] == vals[f]) {
        block++;
        
    }
    else if (vals[c] == 1 && vals[f] == 4) {
        turn = false;
        turnList[x] = turn;
        distList[x] = (block * 764) - 340;
        block = 1;
        
       
    }
    else if (vals[c] == 4 && vals[f] == 1) {
        turn = true;
        turnList[x] = turn;
        distList[x] = (block * 764) - 340;
        block = 1;
        
        
    }
    else if (vals[c] != 4 && vals[c] < vals[f]) {
        turn = true;
        turnList[x] = turn;
        distList[x] = (block * 764) - 340;
        block = 1;
        
    }
    else {
        turn = false;
        turnList[x] = turn;
        distList[x] = (block * 764) - 340;
        block = 1;
        
    }

    c++;
    f++;
    x++;
    directionsSize++;
}

void go(){
  /*if(dir == 1 && vals[0] == 2){
    turnRight();
  }
  if(dir == 1 && vals[0] == 3){
    turnLeft();
  }
  */
  for(int i = 0; i<moveCount; i++){
    if(distList[i] != 0){
      goStraight(distList[i]);

      if(turnList[i] == true){
        turnRight();
      }
      else if(turnList[i] == false){
        turnLeft();
      }
    }
  }
}

//-----------------------------------------------

void turnLeft(void) {
  mystepperRight.setCurrentPosition(0);
  mystepperLeft.setCurrentPosition(0);

  dir = dir - 1;
  if(dir < 1){
    dir = 4;
  }

  checkspot = 382;

  mystepperRight.move(490);

  while (mystepperLeft.distanceToGo() > 0 || mystepperRight.distanceToGo() > 0) {
    mystepperRight.run();
  }
}
void turnRight(void) {

  mystepperRight.setCurrentPosition(0);
  mystepperLeft.setCurrentPosition(0);

  dir = dir + 1;
  if(dir > 4){
    dir = 1;
  }

  checkspot = 382;

  mystepperLeft.move(490);

  while (mystepperLeft.distanceToGo() > 0 || mystepperRight.distanceToGo() > 0) {
    mystepperLeft.run();
  }
  
}
void goStraight(int straightCount) {

  mystepperRight.setCurrentPosition(0);
  mystepperLeft.setCurrentPosition(0);

  mystepperLeft.move(straightCount);
  mystepperRight.move(straightCount);

  while (mystepperLeft.distanceToGo() > 0 || mystepperRight.distanceToGo() > 0) {
    // Run the stepper motors towards their set positions
    mystepperLeft.run();
    mystepperRight.run();
    SR = analogRead(A0);
    FR = analogRead(A1);
    FL = analogRead(A2);
    SL = analogRead(A3);

    unfuck();
    posup();

    if ((FR < 950 || FL < 950) && searching  == 1) {
      stop();
      break;
    }
  }
}
void oneeightyRight(void) {

  //mystepperRight.setAcceleration(4000);
  //mystepperLeft.setAcceleration(4000);

  dir = dir + 2;
  if(dir == 5){
    dir = 1;
  }
  if(dir == 6){
    dir = 2;
  }

  mystepperRight.setCurrentPosition(0);
  mystepperLeft.setCurrentPosition(0);

  mystepperLeft.moveTo(480);
  mystepperRight.moveTo(-480);


  while ((mystepperLeft.distanceToGo() > 0) || mystepperRight.distanceToGo() > 0) {
    mystepperLeft.run();
    mystepperRight.run();
  }
  //mystepperRight.setAcceleration(8000);
  //mystepperLeft.setAcceleration(8000);
}
void oneeightyLeft(void) {

  //mystepperRight.setAcceleration(4000);
  //mystepperLeft.setAcceleration(4000);

  dir = dir - 2;
  if(dir == 0){
    dir = 4;
  }
  if(dir == -1){
    dir = 3;
  }

  mystepperRight.setCurrentPosition(0);
  mystepperLeft.setCurrentPosition(0);

  mystepperLeft.moveTo(-480);
  mystepperRight.moveTo(480);


  while ((mystepperLeft.distanceToGo() > 0) || mystepperRight.distanceToGo() > 0) {
    mystepperLeft.run();
    mystepperRight.run();
  }
  //mystepperRight.setAcceleration(4000);
  //mystepperLeft.setAcceleration(4000);
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
void stop(void) {
  mystepperLeft.move(250);
  mystepperRight.move(250);
  while (mystepperLeft.distanceToGo() > 0 || mystepperRight.distanceToGo() > 0) {
    // Run the stepper motors towards their set positions
    mystepperLeft.run();
    mystepperRight.run();
  }
  checkspot = 0;
  posup();
  cruiseL = cruise;
  cruiseR = cruise;
  mystepperLeft.setMaxSpeed(cruise);
  mystepperRight.setMaxSpeed(cruise);
}
void posup(void){
  if(mystepperLeft.currentPosition() >= checkspot){
    checkspot = checkspot + 764;
    if(dir == 1){
      ypos = ypos + 1;
    }
    if(dir == 2){
      xpos = xpos + 1;
    }
    if(dir == 3){
      ypos = ypos - 1;
    }
    if(dir == 4){
      xpos = xpos - 1;
    }
  }
  wallcheck();
  mazeup();
}
void mazeup(void){
  if(maze[xpos][ypos] == 0 && branch == 0 && deadend  == 0){
      dis = dis + 1;
      maze[xpos][ypos] = dis;
    }
    if(deadend  == 1){
      dis = dis + 1;
      maze[xpos][ypos] = dis + 200;
    }
    if(maze[xpos][ypos] == 0 && branch == 1){
      dis = dis + 1;
      maze[xpos][ypos] = dis + 100;
    }
    if(maze[xpos][ypos] != 0 && maze[xpos][ypos] < 100){
      dis = maze[xpos][ypos];
      maze[xpos][ypos] = maze[xpos][ypos] + 200;
    }
    if(maze[xpos][ypos] > 100 && maze[xpos][ypos] < 200 && branch == 0){
      dis = maze[xpos][ypos] - 100;
      maze[xpos][ypos] = maze[xpos][ypos] - 100;
    }
    if(maze[xpos][ypos] > 100 && maze[xpos][ypos] < 200 && branch == 1){
      dis = maze[xpos][ypos] - 100;
    }
    deadend = 0;
    branch = 0;
}
void wallcheck(void){
  if (FR < 950 && FL < 950 && SR < 950 && SL < 950){
    deadend = 1;
  }
  if(FR > 950 && SR > 950){
    branch = 1;
    if(dir == 1 && (maze[xpos][ypos + 1] != 0 || maze[xpos + 1][ypos] != 0)){
      branch = 0;
    }
    if(dir == 2 && (maze[xpos][ypos - 1] != 0 || maze[xpos + 1][ypos] != 0)){
      branch = 0;
    }
    if(dir == 3 && (maze[xpos][ypos - 1] != 0 || maze[xpos - 1][ypos] != 0)){
      branch = 0;
    }
    if(dir == 4 && (maze[xpos][ypos + 1] != 0 || maze[xpos - 1][ypos] != 0)){
      branch = 0;
    }
  }
  if(FL > 950 && SL > 950){
    branch = 1;
    if(dir == 1 && (maze[xpos][ypos + 1] != 0 || maze[xpos - 1][ypos] != 0)){
      branch = 0;
    }
    if(dir == 2 && (maze[xpos][ypos + 1] != 0 || maze[xpos + 1][ypos] != 0)){
      branch = 0;
    }
    if(dir == 3 && (maze[xpos][ypos - 1] != 0 || maze[xpos + 1][ypos] != 0)){
      branch = 0;
    }
    if(dir == 4 && (maze[xpos][ypos - 1] != 0 || maze[xpos - 1][ypos] != 0)){
      branch = 0;
    }
  }
  if(SR > 950 && SL > 950){
    branch = 1;
    if(dir == 1 && (maze[xpos + 1][ypos] != 0 || maze[xpos - 1][ypos] != 0)){
      branch = 0;
    }
    if(dir == 2 && (maze[xpos][ypos + 1] != 0 || maze[xpos][ypos - 1] != 0)){
      branch = 0;
    }
    if(dir == 3 && (maze[xpos + 1][ypos] != 0 || maze[xpos - 1][ypos] != 0)){
      branch = 0;
    }
    if(dir == 4 && (maze[xpos][ypos + 1] != 0 || maze[xpos][ypos - 1] != 0)){
      branch = 0;
    }
  }


}


