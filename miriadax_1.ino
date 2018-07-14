#include <SPI.h>
#include <Ethernet.h>
#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#define WIFI_AP "siplab"
#define WIFI_PASSWORD "abcd1234"

// IP del servidor
IPAddress mqtt_server(192, 168, 1, 12);

int status = WL_IDLE_STATUS;

// Topic con el que trabajamos
const char* topicName = "light";
const char* lighton = "light on";
const char* lightoff = "light off";
const char* &data = lightoff;



WiFiEspClient espClient;
PubSubClient client(espClient);

SoftwareSerial BT1(2, 3); // RX | TX
int buttonState = 0;
float temp;


void reconnect(){
  while (!client.connected()) {
    Serial.print("Connecting to Broker ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("Arduino Uno Device") ) {
      Serial.println( "Connected" );
    }
    else{
      delay( 3000 );
    }
  }
}

void setup()
{
  pinMode(5, INPUT);
  pinMode(9, OUTPUT);
  Serial.begin(19200);
  BT1.begin(19200);

  //wifi serial comunication
  WiFi.init(&soft);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  
  //wifi connect
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.print(WIFI_AP);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    delay(500);
  }
  client.setServer(mqtt_server, 1883);
}

void loop()
{
  // read the state of the pushbutton value
  buttonState = digitalRead(5);
  temp = ((analogRead(1) * 0.004882814) - 0.5) * 100;
  if (temp > 25 && buttonState == HIGH){
    digitalWrite(9, HIGH);
  } else {
    // turn light off
    digitalWrite(9, LOW);
  }
  delay(10);
  
  if ( !client.connected() ) {
    reconnect();
  }
  // Transmision time lapse
  delay(8000);
  if (digitalRead(9) == HIGH){
  	data = lighton;
  }else{
  	data = lightoff;
  }
  client.publish(topicName, data);  
  
}
