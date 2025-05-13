/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in generic_reaction_wheel_device header
 */

#include "generic_reaction_wheel_device.h"
#include "utgenstub.h"
#include "libuart.h"

/*
 * ----------------------------------------------------
 * Generated stub function for GetCurrentMomentum()
 * ----------------------------------------------------
 */
int32_t GetCurrentMomentum(uart_info_t *wheel, double *momentum)
{
    UT_GenStub_SetupReturnBuffer(GetCurrentMomentum, int32_t);

    UT_GenStub_AddParam(GetCurrentMomentum, uart_info_t *, wheel);
    UT_GenStub_AddParam(GetCurrentMomentum, double *, momentum);

    UT_GenStub_Execute(GetCurrentMomentum, Basic, NULL);

    return UT_GenStub_GetReturnValue(GetCurrentMomentum, int32_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for SetRWTorque()
 * ----------------------------------------------------
 */
int32_t SetRWTorque(uart_info_t *wheel, double torque)
{
    UT_GenStub_SetupReturnBuffer(SetRWTorque, int32_t);

    UT_GenStub_AddParam(SetRWTorque, uart_info_t *, wheel);
    UT_GenStub_AddParam(SetRWTorque, double, torque);

    UT_GenStub_Execute(SetRWTorque, Basic, NULL);

    return UT_GenStub_GetReturnValue(SetRWTorque, int32_t);
}
