#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebServer.h>

#include "secrets.h"

#define SERVER_PORT 1144 
#define WOL_REPEAT 3

#include <WakeOnLan.h>

WiFiUDP UDP;
WakeOnLan WOL(UDP);
WebServer server(SERVER_PORT);


IPAddress ipAddress(192, 168, 0, 150);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void wakeServer()
{
  Serial.println("Wake On Lan request received.");
  WOL.sendMagicPacket(LAB_MAC); // Send Wake On Lan packet with the above MAC address. Default to port 9.
                                   // WOL.sendMagicPacket(MACAddress, 7); // Change the port number
  Serial.println("Wake On Lan packet sent.");
  server.send(200, "text/plain", "Wake On Lan packet sent.");
}

void setup()
{
  Serial.begin(115200);

  if (!WiFi.config(ipAddress, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
  }
  Serial.println("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  WOL.setRepeat(WOL_REPEAT, 100); // Optional, repeat the packet three times with 100ms between. WARNING delay() is used between send packet function.

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/wake", wakeServer);
  server.onNotFound([]() {
    Serial.println("Not found");
    server.send(404, "text/plain", "Not found");
  });
  server.begin();
  Serial.print("HTTP server started on port ");
  Serial.println(SERVER_PORT);

 
  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask()); // Optional  => To calculate the broadcast address, otherwise 255.255.255.255 is used (which is denied in some networks).
}

void loop()
{
  if (5000 < millis())
  {
    server.handleClient();
  }
  
}