#include "State.h"
#include "StateMachine.h"

void StateMenu::exit(StateMachine* sm) {
  Serial.println("MENU EXIT");
}
void StateMenu::enter(StateMachine* sm) {
  Serial.println("MENU ENTER");
}
void StateMenu::run(StateMachine* sm) {
  if (sm->getPressedA()) sm->transTo(StateMachine::ID::SETUP);
  if (sm->getPressedB()) sm->transTo(StateMachine::ID::MONITOR);
}