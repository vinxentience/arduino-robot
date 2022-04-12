#include "Constants.h"

// #include <NewTone.h>
#include <Servo.h>
#include <AFMotor.h>
#include <NewPing.h>

#define MAX_DISTANCE 200
#define MAX_SPEED 150 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

AF_DCMotor leftMotor(1, MOTOR12_1KHZ);
AF_DCMotor rightMotor(3, MOTOR12_1KHZ);

NewPing sonar(PIN_TRIG, PIN_ECHO, MAX_DISTANCE);

Servo servo;

long duration;
int ultraSonicDistance; // centimeter

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    servo.attach(PIN_SERVO);

    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
    delay(500);

    leftMotor.setSpeed(MAX_SPEED);
    rightMotor.setSpeed(MAX_SPEED);

    delay(500);
}

void loop()
{

    int distanceLeft = 0;
    int distanceRight = 0;
    delay(40);

    ultraSonicDistance = lookForward();
    delay(100);
    Serial.print("Distance: ");
    Serial.print(ultraSonicDistance);
    Serial.println(" cm");

    if (ultraSonicDistance <= 15)
    {

        moveStop();
        delay(300);

        playBuzzer();

        // Get distance from left and right
        distanceRight = lookRight();
        delay(300);
        distanceLeft = lookLeft();
        delay(300);

        if (distanceRight >= distanceLeft)
        {
            turnRight();
            moveStop();
        }
        else
        {
            turnLeft();
            moveStop();
        }
    }
    else
    {
        moveForward();
    }
}

void playBuzzer()
{
    // NewTone(PIN_BUZZER, 1500, 300);
    // delay(500);
    // NewTone(PIN_BUZZER, 500, 300);
    // delay(500);
    // NewTone(PIN_BUZZER, 1500, 300);
    // delay(300);
}

int readDistance()
{
    return sonar.ping_cm();
}

int lookForward()
{
    servo.write(90);
    int value = readDistance();

    return value;
}

int lookLeft()
{
    servo.write(180);
    int value = readDistance();
    delay(500);
    servo.write(90);

    return value;
}

int lookRight()
{
    servo.write(0);
    int value = readDistance();
    delay(500);
    servo.write(90);

    return value;
}

void moveStop()
{

    Serial.println("Move Stop");

    leftMotor.run(RELEASE);
    rightMotor.run(RELEASE);
}

void moveForward()
{
    Serial.println("Move Forward");

    leftMotor.run(FORWARD);
    rightMotor.run(FORWARD);
}

void moveBackward()
{
    Serial.println("Move Backward");

    leftMotor.run(BACKWARD);
    rightMotor.run(BACKWARD);
}

void turnRight()
{
    Serial.println("Turn Right");

    leftMotor.run(FORWARD);
    rightMotor.run(BACKWARD);
    delay(500);
    leftMotor.run(FORWARD);
    rightMotor.run(FORWARD);
}

void turnLeft()
{
    Serial.println("Turn left");

    leftMotor.run(BACKWARD);
    rightMotor.run(FORWARD);
    delay(500);
    leftMotor.run(FORWARD);
    rightMotor.run(FORWARD);
}
