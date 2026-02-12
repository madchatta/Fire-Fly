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
enum class STATEID {
  setup,
  monitor,
  filling,
  send,

};
struct StateMachine;


struct State {
  StateMachine *machine;
  void bind(StateMachine &sm) {
    machine = &sm;
  }
  virtual ~State() = default;
  virtual void enter() = 0;
  virtual void run(float) = 0;
  virtual void exit() = 0;
};

struct StateSetup : State {
  void enter() override {
    Serial.println("Enter Setup state");
  }
  void run(float f) override {
    // Serial.println("Setup state run");
  }
  void exit() override {
    Serial.println("Exit Setup stat");
  }
};
struct StateMonitor : State {
  void enter() override {
    Serial.println("Enter Monitor state");
    // Serial.println(machine->raw);
  }
  void run(float f) override {
    // Serial.println("Monitor state run");
    
  }
  void exit() override {
    Serial.println("Exit Monitor state");
  }
};
struct StateFill : State {
  void enter() override {
    Serial.println("Enter Fill state");
    // Serial.println(machine->raw);
  }
  void run(float f) override {
    // Serial.println("Fill state run");
    
  }
  void exit() override {
    Serial.println("Exit Fill state");
  }
};

struct StateMachine {
  float raw;
  State *current = nullptr;
  StateSetup setup;
  StateMonitor monitor;
  StateFill fill;

  void begin() {
    setup.bind(*this);
    monitor.bind(*this);
    fill.bind(*this);
    setState(setup);
  }
  void setState(State &state) {
    if (current) current->exit();
    current = &state;
    if (current) current->enter();
    Serial.println(raw);
  }
  void run(float r) {
    raw = r;
    if (current) current->run(raw);
  }
  float getRaw() {
    return raw;
  };
};




struct Transistions {
  EVENT button;
  const char *description;
} transistions[]{
  { EVENT::pressedA, "Button A pressed" },
  { EVENT::pressedB, "Button B pressed" },
  { EVENT::pressedC, "Button C pressed" }
};








Button btnA;
Button btnB;
Button btnC;
EventQue queue;
StateMachine machine;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  machine.begin();
  queue.begin();
  btnA.begin(BUTTON_A);
  btnB.begin(BUTTON_B);
  btnC.begin(BUTTON_C);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (btnA.pressed()) queue.push(EVENT::pressedA);
  if (btnB.pressed()) queue.push(EVENT::pressedB);
  if (btnC.pressed()) queue.push(EVENT::pressedC);

  if (queue.registeredEvent()) {
    // Serial.println(static_cast<uint8_t>(queue.pop()));
    // transistion_handle(queue.pop());
    EVENT event = queue.pop();
    transistion_handle(event);
    switch (event) {
      case EVENT::pressedA: machine.setState(machine.setup); break;
      case EVENT::pressedB: machine.setState(machine.monitor); break;
      case EVENT::pressedC: machine.setState(machine.fill); break;
    }
  }
  static float f = 0.0;
  machine.run(f);
  f += 0.01;
}

void transistion_handle(EVENT event) {
  for (Transistions &t : transistions) {
    // if (t.button == event) Serial.println(t.description);
  }
}
