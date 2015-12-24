#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#include "wifiConfig.h"
/*
 * Define your SSID and WiFi password at [wifiConfig.h] using [TemplateWifiConfig.h]
 * 
 */

/*
 * v0.2 2015 Dec. 25
 *   - correct typo
 * v0.1 2015 Dec. 19
 *   can echo back
 *   - add loop()
 *   - add setup()
 *   - add Serial_setup()
 *   - add WiFi_setup()
 *   - add wifiConfig.h
 */

static WiFiUDP wifiUdp; 
#if 0
static const char *kRemoteIpadr = "192.168.10.4";
static const int kRmoteUdpPort = 9000;
#endif

static char receivedBuffer[255];

static void WiFi_setup()
{
  static const int kLocalPort = 7000;

  WiFi.begin(kWifiSsid, kWifiPass);
  while( WiFi.status() != WL_CONNECTED) {
    delay(500);  
  }  
  wifiUdp.begin(kLocalPort);
}

static void Serial_setup()
{
  Serial.begin(115200);
  Serial.println(""); // to separate line  
}

static void WiFi_printConnectionInfo()
{
  Serial.println("\nLocal IP=");
  Serial.println(WiFi.localIP());
  Serial.println("\nLocal port=");
  Serial.println(wifiUdp.localPort());
}

void setup() {
  Serial_setup();
  WiFi_setup();
  WiFi_printConnectionInfo();
}

void loop() 
{
  int rcvdSize = wifiUdp.parsePacket();
  if (rcvdSize == 0) {
    delay(100);
    return;
  }

  Serial.println("received");

  int len = wifiUdp.read(receivedBuffer, 255);
  if (len == 0) {
    return;
  }
  receivedBuffer[len] = 0x00;
  
  wifiUdp.beginPacket( wifiUdp.remoteIP(), wifiUdp.remotePort() );
  wifiUdp.write(receivedBuffer);
  wifiUdp.endPacket();

}
