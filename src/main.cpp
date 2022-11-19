#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define ENA 14  // (D5) => ENA - LEFT
#define ENB 12  // (D6) => ENB - RIGHT
#define IN_1 15 // (D8) => in1 - LEFT
#define IN_2 13 // (D7) => in2 - LEFT
#define IN_3 2  // (D4) => in3 - RIGHT
#define IN_4 0  // (D3) => in4 - RIGHT

String keyState;
String lastKeyState; 
String keyPressed;
String lastKeyPressed; 
int speedCar = 255; // 0 a 255
bool cringe = false;

const char *ssid = "Make DIY";
ESP8266WebServer server(80);

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
}


void loop()
{
  server.handleClient();

  keyState = server.arg("State");
  keyPressed = server.arg("Key");

  if (keyState != lastKeyState || keyPressed != lastKeyPressed)
  {
    Serial.print(keyState);
    Serial.println(keyPressed);
    if(keyPressed == "D"){
      if (keyState == "w") forward();
      if (keyState == "S") backward();
      if (keyState == "A") turnLeft();
      if (keyState == "D") turnRight();
      if (keyState == "U") forwardLeft(speedCar);
      if (keyState == "J") backwardLeft(speedCar);
      if (keyState == "O") forwardRight(speedCar);
      if (keyState == "L") backwardRight(speedCar);
    }
    if(keyPressed == "U"){
      stopRobot();
    }
    lastKeyPressed = keyPressed;
    lastKeyState = keyState;
  }
}