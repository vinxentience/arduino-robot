#include <dht.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
Servo servo;

//DHT11
const int dht_pin = A0;
int degree = 0;
//Buzzer
const int buzzer = 7;

//RGB LED
const int red_1 = 2;
const int green_1 = 3;
const int blue_1 = 4;
const int red_2 = 13;
const int green_2 = 12;
const int blue_2 = 11;
const int lineSensor_1 = 5;
const int lineSensor_2 = 6;
const int servo_pin = 9;
//Ultrasonic
long duration;
int distance;
const int trigPin = 8;
const int echoPin = 10;

//Objects
LiquidCrystal_I2C lcd(0x27, 16, 2);
dht DHT;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin();
  servo.attach(servo_pin);
  
  delay(2000);
  pinMode(buzzer, OUTPUT);
  pinMode(red_1, OUTPUT);
  pinMode(green_1, OUTPUT);
  pinMode(blue_1, OUTPUT);
  pinMode(red_2, OUTPUT);
  pinMode(green_2, OUTPUT);
  pinMode(blue_2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(lineSensor_1, INPUT);
  pinMode(lineSensor_2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  dht();

//  for(degree = 0; degree <= 180; degree += 1){
//    servo.write(degree);
//    delay(10);
//  }
  
  //ULTRASONIC
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  Serial.println(digitalRead(lineSensor_1));
  Serial.println(digitalRead(lineSensor_2));
  
  changeLightFromDistance(distance);

  lcd.clear();
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

void biot(){
  lightRed();
  delay(300);
  lightGreen();
  delay(300);
  lightBlue();
  delay(300);
}

void dht(){
  //DHT
  DHT.read11(dht_pin);
  double temp = DHT.temperature;
  if(temp > 33){
    lcd.setCursor(0,1);
    lcd.print("Hot temp");
    delay(300);
  }else if(temp < 29){
    lcd.setCursor(0,0);
    lcd.print("Cold temp");
    delay(300);
  }else{
    lcd.setCursor(0,0);
    lcd.print("Normal temp");
  }
}

void changeLightFromDistance(int distance) {
  
    if (distance < 15) { 
      lightRed();
      tone(buzzer, 700, 1000);
      lcd.setCursor(0,1);
      lcd.print("Obstacle found");
      checkDirection();
      delay(1000);
    } else {
      biot();
      lcd.setCursor(0,1);
      lcd.print("");
      noTone(buzzer);
      delay(500);
    }
}

void checkDirection(){
  servo.write(-180);
  delay(700);
  servo.write(180);
  delay(700);
  servo.write(90);
}
