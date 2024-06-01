
/* 

PROGRAMMINFO:

Funktion: ESP32 WEB Server für RoboCar

Version: 27.02.2021

(x) C++ Arduino IDE V1.8.13
() Python Thonny V3.3.3
() OpenRoberta https://lab.open-roberta.org/ 

Board: ESP32 DEV Module 

Einstellungen:  https://dl.espressif.com/dl/package_esp32_index.json




Librarys
- WiFi.h V0.16.1


Div. INFO
() I2C BME280 SDA(GPIO21), SDL(GPIO22)
() GPIO
() BlueHost
() Github A

// Motor 1
int motor1Pin1 = 26; 
int motor1Pin2 = 27; 
int enable1Pin = 14; 

// Motor 2
int motor2Pin1 = 25; 
int motor2Pin2 = 33; 
int enable2Pin = 32;
  
 */

// Wi-Fi library
#include <WiFi.h>

// Passwort
const char* ssid     = "xxx";
const char* password = "xxx";


WiFiServer server(80);


String header;

// Motor 1
int motor1Pin1 = 26; 
int motor1Pin2 = 27; 
int enable1Pin = 14; 

// Motor 2
int motor2Pin1 = 25; 
int motor2Pin2 = 33; 
int enable2Pin = 32;

// PWM
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 0;

// HTTP GET value
String valueString = String(5);
int pos1 = 0;
int pos2 = 0;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// 2000ms = 2s
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  
  // Motor Pins
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // PWM Channel
  ledcSetup(pwmChannel, freq, resolution);
  
  // Attach PWM Channel
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel); 


  ledcWrite(pwmChannel, dutyCycle);
  
  // WiFi Verbindung
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print IP Adresse
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   

  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  
      currentTime = millis();
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
          
          if (currentLine.length() == 0) {
       
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            
            if (header.indexOf("GET /forward") >= 0) {
              Serial.println("Forward");
              digitalWrite(motor1Pin1, LOW);
              digitalWrite(motor1Pin2, HIGH); 
              digitalWrite(motor2Pin1, LOW);
              digitalWrite(motor2Pin2, HIGH);
            }  else if (header.indexOf("GET /right") >= 0) {
              Serial.println("Left");
              digitalWrite(motor1Pin1, LOW); 
              digitalWrite(motor1Pin2, LOW); 
              digitalWrite(motor2Pin1, LOW);
              digitalWrite(motor2Pin2, HIGH);
            }  else if (header.indexOf("GET /stop") >= 0) {
              Serial.println("Stop");
              digitalWrite(motor1Pin1, LOW); 
              digitalWrite(motor1Pin2, LOW); 
              digitalWrite(motor2Pin1, LOW);
              digitalWrite(motor2Pin2, LOW);             
            } else if (header.indexOf("GET /left") >= 0) {
              Serial.println("Right");
              digitalWrite(motor1Pin1, LOW); 
              digitalWrite(motor1Pin2, HIGH); 
              digitalWrite(motor2Pin1, LOW);
              digitalWrite(motor2Pin2, LOW);    
            } else if (header.indexOf("GET /reverse") >= 0) {
              Serial.println("Reverse");
              digitalWrite(motor1Pin1, HIGH);
              digitalWrite(motor1Pin2, LOW); 
              digitalWrite(motor2Pin1, HIGH);
              digitalWrite(motor2Pin2, LOW);          
            }
            // WEB-Seite
            client.println("<!DOCTYPE HTML><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS 
          
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none; user-select: none; background-color: #00BFFF;");
            client.println("border: none; color: white; padding: 12px 28px; text-decoration: none; font-size: 26px; margin: 1px; cursor: pointer;}");
            client.println(".button2 {background-color: #FE2E2E;}</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script></head>");
            
            // WEB-Seite       
            client.println("<p><button class=\"button\" onclick=\"moveForward()\">Vorw&auml;rts</button></p>");
            client.println("<div style=\"clear: both;\"><p><button class=\"button\" onclick=\"moveLeft()\">Links </button>");
            client.println("<button class=\"button button2\" onclick=\"stopRobot()\">STOPP</button>");
            client.println("<button class=\"button\" onclick=\"moveRight()\">Rechts</button></p></div>");
            client.println("<p><button class=\"button\" onclick=\"moveReverse()\">Zur&uuml;ck</button></p>");
            client.println("<p>Motor Speed: <span id=\"motorSpeed\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"100\" step=\"25\" id=\"motorSlider\" onchange=\"motorSpeed(this.value)\" value=\"" + valueString + "\"/>");
            
            client.println("<script>$.ajaxSetup({timeout:1000});");
            client.println("function moveForward() { $.get(\"/forward\"); {Connection: close};}");
            client.println("function moveLeft() { $.get(\"/left\"); {Connection: close};}");
            client.println("function stopRobot() {$.get(\"/stop\"); {Connection: close};}");
            client.println("function moveRight() { $.get(\"/right\"); {Connection: close};}");
            client.println("function moveReverse() { $.get(\"/reverse\"); {Connection: close};}");
            client.println("var slider = document.getElementById(\"motorSlider\");");
            client.println("var motorP = document.getElementById(\"motorSpeed\"); motorP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; motorP.innerHTML = this.value; }");
            client.println("function motorSpeed(pos) { $.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");
           
            client.println("</html>");
            
            
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
             
              if (valueString == "0") {
                ledcWrite(pwmChannel, 0);
                digitalWrite(motor1Pin1, LOW); 
                digitalWrite(motor1Pin2, LOW); 
                digitalWrite(motor2Pin1, LOW);
                digitalWrite(motor2Pin2, LOW);   
              }
              else { 
                dutyCycle = map(valueString.toInt(), 25, 100, 200, 255);
                ledcWrite(pwmChannel, dutyCycle);
                Serial.println(valueString);
              } 
            }         
            
            client.println();
            
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    
    header = "";
    
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
