#include "State.h"
#include "StateMachine.h"

void StateFill::exit(StateMachine* sm) {
  Serial.println("FILL EXIT");
}
void StateFill::enter(StateMachine* sm) {
  Serial.println("FILL ENTER");
}
void StateFill::run(StateMachine* sm){}