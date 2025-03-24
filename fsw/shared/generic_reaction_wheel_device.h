/*******************************************************************************
** File: generic_reaction_wheel_device.h
**
** Purpose:
**   This is the header file for the GENERIC_REACTION_WHEEL device.
**
*******************************************************************************/
#ifndef _GENERIC_REACTION_WHEEL_DEVICE_H_
#define _GENERIC_REACTION_WHEEL_DEVICE_H_

/*
** Required header files.
*/
#include "device_cfg.h"
#include "generic_reaction_wheel_platform_cfg.h"
#include "hwlib.h"

/* Forward declarations */
int32_t GetCurrentMomentum(uart_info_t *wheel, double *momentum);
int32_t SetRWTorque(uart_info_t *wheel, double torque);

#endif /* _SAMPLE_DEVICE_H_ */