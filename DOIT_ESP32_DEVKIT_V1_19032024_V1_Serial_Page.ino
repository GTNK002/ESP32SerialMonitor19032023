#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <AsyncWebSocket.h>

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

const char* ssid = "----------------";
const char* password = "-----------";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Smart Home</title>
<style>
body {
    font-family: Arial, sans-serif;
    background-color: Black;
    margin: 0;
    padding: 0;
}

.container {
    max-width: 360px;
    height: 500px;
    margin: 0 top;
    padding: 20px;
    background-color: grey;
    border-radius: 10px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
}

.smart-home {
    text-align: center;
    margin-bottom: 20px;
}

.smart-home h1 {
    color: #333;
}

.smart-home h2 {
    color: #666;
    font-size: 18px;
}

.button-container {
    display: flex;
    flex-wrap: wrap;
    justify-content: center;
}

.button-container button {
    width: 100px;
    margin: 1px;
    padding: 10px;
    border: none;
    border-radius: 5px;
    background-color: #646362;
    color: #CBCBCB;
    font-size: 14px;
    cursor: pointer;
    transition: background-color 0.3s;
}

a {
    text-decoration: none;
    color: #333;
}

a button {
    background-color: #008CBA;
}

a button:hover {
    background-color: #0073a5;
}
#serialMonitorContent {
    display: none;
    background-color: #f0f0f0;
    border-radius: 5px;
    padding: 10px;
    height: 200px;
    overflow: auto;
}
</style>
</head>
<body>
<div class="container">
  <div class="smart-home">
    <h1>Smart Home</h1>
    <h2>Bedroom 2 | Fan: <span id="fanStatus">Speed4</span></h2>
  </div>

  <div class="button-container">
    <button onclick="sendws('switch1')">Socket<br><span id="socketStatus">On</span></button>
    <button onclick="sendws('switch2')">Night<br><span id="nightStatus">Off</span></button>
    <button onclick="sendws('switch3')">Light<br><span id="lightStatus">On</span></button>
    <button onclick="sendws('fanOff')">Fan<br>OFF</button>
    <button onclick="sendws('fanSpeed1')">Fan<br>Speed 1</button>
    <button onclick="sendws('fanSpeed2')">Fan<br>Speed 2</button>
    <button onclick="sendws('fanSpeed3')">Fan<br>Speed 3</button>
    <button onclick="sendws('fanSpeed4')">Fan<br>Speed 4</button>
    <button onclick="openSerial()">Nerd<br>Page</button>
    <button onclick="sendws('tvButton')">TV<br>On/Off</button>
    <button onclick="sendws('acButton')">AC<br>On/Off</button>
    <button onclick="sendws('htButton')">Speaker<br>On/Off</button>
    <a href="/tv"><button>TV<br>Page</button></a>
    <a href="/ac"><button>AC<br>Page</button></a>
    <a href="/ht"><button>Speaker<br>Page</button></a>
  </div>
  <!-- Serial Monitor Content -->
  <div id="serialMonitorContent">
    <h2>Serial Monitor <button onclick="clearSerialMonitor()">Clear</button></h2>
    <div id="serialMonitorContentInner">
    </div>
    <button onclick="clearSerialMonitor()">Clear</button>
  </div>
