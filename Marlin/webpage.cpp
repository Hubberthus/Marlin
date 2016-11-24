#ifndef ESP8266

void setup_webpage() {}

void webpage_update() {}

#else

#include "webpage.h"

#define BUTTON_PIN -1
#define MAX_WIFI_FAIL 50
#define MAX_LOGGED_IN_CLIENTS 3

char ssid[32], pass[64], webhostname[64], webpassword[32];
IPAddress sessions[MAX_LOGGED_IN_CLIENTS];
uint8_t loggedInClientsNum = 0;
MDNSResponder mdns;
ESP8266WebServer server(80);
WiFiServer tcp(23);
WiFiClient tcpclient;
String lastResponse = "";
String serialData;
String fileUploading = "";
String lastUploadedFile = "";
DNSServer dns;

void enqueue_command(String command) {
	while(command.indexOf('\n') > 0) {
		enqueue_and_echo_command(command.substring(0, command.indexOf('\n')).c_str(), true);
		command = command.substring(command.indexOf('\n') + 1);
	}
	enqueue_and_echo_command(command.c_str(), true);
}

void setup_webpage() {
  
#if (BUTTON_PIN > -1)
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  if (digitalRead(BUTTON_PIN) == LOW) { // Clear wifi config
	enqueue_command("M117 ESPRINTER ERASE");
    EEPROM.put(0, FPSTR(STR_EEPROM_DUMMY));
    EEPROM.put(32, FPSTR(STR_EEPROM_DUMMY));
    EEPROM.put(32+64+64, FPSTR(STR_EEPROM_DEFAULT_WEBPASSWORD));
    EEPROM.commit();
  }
#endif

  /*EEPROM.get(0, ssid);
  EEPROM.get(32, pass);
  EEPROM.get(32+64, webhostname);
  EEPROM.get(32+64+64, webpassword);*/
  strcpy(ssid, STR_EEPROM_DUMMY);
  strcpy(pass, STR_EEPROM_DUMMY);
  strcpy(webhostname, "espextra");
  strcpy(webpassword, STR_EEPROM_DEFAULT_WEBPASSWORD);

  /*uint8_t failcount = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    failcount++;
    if (failcount % 2 == 0) {
    	SERIAL_ECHO_START;
    	SERIAL_ECHOLN("WAIT WIFI " + String(MAX_WIFI_FAIL/2 - (failcount/2)));
    }
    
    if (failcount > MAX_WIFI_FAIL) { // 1 min
      SERIAL_ECHO_START;
      SERIAL_ECHOLN("WIFI ERROR");
      WiFi.mode(WIFI_STA);
      WiFi.disconnect();
      delay(100);

      uint8_t num_ssids = WiFi.scanNetworks();
      // TODO: NONE? OTHER?
      String wifiConfigHtml = F("<html><body><h1>Select your WiFi network:</h1><br /><form method=\"POST\">");
      for (uint8_t i = 0; i < num_ssids; i++) {
         wifiConfigHtml += "<input type=\"radio\" id=\"" + WiFi.SSID(i) + "\"name=\"ssid\" value=\"" + WiFi.SSID(i) + "\" /><label for=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</label><br />";
      }
      wifiConfigHtml += F("<label for=\"password\">WiFi Password:</label><input type=\"password\" id=\"password\" name=\"password\" /><br />");
      wifiConfigHtml += F("<p><label for=\"webhostname\">ESPrinter Hostname: </label><input type=\"text\" id=\"webhostname\" name=\"webhostname\" value=\"esprinter\"/><br />");
      wifiConfigHtml += F("<i>(This would allow you to access your printer by name instead of IP address. I.e. http://esprinter.local/)</i></p>");
      wifiConfigHtml += F("<p><label for=\"webpassword\">Web Password:</label><input type=\"password\" id=\"webpassword\" name=\"webpassword\"><br />");
      wifiConfigHtml += F("<i>(This is the password for logging into the web interface. Default: \"reprap\")</i></p>");
      wifiConfigHtml += F("<input type=\"submit\" value=\"Save and reboot\" /></form></body></html>");

      SERIAL_ECHO_START;
      SERIAL_ECHOLN("FOUND " + String(num_ssids) + " WIFI");

      delay(5000);
      DNSServer dns;
      IPAddress apIP(192, 168, 1, 1);
      WiFi.mode(WIFI_AP);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP("ESPrinter");
      SERIAL_ECHO_START;
      SERIAL_ECHOLN("WiFi -> ESPrinter");
      dns.setErrorReplyCode(DNSReplyCode::NoError);
      dns.start(53, "*", apIP);

      server.on("/", HTTP_GET, [&wifiConfigHtml]() {
        server.send(200, FPSTR(STR_MIME_TEXT_HTML), wifiConfigHtml);
      });

      server.on("/", HTTP_POST, []() {
        if (server.args() <= 0) {
          server.send(500, FPSTR(STR_MIME_TEXT_PLAIN), F("Got no data, go back and retry"));
          return;
        }
        for (uint8_t e = 0; e < server.args(); e++) {
          String argument = server.arg(e);
          urldecode(argument);
          if (server.argName(e) == "password") argument.toCharArray(pass, 64);//pass = server.arg(e);
          else if (server.argName(e) == "ssid") argument.toCharArray(ssid, 32);//ssid = server.arg(e);
          else if (server.argName(e) == "webhostname") argument.toCharArray(webhostname, 64);
          else if (server.argName(e) == "webpassword") argument.toCharArray(webpassword, 32);
        }
        if (String(webpassword) == "") snprintf(webpassword, 32, "%s", FPSTR(STR_EEPROM_DEFAULT_WEBPASSWORD));
        //EEPROM.put(0, ssid);
        //EEPROM.put(32, pass);
        //EEPROM.put(32+64, webhostname);
        //EEPROM.put(32+64+64, webpassword);
        //EEPROM.commit();
        server.send(200, FPSTR(STR_MIME_TEXT_HTML), F("<h1>All set!</h1><br /><p>(Please reboot me.)</p>"));
        SERIAL_ECHO_START;
        SERIAL_ECHOLN("SSID: " + String(ssid) + ", PASS: " + String(pass));
        delay(50);
        ESP.restart();
      });
      server.begin();
      SERIAL_ECHO_START;
      SERIAL_ECHOLN(WiFi.softAPIP().toString());
      for (;;) { // THIS ONE IS FOR WIFI AP SETTINGS PAGE
        server.handleClient();
        dns.processNextRequest();
        delay(1);
      }
    }
  }*/

  IPAddress apIP(192, 168, 1, 1);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("ESPrinter");
  dns.setErrorReplyCode(DNSReplyCode::NoError);
  dns.start(53, "*", apIP);

  /*if (mdns.begin(webhostname, WiFi.localIP())) {
    MDNS.addService("http", "tcp", 80);
  }*/
    
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(webhostname);
  SSDP.setSerialNumber(WiFi.macAddress());
  SSDP.setURL("index.html");
  SSDP.begin();
    
  SPIFFS.begin();

  server.onNotFound(fsHandler);
  server.on("/rr_connect", handleConnect);
  server.on("/rr_disconnect", handleDisconnect);
  server.on("/rr_status", handleStatus);
  server.on("/rr_reply", handleReply);
  server.on("/rr_filelist", handleFiles);
  server.on("/rr_gcode", handleGcode);
  server.on("/rr_config", handleConfig);
  server.on("/rr_upload_begin", handleUploadStart);
  server.on("/rr_upload", handleUploadData);
  server.on("/rr_upload_end", handleUploadEnd);
  server.on("/rr_upload_cancel", handleUploadCancel);
  server.on("/rr_delete", handleDelete);
  server.on("/rr_fileinfo", handleFileinfo);
  server.on("/rr_mkdir", handleMkdir);
  server.on("/description.xml", HTTP_GET, [](){SSDP.schema(server.client());});

  // UNSUPPORTED STUFF
  server.on("/rr_move", handleUnsupported);
  server.begin();
  /*tcp.begin();
  tcp.setNoDelay(true);*/
}

