#pragma once
#include <Arduino.h>
// #include <cstddef>  // offsetof
// #include <cstring>  // strncpy, strcmp
// #include <cstdlib>  // strtoul

#include "State.h"
#include "Sensor.h"

class StateMachine {
public:
  enum class ID : uint8_t { SETUP = 0,
                            MONITOR,
                            FILL,
                            MENU,
                            EDIT_CMIN,
                            EDIT_CMAX,
                            EDIT_NAME,
                            CONFIRM };

  void begin(uint8_t s, uint8_t p) {
    setState(&setup);
    sensor.begin(s, p);
  }

  void handle() {
    sensor.update();
    if (current) current->run(this);
  }

  void transTo(ID id) {
    switch (id) {
      case ID::SETUP: setState(&setup); break;
      case ID::MONITOR: setState(&monitor); break;
      case ID::FILL: setState(&fill); break;
      case ID::MENU: setState(&menu); break;
    }
  }
  void loadConfig() {
    config();
  }
  const char* getName() const {
    return setting.name;
  }
  uint16_t getCMin() const {
    return setting.cMin;
  }
  uint16_t getCMax() const {
    return setting.cMax;
  }
  float getPercent() {
    return sensor.getPercent5();
  }
  void setPressed(bool a, bool b, bool c) {
    btn.a = a;
    btn.b = b;
    btn.c = c;
  }
  bool getPressedA() const {
    return btn.a;
  }
  bool getPressedB() const {
    return btn.b;
  }
  bool getPressedC() const {
    return btn.c;
  }
  void setCMin(int16_t m = -1) {
    if (m >= 0) setting.cMin = (uint16_t)m;
    sensor.setMin(setting.cMin);
  }
  void setCMax(int16_t m = -1) {
    if (m >= 0) setting.cMax = (uint16_t)m;
    sensor.setMax(setting.cMax);
  }

private:
  struct Button {
    bool a;
    bool b;
    bool c;
  };

  struct Setting {
    char name[20] = { 0 };
    uint16_t cMin = 0;
    uint16_t cMax = 0;
  };

  enum class FieldType : uint8_t {
    STR_T = 0,
    UINT16_T,
  };

  struct ConfigField {
    const char* key;
    size_t offset;
    FieldType type;
  };

  static constexpr ConfigField configFields[] = {
    { "name", offsetof(Setting, name), FieldType::STR_T },
    { "min", offsetof(Setting, cMin), FieldType::UINT16_T },
    { "max", offsetof(Setting, cMax), FieldType::UINT16_T },
  };

  uint32_t updateTimer = 0, duration = 0;
  Sensor sensor;
  Button btn;

  Setting setting;

  StateSetup setup;
  StateMonitor monitor;
  StateFill fill;
  StateMenu menu;
  State* current = nullptr;

  void setState(State* next) {
    if (!next) return;
    if (current) current->exit(this);
    current = next;
    current->enter(this);
  }

  void config() {

    char file[]{
#include "config.txt"
    };
    char line[48] = {};
    const char* lineStart = file;
    const char* lineEnd;

    while (lineStart && *lineStart) {
      lineEnd = strchr(lineStart, '\n');

      size_t len = lineEnd ? (size_t)(lineEnd - lineStart) : strlen(lineStart);

      if (len >= sizeof(line)) {
        len = sizeof(line) - 1;
      }

      memcpy(line, lineStart, len);
      line[len] = '\0';

      if (line[0] != '#' && line[0] != '\0') {
        char* comma = strchr(line, ',');
        if (!comma) return;
        *comma = '\0';
        applyConfigKV(line, comma + 1);
      }

      lineStart = lineEnd ? lineEnd + 1 : NULL;
    }
  }

  void applyConfigKV(const char* key, const char* value) {
    for (const auto& f : configFields) {
      if (strcmp(key, f.key) != 0) continue;

      uint8_t* base = reinterpret_cast<uint8_t*>(&setting);
      void* dst = base + f.offset;

      switch (f.type) {
        case FieldType::STR_T:
          {
            char* out = reinterpret_cast<char*>(dst);
            strncpy(out, value, sizeof(setting.name) - 1);
            out[sizeof(setting.name) - 1] = '\0';
            break;
          }

        case FieldType::UINT16_T:
          {
            unsigned long v = strtoul(value, nullptr, 10);
            if (v > 65535UL) v = 65535UL;
            *reinterpret_cast<uint16_t*>(dst) = static_cast<uint16_t>(v);
            break;
          }
      }
    }
  }
};

// Definition required for static constexpr array in some Arduino toolchains:***
constexpr StateMachine::ConfigField StateMachine::configFields[];
