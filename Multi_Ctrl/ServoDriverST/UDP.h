#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <stdint.h>

typedef char s8;
typedef unsigned char u8; 
typedef unsigned short u16; 
typedef short s16;
typedef unsigned long u32;  
typedef long s32;

WiFiUDP Udp;
const int udpPort = 12345; // Port number for UDP communication
IPAddress local_ip(192, 168, 4, 5); // static local IP   
IPAddress gateway(192, 168, 4, 1); // AP    
IPAddress subnet(255, 255, 255, 0);

void setRGBColor(byte r, byte g, byte b); // Control all LED light on board

// Set board as STA
// Connect to know wifi (Target wifi needs to satisfy - "2.4GHz" && "WPA2")
void wifiConnect(){
  WiFi.config(local_ip, gateway, subnet);
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

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    const char* cmd = doc["Cmd"];  
    
    if(strcmp(cmd, "RGB") == 0){
      u8 id = doc["ID"];
      byte r = doc["R"];
      byte g = doc["G"];
      byte b = doc["B"];
      setRGBColor(r, g, b);
    }

    else if(strcmp(cmd, "WritePosEx") == 0){
      u8 id = doc["ID"];
      s16 pos = doc["pos"];
      u16 vel = doc["vel"];
      u8 acc = doc["acc"];
      // WritePosEx(u8 ID, s16 Position, u16 Speed, u8 ACC = 0);
      st.WritePosEx(id, pos, vel, acc);
    }

    else if(strcmp(cmd, "RegWritePosEx") == 0){
      u8 id = doc["ID"];
      s16 pos = doc["pos"];
      u16 vel = doc["vel"];
      u8 acc = doc["acc"];
      // RegWritePosEx(u8 ID, s16 Position, u16 Speed, u8 ACC = 0);
      st.RegWritePosEx(id, pos, vel, acc);
    }

    else if(strcmp(cmd, "SyncWritePosEx") == 0){
      JsonArray ID_list = doc["ID_list"];
      u8 IDNum = doc["IDNum"];
      JsonArray pos_list = doc["pos_list"];
      JsonArray vel_list = doc["vel_list"];
      JsonArray acc_list = doc["acc_list"];
      
      u8 IDArray[ID_list.size()];
      s16 posArray[pos_list.size()];
      u16 velArray[vel_list.size()];
      u8 accArray[acc_list.size()];
      
      for (int i = 0; i < ID_list.size(); i++) IDArray[i] = ID_list[i].as<u8>();
      for (int i = 0; i < pos_list.size(); i++) posArray[i] = pos_list[i].as<s16>();  
      for (int i = 0; i < vel_list.size(); i++) velArray[i] = vel_list[i].as<u16>();  
      for (int i = 0; i < acc_list.size(); i++) accArray[i] = acc_list[i].as<u8>();   
      // SyncWritePosEx(u8 ID[], u8 IDN, s16 Position[], u16 Speed[], u8 ACC[]);
      st.SyncWritePosEx(IDArray, IDNum, posArray, velArray, accArray);
    }

    
    else if(strcmp(cmd, "Position+") == 0){
      u8 id = doc["ID"];
      // WritePosEx(u8 ID, s16 Position, u16 Speed, u8 ACC = 0);
      st.WritePosEx(id, ServoDigitalRange_ST - 1, activeServoSpeed, ServoInitACC_ST);
    }
    
    else if(strcmp(cmd, "Position-") == 0){
      u8 id = doc["ID"];
      // WritePosEx(u8 ID, s16 Position, u16 Speed, u8 ACC = 0);
      st.WritePosEx(id, 0, activeServoSpeed, ServoInitACC_ST);
    }

    else if(strcmp(cmd, "EnableTorque") == 0){
      JsonArray ID_list = doc["ID_list"];
      u8 enable = doc["Enable"];
      // EnableTorque(u8 ID, u8 Enable)
      for (int i = 0; i < ID_list.size(); i++) st.EnableTorque(ID_list[i].as<u8>(), enable);
    }
    else if(strcmp(cmd, "SetTorque") == 0){
      JsonArray ID_list = doc["ID_list"];
      JsonArray NewTorque_list = doc["NewTorque_list"];
      for (int i = 0; i < ID_list.size(); i++) st.SetTorque(ID_list[i].as<u8>(), NewTorque_list[i].as<u16>());
    }
    else if(strcmp(cmd, "SetMode") == 0){
      JsonArray ID_list = doc["ID_list"];
      u8 Mode = doc["Mode"];
      for (int i = 0; i < ID_list.size(); i++) st.SetMode(ID_list[i].as<u8>(), Mode);
    }
    else if(strcmp(cmd, "SetTime") == 0){
      JsonArray ID_list = doc["ID_list"];
      JsonArray Time_list = doc["Time_list"];
      JsonArray Direction_list = doc["Direction_list"];
      for (int i = 0; i < ID_list.size(); i++) st.SetTime(ID_list[i].as<u8>(), Time_list[i].as<u16>(), Direction_list[i].as<u16>());
      // Pos, Speed, Current
      StaticJsonDocument<200> jsonDoc;
      JsonArray Pos_list = jsonDoc.createNestedArray("Pos_list");
      JsonArray Speed_list = jsonDoc.createNestedArray("Speed_list");
      JsonArray Current_list = jsonDoc.createNestedArray("Current_list");
      
      for (int i = 0; i < ID_list.size(); i++) {
        Pos_list.add(st.ReadPos(ID_list[i]));
        Speed_list[i].add(st.ReadSpeed(ID_list[i]));
        Current_list[i].add(st.ReadCurrent(ID_list[i]));
      }

      char jsonBuffer[200];
      serializeJson(jsonDoc, jsonBuffer);
      Udp.beginPacket("192.168.4.10", 12345);
      Udp.print(jsonBuffer);
      Udp.endPacket();  
    }

    else if(strcmp(cmd, "State") == 0){ 
      int id = doc["ID"];
      int pos = st.ReadPos(id); 
      Udp.beginPacket("192.168.4.10", 12345);
      Udp.print(pos);
      Udp.endPacket();
//      Serial.println(WiFi.localIP());
//      Serial.print("MAC Address: ");
//      Serial.println(WiFi.macAddress()); 
//      Serial.print("Position read: ");
//      Serial.println(pos);
//      if (WiFi.status() == WL_CONNECTED) {
//        Serial.println("Wi-Fi is connected.");
//        if (Udp.beginPacket("192.168.4.10", 12345) == 1) {
//          Serial.print("Sending position: ");
//          Serial.println(pos);
//          Udp.print(pos);
//          if (Udp.endPacket() == 1) {
//            Serial.println("Message sent successfully.");
//          } else {
//            Serial.println("Failed to send message.");
//          }
//        } else {
//          Serial.println("Failed to begin packet.");
//        }
//      } else {
//        Serial.println("Wi-Fi not connected.");
//      }
    }
  }
}

void setRGBColor(byte r, byte g, byte b) {
  // Implement RGB LED control
  setSingleLED(0, matrix.Color(r, g, b));
  setSingleLED(1, matrix.Color(r, g, b));
}
