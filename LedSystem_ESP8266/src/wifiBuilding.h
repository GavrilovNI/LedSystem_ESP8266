#ifndef WIFI_BUILDING_H
#define WIFI_BUILDING_H

#include <ArduinoOTA.h>

//functions of uploads stages (start, progress, end and error)
void startOTA()
{
   String type;
   
    //if the update is burning in external flash memory, then show "flash"  
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "flash";
    else  //if the update is on the internal memory (file system),then show "filesystem"
      type = "filesystem"; // U_SPIFFS

    //print message and the burn type  
    Serial.println("Start updating " + type);
}

//just show message "End"
void endOTA()
{
  Serial.println("\nEnd");
}

//shows the progress in porcent
void progressOTA(unsigned int progress, unsigned int total)
{
   Serial.printf("Progress: %u%%\r", (progress / (total / 100))); 
}

//if an error occurred, shows especificaly the error type 
void errorOTA(ota_error_t error)
{  
    Serial.printf("Error[%u]: ", error);
      
    if (error == OTA_AUTH_ERROR) 
      Serial.println("Auth Failed");
    else
    if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else 
    if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else
    if (error == OTA_RECEIVE_ERROR) 
      Serial.println("Receive Failed");
    else 
    if (error == OTA_END_ERROR)
      Serial.println("End Failed");
}

void setupWifiBuilding()
{
  //configures what will be executed when ArduinoOTA starts
  ArduinoOTA.onStart( startOTA ); //startOTA is a function created to simplificate the code

  //configures what will be executed when ArduinoOTA ends
  ArduinoOTA.onEnd( endOTA ); //endOTA is a function created to simplificate the code

  //configures what will be executed when the sketches is burning in the esp
  ArduinoOTA.onProgress( progressOTA ); //progressOTA is a function created to simplificate the code

  //configures what will be executed when ArduinoOTA finds an error
  ArduinoOTA.onError( errorOTA );//errorOTA is a function created to simplificate the code
  
  //initializes ArduinoOTA
  ArduinoOTA.begin();
}

void loopWifiBuilding()
{
  //Handle is an abstract reference to a resource. That references variables on the blocks of memory
  ArduinoOTA.handle();
}

#endif // WIFI_BUILDING_H
