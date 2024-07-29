#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Initialize Motors
#define ENA 33
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14
#define ENB 12

// States
int w = 0;
int a = 0;
int s = 0;
int d = 0;
int shift = 0;

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN4, HIGH); digitalWrite(IN3, LOW); 
}

void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN4, LOW); digitalWrite(IN3, HIGH);
}

void left() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN4, HIGH); digitalWrite(IN3, LOW);
}

void right() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW); digitalWrite(IN3, HIGH);
}

void halt() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW); digitalWrite(IN3, LOW);
}

// WiFi Setup
const char* ssid = "WIFI NAME";
const char* password = "WIFI PASSWORD";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
	<head>
		<title>DON QUIXOTE CONTROL PANEL</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Play:wght@400;700&display=swap" rel="stylesheet">
    <style>
      body {
        color: #75FEC6;
        background-color: #000000;
        font-family: "Play", sans-serif;
      }
      .grid-container {
        display: grid;
        grid-template-columns: 250px 100px 250px;
        grid-template-rows: 100px 100px;
        grid-gap: 20px;
      }
      .grid-item {
        border-style: solid;
        border-width: 2px;
        border-radius: 6px;
      }
      #status_div { padding: 5px; }
      @keyframes typing {
        from { width: 0 }
      }
      @keyframes blink {
        50% { opacity: 0; }
      }
      #command_label { font-size: 12px; }
      #icon {
        width: 96px;
        height: 96px;
      }
      #msg {
        padding: 25px;
        text-align: center;
        font-size: 10px;
        color: white;
        border-color: #75FEC6;
        grid-column: 1 /span 2;
      }
      #sprint {
        margin-top: -15px;
        font-size: 10px;
      }
    </style>
	</head>
	<body>
		<h1>DON QUIXOTE CONTROL PANEL</h1>
		<div id="root" class="grid-container"></div>
		<script>
			const gateway = `ws://${window.location.hostname}/ws`;
			let websocket;
			let root;
      let status_div;
      let machine_name;
      let sprint;
      let command_label;
      let icon;
      let time;
      let msg;

      // States
      let w = false;
      let a = false;
      let s = false;
      let d = false;

      function makeElement(type, id, c, parent) {
        let ret = document.createElement(type);
        ret.id = id;
        if (c) ret.className = c;
        parent.appendChild(ret);
        return ret;
      } 

			window.addEventListener('load', onLoad);
			function initWebSocket() {
				console.log('Establishing a connection...');
				websocket = new WebSocket(gateway);
				websocket.onopen = onOpen;
				websocket.onclose = onClose;
				websocket.onmessage = onMessage;
			}
			function onOpen(event) { console.log('Connection established!'); }
			function onClose(event) {
				console.log('Connection lost');
				setTimeout(initWebSocket, 2000);
			}
			function onMessage(event) {
        time.innerText = event.data;
			}
			function onLoad(event) {
				initWebSocket();
				root = document.getElementById("root");

        status_div = makeElement("div", "status_div", "grid-item", root);
        machine_name = makeElement("h5", "machine_name", null, status_div);
        machine_name.innerText = "don_quixote";
        machine_name.style = "position: relative; top: -20px; animation: blink 3s step-start infinite;";
        sprint = makeElement("p", "sprint", null, status_div);
        sprint.innerText = "ACCELERATE";
        sprint.style.visibility = "hidden";
        command_label = makeElement("p", "command_label", null, status_div);
        command_label.innerText = "command: halt";
        icon = makeElement("img", "icon", "grid-item", root);
        icon.src = "https://drive.google.com/thumbnail?id=1LKKNp49oR8WXimO0zUtaUB1qXL-hu8AX";
        time = makeElement("p", "time", null, root);
        msg = makeElement("p", "msg", "grid-item", root);
        msg.innerText = "Message Area";

        document.body.addEventListener("keydown", (event) => {
          if (event.key.toLowerCase() === "w") {
            websocket.send("w");
            w = true;
            command_label.innerText = "command: w";
          }
          if (event.key.toLowerCase() === "a") {
            websocket.send("a");
            a = true;
            command_label.innerText = "command: a";
          }
          if (event.key.toLowerCase() === "s") {
            websocket.send("s");
            s = true;
            command_label.innerText = "command: s";
          }
          if (event.key.toLowerCase() === "d") {
            websocket.send("d");
            d = true;
            command_label.innerText = "command: d";
          }
          if (event.shiftKey) {
            websocket.send("shift");
            sprint.style.visibility = "visible";
          }
        });
        document.body.addEventListener("keyup", (e) => {
          if (e.key.toLowerCase() === "w") {
            websocket.send("w_r");
            w = false;
          }
          else if (e.key.toLowerCase() === "a") {
            websocket.send("a_r");
            a = false;
          }
          else if (e.key.toLowerCase() === "s") {
            websocket.send("s_r");
            s = false;
          }
          else if (e.key.toLowerCase() === "d") {
            websocket.send("d_r");
            d = false;
          }
          if (!e.shiftKey) {
            websocket.send("shift_r");
            sprint.style.visibility = "hidden";
          }
          if (!w && !a && !s && !d) command_label.innerText = "command: halt";
        });
        function msgAnimate(message, s) {
            msg.innerText = "";
            let p = document.createElement("p");
            p.innerText = message;
            p.style = `text-align: center; font-size: 10px; color: white; width: ${s}ch; animation: typing 2s steps(${s}); white-space: nowrap; overflow: hidden;`;
            msg.appendChild(p);
            setTimeout(() => {
              p.style = "text-align: center; font-size: 10px; color: white;";
              p.innerText = "";
            }, 4000);
        }
        document.body.addEventListener("keydown", (e) => {
          if (e.repeat) return;
          if (e.key === "w") { msgAnimate("WE CHARGE FORWARD!", 19); }
          else if (e.key === "a") { msgAnimate("WE MUST GO LEFT!", 15); }
          else if (e.key === "s") { msgAnimate("DRAW BACK!", 10); }
          else if (e.key === "d") { msgAnimate("WE VENTURE RIGHT!", 17); }
          for (let i = 0; i < 2; i++) {
            setTimeout(() => {icon.src = "https://drive.google.com/thumbnail?id=19SfsdVzVxLe951z70MkYCj10ph9Hyy4O"}, i*500);
            setTimeout(() => {icon.src = "https://drive.google.com/thumbnail?id=1LKKNp49oR8WXimO0zUtaUB1qXL-hu8AX"}, i*500 + 250);
          }
          for (let j = 2; j < 4; j++) {
            setTimeout(() => {icon.src = "https://drive.google.com/thumbnail?id=19SfsdVzVxLe951z70MkYCj10ph9Hyy4O"}, (j+1)*500);
            setTimeout(() => {icon.src = "https://drive.google.com/thumbnail?id=1LKKNp49oR8WXimO0zUtaUB1qXL-hu8AX"}, (j+1)*500 + 250);
          }
        });
			}
		</script>
	</body>
