#ifndef MAIN_KITCHEN_LED
#define MAIN_KITCHEN_LED

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESPAsyncWebServer.h>


#include "leds.h"
#include "wifiSettings.h"

#include "ledMods/LedMode.h"
#include "ledMods/SimpleMode.h"
#include "ledMods/RainbowMode.h"
#include "ledMods/RainbowFadeMode.h"
#include "ledMods/GrowMode.h"

#include "utils.h"
#include "html.h"

ESP8266WiFiMulti wifiMulti;


AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


void connectToWifi()
{
  Serial.print("Connecting to "+String(ssid)+" ...");

  while(wifiMulti.run()!=WL_CONNECTED)
  {
    delay(250);
    Serial.print('.');
  }
  
  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());
}


void tryConnectToWifi()
{
  Serial.print("Connecting to "+String(ssid)+" ...");

  if(wifiMulti.run()==WL_CONNECTED)
  {
    Serial.print("Connected. IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.print("Not connected.");
  }
  
  
}



LedMode* ledMode;
String currMode = "off";

int speed = 500;


void UpdateMode(AsyncWebServerRequest *request)
{
  if (request->hasParam("mode"))
  {
    const String mode = request->getParam("mode")->value();


    if(currMode == mode)
    {
      ledMode->Update(request);
    }
    else
    {
      CreateNewMode(mode, request);
      currMode = mode;
    }
  }
}

bool CreateNewMode(String mode, AsyncWebServerRequest *request)
{
  if(ledMode!=nullptr)
  {
    delete ledMode;
  }

  if(mode=="off")
  {
    fill(CRGB::Black);
    LEDS.show();
  }
  else if(mode=="simple")
  {
    ledMode = new SimpleMode(request);
  }
  else if(mode=="rainbow")
  {
    ledMode = new RainbowMode();
  }
  else if(mode=="rainbowfade")
  {
    ledMode = new RainbowFadeMode();
  }
  else if(mode=="growing")
  {
    ledMode = new GrowMode(request);
  }
  else
  {
    return false;
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("starting");

  wifiMulti.addAP(ssid, password);
  connectToWifi();
  setupWifiBuilding();

  setupLeds();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request)
  {
    if (request->hasParam("brightness"))
    {
      const int brightness = request->getParam("brightness")->value().toInt();
      LEDS.setBrightness(brightness);
    }
    if (request->hasParam("speed"))
    {
      speed = StrToInt(request->getParam("speed")->value());
      if(speed>100)
        speed=100;
      if(speed<-100)
        speed=-100;
    }

    UpdateMode(request);
    request->send(200, "text/html", index_html);

  });

  
  server.onNotFound(notFound);
  server.begin();
}



void loop() 
{
  if(wifiMulti.run()!=WL_CONNECTED)
  {
    tryConnectToWifi();
  }
  loopWifiBuilding();
  

  if(ledMode!=nullptr)
  {
    
    ledMode->Draw();
    LEDS.show();

    if (speed==0)
    {
      delay(100);
    }
    else if(speed>0)
    {
      ++(*ledMode);
      delay(1000-speed*10+1);
    }
    else
    {
      --(*ledMode);
      delay(1000+speed*10+1);
    }
  }
  else
  {
    delay(100);
  }
}

#endif // MAIN_KITCHEN_LED
