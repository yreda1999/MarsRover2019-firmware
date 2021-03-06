/* mbed simple H-bridge motor controller
 * Copyright (c) 2007-2010, sford, http://mbed.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#include "Motor.h"
#include "mbed.h"
#include <algorithm>

Motor::Motor(PinName pwm, PinName dir, bool inverted, int freqInHz, float limit) :
	_pwm(pwm), _dir(dir), _inverted(inverted), _limit(limit) {

#ifndef DISABLE_SETTING_MOTOR_PWM_FREQ
    // Set initial condition of PWM
    _pwm.period(1.0 / freqInHz);
#endif

    _pwm = 0.0;
 
    // Initial condition of output enables
    _dir = 0.0;
}

Motor::Motor(t_motorConfig motorConfig) : Motor(motorConfig.pwmPin, motorConfig.dirPin, motorConfig.inverted,
        motorConfig.freqInHz, motorConfig.limit) {}

void Motor::setDutyCycle(float dutyCycle) {
    _dir = (dutyCycle > 0.0) != _inverted;
    _pwm = min( (float) fabs(dutyCycle), _limit);
}

Motor& Motor::operator=(int dutyCycle) {
    this->setDutyCycle(dutyCycle);
    return *this;
}

float Motor::getDutyCycle() {
    if (_dir) {
        return _pwm.read();
    }
    else {
        return -_pwm.read();
    }
}