</html>
)rawliteral";

void notifyClients() { 
  char buffer[5000];
  unsigned long time = millis();
  unsigned long mins = time / 60000;
  time %= 60000;
  unsigned long sec = time / 1000;
  time %= 1000;
  if (mins < 10 && sec < 10) sprintf(buffer, "0%lu : 0%lu : %lu", mins, sec, time);
  else if (mins < 10) sprintf(buffer, "0%lu : %lu : %lu", mins, sec, time);
  else if (sec < 10) sprintf(buffer, "%lu : 0%lu : %lu", mins, sec, time);
  else sprintf(buffer, "%lu : %lu : %lu", mins, sec, time);
  ws.textAll(String(buffer)); 
}

void handleWebSocketMessage(void* arg, uint8_t* data, size_t len) {
  AwsFrameInfo* info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "w") == 0) { w = 1; a = 0; s = 0; d = 0; }
    else if (strcmp((char*)data, "a") == 0) { w = 0; a = 1; s = 0; d = 0; }
    else if (strcmp((char*)data, "s") == 0) { w = 0; a = 0; s = 1; d = 0; }
    else if (strcmp((char*)data, "d") == 0) { w = 0; a = 0; s = 0; d = 1; }
    else if (strcmp((char*)data, "shift") == 0) shift = 1;
    else if (strcmp((char*)data, "w_r") == 0) w = 0;
    else if (strcmp((char*)data, "a_r") == 0) a = 0;
    else if (strcmp((char*)data, "s_r") == 0) s = 0;
    else if (strcmp((char*)data, "d_r") == 0) d = 0;
    else if (strcmp((char*)data, "shift_r") == 0) shift = 0;
  }
}

void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  switch(type) {
    case WS_EVT_CONNECT:
      break;
    case WS_EVT_DISCONNECT: 
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

String processor(const String& var) {
  if (var == "STATE") {
    if (w == 1) return "FORWARD";
    if (a == 1) return "LEFT";
    if (s == 1) return "BACKWARD";
    if (d == 1) return "RIGHT";
    else return "HALT";
  }
  return String();
}


void setup() {
  // Motor Setup
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  ledcSetup(0, 3000, 8);
  ledcSetup(1, 3000, 8);
  ledcAttachPin(ENA, 0);
  ledcAttachPin(ENB, 1);
  ledcWrite(0, 230); ledcWrite(1, 230);

  // WiFi Connection
  WiFi.begin(ssid, password);

  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
}

void loop() {
  ws.cleanupClients();
  if (shift == 1) {
    ledcWrite(0, 250); ledcWrite(1, 250);
  }
  else {
    ledcWrite(0, 180); ledcWrite(1, 180);
  }
  if (w == 1) forward();
  else if (a == 1) right();
  else if (s == 1) backward();
  else if (d == 1) left();
  else halt();
  if (micros() % 19000 == 0) notifyClients();
}
