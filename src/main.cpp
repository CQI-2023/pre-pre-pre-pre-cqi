#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "ESP8266_ISR_Servo.h"
// #include <pwm.h>

#define ENA 14  // (D5) => ENA - LEFT
#define ENB 12  // (D6) => ENB - RIGHT
#define IN_1 15 // (D8) => in1 - LEFT
#define IN_2 13 // (D7) => in2 - LEFT
#define IN_3 2  // (D4) => in3 - RIGHT
#define IN_4 0  // (D3) => in4 - RIGHT

#ifndef ESP8266
  #error This code is designed to run on ESP8266 platform! Please check your Tools->Board setting.
#endif

#define TIMER_INTERRUPT_DEBUG       1
#define ISR_SERVO_DEBUG             1

// Published values for SG90 servos; adjust if needed
#define MIN_MICROS      554  //544
#define MAX_MICROS      2450 //2450

#define NUM_SERVOS    2


//wifi
const char *ssid = "Make DIY";
ESP8266WebServer server(80);

// Keys
String keyState;
String lastKeyState; 
String keyPressed;
String lastKeyPressed; 

// motors
int speedCar = 255; // 0 a 255
bool cringe = false;

//Servos
int position_servo1 = 20;
int position_servo2 = 20;
// const int position_pelle_max = 160;
// const int position_pelle_min = 20;
// const int position_pelle_init = 30;

typedef struct
{
  int     servoIndex;
  uint8_t servoPin;
} ISR_servo_t;

ISR_servo_t ISR_servo[NUM_SERVOS] =
{
  { -1, D0 }, { -1, D1 }//, { -1, D2 }, { -1, D5 }, { -1, D6 }, { -1, D7 }
};

void HTTP_handleRoot(void)
{
  // if (server.hasArg("State"))
  // {
  //   Serial.println(server.arg("State"));
  // }
  server.send(200, "text/html", "");
  delay(1);
}

// motor drive
void forwardRight(int speed){
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speed);
}

void backwardRight(int speed){
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speed);
}

void forwardLeft(int speed){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speed);
}

void backwardLeft(int speed){
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speed);
}

void stopRight(){
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, 0);
}

void stopLeft(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, 0);
}

// robot move
void forward()
{
  forwardLeft(speedCar);
  forwardRight(speedCar);
}

void backward()
{
  backwardLeft(speedCar);
  backwardRight(speedCar);
}

void turnRight()
{
  backwardRight(speedCar);
  forwardLeft(speedCar);
}

void turnLeft()
{
  backwardLeft(speedCar);
  forwardRight(speedCar);
}

void stopRobot()
{
  stopLeft();
  stopRight();
}

//ServoHandler
void leverCrucifix(int servo){
  ISR_Servo.setPosition(ISR_servo[servo].servoIndex, 0);
  delay(125);
}
void descendreLaColereDeDieu(int servo){
  ISR_Servo.setPosition(ISR_servo[servo].servoIndex, 180);
  delay(125);
}

void approcheHeaven(int servo, int step){
  int pos;
  if (servo == 0){
    position_servo1 += step;
    pos = position_servo1;
  }
  else if(servo == 1){
    position_servo2 += step;
    pos = position_servo2;
  }
  ISR_Servo.setPosition(ISR_servo[servo].servoIndex, pos);
  delay(125);
}

void approcheHeaven(int servo, int step){
  int pos;
  if (servo == 0){
    position_servo1 += step;
    pos = position_servo1;
  }
  else if(servo == 1){
    position_servo2 += step;
    pos = position_servo2;
  }
  ISR_Servo.setPosition(ISR_servo[servo].servoIndex, pos);
  delay(125);
}

void approcheHell(int servo, int step){
  int pos;
  if (servo == 0){
    position_servo1 -= step;
    pos = position_servo1;
  }
  else if(servo == 1){
    position_servo2 -= step;
    pos = position_servo2;
  }
  ISR_Servo.setPosition(ISR_servo[servo].servoIndex, pos);
  delay(125);
}

void setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  // Connecting WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Starting WEB-server
  server.on("/", HTTP_handleRoot);
  server.onNotFound(HTTP_handleRoot);
  server.begin();

  for (int index = 0; index < NUM_SERVOS; index++)
  {
    ISR_servo[index].servoIndex = ISR_Servo.setupServo(ISR_servo[index].servoPin, MIN_MICROS, MAX_MICROS);

    if (ISR_servo[index].servoIndex != -1)
      Serial.println("Setup OK Servo index = " + String(ISR_servo[index].servoIndex));
    else
      Serial.println("Setup Failed Servo index = " + String(ISR_servo[index].servoIndex));
  }

}


void loop()
{
  server.handleClient();

  keyState = server.arg("State");
  keyPressed = server.arg("Key");

  // if (servo1Last-micros())
  //   Serial.println();

  // servo handler pas fini
  // if(keyPressed == "r"){
  //   if (keyState == "DOWN") leverPelle();
  //   if (keyPressed == "f") descendrePelle();
  // }

  // motor hanlder fonctionne
  if (keyState != lastKeyState || keyPressed != lastKeyPressed)
  {
    Serial.println(keyState);
    Serial.println(keyPressed);
    Serial.println("-------");
    if(keyState == "DOWN"){
      if (keyPressed == "w") forward();
      if (keyPressed == "s") backward();
      if (keyPressed == "a") turnLeft();
      if (keyPressed == "d") turnRight();
      if (keyPressed == "u") forwardLeft(speedCar);
      if (keyPressed == "j") backwardLeft(speedCar);
      if (keyPressed == "o") forwardRight(speedCar);
      if (keyPressed == "l") backwardRight(speedCar);
      if (keyPressed == "r") leverCrucifix(0);
      if (keyPressed == "f") descendreLaColereDeDieu(0);
      if (keyPressed == "t") approcheHeaven(0, 10);
      if (keyPressed == "g") approcheHell(0, 10);
    }
    if(keyPressed == "UP"){
      if (keyPressed == "w" && keyPressed == "s" && keyPressed == "a" && 
      keyPressed == "d" && keyPressed == "u" && keyPressed == "j" 
      && keyPressed == "o" && keyPressed == "l"){
        stopRobot();
      }
    }
    lastKeyPressed = keyPressed;
    lastKeyState = keyState;
  }
}