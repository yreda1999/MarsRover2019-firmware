// Controller for the auger drill

#include "AugerController.h"
#include "mbed.h"
#include "Motor.h"
#include "PwmIn.h"
#include "PID.h"
#include "PinNames.h"

AugerController::AugerController( AugerController::t_augerConfig controllerConfig )
:   m_augerConfig( controllerConfig ),
    m_motor( controllerConfig.motor )
{}

mbed_error_status_t AugerController::setMotorDutyCycle(float percent)
{
    m_motor.setDutyCycle(percent);
    return MBED_SUCCESS;
}

float AugerController::getDutyCycle(void) {
    return m_motor.getDutyCycle();
}
