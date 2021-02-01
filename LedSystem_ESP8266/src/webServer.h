#include <Arduino.h>
#include <SD.h>

#include <map>
#include <functional>
#include "utils.h"


#include <uri/UriBraces.h>

//#define USE_SECURE_WEB_SERVER


  //#include <ESP8266mDNS.h>
  #include <ESP8266WebServer.h>
#ifdef USE_SECURE_WEB_SERVER
  #include <ESP8266WebServerSecure.h>
#endif



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

void UpdateLeds(const std::map<String, String>* args);
bool sendFile(String path);
void sendError(int code, String str);


ESP8266WebServer server(80);

void notFound()
{
  sendError(404, "Not found");
}

#ifdef USE_SECURE_WEB_SERVER
  BearSSL::ESP8266WebServerSecure secureServer(445);
  void notFoundSecure()
  {
    if(loadFromSdCard(server.uri())) return;
    secureServer.send(404, "text/plain", "Not found");
  }
#endif


String getContentType(String filename)
{
  String contentType = "text/plain";
  if(filename.endsWith(".html") || filename.endsWith(".htm")) contentType = "text/html";
  else if(filename.endsWith(".css")) contentType = "text/css";
  else if(filename.endsWith(".js")) contentType = "application/javascript";
  else if(filename.endsWith(".png")) contentType = "image/png";
  else if(filename.endsWith(".gif")) contentType = "image/gif";
  else if(filename.endsWith(".jpg")) contentType = "image/jpeg";
  else if(filename.endsWith(".ico")) contentType = "image/x-icon";
  else if(filename.endsWith(".xml")) contentType = "text/xml";
  else if(filename.endsWith(".pdf")) contentType = "application/pdf";
  else if(filename.endsWith(".zip")) contentType = "application/zip";
  return contentType;
}

void sendMainPage()
{
  if(!sendFile("index.html"))
    sendError(404, "Not found");
}
void sendError(int code, String str)
{
  server.send(code, "text/plain", String(code)+" : '"+str+"'");
}
bool sendFile(String path) // send the right file to the client (if it exists)
{ 
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);

  Serial.println("send '"+path+"'");
  if (SD.exists(path))
  {
    File file = SD.open(path, FILE_READ);
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}



File uploadingFile;
void uploadFile(std::function<void()> onFinish, std::function<void()> onError)
{
  HTTPUpload& upload = server.upload();
  
  if(upload.status == UPLOAD_FILE_START)
  {
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;

    Serial.print("uploadFile Name: "); Serial.println(filename);

    if(SD.exists(filename))
      SD.remove(filename);

    uploadingFile = SD.open(filename, FILE_WRITE);
  }
  else if(upload.status == UPLOAD_FILE_WRITE)
  {
    if(uploadingFile)
      uploadingFile.write(upload.buf, upload.currentSize);
    Serial.print("Upload: WRITE, Bytes: "); Serial.println(upload.currentSize);
  }
  else if(upload.status == UPLOAD_FILE_END)
  {
    if(uploadingFile)
    {
      uploadingFile.close();
      Serial.print("Upload: END, Size: "); Serial.println(upload.totalSize);
      onFinish();
    }
    else
    {
      onError();
    }
    
  }
  
}

#pragma region FileSystem

void returnOK() {
  server.send(200, "text/plain", "");
}

void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}

void deleteRecursive(String path){
  File file = SD.open((char *)path.c_str());
  if(!file.isDirectory()){
    file.close();
    SD.remove((char *)path.c_str());
    return;
  }

  file.rewindDirectory();
  while(true) {
    File entry = file.openNextFile();
    if (!entry) break;
    String entryPath = path + "/" +entry.name();
    if(entry.isDirectory()){
      entry.close();
      deleteRecursive(entryPath);
    } else {
      entry.close();
      SD.remove((char *)entryPath.c_str());
    }
    yield();
  }

  SD.rmdir((char *)path.c_str());
  file.close();
}

