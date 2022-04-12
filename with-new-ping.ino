#include <AFMotor.h>//Adafruit Motor Shield Library
#include <NewPing.h> //Ultrasonic Sensor Library
#include <Servo.h>
//variable declarations
#define TRIG_PIN 8
#define ECHO_PIN 10
#define MAX_DISTANCE 300 //equivalent to 300cm
#define COLL_DIST 30 //3cm distance before collision
#define MAX_SPEED 160
#define MAX_SPEED_OFFSET 40 // this sets offset to allow for differences between the two DC traction motors
#define TURN_DISTCOLL_DIST + 20 //turn distance
AF_DCMotor leftMotor1(3, MOTOR12_1KHZ);
AF_DCMotor rightMotor1(1, MOTOR12_1KHZ);
Servo myservo;
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);//sets up sensror library to use the correct pins to measure distance
int leftDistance, rightDistance;
int ourDist=0;
String motorSet="";
int speedSet=0;

const int red_1 = 2;
const int green_1 = 3;
const int blue_1 = 4;
const int red_2 = 13;
const int green_2 = 12;
const int blue_2 = 11;


void setup() {
 myservo.attach(9); //Pin # 10 where your servo is attached
 myservo.write(90); //rotate 90 degrees
  pinMode(red_1, OUTPUT);
  pinMode(green_1, OUTPUT);
  pinMode(blue_1, OUTPUT);
  pinMode(red_2, OUTPUT);
  pinMode(green_2, OUTPUT);
  pinMode(blue_2, OUTPUT);
 delay(1000);
}
void loop() {
 myservo.write(90); //eyes in front
 delay(90); //delay of 90 ms
 ourDist=readPing(); //read data from distance sensor
 if(ourDist<COLL_DIST) //compares data with coll dist.
 //if ourDist is < coll dist, it will change path
//if greater than, it will continue its motion(move forward)
 {
 changePath();
 }
 lightBlue();
  lightRed();
  lightGreen();
 moveForward();
 delay(500);
}
//---------------------------CHANGE PATH-------------------------------
//defining changePath function(stop, right, left, check
void changePath()
{
 moveStop();
 myservo.write(36); //check distance to the right
 delay(500);
 rightDistance=readPing(); //set right distance
 delay(500);
 myservo.write(144); //check distance to the left
 delay(700);
 leftDistance=readPing(); //set left distance
 delay(500);
 myservo.write(90); //return to center
 delay(100);
 compareDistance();

}
//------------Compare Distance---------------------------
void compareDistance() //find the longest distance
{
 if(leftDistance>rightDistance) //if left is obstructed
 {
 turnLeft();
 }
 if(rightDistance>leftDistance) //if right is less obstructed
 {
 turnRight();
 }
 else //if they are equally obstructed
 {
 turnAround();
 }
}
//-------------------------readPing()--------------------
int readPing() //read ultrasonic sensor distance
{
 delay(70);
 unsigned int uS=sonar.ping();
 int cm=uS/US_ROUNDTRIP_CM;
 return cm;

}
//---------------------moveStop--------------(stop robot from moving any direction)
void moveStop() //stops the motors
{
 leftMotor1.run(RELEASE);
 rightMotor1.run(RELEASE);
}
//--------------------moveForward-------------
void moveForward()
{
 motorSet="FORWARD";
 leftMotor1.run(FORWARD);
 rightMotor1.run(FORWARD);
 for(speedSet=0; speedSet< MAX_SPEED; speedSet +=2) //slowly bring the speed up to avoid loading the batteries too quick.
 {
 leftMotor1.setSpeed(speedSet);
 rightMotor1.setSpeed(speedSet);
 delay(5);

 }

}
//-----------------moveBackward()---------------------------
//Just copy paste moveForward and change forward to backward
void moveBackward()
{
 motorSet="BACKWARD";
 leftMotor1.run(BACKWARD);
 rightMotor1.run(BACKWARD);
 for(speedSet=0; speedSet< MAX_SPEED; speedSet +=2)//slowly bring the speed up to avoid loading the batteries too quick.
 {
 leftMotor1.setSpeed(speedSet);
 rightMotor1.setSpeed(speedSet);
 delay(5);
 }

}
//-------------------------moveRight--------------------------
void turnRight()
{
 motorSet="RIGHT";
 leftMotor1.run(FORWARD);//turn motor 1 forward
 rightMotor1.run(BACKWARD); //turn motor 2 backward

 rightMotor1.setSpeed(speedSet + MAX_SPEED_OFFSET);//slowly bring the speed up to avoid loading the batteries too quick.
 delay(1500);
 motorSet="FORWARD";
 leftMotor1.run(FORWARD); //set motors back to forward
 rightMotor1.run(FORWARD);

}
//-------------------------moveLeft--------------------------
void turnLeft()
{
 motorSet="LEFT";
 leftMotor1.run(BACKWARD);//turn motor 1 backward
leftMotor1.setSpeed(speedSet + MAX_SPEED_OFFSET);

 rightMotor1.run(FORWARD); //turn motor 2 forward
 delay(1500);

 motorSet="FORWARD";
 leftMotor1.run(FORWARD); //set motors back to forward
 rightMotor1.run(FORWARD);


}
//-------------------------turnAround--------------------------
void turnAround() {
motorSet="RIGHT";
 leftMotor1.run(FORWARD);//turn motor 1 forward
 rightMotor1.run(BACKWARD); //turn motor 2 forward

 rightMotor1.setSpeed(speedSet + MAX_SPEED_OFFSET);//slowly bring the speed up to avoid loading the batteries too quick.
 delay(1700);
 motorSet="FORWARD";
 leftMotor1.run(FORWARD); //set motors back to forward
 rightMotor1.run(FORWARD);
}


void setColor(int redVal, int greenVal, int blueVal){
  analogWrite(red_1, redVal);
  analogWrite(green_1, greenVal);
  analogWrite(blue_1, blueVal);
}
void setColor2(int redVal, int greenVal, int blueVal){
  analogWrite(red_2, redVal);
  analogWrite(green_2, greenVal);
  analogWrite(blue_2, blueVal);
}

void lightBlue() {
  setColor(0, 0, 255);
  setColor2(0, 0, 255);
}

void lightRed() {
  setColor(255, 0,0);
  setColor2(255, 0,0);
}

void lightGreen() {
  setColor(0, 255, 0);
  setColor2(0, 255, 0);
}
