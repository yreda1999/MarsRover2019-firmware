#ifndef AUGER_CONTROLLER_H
#define AUGER_CONTROLLER_H
// Controller for the Auger

#include "mbed.h"
#include "Motor.h"
#include "PwmIn.h"
#include "PID.h"
#include "PinNames.h"

class AugerController{
    public:
        typedef struct{
            // Motor Configuration
            Motor::t_motorConfig motor;

        } t_augerConfig;
        
        AugerController( t_augerConfig controllerConfig );
                         
        mbed_error_status_t setMotorSpeedPercent( float percent );
    private:
        t_augerConfig m_augerConfig;

        Motor m_motor;

        float m_inversionMultiplier;  
};


#endif // AUGER_CONTROLLER_H