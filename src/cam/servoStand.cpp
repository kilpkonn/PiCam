//
// Created by tavo on 26.07.20.
//
#include <cmath>
#include <algorithm>

#include "servoStand.h"

ServoStand::ServoStand(uint8_t xAxisServoPin, uint8_t yAxisServoPin) :
        xAxisServoPin_{xAxisServoPin},
        yAxisServoPin_{yAxisServoPin},
        currentXAngle{0.0f},
        currentYAngle{0.0f} {
    isGpioInitalized = gpioInitialise();
    gpioSetMode(xAxisServoPin_, PI_OUTPUT);
    gpioSetMode(yAxisServoPin_, PI_OUTPUT);
    gpioSetPWMfrequency(xAxisServoPin_, servoPwmFrequency);
    gpioSetPWMfrequency(yAxisServoPin_, servoPwmFrequency);
}

void ServoStand::rotate(float x, float y) {
    x = std::clamp(x, -M_PIf32 / 2, M_PIf32 / 2);
    y = std::clamp(y, -M_PIf32 / 2, M_PIf32 / 2);
    unsigned xVal = minServoValue + static_cast<unsigned>((x + M_PIf32 / 2) / M_PI * (maxServoValue - minServoValue));
    unsigned yVal = minServoValue + static_cast<unsigned>((y + M_PIf32 / 2) / M_PI * (maxServoValue - minServoValue));
    currentXAngle = x;
    currentYAngle = y;
    gpioServo(xAxisServoPin_, xVal);
    gpioServo(yAxisServoPin_, yVal);
}

ServoStand::~ServoStand() {
    gpioTerminate();
}
