/*
 * This code handles the robot navigation
 * on the left side of the maze (Round 1).
*/

#include <KNWRobot.h>
#include <NewPing.h>

KNWRobot *robot;

//constant
const int MAX_DISTANCE = 95;
const int MIN_DISTANCE = 70;
const int WALL_DISTANCE = 31;

long distance = 0;

NewPing SonarFront(27, 26, 300);
NewPing SonarLeft(25, 24, 150);
NewPing SonarRight (29, 28,150);

//Movement
void moveForward();
void slideRight();
void slideLeft();
void stopAll();

void moveForwardFor(int duration);
void slideRightFor(int duration);
void slideLeftFor(int duration);

//Sensor

//Etc.
void selfCorrectLeft(int diff);
void selfCorrectRight(int diff);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  robot = new KNWRobot();
  delay(500);
  
  //Setup DC Motors
  const int PIN_FRONT_MOTOR = 6;
  const int PIN_LEFT_MOTOR = 3; 
  const int PIN_BACK_MOTOR = 4;
  const int PIN_RIGHT_MOTOR = 5;

  robot -> setupMotor(1, PIN_FRONT_MOTOR, 94);
  robot -> setupMotor(2, PIN_LEFT_MOTOR, 94);
  robot -> setupMotor(3, PIN_BACK_MOTOR, 94); 
  robot -> setupMotor(4, PIN_RIGHT_MOTOR, 94);
}

void loop() {
//  bridge crossing Part: OK
  slideRightFor(900);
  stopAll();
  robot -> pcaDC2MotorsTime(1, 100, 3, 85, 4500);
  stopAll();
  delay(1000);

  // slide right here, preparing to rotate
  robot -> pcaDC2MotorsTime(1, 116, 3, 88, 400);
  stopAll();
  delay(1000);

  // rotate right, position itself for maze navigation
  robot -> pcaDC2Motors(2, 118, 4, 118);
  
  //robot -> pcaDC2Motors(2, 105, 4, 103);
  distance = SonarFront.ping_cm();
  while(distance < MAX_DISTANCE && distance > MIN_DISTANCE){
      distance = SonarFront.ping_cm();
      delay(1);
  }
  stopAll();
  distance = 0;
  

//  Maze Navigation Part: GOOD
//  Robot Moves forward until 'x' cm away from first wall
  moveForward();
  distance = SonarFront.ping_cm();  
  while(distance > WALL_DISTANCE){
    delay(100);
    distance = SonarFront.ping_cm();
  }
  
  distance = 0;
  stopAll();
  delay(1000);

  //Slide left until first opening
  unsigned long start_t = millis();
  slideLeft();
  distance = SonarFront.ping_cm();
  while(distance < 31){
    long previous = SonarFront.ping_cm();
    delay(100);
    distance = SonarFront.ping_cm();
    selfCorrectLeft(distance - previous);
  }

  //opening found
  stopAll();
  distance = 0;
  unsigned long end_t = millis() - 1100;
  
  delay(1000);
  unsigned long Time = end_t - start_t;

  // HarmonyRobot moves forward until second wall is met
  moveForward();
  distance = SonarFront.ping_cm();
  while(distance > WALL_DISTANCE){
    delay(200);
    distance = SonarFront.ping_cm();
  }

  distance = 0;
  stopAll();
  delay(1000);

  //Slides right for "Time" seconds
  //slideRightFor(Time - 100);
  slideRight();
  int currentTime = millis();
  while(millis() < currentTime + Time){
    long previous = SonarFront.ping_cm();
    delay(300);
    distance = SonarFront.ping_cm();
    selfCorrectRight(distance - previous);
  }
  

  distance = 0;
  stopAll();
  delay(1000);

  //slide left until opening is found
  distance = SonarFront.ping_cm();
  slideLeft();
  while(distance < 31){
    long previous = SonarFront.ping_cm();
    delay(100);
    distance = SonarFront.ping_cm();
    selfCorrectLeft(distance - previous);
  }

  stopAll();
  distance = 0;
  delay(1000);

  //HarmonyRobot moves forward for 2 seconds.
  //Maze successfully navigated
  moveForwardFor(2000);
  stopAll();
  delay(1000);

  exit(0);
}

void moveForward(){
  robot -> pcaDC2Motors(2, 108, 4, 80);
}

void slideRight(){
  robot -> pcaDC2Motors(1, 109, 3, 77);
}

void slideLeft(){
//  (1, 77, 3, 113)
  robot -> pcaDC2Motors(1, 80, 3, 109);
}

void stopAll(){
  robot -> pcaStopAll();
}

void moveForwardFor(int duration){
  robot -> pcaDC2MotorsTime(2, 108, 4, 80, duration);
}

void slideRightFor(int duration){
  robot -> pcaDC2MotorsTime(1, 109, 3, 77, duration);
}

void slideLeftFor(int duration){
  robot -> pcaDC2MotorsTime(1, 80, 3, 109, duration);
}

void selfCorrectLeft(int diff){
  if(diff < 0){
    //power up front motor(1)
    robot -> pcaDC2MotorsTime(1, 75, 3, 103, 200);
    robot -> pcaStopAll();
    return;
  }
  else if(diff > 0){
    //power up back motor(3)
    robot -> pcaDC2MotorsTime(1, 85, 3, 117, 200);
    robot -> pcaStopAll();
    return;
  } 
}

void selfCorrectRight(int diff){
  if(diff < 0){
    //spin up front motor(1)
    robot -> pcaDC2MotorsTime(1, 114, 3, 80, 300);
    robot -> pcaStopAll();
    return;
  }
  else if(diff > 0){
    //spin up back motor(3)
    robot -> pcaDC2MotorsTime(1, 103, 3, 80, 300);
    robot -> pcaStopAll();
    return;
  }

}
