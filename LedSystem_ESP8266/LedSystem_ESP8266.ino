#ifndef LED_SYSTEM_ESP8266_INO
#define LED_SYSTEM_ESP8266_INO

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESPAsyncWebServer.h>


#include "leds.h"
#include "wifiBuilding.h"
#include "wifiSettings.h"

#include "ledModes/LedMode.h"
#include "ledModes/SimpleMode.h"
#include "ledModes/RainbowMode.h"
#include "ledModes/RainbowFadeMode.h"


#include "ledMasks/FullMask.h"
#include "ledMasks/SplitMask.h"
#include "ledMasks/GrowNBackMask.h"
#include "ledMasks/GrowNBack2CenterMask.h"
#include "ledMasks/RunnerMask.h"

#include "utils.h"
#include "html.h"

ESP8266WiFiMulti wifiMulti;


AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


void connectToWifi()
{
  Serial.print("Connecting to " + String(ssid) + " ...");

  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(250);
    Serial.print('.');
  }

  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());
}


void tryConnectToWifi()
{
  Serial.print("Connecting to " + String(ssid) + " ...");

  if (wifiMulti.run() == WL_CONNECTED)
  {
    Serial.print("Connected. IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.print("Not connected.");
  }


}


Leds* leds;
LedMode* ledMode;
LedMask* ledMask;
String currMode;
String currMask;

float modeSpeed = 50;
float maskSpeed = 50;

float updatePeriod = 20; // in ms
unsigned long timeBeforeDelay;

void UpdateMode(AsyncWebServerRequest *request)
{
  if (request->hasParam("mode"))
  {
    const String mode = request->getParam("mode")->value();


    if (currMode == mode)
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
  if (ledMode != nullptr)
  {
    delete ledMode;
  }

  if (mode == "off")
  {
    ledMode = new SimpleMode(CRGB::Black);
  }
  else if (mode == "simple")
  {
    ledMode = new SimpleMode(request);
  }
  else if (mode == "rainbow")
  {
    ledMode = new RainbowMode();
  }
  else if (mode == "rainbowfade")
  {
    ledMode = new RainbowFadeMode();
  }
  else
  {
    return false;
  }

  return true;
}

void UpdateMask(String mask)
{
  if(currMask != mask)
  {
    if (ledMask != nullptr)
    {
      delete ledMask;
    }
  
    if (mask == "full")
    {
      ledMask = new FullMask(0, leds->GetCount());
    }
    else if (mask == "grownback")
    {
      ledMask = new GrowNBackMask(0, leds->GetCount());
    }
    else if (mask == "grownback2center")
    {
      ledMask = new GrowNBack2CenterMask(0, leds->GetCount());
    }
    else if (mask == "runner")
    {
      ledMask = new RunnerMask(5, 1, 0, leds->GetCount());
    }
    else
    {
      ledMask = new FullMask(0, leds->GetCount());
    }
  
    currMask=mask;
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("starting");

  wifiMulti.addAP(ssid, password);
  connectToWifi();
  setupWifiBuilding();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request)
  {
    if (request->hasParam("brightness"))
    {
      const int brightness = request->getParam("brightness")->value().toInt();
      LEDS.setBrightness(brightness);
    }
    if (request->hasParam("update_period"))
    {
      updatePeriod = clamp(request->getParam("update_period")->value().toInt(), 1, 1000);
    }

    if (request->hasParam("mode_speed"))
    {
      modeSpeed = clamp(request->getParam("mode_speed")->value().toInt(), 0, 100);
      if(request->hasParam("mode_inverted") && request->getParam("mode_inverted")->value()=="on")
        modeSpeed = -modeSpeed;
    }
    if (request->hasParam("mask_speed"))
    {
      maskSpeed = clamp(request->getParam("mask_speed")->value().toInt(), 0, 100);
      if(request->hasParam("mask_inverted") && request->getParam("mask_inverted")->value()=="on")
        maskSpeed = -maskSpeed;
    }
      

    if (request->hasParam("mask"))
    {
      UpdateMask(request->getParam("mask")->value());
    }
    UpdateMode(request);

    
    request->send(200, "text/html", index_html);

  });


  server.onNotFound(notFound);
  server.begin();

  leds = new Leds();
  CreateNewMode("off", nullptr);
  UpdateMask("full");

  timeBeforeDelay = millis();
}



void loop()
{
  if (wifiMulti.run() != WL_CONNECTED)
  {
    tryConnectToWifi();
  }
  loopWifiBuilding();


  if (ledMode != nullptr)
  {
    leds->fill(CRGB::Black);
    ledMode->Draw(leds, ledMask);
    //ledMode->Draw(leds);
    leds->Show();

    unsigned long realTime = millis() - timeBeforeDelay;
    (*ledMode) += modeSpeed*realTime/1000;
    (*ledMask) += maskSpeed*realTime/1000;
  }
  timeBeforeDelay = millis();

  delay(updatePeriod);
}

#endif // LED_SYSTEM_ESP8266_INO
