//
// Created by tavo on 26.07.20.
//

#include "servoStand.h"

ServoStand::ServoStand() {
    isGpioInitalized = gpioInitialise();
}
