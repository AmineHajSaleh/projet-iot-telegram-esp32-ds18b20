#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// --- MODIFIEZ CES INFOS APRES AVOIR REVOQUÉ LE TOKEN ---
const char* ssid = "VOTRE_WIFI_SSID";
const char* password = "VOTRE_WIFI_PASSWORD";
#define BOTtoken "VOTRE_TOKEN_TELEGRAM" 
#define CHAT_ID "VOTRE_ID_TELEGRAM"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);



int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

// Sur ESP32, la LED intégrée est souvent sur le GPIO 2
const int ledPin1 = 13;
const int ledPin2 = 12;
bool ledState = LOW;

const float tempSeuil=23;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "/led1_on : Turn ON \n";
      welcome += "/led1_off : Turn OFF \n";
      welcome += "/led2_on : Turn ON \n";
      welcome += "/led2_off : Turn OFF \n";
      welcome += "/state : Status \n";
      welcome += "/reading : Temperature \n";
      bot.sendMessage(chat_id, welcome, "");
    }

   if (text == "/led1_on") {
      bot.sendMessage(chat_id, "LED ON", "");
      ledState = HIGH;
      digitalWrite(ledPin1, ledState);
    }
    if (text == "/led2_on") {
      bot.sendMessage(chat_id, "LED ON", "");
      ledState = HIGH;
      digitalWrite(ledPin2, ledState);
    }

    if (text == "/led1_off") {
      bot.sendMessage(chat_id, "LED OFF", "");
      ledState = LOW;
      digitalWrite(ledPin1, ledState);
    }
        if (text == "/led2_off") {
      bot.sendMessage(chat_id, "LED2 OFF", "");
      ledState = LOW;
      digitalWrite(ledPin2, ledState);
    }

    if (text == "/state") {
      if (digitalRead(ledPin1)){
        bot.sendMessage(chat_id, "LED1 is ON", "");
      } else {
        bot.sendMessage(chat_id, "LED1 is OFF", "");
      }
      if (digitalRead(ledPin2)){
        bot.sendMessage(chat_id, "LED2 is ON", "");
      } else {
        bot.sendMessage(chat_id, "LED2 is OFF", "");
      }
    }

    if (text == "/reading") {
      sensors.requestTemperatures();
      delay(750);
      float temperatureC = sensors.getTempCByIndex(0);
      float temperatureF = sensors.getTempFByIndex(0);
      bot.sendMessage(chat_id, "Temperature: "+String(temperatureC,2)+" C\n"+String(temperatureF,2)+" F\n");
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Cette ligne permet d'ignorer la verification SSL sur ESP32 et ESP8266
  client.setInsecure();

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, ledState);
  digitalWrite(ledPin2, ledState);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  sensors.begin();
  sensors.setResolution(9);
  
  // Attente connexion
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      Serial.println("Response received");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
  sensors.requestTemperatures();
  delay(750);
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  if (temperatureC > tempSeuil){
    digitalWrite(ledPin1,HIGH);
    bot.sendMessage(chat_id, "ATTENTION  A DEPASSER LA SEUIL Temperature: "+String(temperatureC,2)+" C\n"+String(temperatureF,2)+" F\n");
  }
  else {
    digitalWrite(ledPin1,LOW);
    bot.sendMessage(chat_id, "Temperature: "+String(temperatureC,2)+" C\n"+String(temperatureF,2)+" F\n");
  }
  
}