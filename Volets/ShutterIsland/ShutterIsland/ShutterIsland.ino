/*
  This a simple example of the aREST Library for Arduino (Uno/Mega/Due/Teensy)
  using the Ethernet library (for example to be used with the Ethernet shield).
  See the README file for more details.

  Written in 2014 by Marco Schwartz under a GPL license.
*/

// Libraries
//#include <SPI.h>
#include <Ethernet.h>
#include <aREST.h>
//#include <avr/wdt.h>

// Enter a MAC address for your controller below.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// Duration of the button push
int pushDelay = 500;

// IP address in case DHCP fails
IPAddress ip(192,168,1,200);

// Ethernet server
EthernetServer server(80);

// Create aREST instance
aREST rest = aREST();

void setup(void)
{

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  
  pinMode(3, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
 
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
  pinMode(A0, OUTPUT); // 18
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT); // 23

  digitalWrite(0,1); 
  digitalWrite(1,1); 
  digitalWrite(2,1); 

  digitalWrite(3,1); 
  digitalWrite(8,1); 
  digitalWrite(9,1); 
  
  digitalWrite(5,1); 
  digitalWrite(6,1); 
  digitalWrite(7,1); 

  digitalWrite(A0,1); 
  digitalWrite(A1,1); 
  digitalWrite(A2,1); 

  digitalWrite(A3,1); 
  digitalWrite(A4,1); 
  digitalWrite(A5,1); 
  
  // Start Serial
//  Serial.begin(9600);
  //Serial.println("Starting...");
  // Function to be exposed
  rest.function("shutterControl",shutterControl);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("001");
  rest.set_name("ShutterController");

  // Start the Ethernet connection and the server
  //Ethernet.begin(mac, ip);
    // Start the Ethernet connection and the server
  if (Ethernet.begin(mac) == 0) {
    //Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  server.begin();
 // Serial.print("server is at ");
 // Serial.println(Ethernet.localIP());

  // Start watchdog
  //wdt_enable(WDTO_4S);
}

void loop() {

  // listen for incoming clients
  EthernetClient client = server.available();
  rest.handle(client);
  //wdt_reset();

}

int shutterControl(String command) {
//Serial.println(command);
  // Split values from the string
  int shutterId = getValue(command, ':', 0);
  int shutterPosition = getValue(command, ':', 1);
//  Serial.print("Shutter ID: ");
 // Serial.println(shutterId);

//  Serial.print("Shutter Position: ");
 // Serial.println(shutterPosition);
 
  if(shutterId != 1 && shutterPosition != -1)
  {
      digitalWrite(shutterId,0);  
      delay(pushDelay);
      digitalWrite(shutterId,1);   
  }
  return 1;
}

int getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]).toInt() : -1;
}
