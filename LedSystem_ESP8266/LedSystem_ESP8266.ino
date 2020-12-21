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

int speed = 50;


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
    if (request->hasParam("speed"))
    {
      speed = StrToInt(request->getParam("speed")->value());
      if (speed > 100)
        speed = 100;
      if (speed < -100)
        speed = -100;
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

    if (speed == 0)
    {
      delay(100);
    }
    else if (speed > 0)
    {
      ++(*ledMode);
      ++(*ledMask);
      delay(1000 - speed * 10 + 1);
    }
    else
    {
      --(*ledMode);
      --(*ledMask);
      delay(1000 + speed * 10 + 1);
    }
    Serial.println("here2");
  }
  else
  {
    delay(100);
  }
}

#endif // LED_SYSTEM_ESP8266_INO
