/*
  +-- DC Motor Monitoring System --+

  Control and monitor a DC Motor with Encoder using a JAVA interface.
  The interface communicates with the Arduino through a server deployed
  by the Arduino via WiFi.

  Created November 3rd 2021
  by Jean Fernandez (jean-fernandez-imt)
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

// Heartbeat interval
const long interval = 250;

// Program Initial states
int ledState = LOW;
unsigned long previousMillis = 0;

// Addresses and constants for Arduino Motor Shield
const int motorPWM_A = 3;
const int motorBrk_A = 9;
const int motorDir_A = 12;

const int CW = HIGH;
const int CCW = LOW;

// Encoder Addresses
const int encoderA_A = 6;
const int encoderB_A = 7;

// Encoder Initial state
signed long pos = 0;

// Set the LCD address to 0x27 for a 16x2 Display
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int posAreaSize = 11;

// WiFi Connection secret credentials
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// Wifi Connection
WiFiServer server(80);
int status = WL_IDLE_STATUS;
IPAddress ip;

void setup() {
  // Pin Initialization
  pinMode(LED_BUILTIN, OUTPUT);

  // Motor Shield Pin Initialziation
  pinMode(motorPWM_A, OUTPUT);
  pinMode(motorBrk_A, OUTPUT);
  pinMode(motorDir_A, OUTPUT);

  // Motor Initial state
  stopMotor();

  // Encoder Pin Initialization
  pinMode(encoderA_A, INPUT);
  pinMode(encoderB_A, INPUT);

  // Encoder Interrupt
  attachInterrupt(digitalPinToInterrupt(encoderA_A), readEncoder, RISING);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Connecting...");

  // Initialize Serial Communications
  Serial.begin(9600);
  Serial.setTimeout(100);

  // Initialize WiFi Server
  Serial.println("WiFi Web Server");

  // Check for the WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware!");
  }

  // Connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  server.begin();
  printWiFiStatus();

  // Print IP address to LCD
  lcd.clear();
  lcd.print(ip);
}

void loop() {
  // Actions to execute every 250 ms
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Program Heartbeat
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(LED_BUILTIN, ledState);
  }

  // WiFi Server Client handling
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client!");

    // Incoming data storage
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        // End of HTTP request
        if (c == '\n') {
          // HTTP response header
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // HTTP response content
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<h1>Motor Monitoring System</h1>");
            client.println("<p>This API controls and monitors a DC motor connected to an Arduino UNO WiFi Rev2.</p>");
            client.println("<p>The readings from the Encoder are written into the LCD Display.</p>");
            client.println("<p>The API has the following endpoints:</p>");
            client.println("<ul>");
            client.println("<li><b>/left</b> - The motor rotates counter-clockwise.<br></li>");
            client.println("<li><b>/right</b> - The motor rotates clockwise.<br></li>");
            client.println("<li><b>/stop</b> - The motor stops.<br></li>");
            client.println("</ul>");
            client.println("<html>");
            client.println();

            break;
          } else {
            currentLine = "";
          }
        }

        // Single CR gets added to currentLine
        else if (c != '\r') {
          currentLine += c;
        }

        // Server Endpoints
        if (currentLine.endsWith("GET /left")) {
          // Motor Action
          setMotor(CCW, 225);

          // LCD Output
          lcd.clear();
          lcd.print("Going Left!");
        }

        if (currentLine.endsWith("GET /right")) {
          // Motor Action
          setMotor(CW, 225);

          // LCD Output
          lcd.clear();
          lcd.print("Going Right!");
        }

        if (currentLine.endsWith("GET /stop")) {
          // Motor Action
          stopMotor();

          // LCD Output
          lcd.clear();
          lcd.print(ip);
        }
      }
    }

    // Browser time to receive data
    delay(10);

    // Close connection
    client.stop();
    Serial.println("Client disconnected");
  }

  // Print Encoder reading
  lcd.setCursor(0, 1);
  lcd.print("Pos: ");
  printPos(pos);
}

void setMotor(int dir, int pwm) {
  digitalWrite(motorBrk_A, LOW);

  digitalWrite(motorDir_A, dir);
  analogWrite(motorPWM_A, pwm);
}

void stopMotor() {
  digitalWrite(motorBrk_A, HIGH);
}

void readEncoder() {
  int b = digitalRead(encoderB_A);

  if (b > 0) {
    pos++;
  } else {
    pos--;
  }
}

void printWiFiStatus() {
  // Print the SSID of the network
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your WiFi shield's IP address
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print received signal's strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void printPos(signed long pos) {
  char posChars[posAreaSize];

  lcd.setCursor(5, 1);

  // Reset every character to [SPACE]
  for (int i = 0; i < posAreaSize; i++) {
    posChars[i] = 0x20;
  }

  if (pos != 0) {
    String posString = String(pos);

    // Prepare the String char by char
    for (int j = 0; j < posString.length(); j++) {
      posChars[j] = posString[j];
    }
  } else {
    posChars[0] = 0x30;
  }
  
  lcd.print(posChars);
}
