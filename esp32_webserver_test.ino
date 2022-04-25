/*
 * ESP32 AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "index.h"  //Web page header file

WebServer server(80);

//Enter your SSID and PASSWORD
const char* ssid = "Telstra9D0014";
const char* password = "698a9e7e2p";

// Garage door sw
bool limitSwitch = true;

// Delay time for servo acuation (ms)
const int servoTimer = 1000;
int timePast = 0;

// Servo states
#define S_OPEN = 0x00;
#define S_ACTUATE = 0x01;
int servoState = S_OPEN;

#define SERVO_PIN 13

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
    String s = MAIN_page; //Read HTML contents
    erver.send(200, "text/html", s); //Send web page
}
 
// Return status of garage door limit switch
void handleLimitSW() {
    String doorStatus = (limitSwitch) ? "down" : "up";
    server.send(200, "text/plane", doorStatus);
}

// Recieve garage door toggle request
void handleLimitSW() {
    actuateDoor = true;
    server.send(200, "text/plane", "Success");
}

//===============================================================
// Setup
//===============================================================

void setup(void){
    Serial.begin(115200);
    Serial.println();
    Serial.println("Booting Sketch...");

    // Zero servo motor
    analogWrite(0);

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
    server.on("/toggleGarage", handleGarageToggle);
 
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

            // On Transition
            if (actuateDoor) {
                analogWrite(1500);
                timerPast = millis();
                servoState = S_ACTUATE;
            }
            break;
        case S_ACTUATE:
            analogWrite(1500);

            // On Transition
            if ((millis() - timerPast) >= servoTimer) {
                analogWrite(0);
                actuateDoor = false;
                servoState = S_OPEN;

                // Temp limit switch sim
                limitSwitch = !limitSwitch;
            }
            break;
    }
    delayMillis(100);
}
