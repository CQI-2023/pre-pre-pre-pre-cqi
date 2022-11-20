#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "Servo.h"

#define ENA 14  // (D5) => ENA - LEFT
#define ENB 12  // (D6) => ENB - RIGHT
#define IN_1 15 // (D8) => in1 - LEFT
#define IN_2 13 // (D7) => in2 - LEFT
#define IN_3 2  // (D4) => in3 - RIGHT
#define IN_4 0  // (D3) => in4 - RIGHT

#define SERVO1 5 // (D1)
#define SERVO2 4 // (D2)

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
Servo servo1;
Servo servo2;
int position_servo1 = 20;
int position_servo2 = 20;
const int position_pelle_max = 160;
const int position_pelle_min = 20;
const int position_pelle_init = 30;

void HTTP_handleRoot(void)
{
  if (server.hasArg("State"))
  {
    Serial.println(server.arg("State"));
  }
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
void leverPelle(){
  if(servo1.read() < position_pelle_max){
    servo2.write(position_servo1++);
  }
  if(servo2.read() < position_pelle_max){
    servo2.write(position_servo2++);
  }
}

void descendrePelle(){
  if(servo1.read() > position_pelle_min){
    servo2.write(position_servo1--);
  }
  if(servo2.read() > position_pelle_min){
    servo2.write(position_servo2--);
  }
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

  servo1.attach(SERVO1);
  servo2.attach(SERVO2);
  servo1.write(position_pelle_init);
  servo2.write(position_pelle_init);
}


void loop()
{
  server.handleClient();

  keyState = server.arg("State");
  keyPressed = server.arg("Key");

  // servo handler pas fini
  if(keyPressed == "r"){
    if (keyState == "DOWN") leverPelle();
    if (keyPressed == "f") descendrePelle();
  }
  // motor hanlder fonctionne
  if (keyState != lastKeyState || keyPressed != lastKeyPressed)
  {
    Serial.print(keyState);
    Serial.println(keyPressed);
    if(keyState == "DOWN"){
      if (keyPressed == "w") forward();
      if (keyPressed == "s") backward();
      if (keyPressed == "a") turnLeft();
      if (keyPressed == "d") turnRight();
      if (keyPressed == "u") forwardLeft(speedCar);
      if (keyPressed == "j") backwardLeft(speedCar);
      if (keyPressed == "o") forwardRight(speedCar);
      if (keyPressed == "l") backwardRight(speedCar);
    }
    if(keyState == "UP"){
      stopRobot();
    }
    lastKeyPressed = keyPressed;
    lastKeyState = keyState;
  }
}