</div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);

  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onLoad(event) {
    initWebSocket();
  }
  function sendws(x) {
    var serialMonitorContentInner = document.getElementById('serialMonitorContentInner');
    serialMonitorContentInner.innerHTML += '<p>' + x + '</p>';
    
    websocket.send(x);
  }
  function onMessage(event) {
    var data = event.data;

    var serialMonitorContentInner = document.getElementById('serialMonitorContentInner');
    serialMonitorContentInner.innerHTML += '<p>' + data + '</p>';
    
    if (data === 'switch1On') {
      document.getElementById('socketStatus').innerText = 'On';
    } 
    else if (data === 'switch1Off') {
      document.getElementById('socketStatus').innerText = 'Off';
    }
    else if (data === 'switch2On') {
      document.getElementById('nightStatus').innerText = 'On';
    } 
    else if (data === 'switch2Off') {
      document.getElementById('nightStatus').innerText = 'Off';
    }
    else if (data === 'switch3On') {
      document.getElementById('lightStatus').innerText = 'On';
    } 
    else if (data === 'switch3Off') {
      document.getElementById('lightStatus').innerText = 'Off';
    }
    else if (data === 'fanOff') {
      document.getElementById('fanStatus').innerText = 'Off';
    }
    else if (data === 'fanSpeed1') {
      document.getElementById('fanStatus').innerText = 'Speed1';
    }
    else if (data === 'fanSpeed2') {
      document.getElementById('fanStatus').innerText = 'Speed2';
    }
    else if (data === 'fanSpeed3') {
      document.getElementById('fanStatus').innerText = 'Speed3';
    }
    else if (data === 'fanSpeed4') {
      document.getElementById('fanStatus').innerText = 'Speed4';
    }
    else if (data === 'fanSpeed5') {
      document.getElementById('fanStatus').innerText = 'Speed5';
    }
  }
  function openSerial() {
    var serialMonitorContent = document.getElementById('serialMonitorContent');
    if (serialMonitorContent.style.display === 'none') {
        serialMonitorContent.style.display = 'block';
    } else {
        serialMonitorContent.style.display = 'none';
    }
  }
  function clearSerialMonitor() {
    var serialMonitorContentInner = document.getElementById('serialMonitorContentInner');
    serialMonitorContentInner.innerHTML = '';
  }
</script>
</body>
</html>
)rawliteral";


String switch1 = "On";
String switch2 = "Off";
String switch3 = "On";
String Fan = "Speed4";

int switch1pin   = 4;  //Socket  D0
int switch2pin   = 18; //Night   D3
int    fan1pin   = 19; //Fanpins D5
int    fan2pin   = 21; //Fanpins D6
int    fan3pin   = 22; //Fanpins D7
int switch3pin   = 23; //Light   D8

//int switch1Input = ; //Socket
int switch2Input = 35; //Night
int     fanInput = 32; //Fan
int switch3Input = 33; //Light 

int IR_Reciver   = 13; //IR Reciver

//bool pre_sw1_val = 0; //socket
bool pre_sw2_val = 0; //Night
bool pre_fan_val = 1; //Fan
bool pre_sw3_val = 0; //Light

bool sw2 = 0;
bool fan = 1;
bool sw3 = 1;

#define IR1            0x1FE20DF // eq
#define IR2            0x1FEA05F // v+
#define IR3            0x1FE609F // v-
#define IR4            0x1FE10EF // rpt
#define IR5            0x1FE906F // u/sd
#define IRF0           0x1FEE01F // 0
#define IRF1           0x1FE50AF // 1
#define IRF2           0x1FED827 // 2
#define IRF3           0x1FEF807 // 3
#define IRF4           0x1FE30CF // 4

IRrecv irrecv(IR_Reciver);
decode_results results;

void ir_remote() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);  
    if (results.value==IR1) {
      switch1function();
    }
    else if (results.value==IR4) {
      switch2function();
    }
    else if (results.value==IR5) {
      switch3function();
    }
    else if (results.value==IRF0) {
      fanOFF();
    }
    else if (results.value==IRF1) {
      fanSpeed1();
    }
    else if (results.value==IRF2) {
      fanSpeed2();
    }
    else if (results.value==IRF3) {
      fanSpeed3();
    }
    else if (results.value==IRF4) {
      fanSpeed4();
    }
    else {
      Serial.println("Wrong IR button pressed"); 
      delay(100);
    }
    delay(200);
    irrecv.resume(); // receive the next value
    String irCode = String(results.value, HEX);
    ws.textAll(irCode.c_str());
  }
}

void switch1function(){
  if (switch1 == "ON") {
    switch1 = "OFF";
    digitalWrite(switch1pin, HIGH);
    ws.textAll("switch1Off");
  }
  else {
    switch1 = "ON";
    digitalWrite(switch1pin, LOW);
    ws.textAll("switch1On");
  }
}

void switch2function(){
  if (switch2 == "ON") {
    switch2 = "OFF";
    digitalWrite(switch2pin, HIGH);
    ws.textAll("switch2Off");
  }
  else {
    switch2 = "ON";
    digitalWrite(switch2pin, LOW);
    ws.textAll("switch2On");
  }
}

void switch3function(){
  if (switch3 == "ON") {
    switch3 = "OFF";
    digitalWrite(switch3pin, HIGH);
    ws.textAll("switch3Off");
  }
  else {
    switch3 = "ON";
    digitalWrite(switch3pin, LOW);
    ws.textAll("switch3On");
  }
}

