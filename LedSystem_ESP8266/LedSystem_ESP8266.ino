#ifndef LED_SYSTEM_ESP8266_INO
#define LED_SYSTEM_ESP8266_INO

#include <map>

#include <ESP8266WiFi.h>

//#define USE_ASYNC_WEB_SERVER
#define USE_SECURE_WEB_SERVER

#ifdef USE_ASYNC_WEB_SERVER
  #include <ESPAsyncWebServer.h>
#else
  //#include <ESP8266mDNS.h>
  #include <ESP8266WebServer.h>
#ifdef USE_SECURE_WEB_SERVER
  #include <ESP8266WebServerSecure.h>
#endif
#endif

#include "leds.h"
#include "wifiBuilding.h"
#include "wifiSettings.h"

#include "ledModes/LedMode.h"
#include "ledModes/SimpleMode.h"
#include "ledModes/RainbowMode.h"
#include "ledModes/RainbowFadeMode.h"


#include "ledMasks/FullMask.h"
#include "ledMasks/GrowNBackMask.h"
#include "ledMasks/GrowNBack2CenterMask.h"
#include "ledMasks/RunnerMask.h"

#include "utils.h"
#include "html.h"


#ifdef USE_ASYNC_WEB_SERVER
  std::map<String, String> getRequestArgs(AsyncWebServerRequest* request)
  {
    std::map<String, String> result;
    for(size_t i = 0; i < request->params(); i++)
    {
      AsyncWebParameter* param = request -> getParam(i);
      result.insert(std::pair<String, String>(param->name(), param->value()));
    }
  
    return result;
  }
#else

  template<typename ServerType>
  std::map<String, String> getRequestArgs(esp8266webserver::ESP8266WebServerTemplate<ServerType>* server)
  {
    std::map<String, String> result;
    for(int i = 0; i < server->args(); i++)
    {
      result.insert(std::pair<String, String>(server->argName(i), server->arg(i)));
    }
  
    return result;
  }
#endif

#ifdef USE_ASYNC_WEB_SERVER
  AsyncWebServer server(80);

  void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
  }
#else
  ESP8266WebServer server(80);
  
  void notFound() {
    server.send(404, "text/plain", "Not found");
  }

  #ifdef USE_SECURE_WEB_SERVER
    BearSSL::ESP8266WebServerSecure secureServer(445);
    void notFoundSecure() {
      secureServer.send(404, "text/plain", "Not found");
    }
  #endif
  
#endif


bool connectToWifi()
{
  //Serial.print("Connecting to ");
  //Serial.println(ssid);

  if(WiFi.status() == WL_CONNECTED)
  {
    //Serial.println("Already connected!");
    return true;
  }

  Serial.print("Connecting to ");
  Serial.println(ssid);

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

void UpdateLeds(std::map<String, String> args)
{
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
}

void setup()
{
  Serial.begin(115200);
  Serial.println("ESP started");

  connectToWifi();
  setupWifiBuilding();

#ifdef USE_ASYNC_WEB_SERVER
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request)
  {
    UpdateLeds(getRequestArgs(request));
    request->send(200, "text/html", index_html);

  });
#else

  

  server.on("/", HTTP_GET, [](){
    server.send(200, "text/html", index_html);
  });

  server.on("/get", HTTP_GET, [](){
    UpdateLeds(getRequestArgs(&server));
    server.send(200, "text/html", index_html);
  });
#ifdef USE_SECURE_WEB_SERVER

  //configTime(8 * 3600, 0, "0.ru.pool.ntp.org", "1.ru.pool.ntp.org", "2.ru.pool.ntp.org");

  secureServer.getServer().setRSACert(&certificate, &privateKey);
  secureServer.on("/", HTTP_GET, [](){
    secureServer.send(200, "text/html", index_html);
  });

  secureServer.on("/get", HTTP_GET, [](){
    UpdateLeds(getRequestArgs(&secureServer));
    secureServer.send(200, "text/html", index_html);
  });


  secureServer.onNotFound(notFoundSecure);
  secureServer.begin();
  
#endif
#endif

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

#ifndef USE_ASYNC_WEB_SERVER
  server.handleClient();
#ifdef USE_SECURE_WEB_SERVER
  secureServer.handleClient();
#endif
  //MDNS.update();
#endif

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
