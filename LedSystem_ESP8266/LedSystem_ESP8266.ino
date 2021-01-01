#ifndef LED_SYSTEM_ESP8266_INO
#define LED_SYSTEM_ESP8266_INO


#include <map>

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>

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


AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


bool connectToWifi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Already connected!");
    return true;
  }

  WiFi.begin(ssid, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i<20)
  {
    delay(500);
    Serial.print('.');
    i++;
  }

  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  else
  {
    Serial.println("Fail. Not connected.");
    return false;
  }
}


Leds* leds;
LedMode* ledMode;
LedMask* ledMask;
String currMode;
String currMask;

float modeSpeed = 50;
float maskSpeed = 50;

bool modeInverted = false;
bool maskInverted = false;

float updatePeriod = 20; // in ms
unsigned long timeBeforeDelay;

void UpdateMode(const std::map<String, String>* args)
{
  auto modeIt = args->find("mode");
  if (modeIt != args->end())
  {
    const String mode = modeIt->second;


    if (currMode == mode)
    {
      ledMode->Update(args);
    }
    else
    {
      CreateNewMode(mode, args);
      currMode = mode;
    }
  }
}

bool CreateNewMode(String mode, const std::map<String, String>* args)
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
    ledMode = new SimpleMode(args);
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

  if(!connectToWifi())
  {
    delay(1000);
    ESP.restart();
    return;
  }
  setupWifiBuilding();

  

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request)
  {
    auto args = getRequestArgs(request);

    for(auto it = args.begin(); it != args.end(); it++)
    {
      String key = it->first;
      String value = it->second;
      if(key == "brightness")
      {
        LEDS.setBrightness(clamp(value.toInt(), 0, 255));
      }
      else if(key == "update_period")
      {
        updatePeriod = clamp(value.toInt(), 1, 1000);
      }
      else if(key == "mode_speed")
      {
        modeSpeed = clamp(value.toInt(), 0, 100);
      }
      else if(key == "mode_inverted")
      {
        modeInverted = value == "1";
      }
      else if(key == "mask_speed")
      {
        maskSpeed = clamp(value.toInt(), 0, 100);
      }
      else if(key == "mask_inverted")
      {
        maskInverted = value == "1";
      }
      else if(key == "mask")
      {
        UpdateMask(value);
      }
    }

    UpdateMode(&args);

    
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
  connectToWifi();
  loopWifiBuilding();


  if (ledMode != nullptr)
  {
    leds->fill(CRGB::Black);
    ledMode->Draw(leds, ledMask);
    //ledMode->Draw(leds);
    leds->Show();

    unsigned long realTime = millis() - timeBeforeDelay;

    float modeDelta = modeSpeed * realTime / 1000;
    float maskDelta = maskSpeed * realTime / 1000;

    
    (*ledMode) += modeInverted ? -modeDelta : modeDelta;
    (*ledMask) += maskInverted ? -maskDelta : maskDelta;
  }
  timeBeforeDelay = millis();

  delay(updatePeriod);
}

#endif // LED_SYSTEM_ESP8266_INO
