#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "credentials.h"
#ifndef credentials_exists
const char* ssid = "";
const char* password = "";
#endif

#include "res/style.css.h"


ESP8266WebServer server(80);

void handleIndex() {
  server.send(200, "text/html", "<!doctype html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><meta name='author' content='CJ Patoilo'><meta name='description' content='Milligram provides a minimal setup of styles for a fast and clean starting point. Specially designed for better performance and higher productivity with fewer properties to reset resulting in cleaner code.'><title>Milligram | A minimalist CSS framework.</title><link rel='stylesheet' href='/style.css'></head><body><a class='button' href='#'>Default Button</a><button class='button button-outline'>Outlined Button</button><input class='button button-clear' type='submit' value='Clear Button'></body></html>");
}

void handleStyles() {
  Serial.print("Request for: styles.css\n");
 
  Serial.print("Reponse length:"+String(strlen_P(const_style_css))+"\n");
  
  server.send_P(200, "text/css", const_style_css, strlen_P(const_style_css));
  Serial.print("Request for: styles.css - Done\n");
  
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleIndex);
  server.on("/style.css", handleStyles);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
