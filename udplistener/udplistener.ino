// Demonstrates usage of the new udpServer feature.
// You can register the same function to multiple ports,
// and multiple functions to the same port.
//
// 2013-4-7 Brian Lee <cybexsoft@hotmail.com>
//
// License: GPLv2

#include <EtherCard.h>
#include <IPAddress.h>

#define STATIC 0  // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
// Ethernet interface IP address
static byte myip[] = { 192,168,0,200 };
// Gateway IP address
static byte gwip[] = { 192,168,0,1 };
#endif

// Ethernet MAC address - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // TCP/IP send and receive buffer

void ledLightUp()
{
  digitalWrite(7, HIGH);
  delay(500);
  digitalWrite(7, LOW);
}

// Callback that prints received packets to the serial port
void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len){
  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);

  Serial.print("dest_port: ");
  Serial.println(dest_port);
  Serial.print("src_port: ");
  Serial.println(src_port);


  Serial.print("src_ip: ");
  ether.printIp(src_ip);
  Serial.println("\ndata: ");
  Serial.println(data);
  ledLightUp();
}

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

  // Register udpSerialPrint() to port 1337
  ether.udpServerListenOnPort(&udpSerialPrint, 42);

}

void loop(){
  // This must be called for ethercard functions to work.
  ether.packetLoop(ether.packetReceive());
}

