// ***********************************************
// RFID Card Logger
// ***********************************************
// This Arduino Sketch makes the PODuino read RFID
// cards and log them on iotile.cloud
//
// PINOUT:
// RC522 MODULE    Poduino
// SDA             D9
// SCK             D52
// MOSI            D51
// MISO            D50
// IRQ             N/A
// GND             GND
// RST             D8
// 3.3V            3.3V
//
// Include the standard Arduino SPI library
#include <SPI.h>
// Include the RFID library
#include <RFID.h>
// Include IOTile Bridge Library
#include <IOTileBridgeMega.h>

//***************************************************
// Usual RFID-RC522 Definitions
// ----------------------------
// Define the DIO used for the SDA (SS) and RST (reset) pins.
#define SDA_DIO 9
#define RESET_DIO 8
// Create an instance of the RFID library
RFID RC522(SDA_DIO, RESET_DIO); 
//***************************************************

//***************************************************
// Usual IOTileBridge Definitions
// ------------------------------
// Pins connected to IOTile Device
#define kAttentionPin 39
#define kEventReceivedPin 3
// onEvent Callback
void onEventReceived(unsigned int event);
// Bridge instantiation
IOTileBridge bridge(kAttentionPin, onEventReceived);
// input 10 ==> Sensor Graph will copy to output 0x5001 
#define kIotileStreamId 10
//***************************************************

//***************************************************
// Specific definitions for this example

// Remember last Card detected
byte last_id[5]={0,0,0,0,0};
//***************************************************

void setup()
{ 
  // Start the PODuino Library
  bridge.begin();
  
  // Arduino Debug Terminal
  Serial.begin(9600);
  
  // Enable the SPI interface
  SPI.begin(); 
  
  // Initialise the RFID reader
  RC522.init();
}

void loop()
{
  // Has a card been detected?
  if (RC522.isCard())
  {
    bool new_card_detected = false;
    
    // read RFID
    RC522.readCardSerial();
    
    for(int i=0;i<5;i++)
    {
      if (RC522.serNum[i] != last_id[i])
      {
        new_card_detected = true;
        break;
      }
    }

    if (new_card_detected)
    {
      Serial.println("New Card detected:");
      for(int i=0;i<5;i++)
      {
        // Serial.print(RC522.serNum[i],DEC);
        Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
        if (i < 4) Serial.print("-");

        last_id[i] = RC522.serNum[i];
      }
      Serial.println();

      // Send ID to the cloud
      // We will only send 3 bytes
      unsigned long value = last_id[2];
      value = (value << 8) + last_id[3];
      value = (value << 8) + last_id[4];
      Serial.println(value, HEX);
      bridge.sendEvent(kIotileStreamId, value);
      
      Serial.println();
    }
  }
  bridge.checkReceivedEvents();
  delay(100);
}

void onEventReceived(unsigned int event)
{
  // Assume the event is the value to set count
  if ( event == 0 )
  {
    Serial.println("Event 0 Received");
  }
}
