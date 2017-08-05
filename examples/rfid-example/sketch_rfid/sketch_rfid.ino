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

// Define the DIO used for the SDA (SS) and RST (reset) pins.
#define SDA_DIO 9
#define RESET_DIO 8
// Create an instance of the RFID library
RFID RC522(SDA_DIO, RESET_DIO); 

void setup()
{ 
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
    // If so then get its serial number
    RC522.readCardSerial();
    Serial.println("Card detected:");
    for(int i=0;i<5;i++)
    {
    Serial.print(RC522.serNum[i],DEC);
    Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
    }
    Serial.println();
    Serial.println();
  }
  delay(1000);
}
