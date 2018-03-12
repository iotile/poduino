#include "disp.h"
#include <IOTileBridgeMega.h>

#define kAttentionPin 39

void onEventReceived(unsigned int event);

IOTileBridge bridge(kAttentionPin, onEventReceived);
disp d;

void onEventReceived(unsigned int event){
  d.print(event);
}

void setup() {
  d.begin();
  bridge.begin();
}

void loop() {
  bridge.checkReceivedEvents();
  delay(100);
}

