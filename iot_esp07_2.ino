#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "iot_rahasia.h"

// Initialize Wifi connection to the router
char ssid[] = WIFI_SSID;
char password[] = WIFI_PSWD; 

// Initialize Telegram BOT
const char* bot_nama = BOT_NAMA;
#define BOTtoken BOT_TOKEN 

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000;
long Bot_lasttime;
bool Start = false;

const int pin_lampu = 13;
const int pin_led_koneksi = 12;
int ledStatus = 0;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    String perintah1 = "/ledon@";
    perintah1 += bot_nama;

    if (text == "/ledon" or text == perintah1) {
      digitalWrite(pin_lampu, HIGH);   // turn the LED on (HIGH is the voltage level)
      ledStatus = 1;
      bot.sendMessage(chat_id, "Lampu LED telah hidup", "");
    }

    String perintah2 = "/ledoff@";
    perintah2 += bot_nama;
    
    if (text == "/ledoff" or text == perintah2) {
      ledStatus = 0;
      digitalWrite(pin_lampu, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Lampu LED telah mati", "");
    }

    String perintah3 = "/status@";
    perintah3 += bot_nama;
    
    if (text == "/status" or text == perintah3) {
      if(ledStatus){
        bot.sendMessage(chat_id, "Posisi lampu LED: hidup", "");
      } else {
        bot.sendMessage(chat_id, "Posisi lampu LED: mati", "");
      }
    }

    String perintah4 = "/start@";
    perintah4 += bot_nama;

    if (text == "/start" or text == perintah4) {
      String welcome = "Selamat datang di aplikasi Universal Arduino Telegram Bot, " + from_name + ".\n";
      welcome += "This is Flash Led Bot example.\n\n";
      welcome += "/ledon : untuk menyalakan lampu LED \n";
      welcome += "/ledoff : untuk mematikan lampu LED \n";
      welcome += "/status : menginfokan status lampu \n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(pin_led_koneksi, OUTPUT); // initialize digital pin_led_koneksi as an output.
  digitalWrite(pin_led_koneksi, HIGH);

  pinMode(pin_lampu, OUTPUT); // initialize digital pin_lampu as an output.
  delay(10);
  digitalWrite(pin_lampu, LOW); // initialize pin as off
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}
