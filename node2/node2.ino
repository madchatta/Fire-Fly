#include "button.h"
#include <StateMachine.h>

Button btnA, btnB, btnC;
StateMachine machine;
StateMachine* active = nullptr;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  btnA.begin(39);
  btnB.begin(38);
  btnC.begin(37);
  machine.begin(36, 26);  //sensor pin,sensor power
  active = &machine;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (active)
    active->setPressed(btnA.pressed(), btnB.pressed(), btnC.pressed());
  machine.handle();
}