void webpage_update() {
  server.handleClient();
  dns.processNextRequest();

  /*while (MYSERIAL.available() > 0) {
    char character = MYSERIAL.read();
    if (character == '\n' || character == '\r') {
      if (serialData.startsWith(FPSTR(STR_OK))) {
          serialData = "";
          continue;
      }
      tcpclient.write(serialData.c_str(), strlen(serialData.c_str()));
      tcpclient.flush();
      delay(1);
      lastResponse = String(serialData);
      serialData = "";
    } else {
      serialData.concat(character);
    }
  }*/

  /*// DISCONNECT ALL IF SOMEONE IS ALLREADY CONNECTED
  if (tcp.hasClient()) {
      if (tcpclient && tcpclient.connected()) {
          WiFiClient otherGuy = tcp.available();
          otherGuy.stop();
      } else {
          tcpclient = tcp.available();
      }
  }

  // PUSH FRESH DATA FROM TELNET TO SERIAL
  if (tcpclient && tcpclient.connected()) {
    while (tcpclient.available()) {
      uint8_t data = tcpclient.read();
      tcpclient.write(data); // ECHO BACK TO SEE WHATCHA TYPIN
      //MYSERIAL.write(data);
    }
  }*/
}

void fsHandler() {
  String path = server.uri();
  if (path.endsWith("/")) path += F("index.html");
  else if (!path.endsWith(".html") && !path.startsWith("/rr_")) path += F(".gz");
  File dataFile = SPIFFS.open(path, "r");
  if (!dataFile) {
	path = F("/html404.html");
	dataFile = SPIFFS.open(path, "r");
  }
  if (!dataFile) {
    server.send(404, FPSTR(STR_MIME_APPLICATION_JSON), "{\"err\": \"404: " + server.uri() + " NOT FOUND\"}");
    return;
  }
  server.sendHeader(FPSTR(STR_CONTENT_LENGTH), String(dataFile.size()));
  String dataType = FPSTR(STR_MIME_TEXT_PLAIN);
  //if (path.endsWith(".gz")) server.sendHeader(F("Content-Encoding"), "gzip");
  if (path.endsWith(".html")) dataType = FPSTR(STR_MIME_TEXT_HTML);
  else if (path.endsWith(".css")) dataType = F("text/css");
  else if (path.endsWith(".js")) dataType = F("application/javascript");
  else if (path.endsWith(".js.gz")) dataType = F("application/javascript");
  else if (path.endsWith(".css.gz")) dataType = F("text/css");
  else if (path.endsWith(".gz")) dataType = F("application/x-gzip");
  server.streamFile(dataFile, dataType);
  dataFile.close();
}

