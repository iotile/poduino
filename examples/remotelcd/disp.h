#ifndef disp_H
#define disp_H

#include <Arduino.h>

#define ASCII_END 24
#define ASCII_CARRIAGE_RETURN 13

const char CUSTOM_0[8]{
  B00010,
  B00010,
  B00010,
  B00010,
  B00010,
  B00010,
  B00010,
  B00010,
};

const char CUSTOM_1[8]{
  B11111,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B11111
};

const char CUSTOM_2[8]{
  B11111,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B11111
};

const char CUSTOM_3[8]{
  B10000,
  B10000,
  B10000,
  B10000,
  B10010,
  B10010,
  B10010,
  B11111
};

const char CUSTOM_4[8]{
  B11111,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001
};

const char CUSTOM_5[8]{
  B11111,
  B00001,
  B00001,
  B00001,
  B00011,
  B00010,
  B00010,
  B00010,
};

const char CUSTOM_6[8]{
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
};

const char CUSTOM_7[8]{
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};

class disp{
  public:
    disp();
    ~disp();
    void print(char c);
    void begin();
};

#endif
