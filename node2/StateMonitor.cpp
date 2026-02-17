#include "State.h"
#include "StateMachine.h"

void StateMonitor::exit(StateMachine* sm) {
  Serial.println("MONITOR EXIT");
}
void StateMonitor::enter(StateMachine* sm) {
  Serial.println("MONITOR ENTER");
  timer = millis();
}
void StateMonitor::run(StateMachine* sm) {
  if (millis() - timer >= 5000) {
    timer += 5000;
    Serial.print(sm->getName());
    Serial.print(' ');
    Serial.println(sm->getPercent());
  }
  if (sm->getPressedA()) sm->transTo(StateMachine::ID::MENU);
}