#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "PASSWORD"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

// initialize server
ESP8266WebServer server (80);

// create servo object
Servo servo;

// servo motor control pin, D4 on esp8266
const int servoPin = 2;

void rotate ()
{
	if (server.args () != 2) {
		server.send (400, "text/plain", "Usage: /rotate?amt=INT&delay=INT");
		return;
	}

  // get position and delay query arguments 
  int position = server.arg (0).toInt ();
  int delayMs = server.arg (1).toInt ();

  servo.write (position);
  delay (delayMs);
  servo.write (0);
  delay (delayMs);

  server.send (200, "text/plain", "Rotation Done");
}

void debug ()
{
	char responseBody[1024];
  int second = millis () / 1000;
  int minute = second / 60;
  int hour = minute / 60;

  snprintf (responseBody, 1400,
            "<!doctype html>\
             <head>\
               <meta http-equiv='refresh' content='5'/>\
               <title>Debug</title>\
               <style>\
                 html, body {\ 
                   width: 100%; height: 100vh; overflow: hidden;\
                   display: flex; justify-content: center; align-items: center;\
                   text-align: center; background-color: black;\
                   font: 1.3rem Inconsolata, monospace; color: green;\
                 }\
               </style>\
             </head>\
             <body>\
               <h1>Device uptime: %02d:%02d:%02d</h1>\
             </body>\
             </html>", hour, minute % 60, second % 60);

  server.send (200, "text/html", responseBody);
}

void setup (void) 
{
  Serial.begin (115200);
  
  WiFi.mode (WIFI_STA);
  WiFi.hostname ("finger1");
  WiFi.begin (ssid, password);

  // Wait for connection
  while (WiFi.status () != WL_CONNECTED) {
    delay (500);
    Serial.print (".");
  }
  
  Serial.println ("");
  Serial.print ("Connected to ");
  Serial.println (ssid);
  Serial.print ("IP address: ");
  Serial.println (WiFi.localIP ());

  // attach servo and move to 0
  servo.attach (servoPin);
  servo.write (0);

  server.on ("/", debug);
	server.on ("/debug", debug);
  server.on ("/rotate", rotate);
  server.onNotFound ([]() {
	  server.send (404, "text/plain", "Not Found\n\n");
	});

  server.begin ();
  Serial.println (F("Server started"));
}

void loop (void) 
{
  server.handleClient ();
}