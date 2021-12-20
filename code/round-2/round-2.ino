/*
 * This code handles the robot navigation
 * on the right side of the salinity course (Round 2).
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
void moveDiagonal(int duraton);


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
  // put your main code here, to run repeatedly:
  slideLeft();
  distance = SonarFront.ping_cm();
  while(distance < 35){
    long previous = SonarFront.ping_cm();
    delay(100);
    distance = SonarFront.ping_cm();
    selfCorrectLeft(distance - previous);
  }

  stopAll();
  distance = 0;

  //moveforward for 1.5 seconds, then slide right
  moveForwardFor(2200);
  stopAll();
  distance = 0;

  slideRightFor(1200);
  stopAll();
  distance = 0;

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

void moveDiagonal(int duration){
  //move front motors
  robot -> pcaDC2Motors(1, 80, 4, 80);
  //move back motors
  robot -> pcaDC2Motors(2, 108, 3, 108);
  delay(duration);
  robot -> pcaStop(1, 'm');
  robot -> pcaStop(2, 'm');
  robot -> pcaStop(3, 'm');
  robot -> pcaStop(4, 'm');
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
