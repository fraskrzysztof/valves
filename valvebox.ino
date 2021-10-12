
#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <virtuabotixRTC.h>
//#include <Time.h> 
#include<EEPROM.h>
#define EEPROM_SIZE 256
AsyncWebServer server(80);
virtuabotixRTC myRTC(14, 12, 13);



const char* ssid = "VALVES";
const char* password = "jedenladnydrugibrzydki";
 

char labels[][5] = {"mo","tu","we","th","fr","st","su"};

struct valve
  {
    int days[7];
    int startTime = 0;
    int sh =0;
    int sm =0;
    int endTime = 0;
    int eh=0;
    int em=0;
    int enabled = 0;  
    int manualON = 0;
  };

valve valves[4];

// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>valves</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head>
  <body>
<iframe name="hiddenFrame" width="0" height="0" border="0" style="display: none;"></iframe>
    <body>
        <div class="row">
            <div class="column left" style="background-color:#87d9ff;">
                <h2>Valve 1</h2>
                <p>
                <form action="/set_valve_0" target="hiddenFrame">
                    <label for="pon"> start</label><input type="time" id="start" name="start" value=String(start1)>
                    <label for="pon"> stop</label><input type="time" id="stop" name="stop"><br>
                    <input type="checkbox" id="pn" name="mo" value="1">
                    <label for="pon"> mon</label><br>
                    <input type="checkbox" id="wt" name="tu" value="1">
                    <label for="wt"> tue</label><br>
                    <input type="checkbox" id="sr" name="we" value="1">
                    <label for="sr"> wed</label><br>
                    <input type="checkbox" id="cw" name="th" value="1">
                    <label for="czw"> thr</label><br>
                    <input type="checkbox" id="pt" name="fr" value="1">
                    <label for="pt"> fri</label><br>
                    <input type="checkbox" id="sob" name="st" value="1">
                    <label for="sob"> sat</label><br>
                    <input type="checkbox" id="nd" name="su" value="1">
                    <label for="su"> sun</label><br>
                    <input type="submit" value="Submit"><br>
                    <input type="checkbox" id="manual" name="manual" value="1">
                    <label for="manual"> manual</label><br>
                    <input type="checkbox" id="manualON" name="manualON" value="1">
                    <label for="manualON"> on</label><br>

                </form><br></p>
            </div>

        </div>
        <div class="row">
            <div class="column left" style="background-color:#87d9ff;">
                <h2>Valve 2</h2>
                <p>
                <form action="/set_valve_1" target="hiddenFrame">
                    <label for="pon"> start</label><input type="time" id="start" name="start">
                    <label for="pon"> stop</label><input type="time" id="stop" name="stop"><br>
                    <input type="checkbox" id="pn" name="mo" value="1">
                    <label for="pon"> mon</label><br>
                    <input type="checkbox" id="wt" name="tu" value="1">
                    <label for="wt"> tue</label><br>
                    <input type="checkbox" id="sr" name="we" value="1">
                    <label for="sr"> wed</label><br>
                    <input type="checkbox" id="cw" name="th" value="1">
                    <label for="czw"> thr</label><br>
                    <input type="checkbox" id="pt" name="fr" value="1">
                    <label for="pt"> fri</label><br>
                    <input type="checkbox" id="sob" name="st" value="1">
                    <label for="sob"> sat</label><br>
                    <input type="checkbox" id="nd" name="su" value="1">
                    <label for="nd"> sun</label><br>
                    <input type="submit" value="Submit"><br>
                    <input type="checkbox" id="manual" name="manual" value="1">
                    <label for="manual"> manual</label><br>
                    <input type="checkbox" id="manualON" name="manualON" value="1">
                    <label for="manualON"> on</label><br>

                </form><br></p>
            </div>

        </div>
        <div class="row">
            <div class="column left" style="background-color:#87d9ff;">
                <h2>Valve 3</h2>
                <p>
                <form action="/set_valve_2" target="hiddenFrame">
                    <label for="pon"> start</label><input type="time" id="start" name="start">
                    <label for="pon"> stop</label><input type="time" id="stop" name="stop"><br>
                    <input type="checkbox" id="pn" name="mo" value="1">
                    <label for="pon"> mon</label><br>
                    <input type="checkbox" id="wt" name="tu" value="1">
                    <label for="wt"> tue</label><br>
                    <input type="checkbox" id="sr" name="we" value="1">
                    <label for="sr"> wed</label><br>
                    <input type="checkbox" id="cw" name="th" value="1">
                    <label for="czw"> thr</label><br>
                    <input type="checkbox" id="pt" name="fr" value="1">
                    <label for="pt"> fri</label><br>
                    <input type="checkbox" id="sob" name="st" value="1">
                    <label for="sob"> sat</label><br>
                    <input type="checkbox" id="nd" name="su" value="1">
                    <label for="nd"> sun</label><br>
                    <input type="submit" value="Submit"><br>
                    <input type="checkbox" id="manual" name="manual" value="1">
                    <label for="manual"> manual</label><br>
                    <input type="checkbox" id="manualON" name="manualON" value="1">
                    <label for="manualON"> on</label><br>

                </form><br></p>
            </div>

        </div>
        <div class="row">
            <div class="column left" style="background-color:#87d9ff;">
                <h2>Valve 4</h2>
                <p>
                <form action="/set_valve_3" target="hiddenFrame">
                    <label for="pon"> start</label><input type="time" id="start" name="start">
                    <label for="pon"> stop</label><input type="time" id="stop" name="stop"><br>
                    <input type="checkbox" id="pn" name="mo" value="1">
                    <label for="pon"> mon</label><br>
                    <input type="checkbox" id="wt" name="tu" value="1">
                    <label for="wt"> tue</label><br>
                    <input type="checkbox" id="sr" name="we" value="1">
                    <label for="sr"> wed</label><br>
                    <input type="checkbox" id="cw" name="th" value="1">
                    <label for="czw"> thr</label><br>
                    <input type="checkbox" id="pt" name="fr" value="1">
                    <label for="pt"> fri</label><br>
                    <input type="checkbox" id="sob" name="st" value="1">
                    <label for="sob"> sat</label><br>
                    <input type="checkbox" id="nd" name="su" value="1">
                    <label for="nd"> sun</label><br>
                    <input type="submit" value="Submit"><br>
                    <input type="checkbox" id="manual" name="manual" value="1">
                    <label for="manual"> manual</label><br>
                    <input type="checkbox" id="manualON" name="manualON" value="1">
                    <label for="manualON"> on</label><br>

                </form><br></p>
            </div>
        </div>

        <div class="row">
            <div class="column left" style="background-color:#FFff95;">
                <h2>Set time</h2>
                <p>
                <form action="/set_time" target="hiddenFrame">
                    <label>time</label><input type="time" id="time" name="time"><br>
                    <label>weekday</label><select name="weekday" id="weekday">
                                        <option value="1">mon</option>
                                        <option value="2">tue</option>
                                        <option value="3">wed</option>
                                        <option value="4">thr</option>
                                        <option value="5">fri</option>
                                        <option value="6">sat</option>
                                        <option value="7">sun</option>
                                      </select><br>
                    <input type="submit" value="Submit">
                </form><br></p>
            </div>
        </div>
    </body>



