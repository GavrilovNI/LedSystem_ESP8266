#ifndef WIFI_SETTINGS
#define WIFI_SETTINGS

const char* ssid = "*****";
const char* password = "*****";

#ifdef USE_SECURE_WEB_SERVER
const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";


const char serverKey[] PROGMEM =  R"EOF(
-----BEGIN PRIVATE KEY-----
-----END PRIVATE KEY-----
)EOF";

BearSSL::X509List certificate(serverCert);
BearSSL::PrivateKey privateKey(serverKey);
#endif

#endif // WIFI_SETTINGS
