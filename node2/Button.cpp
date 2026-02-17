#pragma once
#include "button.h"

bool Button::begin(int16_t p = -1) {
  if (p < 0) return 0;
  pin = static_cast<uint8_t>(p);
  pinMode(pin, INPUT_PULLUP);
  count = 0;
  return 1;
}
bool Button::pressed() {
  count <<= 1;
  if (!digitalRead(pin)) count |= 1;
  if (count == MASK) return 1;
  return 0;
}