void handleDelete(){
  if(server.args() == 0) return returnFail("BAD ARGS");
  String path = server.arg(0);
  if(path == "/" || !SD.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }
  deleteRecursive(path);
  returnOK();
}

void handleCreate(){
  if(server.args() == 0) return returnFail("BAD ARGS");
  String path = server.arg(0);
  if(path == "/" || SD.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }

  if(path.indexOf('.') > 0){
    File file = SD.open((char *)path.c_str(), FILE_WRITE);
    if(file){
#ifdef ESP8266
      file.write((const char *)0);
#else
      // TODO Create file with 0 bytes???
      file.write(NULL, 0);
#endif
      file.close();
    }
  } else {
    SD.mkdir((char *)path.c_str());
  }
  returnOK();
}

void printDirectory() {
  if(!server.hasArg("dir")) return returnFail("BAD ARGS");
  String path = server.arg("dir");
  if(path != "/" && !SD.exists((char *)path.c_str())) return returnFail("BAD PATH");
  File dir = SD.open((char *)path.c_str());
  path = String();
  if(!dir.isDirectory()){
    dir.close();
    return returnFail("NOT DIR");
  }
  dir.rewindDirectory();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/json", "");
  WiFiClient client = server.client();

  server.sendContent("[");
  for (int cnt = 0; true; ++cnt) {
    File entry = dir.openNextFile();
    if (!entry)
    break;

    String output;
    if (cnt > 0)
      output = ',';

    output += "{\"type\":\"";
    output += (entry.isDirectory()) ? "dir" : "file";
    output += "\",\"name\":\"";
#ifdef ESP8266
    output += entry.name();
#else
    // Ignore '/' prefix
    output += entry.name()+1;
#endif
    output += "\"";
    output += "}";
    server.sendContent(output);
    entry.close();
 }
 server.sendContent("]");
 // Send zero length chunk to terminate the HTTP body
 server.sendContent("");
 dir.close();
}

#pragma endregion


void webServerSetup()
{
  server.on("/", HTTP_GET, [](){
    sendMainPage();
  });


  server.on("/fileSystem/file", HTTP_GET, [](){
    String filename = server.arg("path");
    if(!sendFile(filename))
      sendError(404, "Not found");
  });
  server.on("/fileSystem", HTTP_GET, [](){
    sendFile("filesystem/index.html");
  });
  server.on("/fileSystem/list", HTTP_GET, printDirectory);
  server.on("/fileSystem/edit", HTTP_DELETE, handleDelete);
  server.on("/fileSystem/edit", HTTP_PUT, handleCreate);
  server.on("/fileSystem/edit", HTTP_POST, [](){ returnOK(); }, [](){ uploadFile([](){},[](){}); });


  server.on("/led", HTTP_GET, [](){
    auto args = getRequestArgs(&server);
    Serial.println("Got not async not secure '/led':");
    printArgs(&args);
    UpdateLeds(&args);
    sendMainPage();
  });

#ifdef USE_SECURE_WEB_SERVER

  //configTime(8 * 3600, 0, "0.ru.pool.ntp.org", "1.ru.pool.ntp.org", "2.ru.pool.ntp.org");

  secureServer.getServer().setRSACert(&certificate, &privateKey);
  secureServer.on("/", HTTP_GET, [](){
    secureServer.send(200, "text/html", index_html);
  });

  secureServer.on("/get", HTTP_GET, [](){
    auto args = getRequestArgs(&secureServer);
    Serial.println("Got not async secure '/get':");
    printArgs(&args);
    UpdateLeds(&args);
    secureServer.send(200, "text/html", index_html);
  });


  secureServer.onNotFound(notFoundSecure);
  secureServer.begin();
  
#endif

  server.onNotFound(notFound);
  server.begin();
}

void webServerLoop()
{
  server.handleClient();
#ifdef USE_SECURE_WEB_SERVER
  secureServer.handleClient();
#endif
  //MDNS.update();
}




