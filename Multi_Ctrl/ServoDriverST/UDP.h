#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;
const int udpPort = 12345; // Port number for UDP communication

void setRGBColor(int r, int g, int b); // Control all LED light on board

// Set board as STA
// Connect to know wifi (Target wifi needs to satisfy - "2.4GHz" && "WPA2")
void wifiConnect(){
  WiFi.begin(STA_SSID, STA_PWD);

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}


// UDP init
void udpInit(){
  Udp.begin(udpPort);
  Serial.print("UDP listening on port ");
  Serial.println(udpPort);
}


// Handle coming msg
void handleUdp(){
  int packetSize = Udp.parsePacket(); // Check is there any msg coming
  if(packetSize){
    char buffer[255];
    int len = Udp.read(buffer, 255); // Read the coming msg
    if (len > 0) {
      buffer[len] = 0; // Null-terminate the string
    }
    String message = String(buffer);
    Serial.println("Received: " + message);

    // =================================================== //
    // message format: "RGB r,g,b"
    if(message.startsWith("RGB")){
      int r, g, b;
      sscanf(message.c_str(), "RGB %d,%d,%d", &r, &g, &b);
      setRGBColor(r, g, b);
    }

    
    if(message.startsWith("WritePosEx")){
      int id, pos, vel, acc;
      sscanf(message.c_str(), "WritePosEx %d,%d,%d,%d", &id, &pos, &vel, &acc);
      st.WritePosEx(id, pos, vel, acc);
    }
    
    if(message.startsWith("Position+")){
      int id;
      sscanf(message.c_str(), "Position+ %d", &id);
      st.WritePosEx(id, ServoDigitalRange_ST - 1, activeServoSpeed, ServoInitACC_ST);
    }
    
    if(message.startsWith("Position-")){
      int id;
      sscanf(message.c_str(), "Position- %d", &id);
      st.WritePosEx(id, 0, activeServoSpeed, ServoInitACC_ST);
    }

    // message format: "CMD cmdT,cmdI,cmdA,cmdB"
    if(message.startsWith("CMD")){
      int cmdT, cmdI, cmdA, cmdB;
      sscanf(message.c_str(), "CMD %d,%d,%d,%d", &cmdT, &cmdI, &cmdA, &cmdB);
      switch(cmdT){
        case 0:activeID(cmdI);break;
        case 1:activeCtrl(cmdI, cmdA, cmdB);break;
        case 9:searchCmd = true;break;
      }
    }

    
  }
}

void setRGBColor(int r, int g, int b) {
  // Implement RGB LED control
  setSingleLED(0, matrix.Color(r, g, b));
  setSingleLED(1, matrix.Color(r, g, b));
  Serial.printf("Setting RGB to: %d,%d,%d\n", r, g, b);
}
