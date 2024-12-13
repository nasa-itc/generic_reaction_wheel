/*******************************************************************************
** File: generic_reaction_wheel_device.c
**
** Purpose:
**   This file contains the source code for the GENERIC_REACTION_WHEEL device.
**
*******************************************************************************/

/*
** Include Files
*/
#include "generic_reaction_wheel_device.h"

/************************************************************************
** Get current momentum data from the UART
*************************************************************************/
int32_t GetCurrentMomentum(int wheel_number, double *momentum)
{
    uint8_t DataBuffer[1024];
    int32_t DataLen;
    char *reply;

    char *request = "CURRENT_MOMENTUM";
    int32_t status = uart_write_port(&RW_UART[wheel_number], (uint8_t*)request, strlen(request));
    if (status < 0) {
        OS_printf("GetCurrentMomentum: Error writing to UART=%d\n", status);
    }
    /* check how many bytes are waiting on the uart */
    DataLen = uart_bytes_available(&RW_UART[wheel_number]);
    if (DataLen > 0)
    {
        /* grab the bytes */
        status = uart_read_port(&RW_UART[wheel_number], DataBuffer, DataLen);
        if (status < 0) {
           OS_printf("GetCurrentMomentum: Error reading from UART=%d\n", status);
        } else {
            DataBuffer[DataLen] = 0; // Ensure null termination
            reply = (char *)DataBuffer;
            if (strncmp(reply, "CURRENT_MOMENTUM=", 17) == 0) {
                *momentum = atof(&reply[17]);
            } else {
                status = 0;
            }
        }
    }

    return status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* SetRWTorque();                                                             */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32_t SetRWTorque( uint8_t wheel_num, double torque )
{
    int32_t status;
    char request[22];

    sprintf(request, "SET_TORQUE=%10.4f", torque);
    status = uart_write_port(&RW_UART[wheel_num], (uint8_t*)request, strlen(request));
    //OS_printf("Generic Reaction Wheel: Sending command:%s\n", request);
    if (status < 0) {
        OS_printf("Generic Reaction Wheel: Error writing to UART=%d\n", status);
    } else {
        /* Read the reply */
        uint8_t DataBuffer[1024];
        int32 DataLen;
        /* check how many bytes are waiting on the uart */
        DataLen = uart_bytes_available(&RW_UART[wheel_num]);
        if (DataLen > 0)
        {
            uart_read_port(&RW_UART[wheel_num], DataBuffer, DataLen);
            DataBuffer[DataLen] = 0; // Ensure null termination
            //OS_printf("Generic Reaction Wheel: Response on UART=%s\n", (char *)DataBuffer);
        }
    }

    return status;
} /* End of SetRWTorque */