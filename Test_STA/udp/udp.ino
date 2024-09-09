#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Shuteng";
const char* password = "12345678";

WiFiUDP Udp;
const int udpPort = 12345; // Port number for UDP communication

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  Udp.begin(udpPort); // Start listening on the UDP port
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    char buffer[255];
    int len = Udp.read(buffer, 255);
    if (len > 0) {
      buffer[len] = 0; // Null-terminate the string
    }
    String message = String(buffer);
    Serial.println("Received: " + message);

    if (message.startsWith("RGB")) {
      // Example message format: RGB r,g,b
      int r, g, b;
      sscanf(message.c_str(), "RGB %d,%d,%d", &r, &g, &b);
      setRGBColor(r, g, b);
    }
  }
}

void setRGBColor(int r, int g, int b) {
  // Implement your RGB LED control here
  Serial.printf("Setting RGB to: %d,%d,%d\n", r, g, b);
}
