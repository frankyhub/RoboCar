
/* 

PROGRAMMINFO:

Funktion: ESP32 WEB Server für RoboCar mit fester IP-Adresse .234

Version: 9.02.2021

C++ Arduino IDE V1.8.13

Board: ESP32 DEV Module 

Einstellungen:  https://dl.espressif.com/dl/package_esp32_index.json

Librarys
- WiFi.h V0.16.1

// Motor 1
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14; 

// Motor 2
int motor2Pin1 = 33; 
int motor2Pin2 = 25; 
int enable2Pin = 32;
  
 */


#include <WiFi.h>

const char* ssid     = "xxx";
const char* password = "xxx";

WiFiServer server(80);

String header;

// Feste IP-Adresse
IPAddress local_IP(192, 168, 1, 234);
// Gateway IP Addresse
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

// Motor 1
int motor1Pin1 = 26; 
int motor1Pin2 = 27; 
int enable1Pin = 14; 

// Motor 2
int motor2Pin1 = 25; 
int motor2Pin2 = 33; 
int enable2Pin = 32;

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 0;

String valueString = String(5);
int pos1 = 0;
int pos2 = 0;

unsigned long currentTime = millis();

unsigned long previousTime = 0; 
// 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  // Feste IP-Adresse konfigurieren
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA konnte nicht konfiguriert werden.");
  }
  
  // Mit Wi-Fi verbinden
  Serial.print("Verbunden mit ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // IP-Adresse ausgeben
  Serial.println("");
  Serial.println("WiFi verbunden.");
  Serial.println("IP Addresse: ");
  Serial.println(WiFi.localIP());
  server.begin();

  
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  
  ledcSetup(pwmChannel, freq, resolution);
  
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel); 

  ledcWrite(pwmChannel, dutyCycle);
  
  Serial.print("Verbunden mit ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi verbunden");
  Serial.println("IP Addresse: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   

  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";                //
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
            client.println("Verbindung geschlossen");
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
            // WEB Seite
            client.println("<!DOCTYPE HTML><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS  
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none; user-select: none; background-color: #00BFFF;");
            client.println("border: none; color: white; padding: 12px 28px; text-decoration: none; font-size: 26px; margin: 1px; cursor: pointer;}");
            client.println(".button2 {background-color: #FE2E2E;}</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script></head>");
            
            // WEB Seite 
            client.println("<body><h2>RoboCar</h2>");
            client.println("<p>IP-Adresse http://192.168.1.234/");       
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
            
            //Request example: GET /?value=100& HTTP/1.1 - sets PWM duty cycle to 100% = 255
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
              //Set motor speed value
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
    Serial.println("Client getrennt");
    Serial.println("");
  }
}
