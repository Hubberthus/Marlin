#ifndef WEBPAGE_WEBPAGE_H_
#define WEBPAGE_WEBPAGE_H_

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266SSDP.h>

#include "Marlin.h"

void setup_webpage();

void webpage_update();

void urldecode(String &input);

void fsHandler();

void handleConnect();

void handleDisconnect();

void handleStatus();

void handleReply();

void handleFiles();

void handleGcode();

void handleConfig();

void handleUploadStart();

void handleUploadData();

void handleUploadEnd();

void handleUploadCancel();

void handleDelete();

void handleFileinfo();

void handleMkdir();

void handleUnsupported();

bool isLoggedIn(IPAddress clientIP);

bool logIn(IPAddress clientIP);

void logOut(IPAddress clientIP);

extern bool enqueue_and_echo_command(const char* cmd, bool say_ok/*=false*/);

extern String getStatus(uint8_t type);

const char STR_EEPROM_DUMMY[] PROGMEM = "AAA";
const char STR_EEPROM_DEFAULT_WEBPASSWORD[] PROGMEM = "reprap";
const char STR_MIME_TEXT_HTML[] PROGMEM = "text/html";
const char STR_MIME_TEXT_PLAIN[] PROGMEM = "text/plain";
const char STR_MIME_APPLICATION_JSON[] PROGMEM = "application/json";

const char STR_OK[] PROGMEM = "ok";
const char STR_TRUE[] PROGMEM = "true";

const char STR_CONTENT_LENGTH[] PROGMEM = "Content-Length";

const char STR_JSON_ERR_0[] PROGMEM = "{\"err\":0}";
const char STR_JSON_BUFF_16[] PROGMEM = "{\"buff\":16}";
const char STR_JSON_ERR_404_NOT_FOUND[] PROGMEM = "{\"err\":\"ERROR 404: NOT FOUND\"}";
const char STR_JSON_ERR_UNSUPPORTED_OPERATION[] PROGMEM = "{\"err\":\"Unsupported operation :(\"}";
const char STR_JSON_ERR_500_NO_DATA_RECEIVED[] PROGMEM = "{\"err\":\"ERROR 500: NO DATA RECEIVED\"}";
const char STR_JSON_ERR_500_EMPTY_COMMAND[] PROGMEM = "{\"err\":\"ERROR 500: EMPTY COMMAND\"}";
const char STR_JSON_ERR_500_NOT_UPLOADING_FILES[] PROGMEM = "{\"err\": \"ERROR 500: NOT UPLOADING FILES\"}";
const char STR_JSON_ERR_500_NO_FILENAME_PROVIDED[] PROGMEM = "{\"err\": \"ERROR 500: NO FILENAME PROVIDED\"}";
const char STR_JSON_ERR_500_NO_DIR_NAME_PROVIDED[] PROGMEM = "{\"err\":\"ERROR 500: NO DIR NAME PROVIDED\"}";

const char STR_M117[] PROGMEM = "M117 ";
const char STR_M408_S[] PROGMEM = "M408 S";
const char STR_M20_S2_P[] PROGMEM = "M20 S2 P";
const char STR_M28[] PROGMEM = "M28 ";
const char STR_M29[] PROGMEM = "M29 ";
const char STR_M30[] PROGMEM = "M30 ";
const char STR_M32[] PROGMEM = "M32 ";
const char STR_M36[] PROGMEM = "M36 ";
const char STR_M575_P1_B115200_S0[] PROGMEM = "M575 P1 B115200 S0";
const char STR_M575_P1_B460800_S0[] PROGMEM = "M575 P1 B460800 S0";

#endif /* WEBPAGE_WEBPAGE_H_ */
