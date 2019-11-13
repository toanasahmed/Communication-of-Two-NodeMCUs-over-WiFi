/*Created by Anas Ahmed
   Hackerspace Karachi
   freelancer.com/toanasahmed
   Dated: July 18, 2019.
*/

/*  Connects to the home WiFi network
    Asks some network parameters
    Starts WiFi server with fix IP and listens
    Receives and sends messages to the client
    Communicates: wifi_client_01.ino
*/
#include <SPI.h>
#include <ESP8266WiFi.h>

const int ledPin = D2;                // the pin that the relay is attached to

//FOR PIR
const int pirsensor = D8;              // the pin that the PIR sensor is attached to
int state = LOW;                      // by default, no motion detected
int pirVal = 0;                       // variable to store the sensor status (value)

//FOR LDR
int ldrPin = A0;                 // select the input pin for LDR
int ldrValue = 0;               // variable to store the value coming from the sensor
int intensity = 50;             // Caliberated value

char ssid[] = "*******";               // SSID of your home WiFi
char pass[] = "*******";               // password of your home WiFi

WiFiServer server(80);

IPAddress ip(192, 168, 0, 200);            // IP address of the server
IPAddress gateway(192, 168, 0, 1);        // gateway of your network
IPAddress subnet(255, 255, 255, 0);       // subnet mask of your network

void setup() {
  Serial.begin(115200);                   // only for debug
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();                         // starts the server
  /*  Serial.println("Connected to wifi");
    Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
    Serial.print("IP: ");     Serial.println(WiFi.localIP());
    Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
    Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
    Serial.print("SSID: "); Serial.println(WiFi.SSID());
    Serial.print("Signal: "); Serial.println(WiFi.RSSI());
    Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());*/
  pinMode(ledPin, OUTPUT);
}

void loop () {
  //PIR SENSOR
  pirVal = digitalRead(pirsensor);   // read sensor value
  Serial.print("pirValue = ");
  Serial.println(pirVal); //prints the values coming from the LDR on the screen

  //LDR
  ldrValue = analogRead(ldrPin); // read the value from the LDR
  ldrValue = map(ldrValue, 0, 1023, 0, 255);
  Serial.print("ldrValue = ");
  Serial.println(ldrValue); //prints the values coming from the LDR on the screen
  delay(100);


  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      digitalWrite(ledPin, LOW);  // to show the communication only (inverted logic)
      Serial.println(".");
      String request = client.readStringUntil('\r');    // receives the message from the client
      Serial.print("From client: "); Serial.println(request);
      client.flush();

      if (pirVal == HIGH && ldrValue < intensity ) {           // check if the sensor is HIGH
        client.println("YES.\r"); // sends the answer to the client
        digitalWrite(ledPin, HIGH);
      }
    }
    client.stop();                // terminates the connection with the client
  }

}
