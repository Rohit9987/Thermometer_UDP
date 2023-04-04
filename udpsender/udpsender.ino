// Demonstrates usage of the new udpServer feature.
// You can register the same function to multiple ports,
// and multiple functions to the same port.
//
// 2013-4-7 Brian Lee <cybexsoft@hotmail.com>
//
// License: GPLv2

// arduino 1
// udp sender or client
#include <EtherCard.h>
#include <IPAddress.h>

#include <OneWire.h>
#include <DallasTemperature.h>


// udp sender
#define STATIC 0  // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
// Ethernet interface IP address
static byte myip[] = { 192,168,0,120 };
// Gateway IP address
static byte gwip[] = { 192,168,0,1 };
#endif

// Ethernet MAC address - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x32 };

byte Ethernet::buffer[500]; // TCP/IP send and receive buffer

char textToSend[] = "0.000";
const int dstPort PROGMEM = 42;
const int srcPort PROGMEM = 42;
static byte destip[] = {192, 168, 0, 100};

float temperature = 0;

// ds18b20
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

DeviceAddress insideThermometer;


void setup(){
  Serial.begin(57600);
  Serial.println(F("\n[backSoon]"));

  // Change 'SS' to your Slave Select pin if you aren't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
#if STATIC
  ether.staticSetup(myip, gwip);
#else
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));
#endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  // temperature sensor
  sensors.begin();
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
}

void loop(){
  sensors.requestTemperatures();
  temperature = sensors.getTempC(insideThermometer);
  Serial.print("temperature: ");
  Serial.println(temperature);
  
  dtostrf(temperature, 4, 2, textToSend);
  
  
  ether.sendUdp(textToSend, sizeof(textToSend), srcPort, destip, dstPort);
  delay(2000);
  //Serial.println("button pressed");
}
