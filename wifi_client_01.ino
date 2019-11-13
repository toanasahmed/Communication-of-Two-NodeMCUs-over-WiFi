/*Created by Anas Ahmed
 * Hackerspace Karachi
 * freelancer.com/toanasahmed
 * Dated: July 18, 2019.
 */



/*  Connects to the home WiFi network
    Asks some network parameters
    Sends and receives message from the server in every 2 seconds
    Communicates: wifi_server_01.ino
*/
#include <SPI.h>
#include <ESP8266WiFi.h>

String yes = "YES.";
const int ledPin = D2;

char ssid[] = "***********";               // SSID of your home WiFi
char pass[] = "***********";               // password of your home WiFi

unsigned long askTimer = 0;

IPAddress server(192, 168, 0, 200);    // the fix IP address of the server
WiFiClient client;

void setup() {
  Serial.begin(115200);               // only for debug
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  /*  Serial.println("Connected to wifi");
    Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
    Serial.print("IP: ");     Serial.println(WiFi.localIP());
    Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
    Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
    Serial.print("SSID: "); Serial.println(WiFi.SSID());
    Serial.print("Signal: "); Serial.println(WiFi.RSSI());*/
  pinMode(ledPin, OUTPUT);
}

void loop () {
  client.connect(server, 80);   // Connection to the server
  digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
  Serial.println(".");
  client.println("Hello server! Is there any movement?\r");  // sends the message to the server
  String answer = client.readStringUntil('\r');   // receives the answer from the sever
  Serial.println("from server: " + answer);
  client.flush();
  WiFiClient client;

  if (answer ==  yes) {
    digitalWrite(ledPin, HIGH);
  }
  delay(1000);                  // client will trigger the communication after 1 seconds
}
