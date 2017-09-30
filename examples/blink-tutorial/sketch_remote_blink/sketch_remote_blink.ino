#include <IOTileBridgeMega.h>

#define kAttentionPin 39
void onEventReceived(unsigned int event);
IOTileBridge bridge(kAttentionPin, onEventReceived);

int active=0;

void setup() {
  // Initialize IOTile Bridge
  bridge.begin();
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (active) {
    // When active, enable LED to blink
    int j;
    for (j=0; j<10; ++j)
    {
      bridge.checkReceivedEvents(); // Check for events
      delay(100);
    }
    digitalWrite(LED_BUILTIN, HIGH);
    for (j=0; j<10; ++j)
    {
      bridge.checkReceivedEvents(); // Check for events
      delay(100);
    }
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    // If not enabled, make sure LED is OFF
    digitalWrite(LED_BUILTIN, LOW);
    bridge.checkReceivedEvents(); // Check for events
    delay(100);
  }
}

void onEventReceived(unsigned int event)
{
  if (event) {
    active = 1; // Enable Blinking
  } else {
    active = 0; // Disable Blinking
  }
  // Log Event on IOTile Output 10
  bridge.sendEvent(10, event);
}
