#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Servo.h>

#define WLAN_SSID       ""             // Your SSID
#define WLAN_PASS       ""        // Your password


#define LED2_CONTROL_PIN D2
#define LED3_CONTROL_PIN D5
#define LED4_CONTROL_PIN D6
#define GATE_CONTROL_PIN D7
/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME  " "
#define AIO_KEY       " "
/************ Global State (you don't need to change this!) ******************/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe LED1= Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Led1"); // FeedName
Adafruit_MQTT_Subscribe LED2= Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Led2"); // FeedName
Adafruit_MQTT_Subscribe LED3= Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Led3"); // FeedName
Adafruit_MQTT_Subscribe LED4= Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Led4"); // FeedName
Adafruit_MQTT_Subscribe GATE= Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/GATE"); // FeedName
Servo servo;
     // Khai báo chân điều khiển servo
void setup() {
  Serial.begin(115200);
 servo.attach(13);
  servo.write(0);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED2_CONTROL_PIN, OUTPUT);
  pinMode(LED3_CONTROL_PIN, OUTPUT);
  pinMode(LED4_CONTROL_PIN, OUTPUT);
  pinMode(GATE_CONTROL_PIN, OUTPUT);
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&LED1);
  mqtt.subscribe(&LED2);
  mqtt.subscribe(&LED3);
  mqtt.subscribe(&LED4);
  mqtt.subscribe(&GATE);
}


void loop() {
 
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) 
  {
    if (subscription == &LED1) 
    {
      Serial.print(F("Got: "));
      Serial.print("LED1");
      Serial.println((char *)LED1.lastread);
    if (!strcmp((char*) LED1.lastread, "ON"))
      {
        //Active low logic
        digitalWrite(LED_BUILTIN, LOW);
      }
      else
      {
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
    if (subscription == &LED2) {
      Serial.print(F("Got: "));
      Serial.print("LED2");
      Serial.println((char *)LED2.lastread);
    if (!strcmp((char*) LED2.lastread, "ON"))
      {
        //Active low logic
        digitalWrite(LED2_CONTROL_PIN, HIGH);
      }
      else
      {
        digitalWrite(LED2_CONTROL_PIN, LOW);
      }
    }

   
   if (subscription == &LED3) {
      Serial.print(F("Got:"));
      Serial.print("LED3");
      Serial.println((char *)LED3.lastread);
    if (!strcmp((char*) LED3.lastread, "ON"))
      {
        //Active low logic
        digitalWrite(LED3_CONTROL_PIN, HIGH);
      }
      else
      {
        digitalWrite(LED3_CONTROL_PIN, LOW);
      }
    }
    if (subscription == &LED4) {
      Serial.print(F("Got: "));
      Serial.print("LED4");
      Serial.println((char *)LED4.lastread);
    if (!strcmp((char*) LED4.lastread, "ON"))
      {
        //Active low logic
        digitalWrite(LED4_CONTROL_PIN, HIGH);
      }
      else
      {
        digitalWrite(LED4_CONTROL_PIN, LOW);
      }
    }
    if (subscription == &GATE) {
      Serial.print(F("Got: "));
      Serial.print("GATE");
      Serial.println((char *)GATE.lastread);
    if (!strcmp((char*) GATE.lastread, "ON"))
    servo.write(110);
    else
      servo.write(0);
    }
}
}
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
