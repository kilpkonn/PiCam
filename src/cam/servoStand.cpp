//
// Created by tavo on 26.07.20.
//
#include <cmath>

#include "servoStand.h"

ServoStand::ServoStand() {
    isGpioInitalized = gpioInitialise();
    gpioSetMode(xAxisServoPin, PI_OUTPUT);
    gpioSetMode(yAxisServoPin, PI_OUTPUT);
    gpioSetPWMfrequency(xAxisServoPin, servoPwmFrequency);
    gpioSetPWMfrequency(yAxisServoPin, servoPwmFrequency);
}

#include "iostream"
void ServoStand::rotate(float x, float y) {
    unsigned xVal = minServoValue + static_cast<unsigned>((x + M_PIf32 / 2) / M_PI * (maxServoValue - minServoValue));
    unsigned yVal = minServoValue + static_cast<unsigned>((y + M_PIf32 / 2) / M_PI * (maxServoValue - minServoValue));
    gpioServo(xAxisServoPin, xVal);
    gpioServo(yAxisServoPin, yVal);
    std::cout << xVal << std::endl;
}

ServoStand::~ServoStand() {
    gpioTerminate();
}