</body>
  </html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setTime(AsyncWebServerRequest *request)
{
  
  int params = request->params();
  for(int i=0;i<params;i++){
    AsyncWebParameter* p = request->getParam(i);
    if(p->isFile()){ //p->isPost() is also true
      Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
    } else if(p->isPost()){
      Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
    } else {
      Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }
  }

  int timeH;
  int timeM;
  int weekday;

    if (request->getParam("time"))
  {
    char time[10];
    request->getParam("time")->value().toCharArray(time, 50);
    time[2]=0;
    timeH =atoi(time);
    timeM = atoi(time+3);

  }
  if (request->getParam("weekday"))
  {
    weekday = request->getParam("weekday")->value().toInt();
  }
  
    myRTC.setDS1302Time(00, timeM, timeH, weekday, 0, 0, 2020); 
}

void dupa0(AsyncWebServerRequest *request)
{
  dupa(request, 0);

}
void dupa1(AsyncWebServerRequest *request)
{
  dupa(request, 1);
}
void dupa2(AsyncWebServerRequest *request)
{
  dupa(request, 2);
}
void dupa3(AsyncWebServerRequest *request)
{
  dupa(request, 3);
}
void dupa(AsyncWebServerRequest *request, int id)
{
//  Serial.print("dupa called, id: ");
//  Serial.println(id);

  for(int i =0; i<7; i++)
  {
    if (request->hasParam(labels[i])) {
      valves[id].days[i] = 1; 
    }
    else
    {
      valves[id].days[i] = 0; 
    }
    EEPROM.write(16+(id*7)+i,valves[id].days[i]);
  }
  
  if (request->hasParam("manual"))
  {
    valves[id].enabled = 0;
  }
  else
  {
    valves[id].enabled = 1;
  }
  EEPROM.write(44+id, valves[id].enabled);

  if (request->hasParam("manualON"))
  {
    valves[id].manualON = 1;  
  }
  else
  {
    valves[id].manualON = 0;
  }
  EEPROM.write(48+id,valves[id].manualON);
  if (request->hasParam("start")) 
  {
  //request->getParam("start")->value()
    char starttb[10];

    request->getParam("start")->value().toCharArray(starttb, 50);
    starttb[2]=0;
    int sh =atoi(starttb);
    int sm = atoi(starttb+3);
    EEPROM.write(id,sh);
    EEPROM.write(id+4,sm);
  }
  if (request->getParam("stop"))
  {
    char endtb[10];
    request->getParam("stop")->value().toCharArray(endtb, 50);
    endtb[2]=0;
    int eh =atoi(endtb);
    int em = atoi(endtb+3);
    EEPROM.write(id+8,eh);
    EEPROM.write(id+12,em);
  }
EEPROM.commit();
}
  int pinMap[] = {27,26,25,33,32};
