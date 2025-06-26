#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFiUdp.h>

// Wi-Fi credentials (replace with your own)
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Web interface login credentials
const char* loginUser = "admin";
const char* loginPass = "admin";

// MAC address of the target device for Wake-on-LAN (replace with actual)
byte mac[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

// IR LED on GPIO13
const uint16_t kIrLed = 13;
IRsend irsend(kIrLed);

// HTTP server and UDP for WoL
AsyncWebServer server(80);
WiFiUDP udp;

// Raw IR codes
uint16_t raw_on[] = {
  8976, 4499, 566, 1686, /* … */ 8977, 2257, 565
};
uint16_t raw_off[] = {
  8978, 4496, 568, 1686, /* … */ 567, 1689, 566
};
uint16_t raw_tv[] = {
  9030, 4558, 541, 609, /* … */ 9032, 2286, 541
};

// Simple HTML interface
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>IR Controller</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; padding: 40px; background: #f0f0f0; }
    button {
      padding: 20px 40px; font-size: 18px; margin: 10px;
      background-color: #007BFF; color: #fff; border: none; border-radius: 5px;
    }
  </style>
</head>
<body>
  <h2>IR Remote Control</h2>
  <div><button onclick="send('on')">Light ON</button></div>
  <div><button onclick="send('off')">Light OFF</button></div>
  <div><button onclick="send('tv')">TV POWER</button></div>
  <div><button onclick="send('pc')">Wake PC</button></div>
  <script>
    function send(cmd) {
      fetch('/send?cmd=' + cmd);
    }
  </script>
</body>
</html>
)rawliteral";

// Build and send the magic packet for Wake-on-LAN
void sendMagicPacket(byte* mac) {
  byte packet[102];
  memset(packet, 0xFF, 6);
  for (int i = 1; i <= 16; i++) {
    memcpy(&packet[i * 6], mac, 6);
  }
  udp.beginPacket("255.255.255.255", 9);
  udp.write(packet, sizeof(packet));
  udp.endPacket();
  Serial.println(">>> Magic Packet sent!");
}

// HTTP 401 response
void notAuthorized(AsyncWebServerRequest *request) {
  request->send(401, "text/plain", "Unauthorized");
}

// Basic HTTP auth check
bool isAuthenticated(AsyncWebServerRequest *request) {
  if (!request->authenticate(loginUser, loginPass)) {
    request->requestAuthentication();
    return false;
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  irsend.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected! IP: " + WiFi.localIP().toString());

  // Prepare UDP for WoL
  udp.begin(9);

  // Serve main page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!isAuthenticated(request)) return;
    request->send_P(200, "text/html", index_html);
  });

  // Handle IR & WoL commands
  server.on("/send", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!isAuthenticated(request)) return;
    if (request->hasParam("cmd")) {
      String cmd = request->getParam("cmd")->value();
      if (cmd == "on") {
        irsend.sendRaw(raw_on, sizeof(raw_on)/sizeof(raw_on[0]), 38);
        Serial.println(">>> Light ON");
      } else if (cmd == "off") {
        irsend.sendRaw(raw_off, sizeof(raw_off)/sizeof(raw_off[0]), 38);
        Serial.println(">>> Light OFF");
      } else if (cmd == "tv") {
        irsend.sendRaw(raw_tv, sizeof(raw_tv)/sizeof(raw_tv[0]), 38);
        Serial.println(">>> TV POWER");
      } else if (cmd == "pc") {
        sendMagicPacket(mac);
      }
    }
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  // No code needed here
}
