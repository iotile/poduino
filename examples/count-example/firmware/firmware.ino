// ***********************************************
// Up/Down Counter Example
// ***********************************************
// This Arduino Sketch makes the PODuino count up
// or down depending on which button is clicked

#include <LiquidCrystal.h>
#include <IOTileBridgeMega.h>

#define kAttentionPin 39
#define kEventReceivedPin 3

void onEventReceived(unsigned int event);

unsigned int i = 0;
IOTileBridge bridge(kAttentionPin, onEventReceived);
 
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnUP     1
#define btnDOWN   2
#define btnNONE   5

int count = 0;
#define kIotileStreamId 10
 
// read the buttons
int read_LCD_buttons()
{
  // read the value from the sensor
  adc_key_in = analogRead(0);      
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  if (adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN; 
  return btnNONE;  // when all others fail, return this...
}

void displayInfo(const char* msg)
{
  lcd.setCursor(0,1);
  lcd.print(msg);
}

void displayCount(int c)
{
  lcd.setCursor(8,1);    
  lcd.print(count); 
  lcd.print(" ");
}

void updateCount(int value)
{
  count = value;
  displayCount(count);

  // This is where we send data to the cloud
  // (assume correct sensor graph)
  bridge.sendEvent(kIotileStreamId, count);
}

void countUpDown(int btn)
{
  if (btn == btnUP) 
  {
    displayInfo("UP  ");
    count++;
    if ( count == 100 ) 
    {
      count = 0;
    }
  } 
  else 
  {
    displayInfo("DOWN");
    count--;
    if ( count == 0 ) 
    {
      count = 100;
    }
  }

  updateCount(count);

  delay(400);
}
 
void setup()
{
  // Start the PODuino Library
  bridge.begin();

  // Start the LCD Library
  lcd.begin(16, 2);

  // Add a simple Welcome Screen
  lcd.setCursor(0,0);
  lcd.print("Poduino Counter");
  displayInfo("v0.1");
  
  updateCount(0);
}
  
void loop()
{
  lcd.setCursor(0,1); 
  lcd_key = read_LCD_buttons();
 
  if ( lcd_key != btnNONE ) {
    countUpDown(lcd_key);
  }

  bridge.checkReceivedEvents();
  delay(10);
}

void onEventReceived(unsigned int event)
{
  // Assume the event is the value to set count
  if ( event == 0 )
  {
    displayInfo("v0.1");
    updateCount(0);
  }
}
