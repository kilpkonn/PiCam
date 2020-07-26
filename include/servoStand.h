//
// Created by tavo on 26.07.20.
//
#pragma once

#include <pigpio.h>


class ServoStand {
public:
    bool isGpioInitalized;

    ServoStand();

    virtual ~ServoStand();

    void rotate(float x, float y);

private:
    const uint8_t xAxisServoPin = 17;
    const uint8_t yAxisServoPin = 18;
    const unsigned servoPwmFrequency = 142857;
    const unsigned minServoValue = 500;
    const unsigned maxServoValue = 2500;
};
