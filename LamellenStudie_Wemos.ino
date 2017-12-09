#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "SSID_name";
const char* password = "SSID_password";
const char* mqtt_server = "192.168.123.123";

const char* MQTTclientID = "ESP8266Client-LamellenStudie";
const char* MQTTusername = "MQTT_username";
const char* MQTTpassword = "MQTT_password";

//define the pins used on your wemos board
const int stp = 16;
const int dir = 4;
const int Sleep = 5; 
const int Sensor = 0;

const int StepperRange = 4000; //how many steps does it take to fully close\open\turn your blinds

int Speed = 5; 
int Speedcounter = 0;
int StepperCount = 0;


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length)  {
  byte* p = (byte*)malloc(length);
  memcpy(p,payload,length);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]");
  
  String StringPayload = ""; //convert byte payload to string StringPayload
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    StringPayload = StringPayload+(char)payload[i];
  }
  
  if (strcmp(topic, "domoticz/out/example/Lamellen") == 0 ) {  //check the topic\room, in domoticz your device needs to be in a roomplan
    int svalue1, loc, StepperTarget;
    String sub, devicename;
      
    loc = StringPayload.indexOf("name"); //see if there is a name string in the payload and write value to string devicename
    if (loc >= 0){ 
      devicename = StringPayload.substring(loc+9, loc+23); //these locations depend on the name of your switch in domoticz
      Serial.println(devicename);
      
      if (devicename == "LamellenStudie"){  
        loc = StringPayload.indexOf("svalue1"); //see if there is a svalue1 string in the payload    
        if (loc >= 0){ 
          sub = StringPayload.substring(loc+12, loc+14);
          svalue1 = sub.toInt();
          /*Serial.println(loc);
          Serial.println(sub);
          Serial.println(svalue1);
          */}
        
        if (svalue1 >= 0 && svalue1 < 16){ // if svalue1 is in range 0-15 run Stepperset 
          StepperTarget = map(svalue1, 0, 15, 0, StepperRange);
          if (StepperTarget != StepperCount){
            Serial.println(StepperTarget);
            StepperSet(StepperTarget);  
          }
        }
      }
    }
  }
  free(p);
} //end callback

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    //clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    //if (client.connect(clientId.c_str()))
    if (client.connect(MQTTclientID, MQTTusername, MQTTpassword)){
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("domoticz\/out\/example\/Lamellen");//enter the roomplan name used in domoticz
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(6000); // Wait 6 seconds before retrying
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(Sleep, OUTPUT);
  pinMode(Sensor, INPUT_PULLUP);
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(Sleep, LOW);
 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void StepperSet(int p1){
  digitalWrite(Sleep, HIGH);
  Serial.println("sleep is high");
  while (StepperCount > p1){
    StepperCCW(); 
  }
  while (StepperCount < p1){
    StepperCW();
  }
  digitalWrite(Sleep, LOW);
  Serial.println("sleep is low");
  Speed = 4;
  Serial.println(Speed);
}

void StepperCCW(){
  digitalWrite(dir, HIGH);  // (HIGH = anti-clockwise / LOW = clockwise)
  bool SensorState = digitalRead(Sensor);
  digitalWrite(stp, HIGH);
  delay(Speed);
  digitalWrite(stp, LOW);
  delay(1);
  StepperCount--;
  Speedcounter++;
  if (Speed != 1 and Speedcounter > 70){
    Speed--;
    Speedcounter = 0;
  }
}    

void StepperCW(){
  digitalWrite(dir, LOW);  // (HIGH = anti-clockwise / LOW = clockwise)
  bool SensorState = digitalRead(Sensor);
  if (SensorState == 1){
    digitalWrite(stp, HIGH);
    delay(Speed);
    digitalWrite(stp, LOW);}
  delay(1);
  StepperCount++;
  Speedcounter++;
  if (Speed != 1 and Speedcounter > 70){
    Speed--;
    Speedcounter = 0;
  }
}    