void handleConnect() {
  String password = "";
  if (server.args() > 0) password = server.arg(0);
  if (password == String(webpassword)) {
    if (!logIn(server.client().remoteIP())) {
      // FUBAR, JUST SEND 403
      server.send(403, FPSTR(STR_MIME_APPLICATION_JSON), "{\"err\":\"403 NOT AUTHORISED\"}");
      return;
    }
  }
  if (isLoggedIn(server.client().remoteIP())) {
    server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_0));
  } else {
    // 403 WILL SHOW ERROR IN FROMTEND, BUT 200 WILL PROMPT FOR PASSWORD
    server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), "{\"err\":\"WRONG PASSWORD\"}");
  }
}

void handleDisconnect() {
  logOut(server.client().remoteIP());
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_0));
}

void handleStatus() {
  uint8_t type = (server.args() < 1) ? 1 : server.arg(0).toInt();
  /*enqueue_command(FPSTR(STR_M408_S) + type);
  MYSERIAL.setTimeout(5000); // 2s
  serialData = MYSERIAL.readStringUntil('\n');
  if (serialData.startsWith(FPSTR(STR_OK))) serialData = MYSERIAL.readStringUntil('\n');
  lastResponse = String(serialData);
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), lastResponse);*/
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), String(getStatus(type)));
}

void handleReply() {
  server.send(200, FPSTR(STR_MIME_TEXT_PLAIN), lastResponse);
}

void handleFiles() {
  String dir = "";
  if (server.args() > 0) {
    dir = server.arg(0);
  }
  urldecode(dir);
  /*enqueue_command(STR_M20_S2_P + dir);
  MYSERIAL.setTimeout(5000);
  serialData = MYSERIAL.readStringUntil('\n');
  if (serialData.startsWith(FPSTR(STR_OK))) serialData = MYSERIAL.readStringUntil('\n');
  lastResponse = String(serialData);
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), lastResponse);*/
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), "{\"dir\": \"" + dir + "\",\"files\": []}");
}

void handleGcode() {
  String gcode = "";
  if (server.args() > 0) {
    gcode = server.arg(0);
  } else {
    server.send(500, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_500_EMPTY_COMMAND));
    return;
  }
  urldecode(gcode);
  enqueue_command(gcode);
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_BUFF_16));
}

void handleConfig() {
  /*enqueue_command(FPSTR(STR_M408_S) + '5');
  MYSERIAL.setTimeout(5000);
  serialData = MYSERIAL.readStringUntil('\n');
  if (serialData.startsWith(FPSTR(STR_OK))) serialData = MYSERIAL.readStringUntil('\n');
  lastResponse = String(serialData);
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), lastResponse);*/
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), String(getStatus(4)));
}

void handleUploadStart() {
  /*if (server.args() > 0) {
    fileUploading = server.arg(0);
  } else {
    server.send(500, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_500_NO_FILENAME_PROVIDED));
    return;
  }
  bool compat = false;
  if (server.args() > 1) {
    compat = (server.arg(1) == FPSTR(STR_TRUE));
  }
  urldecode(fileUploading);
  lastUploadedFile = fileUploading;
  enqueue_command(FPSTR(STR_M28) + fileUploading);
  MYSERIAL.flush();
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_0));*/
}

