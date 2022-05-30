/*
 * ESP32 AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include <ESP32Servo.h>

#include "index.h"  //Web page header file

WebServer server(80);

//Enter your SSID and PASSWORD
// const char* ssid = "Telstra9D0014";
// const char* password = "698a9e7e2p";
const char* ssid = "Excession-2.4G";
const char* password = "The Sleeper Service";

// Garage door sw
bool limitSwitch = true;
bool actuateDoor = false;

// Delay time for servo acuation (ms)
const int servoTimer = 1000;
unsigned long timerPast = 0;

// Servo states
#define S_OPEN 0x00
#define S_ACTUATE 0x01
int servoState = S_OPEN;

#define PWM_HZ 100
#define PWM_MIN 500
#define PWM_MAX 2400

#define SERVO_PIN 15
Servo garageServo;  // create servo object to control a servo

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
    String s = MAIN_page; //Read HTML contents
    server.send(200, "text/html", s); //Send web page
}
 
// Return status of garage door limit switch
void handleLimitSW() {
    String doorStatus = (limitSwitch) ? "down" : "up";
    server.send(200, "text/plane", doorStatus);
}

// Recieve garage door toggle request
void handleGarageToggle() {
    actuateDoor = true;
    server.send(200, "text/plane", "Success");

    String doorStatus = (limitSwitch) ? "down" : "up";
    Serial.print("Recieved toggle request, doorStatus: ");
    Serial.println(doorStatus);
}

//===============================================================
// Setup
//===============================================================

void setup(void){
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	garageServo.setPeriodHertz(PWM_HZ);    // standard 50 hz servo
    // attaches the servo on pin 15 to the servo object
	garageServo.attach(SERVO_PIN, PWM_MIN, PWM_MAX);

    Serial.begin(115200);
    Serial.println();
    Serial.println("Booting Sketch...");

    // Zero servo motor
    garageServo.write(0);

    /*
    //ESP32 As access point
      WiFi.mode(WIFI_AP); //Access Point mode
      WiFi.softAP(ssid, password);
    */
    //ESP32 connects to your wifi -----------------------------------
    WiFi.mode(WIFI_STA); //Connect to your wifi
    WiFi.begin(ssid, password);

    Serial.println("Connecting to ");
    Serial.print(ssid);

    //Wait for WiFi to connect
    while(WiFi.waitForConnectResult() != WL_CONNECTED){      
        Serial.print(".");
    }
      
    //If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
    //----------------------------------------------------------------

    //This is display page
    server.on("/", handleRoot);
    //To get update of grg door state
    server.on("/check_garage_door", handleLimitSW);
    //To operate garage door
    server.on("/toggle_garage_door", handleGarageToggle);
 
    server.begin();                  //Start server
    Serial.println("HTTP server started");

            
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void){
    server.handleClient();
    // Servo state machine
    switch (servoState) {
        case S_OPEN:
            garageServo.write(0);

            // On Transition
            if (actuateDoor) {
                garageServo.write(90);
                timerPast = millis();
                servoState = S_ACTUATE;
            }
            break;
        case S_ACTUATE:
            garageServo.write(90);

            // On Transition
            if ((millis() - timerPast) >= servoTimer) {
                garageServo.write(0);
                actuateDoor = false;
                servoState = S_OPEN;

                // Temp limit switch sim
                // TODO
                limitSwitch = !limitSwitch;
            }
            break;
    }
    delay(100);
}
