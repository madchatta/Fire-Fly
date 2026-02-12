#include "Arduino.h"

constexpr uint8_t BUTTON_A = 39;
constexpr uint8_t BUTTON_B = 38;
constexpr uint8_t BUTTON_C = 37;


constexpr uint32_t MASK = 0x3FFFFFFF;
struct Button {
  bool begin(int16_t b) {
    if (b < 0) return false;
    button = static_cast<uint8_t>(b);
    pinMode(button, INPUT_PULLUP);
    count = 0;
    return true;
  }
  bool pressed() {
    count <<= 1;
    if (!digitalRead(button)) count |= 1;
    if (count == MASK) return true;
    return false;
  }
private:
  uint8_t button;
  uint32_t count;
};
enum class EVENT {
  pressedA = 0,
  pressedB,
  pressedC,
  NONE
};
constexpr size_t EVENT_QUE_SIZE = 10;
struct EventQue {
  EVENT buffer[EVENT_QUE_SIZE];
  uint8_t head, tail, count;
  void begin() {
    head = tail = count = 0;
  }
  bool push(EVENT e) {
    if (count == EVENT_QUE_SIZE) return false;
    buffer[head] = e;
    head = static_cast<uint8_t>((head + 1) % EVENT_QUE_SIZE);
    count++;
    return true;
  }
  EVENT pop() {
    if (count == 0) return EVENT::NONE;
    EVENT event = buffer[tail];
    tail = static_cast<uint8_t>((tail + 1) % EVENT_QUE_SIZE);
    count--;
    return event;
  }
  bool registeredEvent() {
    return (count > 0);
  }
};

struct Transistions {
  EVENT button;
  char *description;
} transistions[]{
  { EVENT::pressedA, "Button A pressed" },
  { EVENT::pressedB, "Button B pressed" },
  { EVENT::pressedC, "Button C pressed" }
};








Button btnA;
Button btnB;
Button btnC;
EventQue queue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  btnA.begin(BUTTON_A);
  btnB.begin(BUTTON_B);
  btnC.begin(BUTTON_C);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (queue.registeredEvent()) {
    // Serial.println(static_cast<uint8_t>(queue.pop()));
    transistion_handle(queue.pop());
  }

  
  if (btnA.pressed()) queue.push(EVENT::pressedA);
  if (btnB.pressed()) queue.push(EVENT::pressedB);
  if (btnC.pressed()) queue.push(EVENT::pressedC);
}

void transistion_handle(EVENT event) {
  for (Transistions &t : transistions) {
    if (t.button == event) Serial.println(t.description);
  }
}
