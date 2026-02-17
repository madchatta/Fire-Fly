#pragma once
#include <Arduino.h>

constexpr uint32_t MASK = 0x3FFFFFFF;

class Button {
  uint8_t pin;
  uint32_t count;
public:
  bool begin(int16_t);
  bool pressed();
};