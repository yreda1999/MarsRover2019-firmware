// Controller for the elevator
// NOTE: the convention established is that positive motor signals drive the elevator downwards
//          and negative signals drive the elevator upwards

#include <ElevatorController.hpp>
#include "../inc/ElevatorController.hpp"

ElevatorController::ElevatorController( ElevatorController::t_elevatorConfig        controllerConfig,
                                        ElevatorController::t_elevatorControlMode   controlMode )
:   m_elevatorControlMode( controlMode ),
    m_elevatorConfig( controllerConfig ),
    m_motor( controllerConfig.motor.pwmPin, controllerConfig.motor.dirPin, controllerConfig.motor.inverted ),
    m_encoder( controllerConfig.encoder ),
    m_limitSwitchTop( controllerConfig.limitSwitchTop),
    m_limitSwitchBottom( controllerConfig.limitSwitchBottom),
    m_positionPIDController( controllerConfig.positionPID.P, controllerConfig.positionPID.I, controllerConfig.positionPID.D, controllerConfig.positionPID.interval )
{
    initializePID();
    timer.start();
}

ElevatorController::t_elevatorControlMode ElevatorController::getControlMode() const
{
    return m_elevatorControlMode;
}

// Get position as encoder pulse count
int ElevatorController::getPosition()
{
    return m_encoder.getPulses();
}

mbed_error_status_t ElevatorController::setControlMode( t_elevatorControlMode controlMode )
{
    m_elevatorControlMode = controlMode;
    m_motor.speed( 0.0f );

    switch (m_elevatorControlMode) {
        case motorDutyCycle:
            break;

        case positionPID:
            m_positionPIDController.reset();
            break;
        default:
            return MBED_ERROR_CODE_INVALID_ARGUMENT;
    }

    timer.reset();

    return MBED_SUCCESS;
}

// Set the motor speed as a percentage of the maximum motor speed [-1.0, 1.0]
mbed_error_status_t ElevatorController::setMotorSpeedPercent(float percent)
{
    if (m_elevatorControlMode != motorDutyCycle) {
        return MBED_ERROR_INVALID_OPERATION;
    }

    // We want double protection incase a sensor goes down or an encoder breaks
    // TODO: ISSUE - WHAT IF A SENSOR/ENCODER GOES OFFLINE AND IS ALWAYS SET TO HIGH?
    if (( getPosition() <= 0 && percent < 0.0f )                                    ||
        ( getPosition() >= m_elevatorConfig.maxEncoderPulses && percent > 0.0f )    ||
        ( m_limitSwitchTop.read() && percent < 0.0f)                                ||
        ( m_limitSwitchBottom.read() && percent > 0.0f))
    {
        percent = 0.0f;
    }

    m_motor.speed(percent);

    return MBED_SUCCESS;
}

// TODO: VALIDATE FUNCTION
mbed_error_status_t  ElevatorController::setEncoderPositionPercent( float percent )
{
    // Has to be in positionPID control
    if( m_elevatorControlMode != positionPID ) {
        return MBED_ERROR_INVALID_OPERATION;
    }

    // Do nothing
    if( percent < 0 || percent > m_elevatorConfig.maxEncoderPulses ){
        percent = getPosition() / m_elevatorConfig.maxEncoderPulses;
    }
    // Convert from percentage to encoder value - for PID
    percent *= m_elevatorConfig.maxEncoderPulses;
    m_positionPIDController.setSetPoint( percent );
    return MBED_SUCCESS;
}

mbed_error_status_t  ElevatorController::maxLower() // Wrapper for setPosition with lower encoder limit
{
    return setEncoderPositionPercent( 1.0f );
}

mbed_error_status_t  ElevatorController::retract() // Wrapper for setPosition with encoder = 0
{
    return setEncoderPositionPercent( 0.0f );
}


void ElevatorController::update() {
    float interval = timer.read();
    timer.reset();

    switch (m_elevatorControlMode) {
        case motorDutyCycle:
            if (( getPosition() <= 0 && m_motor.read() < 0.0f )                                 ||
                ( getPosition() >= m_elevatorConfig.maxEncoderPulses && m_motor.read() > 0.0f ) ||
                ( m_limitSwitchTop.read() && m_motor.read() < 0.0f)                             ||
                ( m_limitSwitchBottom.read() && m_motor.read() > 0.0f))
            {
                m_motor.speed(0.0f);
            }
            break;

        case positionPID:
            m_positionPIDController.setInterval( interval );
            m_positionPIDController.setProcessValue( getPosition() );
            m_motor.speed( m_positionPIDController.compute() );
            break;
    }
}

// TODO: is there any multiplier required on the input limits? - as there is within the ArmLowerController?
void ElevatorController::initializePID( void ) {
    m_positionPIDController.setInputLimits( 0, m_elevatorConfig.maxEncoderPulses );
    m_positionPIDController.setOutputLimits( m_elevatorConfig.PIDOutputMotorMinDutyCycle, m_elevatorConfig.PIDOutputMotorMaxDutyCycle );
    m_positionPIDController.setBias( m_elevatorConfig.positionPID.bias );
    m_positionPIDController.setMode( PID_AUTO_MODE );
}