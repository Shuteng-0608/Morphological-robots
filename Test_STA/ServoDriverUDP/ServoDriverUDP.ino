// WIFI_AP settings.
const char* AP_SSID = "Shuteng";
const char* AP_PWD  = "12345678";

// WIFI_STA settings. (Target wifi needs to satisfy - "2.4GHz" && "WPA2")
const char* STA_SSID = "Shuteng";
const char* STA_PWD  = "12345678";

// the MAC address of the device you want to ctrl.
uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0x93, 0x5F, 0xA8};
// uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


typedef struct struct_message {
  int ID_send;
  int POS_send;
  int Spd_send;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// set the default role here.
// 0 as normal mode.
// 1 as leader, ctrl other device via ESP-NOW.
// 2 as follower, can be controled via ESP-NOW.
#define DEFAULT_ROLE 0

// set the default wifi mode here.
// 1 as [AP] mode, it will not connect other wifi.
// 2 as [STA] mode, it will connect to know wifi.
#define DEFAULT_WIFI_MODE 2

// the uart used to control servos.
// GPIO 18 - S_RXD, GPIO 19 - S_TXD, as default.
#define S_RXD 18
#define S_TXD 19

// the IIC used to control OLED screen.
// GPIO 21 - S_SDA, GPIO 22 - S_SCL, as default.
#define S_SCL 22
#define S_SDA 21

// the GPIO used to control RGB LEDs.
// GPIO 23, as default.
#define RGB_LED   23
#define NUMPIXELS 10

// set the max ID.
int MAX_ID = 20;

// modeSelected.
// set the SERIAL_FORWARDING as true to control the servos with USB.
bool SERIAL_FORWARDING = false;

// OLED Screen Dispaly.
// Row1: MAC address.
// Row2: VCC --- IP address.
// Row3: MODE:Leader/Follower  [AP]/[STA][RSSI]
//       DEFAULT_ROLE: 1-Leader(L)/ 2-Follower(F).
//       DEFAULT_WIFI_MODE: 1-[AP]/ 2-[STA][RSSI] / 3-[TRY:SSID].
//       (no matter what wifi mode you select, you can always ctrl it via ESP-NOW.)
// Row4: the position of servo 1, 2 and 3.
String MAC_ADDRESS;
IPAddress IP_ADDRESS;
byte   SERVO_NUMBER;
byte   DEV_ROLE;
byte   WIFI_MODE;
int    WIFI_RSSI;

// set the interval of the threading.
#define threadingInterval 600
#define clientInterval    10

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include "RGB_CTRL.h"
#include "STSCTRL.h"
#include "CONNECT.h"
#include "BOARD_DEV.h"
#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;
const int udpPort = 12345; // Port number for UDP communication


void setup() {
  Serial.begin(115200);
  while(!Serial) {}

  WiFi.begin(STA_SSID, STA_PWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  Udp.begin(udpPort); // Start listening on the UDP port
  

//  espNowInit();
//
//  getMAC();
//  
//  boardDevInit();
//
//  RGBcolor(0, 64, 255);
//
//  servoInit();
//
//  wifiInit();
//
//  webServerSetup();
//
//  RGBoff();
//
//  delay(1000);
//  pingAll(true);
//
//  threadInit();


  
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
//  delay(300000);

  // st.WritePosEx(1, 25, 600, 0);
  // st.WritePosEx(2, 25, 600, 0);
  // st.WritePosEx(3, 25, 600, 0);
  // delay(2000);

  // st.WritePosEx(1, 1000, 600, 0);
  // st.WritePosEx(2, 1000, 600, 0);
  // st.WritePosEx(3, 1000, 600, 0);
  // delay(2000);
}

void setRGBColor(int r, int g, int b) {
  // Implement your RGB LED control here
  setSingleLED(0, matrix.Color(r, g, b));
  Serial.printf("Setting RGB to: %d,%d,%d\n", r, g, b);
}


// > > > > > > > > > DOC < < < < < < < < <
// === Develop Board Ctrl ===
// get the MAC address and save it in MAC_ADDRESS;
// getMAC();

// Init GPIO.
// pinMode(PIN_NUM, OUTPUT);
// pinMode(PIN_NUM, INPUT_PULLUP);

// set the level of GPIO.
// digitalWrite(PIN_NUM, LOW);
// digitalWrite(PIN_NUM, HIGH);

// PWM output(GPIO).
// int freq = 50;
// resolution = 12;
// ledcSetup(PWM_NUM, frep, resolution);
// ledcAttachPin(PIN_NUM, PWM_NUM);
// ledcWrite(PWM_NUM, PWM);


// === Servo Ctrl ===
// GPIO 18 as RX, GPIO 19 as TX, init the serial and the servos.
// servoInit();

// set the position as middle pos.
// setMiddle(servoID);
// st.WritePosEx(ID, position, speed, acc);



// === Devices Ctrl ===
// ctrl the RGB.
// 0 < (R, G, B) <= 255
// setSingleLED(LED_ID, matrix.Color(R, G, B));

// init the OLED screen, RGB_LED.
// boardDevInit();

// dispaly the newest information on the screen.
// screenUpdate();
