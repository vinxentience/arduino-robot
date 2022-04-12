#include "Constants.h"

#include <Servo.h>
#include <AFMotor.h>

#define MAX_DISTANCE 200
#define MAX_SPEED 100 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

AF_DCMotor leftMotor(1);
AF_DCMotor rightMotor(3);

boolean goesForward = false;
int distance = 100;
int speedSet = 0;

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
}

void loop()
{

    int distanceLeft = 0;
    int distanceRight = 0;
    delay(40);

    ultraSonicDistance = lookForward();

    if (ultraSonicDistance <= 15)
    {

        moveStop();
        delay(100);
        moveBackward();
        delay(300);
        moveStop();
        delay(200);

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
    tone(PIN_BUZZER, 1500, 300);
    delay(500);
    // tone(PIN_BUZZER, 500, 300);
    // delay(500);
    // tone(PIN_BUZZER, 1500, 300);
    // delay(300);
}

int readDistance()
{
    // Clears the trigPin condition
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    int duration = pulseIn(PIN_ECHO, HIGH);

    return duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
}

int lookForward()
{
    servo.write(90);
    int value = readDistance();
    readDistance();

    return value;
}

int lookLeft()
{
    servo.write(180);
    int value = readDistance();
    readDistance();
    servo.write(90);

    return value;
}

int lookRight()
{
    servo.write(0);

    int value = readDistance();
    readDistance();
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

    if (!goesForward)
    {
        goesForward = true;

        leftMotor.run(FORWARD);
        rightMotor.run(FORWARD);
    }
}

void moveBackward()
{
    Serial.println("Move Backward");

    goesForward = false;

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