void handleUploadData() {
  if (fileUploading == "") {
    server.send(500, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_500_NOT_UPLOADING_FILES));
    return;
  }
  String data = "";
  if (server.args() > 0) {
    data = server.arg(0);
  } else {
    server.send(500, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_500_NO_DATA_RECEIVED));
    return;
  }
  urldecode(data);
  enqueue_command(data);
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_0));
}

void handleUploadEnd() {
  if (fileUploading == "") {
    server.send(500, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_500_NOT_UPLOADING_FILES));
    return;
  }
  bool compat = false;
  if (server.args() > 0) {
    compat = (server.arg(0) == FPSTR(STR_TRUE));
  }
  enqueue_command(STR_M29 + fileUploading);
  fileUploading = "";
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_0));
}

void handleUploadCancel() {
  // IS SENT AFTER UPLOAD END
  enqueue_command(STR_M30 + lastUploadedFile);
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_0));
}

void handleDelete() {
  String fileName = "";
  if (server.args() > 0) {
    fileName = server.arg(0);
  } else {
    server.send(500, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_500_NO_FILENAME_PROVIDED));
    return;
  }
  urldecode(fileName);
  enqueue_command(STR_M30 + fileName);
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_0));
}

void handleFileinfo() {
  /*String fileName = "";
  if (server.args() > 0) {
    fileName = server.arg(0);
    urldecode(fileName);
  }
  if (fileName == "") {
    enqueue_command(FPSTR(STR_M36));
  } else {
    enqueue_command(STR_M36 + fileName);
  }
  MYSERIAL.setTimeout(5000);
  serialData = MYSERIAL.readStringUntil('\n');
  if (serialData.startsWith(FPSTR(STR_OK))) serialData = MYSERIAL.readStringUntil('\n');
  lastResponse = String(serialData);
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), lastResponse);*/
}

void handleMkdir() {
  String dirName = "";
  if (server.args() < 2 || server.arg(1) != FPSTR(STR_TRUE)) { // 2 ARGS FOR COMPATMODE OR NOPE
    server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_UNSUPPORTED_OPERATION));
    return;
  }
  dirName = server.arg(0);
  urldecode(dirName);
  if (dirName == "") {
    server.send(500, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_500_NO_DIR_NAME_PROVIDED));
    return;
  }
  enqueue_command(STR_M32 + dirName);
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_0));
}





void handleUnsupported() {
  server.send(200, FPSTR(STR_MIME_APPLICATION_JSON), FPSTR(STR_JSON_ERR_UNSUPPORTED_OPERATION));
}

bool isLoggedIn(IPAddress clientIP) {
  for (int i = 0; i < loggedInClientsNum; i++) {
    if (sessions[i] == clientIP) {
      return true;
    }
  }
  return false;
}

bool logIn(IPAddress clientIP) {
  if (loggedInClientsNum < MAX_LOGGED_IN_CLIENTS) {
    sessions[loggedInClientsNum] = clientIP;
    loggedInClientsNum++;
    return true;
  }
  return false;
}

void logOut(IPAddress clientIP) {
  for (int i = 0; i < loggedInClientsNum; i++) {
    if (sessions[i] == clientIP) {
      for (int e = loggedInClientsNum - 1; e > i; e--) {
        // SHIFT ARRAY
        memcpy(&sessions[e - 1], &sessions[e], sizeof(IPAddress));
      }
      loggedInClientsNum--;
      return;
    }
  }
}


void urldecode(String &input) { // LAL ^_^
  input.replace("%0A", String('\n'));
  input.replace("%20", " ");
  input.replace("+", " ");
  input.replace("%21", "!");
  input.replace("%22", "\"");
  input.replace("%23", "#");
  input.replace("%24", "$");
  input.replace("%25", "%");
  input.replace("%26", "&");
  input.replace("%27", "\'");
  input.replace("%28", "(");
  input.replace("%29", ")");
  input.replace("%30", "*");
  input.replace("%31", "+");
  input.replace("%2C", ",");
  input.replace("%2E", ".");
  input.replace("%2F", "/");
  input.replace("%2C", ",");
  input.replace("%3A", ":");
  input.replace("%3A", ";");
  input.replace("%3C", "<");
  input.replace("%3D", "=");
  input.replace("%3E", ">");
  input.replace("%3F", "?");
  input.replace("%40", "@");
  input.replace("%5B", "[");
  input.replace("%5C", "\\");
  input.replace("%5D", "]");
  input.replace("%5E", "^");
  input.replace("%5F", "-");
  input.replace("%60", "`");
}

#endif
