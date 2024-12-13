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
#include "hwlib.h"
#include "generic_reaction_wheel_platform_cfg.h"

static uart_info_t RW_UART[3] = {
    {.deviceString = &GENERIC_REACTION_WHEEL_1_CFG_STRING[0], .handle = GENERIC_REACTION_WHEEL_1_CFG_HANDLE, .isOpen = GENERIC_REACTION_WHEEL_1_CFG_IS_OPEN, .baud = GENERIC_REACTION_WHEEL_1_CFG_BAUDRATE_HZ},
    {.deviceString = &GENERIC_REACTION_WHEEL_2_CFG_STRING[0], .handle = GENERIC_REACTION_WHEEL_2_CFG_HANDLE, .isOpen = GENERIC_REACTION_WHEEL_2_CFG_IS_OPEN, .baud = GENERIC_REACTION_WHEEL_2_CFG_BAUDRATE_HZ},
    {.deviceString = &GENERIC_REACTION_WHEEL_3_CFG_STRING[0], .handle = GENERIC_REACTION_WHEEL_3_CFG_HANDLE, .isOpen = GENERIC_REACTION_WHEEL_3_CFG_IS_OPEN, .baud = GENERIC_REACTION_WHEEL_3_CFG_BAUDRATE_HZ},
};

/* Forward declarations */
int32_t GetCurrentMomentum(int wheel_number, double *momentum);
int32_t SetRWTorque(uint8_t wheel_num, double torque);

#endif /* _SAMPLE_DEVICE_H_ */