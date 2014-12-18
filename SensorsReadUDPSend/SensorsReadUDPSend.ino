/*
  
  Based on IoTKit Client
  
  Written 2014 by Adrian McEwen, modified by George Profenza
  
  This code is in the public domain
  
*/
#include <SPI.h>         
#include <WiFi.h>
#include <WiFiUdp.h>
//setup sensor pins
const int pinTemp  = A0;
const int pinSound = A1;
const int pinLight = A2;
//setup values
int loudness = 0;
int light = 0;
float temperature;
int B=3975;                  // B value of the thermistor
float resistance;

const int PACKET_BUFFER_SIZE = 255;
char packetBuffer[PACKET_BUFFER_SIZE]; //buffer to hold incoming and outgoing packets 
WiFiUDP Udp;//UDP socket

void setup()  {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  Udp.begin(1234);//listen to UDP port
}

void loop(){    
  int val = analogRead(pinTemp);                               // get analog value
  loudness = analogRead(pinSound);
  light    = analogRead(pinLight);
  
  resistance=(float)(1023-val)*10000/val;                      // get resistance
  temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;     // calc temperature
  
  Serial.print("temp:");
  Serial.print(temperature);
  Serial.print("\tsound:");
  Serial.print(loudness);
  Serial.print("\tlight:");
  Serial.println(light);
  
  sprintf(packetBuffer, "%f,%i,%i\n", temperature,loudness,light);
  sendPacket(packetBuffer);
  
  delay(40);//25 updates per second
}
unsigned long sendPacket(char* aPacket)
{
  IPAddress agent(127,0,0,1);
  Udp.beginPacket(agent, 12345);
  
  Serial.println("Sending packet...");
  Serial.println(aPacket);
  Udp.write((byte*)aPacket, strlen(aPacket));
  Udp.endPacket(); 
}
