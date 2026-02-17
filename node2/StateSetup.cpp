#include "State.h"
#include "StateMachine.h"

void StateSetup::exit(StateMachine* sm) {
  Serial.println("SETUP EXIT");
};
void StateSetup::enter(StateMachine* sm) {
  Serial.println("SETUP ENTER");
  sm->loadConfig();
  sm->setCMin();
  sm->setCMax();
  Serial.print(sm->getName());
  Serial.print(' ');
  Serial.print(sm->getCMin());
  Serial.print(' ');
  Serial.println(sm->getCMax());
}
void StateSetup::run(StateMachine* sm) {
  // if (sm->getPressedA()) sm->transTo(StateMachine::ID::MONITOR);
  // if (sm->getPressedB()) sm->transTo(StateMachine::ID::MONITOR);
  // if (sm->getPressedC()) sm->transTo(StateMachine::ID::MONITOR);
  sm->transTo(StateMachine::ID::MONITOR);
}
