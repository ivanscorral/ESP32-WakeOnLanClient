#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebServer.h>

#define SERVER_PORT 1144
#define WOL_PORT 9
#define WOL_REPEAT 5
#define LAB_MAC "00:00:00:00:00:00"

#include <WakeOnLan.h>

WiFiUDP UDP;
WakeOnLan WOL(UDP);
WebServer server(1144);

const char *ssid = "";
const char *password = "";

IPAddress ipAddress(192, 168, 0, 179);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void wakeServer()
{

  WOL.sendMagicPacket(LAB_MAC); // Send Wake On Lan packet with the above MAC address. Default to port 9.
                                   // WOL.sendMagicPacket(MACAddress, 7); // Change the port number
  server.send(200, "text/plain", "Wake On Lan packet sent.");
}

void setup()
{
  WOL.setRepeat(5, 200); // Optional, repeat the packet three times with 100ms between. WARNING delay() is used between send packet function.
  if (!WiFi.config(ipAddress, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/wake", wakeServer);
  server.begin();
  Serial.print("HTTP server started on port ");
  Serial.println(SERVER_PORT);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask()); // Optional  => To calculate the broadcast address, otherwise 255.255.255.255 is used (which is denied in some networks).
}

void loop()
{
}