/*
   Adapted from the following:
   
   ESP32 Web Server Demo using Accesspoint
   https://circuits4you.com
   21-11-2018
*/


#include <Servo.h>
Servo myservo;  // create servo object to control a servo


// GPIO the servo is attached to
static const int servoPin = 15;      
const int Flight = 2;

int pos = 90; // intitial servo postion to be at 90 degrees to make it straight different from 90 for mechanical adjustment
// wired connections
int MOTOR_PWM = 18;//  Speed Control Enable Pin
int MOTOR_DIR = 19; //  Direction
int MOTOR_DIR1 = 21;//  Direction

// the actual values for "fast" and "slow" depend on the motor
#define dir_Forward 0
#define dir_Back 255

#define PWM_STOP 0
#define PWM_SLOW 220 // arbitrary slow speed PWM duty cycle
#define PWM_FAST 255 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes

const int freq = 5000;
const int ledChannel_PWM = 10;   //  picking 0 messes with the servo
const int resolution = 8;

#include <WiFi.h>
#include <WebServer.h>
#include "index.h"  //Web page header file

//Enter your desired WiFi SSID and PASSWORD
const char* ssid = "ESP_Rider_L298N";
const char* password = "********";  // change the *'s to your favourite vegetable

WebServer server(80);

//===============================================================
// Setup
//===============================================================
void setup(void) {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");

  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin);
  myservo.write(pos);
  Serial.println(pos);

  pinMode(Flight, OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(MOTOR_DIR1, OUTPUT);

  ledcSetup(ledChannel_PWM, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(MOTOR_PWM, ledChannel_PWM);

  ledcWrite( ledChannel_PWM, PWM_STOP );

  digitalWrite(Flight, LOW); //Flashlight will turn off

  //ESP32 As access point IP: 192.168.4.1
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);    //Password length minimum 8 char

 
  server.on("/", handle_OnConnect);
  server.on("/A", handle_Forward_Fast);
  server.on("/B", handle_Forward_Slow);
  server.on("/C", handle_Back_Slow);
  server.on("/D", handle_Back_Fast);
  server.on("/off", handle_Stop);
  server.on("/R", handle_Go_Righter);
  server.on("/L", handle_Go_Lefter);
  server.on("/FO", handle_Flashlight_Off);
  server.on("/Pos", handle_Steering_Slider_Input);


  server.onNotFound(handle_NotFound);

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void) {
  server.handleClient();

}
void handle_NotFound () {

}


void handle_OnConnect() {

  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page

}

void handle_Steering_Slider_Input() {
//  server.send(200, "text/plain", "ok");
  if(server.arg("value") ){
  int slidervalue = server.arg("value").toInt();   //
  
  Serial.print("slidervalue = ");
  
 // myservo.write(slidervalue);
  Serial.println(slidervalue);
  pos = slidervalue;
  myservo.write(pos);
  }
  Serial.print("pos = ");
  Serial.print(pos);
server.send(200, "text/plain", "ok");
}

void handle_Forward_Fast() {
  digitalWrite(Flight, HIGH); // Flash light will turn on
  digitalWrite(MOTOR_DIR, LOW);
  digitalWrite(MOTOR_DIR1, HIGH); 
 
  ledcWrite( ledChannel_PWM, PWM_FAST ); // PWM speed = fast
  server.send(200, "text/plain", "ok");
}

void handle_Forward_Slow() {
  digitalWrite(Flight, LOW);
  digitalWrite(MOTOR_DIR, LOW);
  digitalWrite(MOTOR_DIR1, HIGH); 
  ledcWrite( ledChannel_PWM, PWM_SLOW  ); // PWM speed = fast
  server.send(200, "text/plain", "ok");
}

void handle_Back_Slow() {
  digitalWrite(Flight, LOW);
  digitalWrite(MOTOR_DIR, HIGH);
  digitalWrite(MOTOR_DIR1, LOW); 
  ledcWrite( ledChannel_PWM, PWM_SLOW ); // PWM speed = Slow  ledChannel_PWM, 180 - PWM_SLOW
  server.send(200, "text/plain", "ok");
}

void handle_Back_Fast() {
  digitalWrite(Flight, LOW);
  digitalWrite(MOTOR_DIR, HIGH);
  digitalWrite(MOTOR_DIR1, LOW); 
  ledcWrite( ledChannel_PWM, PWM_FAST ); // PWM speed = Retro
  server.send(200, "text/plain", "ok");
}

void handle_Stop() {
  digitalWrite(Flight, LOW);
  digitalWrite(MOTOR_DIR, LOW);
  digitalWrite(MOTOR_DIR1, LOW); 
  ledcWrite( ledChannel_PWM, PWM_STOP );
  server.send(200, "text/plain", "ok");
}

void handle_Go_Righter() { // Receive an HTTP GET request for steering.  Depending on the position of sg90 servo could change name to Lefter
  digitalWrite(Flight, HIGH);
  pos = pos - 10;
  if (pos < 27) {  //  the 27 here and 153 below are dependant on the free swing room available, the slider bar in the index.h should be set the same
    pos = 27;
  }
  myservo.write(pos);

  server.send(200, "text/plain", "ok");
}


void handle_Go_Lefter() { // Receive an HTTP GET request for steering.  And from above comment this could be renamed Righter instead
  digitalWrite(Flight, HIGH);
  pos = pos + 10;
  if (pos > 153) {
    pos = 153;
  }
  myservo.write(pos);

  server.send(200, "text/plain", "ok");
}

void handle_Flashlight_Off() {
  digitalWrite(Flight, LOW);
  server.send(200, "text/plain", "ok");
}
