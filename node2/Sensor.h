#pragma once

class Sensor {
  uint8_t sensorPin;
  uint16_t cMin = 0u;
  uint16_t cMax = 0x0FFF;
  uint16_t raw = 0u;
  uint32_t filtered = 0ul;
  float clammped = 0.0f;
  float unclammped = 0.0f;
  uint32_t timer = 0ul;
  uint32_t duration = 0ul;
  float fast, slow;
  uint8_t power;

  void read() {
    raw = readBurst(10, 50, 50);
    filtered = ((filtered * 7) + static_cast<uint32_t>(raw)) / 8;
    uint16_t span = (cMax > cMin) ? (cMax - cMin) : 1;
    unclammped = static_cast<float>((int32_t)filtered - (int32_t)cMin) / static_cast<float>(span);
    clammped = constrain(unclammped, 0.0f, 1.0f);
    fast += 0.05f * ((float)raw - fast);
    slow += 0.0033f * ((float)raw - slow);
    // Serial.print(fast);
    // Serial.print(' ');
    // Serial.print(slow);
    // Serial.print(' ');
    // Serial.println(fabs(fast - slow));
    Serial.println(raw);
  }
  uint16_t readBurst(uint8_t count = 10, uint16_t settle = 50, uint8_t rate = 5) {
    uint32_t sum = 0;
    digitalWrite(power, HIGH);
    if (settle) delay(settle);
    for (uint8_t i = 0; i < count; i++) {
      sum += readSingle();
      if (rate) delay(rate);
    }
    digitalWrite(power, LOW);
    return static_cast<uint16_t>(sum / static_cast<uint32_t>(count));
  }
  uint16_t readSingle() {
    return (0x0FFF - analogRead(sensorPin));
  }
  void init() {
    filtered = readBurst(10, 50, 5);
    fast = slow = filtered;
    delay(100);
    for (uint8_t i = 0; i < 50; i++) {
      delay(100);
      read();
    }
  }

public:
  float getPercent5() {
    return (float)(int16_t(clammped * 200.0f + 0.5f)) * 0.5f;
  }
  inline void setMin(uint16_t m) {
    cMin = m;
  }
  inline void setMax(uint16_t m) {
    cMax = m;
  }
  void begin(uint8_t s, uint8_t p) {
    sensorPin = s;
    power = p;
    pinMode(sensorPin, INPUT);
    pinMode(power, OUTPUT);
    init();
    duration = 1ul;
    timer = millis();
  }
  void update() {
    if (millis() - timer >= 100ul) {
      timer += 100ul;
      read();
    }
  }
};