void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  EEPROM.begin(EEPROM_SIZE);
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(32, OUTPUT);

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/set_valve_0", HTTP_GET, dupa0);
  server.on("/set_valve_1", HTTP_GET, dupa1);
  server.on("/set_valve_2", HTTP_GET, dupa2);
  server.on("/set_valve_3", HTTP_GET, dupa3);
  server.on("/set_time", HTTP_GET, setTime);
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  delay(1000);
  myRTC.updateTime();

  int dayofweek = myRTC.dayofweek-1;
  int timeNow = myRTC.hours*60+myRTC.minutes;
  bool states[] = {0,0,0,0};
  bool overallstate = true;

  for(int i =0; i<4; i++)
  {
    valves[i].enabled = EEPROM.read(44+i);
    valves[i].manualON = EEPROM.read(48+i);
    for(int k =0; k<7; k++)
    {
      valves[i].days[k] = EEPROM.read(16+(i*7)+k);
    }
    valves[i].startTime = (EEPROM.read(i)*60+EEPROM.read(i+4));
    valves[i].endTime = (EEPROM.read(i+8)*60+EEPROM.read(i+12));
    if (valves[i].enabled == 1)
    {
      if (valves[i].days[dayofweek] == 1 )      
      {
        if(timeNow >= valves[i].startTime && timeNow < valves[i].endTime)
        {

        states[i] = 1;
        overallstate = false;
        }
      }
    }
    if (valves[i].enabled == 0)
    {
      if (valves[i].manualON == 1)
      states[i] = 1;
      overallstate = false;
    }
  }

  
  digitalWrite(pinMap[4], overallstate);
  if(overallstate == false)
  {
   for(int i =0; i<4; i++)
   {
        digitalWrite(pinMap[i], states[i]);
//        Serial.print("valve nr ");
//        Serial.print(i+1);
//        if(states[i])
//        {
//          Serial.println(" is working");
//        }
//        else
//        {
//          Serial.println(" is not working");
//        }
    }
  }
}