void fanswitchfunction(){
  if (Fan != "OFF") {
    fanOFF();
  }
  else {
    fanSpeed4();
  }
}

void fanOFF(){
  Fan = "OFF";
  digitalWrite(fan1pin, HIGH);
  digitalWrite(fan2pin, HIGH);  
  digitalWrite(fan3pin, HIGH); 
  ws.textAll("fanOff");
}

void fanSpeed1(){
  Fan = "Speed1";
  digitalWrite(fan1pin, HIGH);
  digitalWrite(fan2pin, HIGH);
  digitalWrite(fan3pin, HIGH);
  delay(500);
  digitalWrite(fan1pin, LOW);  
  ws.textAll("fanSpeed1");
}

void fanSpeed2(){
  Fan = "Speed2";
  digitalWrite(fan1pin, HIGH);
  digitalWrite(fan2pin, HIGH);
  digitalWrite(fan3pin, HIGH);
  delay(500);
  digitalWrite(fan2pin, LOW);
  ws.textAll("fanSpeed2");
}

void fanSpeed3(){
  Fan = "Speed3";
  digitalWrite(fan1pin, HIGH);
  digitalWrite(fan2pin, HIGH);
  digitalWrite(fan3pin, HIGH);
  delay(500);
  digitalWrite(fan1pin, LOW);
  digitalWrite(fan2pin, LOW);
  ws.textAll("fanSpeed3");
}

void fanSpeed4(){
  Fan = "Speed4";
  digitalWrite(fan1pin, HIGH);
  digitalWrite(fan2pin, HIGH);
  digitalWrite(fan3pin, HIGH);
  delay(500);
  digitalWrite(fan3pin, LOW);
  ws.textAll("fanSpeed4");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;

    if (strcmp((char*)data, "switch1") == 0) {
      switch1function();
    }
    else if (strcmp((char*)data, "switch2") == 0) {
      switch2function();
    }
    else if (strcmp((char*)data, "switch3") == 0) {
      switch3function();
    }
    else if (strcmp((char*)data, "fanOff") == 0) {
      fanOFF();
    }
    else if (strcmp((char*)data, "fanSpeed1") == 0) {
      fanSpeed1();
    }
    else if (strcmp((char*)data, "fanSpeed2") == 0) {
      fanSpeed2();
    }
    else if (strcmp((char*)data, "fanSpeed3") == 0) {
      fanSpeed3();
    }
    else if (strcmp((char*)data, "fanSpeed4") == 0) {
      fanSpeed4();
    }
  }
}
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  return String();
}

void setup() {
  pinMode(switch1pin,OUTPUT);
  pinMode(2,OUTPUT);
  //pinMode(,OUTPUT);
  pinMode(switch2pin,OUTPUT);
  pinMode(fan1pin,OUTPUT);
  pinMode(fan2pin,OUTPUT);
  pinMode(fan3pin,OUTPUT);
  pinMode(switch3pin,OUTPUT);

  digitalWrite(switch1pin, LOW);
  digitalWrite(2, LOW);
  //digitalWrite(, LOW);
  digitalWrite(switch2pin, HIGH);
  digitalWrite(fan1pin, HIGH);
  digitalWrite(fan2pin, HIGH);
  digitalWrite(fan3pin, LOW);
  digitalWrite(switch3pin, LOW);
  
  pinMode(switch2Input,INPUT);
  pinMode(fanInput,INPUT);
  pinMode(switch3Input,INPUT);

  pre_sw2_val = digitalRead(switch2Input);
  pre_fan_val = digitalRead(fanInput);
  pre_sw3_val = digitalRead(switch3Input);

  irrecv.enableIRIn();
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  
  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  AsyncElegantOTA.begin(&server);

  server.begin();
}

void loop() {
  //switch_control();
  sw2 = digitalRead(switch2Input); //35 Night
  fan = digitalRead(fanInput); //32 Fan
  sw3 = digitalRead(switch3Input); //33 Light

  if (sw2 != pre_sw2_val) {
    switch2function();
    pre_sw2_val = sw2;
  }
  if (sw3 != pre_sw3_val) {
    switch3function();
    pre_sw3_val = sw3;
  }
  if (fan != pre_fan_val) {
    fanswitchfunction();
    pre_fan_val = fan;
  }
  ir_remote();
  ws.cleanupClients();
  AsyncElegantOTA.loop();
}
