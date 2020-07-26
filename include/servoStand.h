//
// Created by tavo on 26.07.20.
//
#pragma once

#include <pigpio.h>


class ServoStand {
public:
    bool isGpioInitalized;

    ServoStand(uint8_t xAxisServoPin, uint8_t yAxisServoPin);

    virtual ~ServoStand();

    /**
     * Rotate stand
     * @param x - X-axis angle in range [-PI/2, PI/2]
     * @param y - Y-axis angle in range [-PI/2, PI/2]
     */
    void rotate(float x, float y);

private:
    const unsigned servoPwmFrequency = 142857;
    const unsigned minServoValue = 500;
    const unsigned maxServoValue = 2500;

    const uint8_t xAxisServoPin_;
    const uint8_t yAxisServoPin_;

    float currentXAngle;
    float currentYAngle;
};
