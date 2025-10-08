#include <Arduino.h>
#include "html.h"
#include <USB.h>
#include <USBHIDKeyboard.h>
#include <USBHIDMouse.h>
#include <interpreter.h>
#include <debug.h>
#include <WebServer.h>
#include <WiFi.h>

USBHIDKeyboard Keyboard;
USBHIDMouse Mouse;

const char ssid[] = "ESPGoose";
const char passphrase[] = "password";
IPAddress local_IP(10, 0, 0, 2);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

bool isNumber(const String &s)
{
  if (s.length() == 0)
    return false;
  for (unsigned int i = 0; i < s.length(); i++)
  {
    if (i == 0 && s[i] == '-')
      continue;
    if (!isDigit(s[i]))
    {
      return false;
    }
  }
  return true;
}

void rootHandler()
{
  SERVER_LOGD("Serving main page");
  server.send(200, "text/html", mainPage);
}

void scriptHandler()
{
  SERVER_LOGD("Received /uploadScript request");

  if (server.hasArg("script") && server.hasArg("lineCount") && isNumber(server.arg("lineCount")))
  {
    String script = server.arg("script");
    int maxLines = server.arg("lineCount").toInt();

    SERVER_LOGD("lineCount: " + String(maxLines));
    SERVER_LOGD("script: " + script);

    String parsedScript[maxLines];
    int parsedLines = splitScript(script, parsedScript, maxLines);

    runScript(parsedScript, maxLines);
  }

  server.send(200, "text/html", "Received");
}

void mouseMoveHandler()
{
  SERVER_LOGD("Received /controlMouse request");

  if (server.hasArg("dx") && server.hasArg("dy") && isNumber(server.arg("dx")) && isNumber(server.arg("dy")))
  {
    int dx = server.arg("dx").toInt();
    int dy = server.arg("dy").toInt();

    SERVER_LOGD("dx: " + String(dx) + ", dy: " + String(dy));
    
    Mouse.move(dx, dy);
  }
  server.send(200, "text/html", "Received");
}

void mouseLeftClickHandler()
{
  SERVER_LOGD("Received /mouseLeftClick request");
  Mouse.click(MOUSE_LEFT);
}

void mouseRightClickHandler()
{
  SERVER_LOGD("Received /mouseRightClick request");
  Mouse.click(MOUSE_RIGHT);
}
void setup()
{

  Serial.begin(115200);

  if (!WiFi.softAPConfig(local_IP, gateway, subnet))
  {
    SERVER_LOGE("AP Config Failed!");
  }

  WiFi.softAP(ssid, passphrase);

  USB.begin();
  Keyboard.begin();
  Mouse.begin();

  server.on("/", HTTP_GET, rootHandler);
  server.on("/uploadScript", HTTP_POST, scriptHandler);
  server.on("/controlMouse", HTTP_POST, mouseMoveHandler);
  server.on("/mouseLeftClick", HTTP_POST, mouseLeftClickHandler);
  server.on("/mouseRightClick", HTTP_POST, mouseRightClickHandler);
  server.begin();
}

void loop()
{
  server.handleClient();
}
