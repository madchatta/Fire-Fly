#pragma once
#include <Arduino.h>

class StateMachine;


class State {
public:
  virtual ~State() = default;
  virtual void exit(StateMachine* sm) = 0;
  virtual void enter(StateMachine* sm) = 0;
  virtual void run(StateMachine* sm) = 0;
  static uint32_t timer;
};



class StateSetup : public State {
public:
  void exit(StateMachine* sm) override;
  void enter(StateMachine* sm) override;
  void run(StateMachine* sm) override;
};

class StateMonitor : public State {
public:
  void exit(StateMachine* sm) override;
  void enter(StateMachine* sm) override;
  void run(StateMachine* sm) override;
};

class StateFill : public State {
public:
  void exit(StateMachine* sm) override;
  void enter(StateMachine* sm) override;
  void run(StateMachine* sm) override;
};
class StateMenu : public State {
public:
  void exit(StateMachine* sm) override;
  void enter(StateMachine* sm) override;
  void run(StateMachine* sm) override;
};
