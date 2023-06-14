/*******************************************************************************
** File: 
**  generic_reaction_wheel_app.c
**
** Purpose:
**   This file contains the source code for the Generic Reaction Wheel App.
**
*******************************************************************************/

/*
** Include Files:
*/
#include "generic_reaction_wheel_events.h"
#include "generic_reaction_wheel_version.h"
#include "generic_reaction_wheel_app.h"
#include "hwlib.h"

#include <string.h>

/*
** global data
*/
GENERIC_RW_AppData_t GENERIC_RW_AppData;

/* Forward declarations */
static int32_t GetCurrentMomentum(double *momentum);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/* GENERIC_RW_AppMain() -- Application entry point and main process loop          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void GENERIC_RW_AppMain( void )
{
    int32  status;

    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp();

    /*
    ** Create the first Performance Log entry
    */
    CFE_ES_PerfLogEntry(GENERIC_RW_APP_PERF_ID);

    /*
    ** Perform application specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    status = GENERIC_RW_AppInit();
    if (status != CFE_SUCCESS)
    {
        GENERIC_RW_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /*
    ** GENERIC_RW Runloop
    */
    while (CFE_ES_RunLoop(&GENERIC_RW_AppData.RunStatus) == true)
    {
        /*
        ** Performance Log Exit Stamp
        */
        CFE_ES_PerfLogExit(GENERIC_RW_APP_PERF_ID);

        /* Pend on receipt of command packet */
        status = CFE_SB_RcvMsg(&GENERIC_RW_AppData.MsgPtr,
                               GENERIC_RW_AppData.CommandPipe,
                               CFE_SB_PEND_FOREVER);

        /*
        ** Performance Log Entry Stamp
        */
        CFE_ES_PerfLogEntry(GENERIC_RW_APP_PERF_ID);

        if (status == CFE_SUCCESS)
        {
            GENERIC_RW_ProcessCommandPacket(GENERIC_RW_AppData.MsgPtr);
        }
        else
        {
            CFE_EVS_SendEvent(GENERIC_RW_PIPE_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "GENERIC_RW APP: SB Pipe Read Error, App Will Exit");

            GENERIC_RW_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }

    }

    /*
    ** Performance Log Exit Stamp
    */
    CFE_ES_PerfLogExit(GENERIC_RW_APP_PERF_ID);

    CFE_ES_ExitApp(GENERIC_RW_AppData.RunStatus);

} /* End of GENERIC_RW_AppMain() */

static char deviceName[] = "/dev/tty2";
static uart_info_t RW_UART = {.deviceString = &deviceName[0], .handle = 2, .isOpen = PORT_CLOSED, .baud = 115200};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* GENERIC_RW_AppInit() --  initialization                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 GENERIC_RW_AppInit( void )
{
    int32    status;

    GENERIC_RW_AppData.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /*
    ** Initialize app command execution counters
    */
    GENERIC_RW_AppData.CmdCounter = 0;
    GENERIC_RW_AppData.ErrCounter = 0;

    /*
    ** Initialize app configuration data
    */
    GENERIC_RW_AppData.PipeDepth = GENERIC_RW_PIPE_DEPTH;

    strcpy(GENERIC_RW_AppData.PipeName, "GEN_RW_CMD_PIPE");

    /*
    ** Initialize event filter table...
    */
    GENERIC_RW_AppData.EventFilters[0].EventID = GENERIC_RW_STARTUP_INF_EID;
    GENERIC_RW_AppData.EventFilters[0].Mask    = 0x0000;
    GENERIC_RW_AppData.EventFilters[1].EventID = GENERIC_RW_COMMAND_ERR_EID;
    GENERIC_RW_AppData.EventFilters[1].Mask    = 0x0000;
    GENERIC_RW_AppData.EventFilters[2].EventID = GENERIC_RW_COMMANDNOP_INF_EID;
    GENERIC_RW_AppData.EventFilters[2].Mask    = 0x0000;
    GENERIC_RW_AppData.EventFilters[3].EventID = GENERIC_RW_COMMANDRST_INF_EID;
    GENERIC_RW_AppData.EventFilters[3].Mask    = 0x0000;
    GENERIC_RW_AppData.EventFilters[4].EventID = GENERIC_RW_INVALID_MSGID_ERR_EID;
    GENERIC_RW_AppData.EventFilters[4].Mask    = 0x0000;
    GENERIC_RW_AppData.EventFilters[5].EventID = GENERIC_RW_LEN_ERR_EID;
    GENERIC_RW_AppData.EventFilters[5].Mask    = 0x0000;
    GENERIC_RW_AppData.EventFilters[6].EventID = GENERIC_RW_PIPE_ERR_EID;
    GENERIC_RW_AppData.EventFilters[6].Mask    = 0x0000;

    /*
    ** Register the events
    */
    status = CFE_EVS_Register(GENERIC_RW_AppData.EventFilters,
                              GENERIC_RW_EVENT_COUNTS,
                              CFE_EVS_EventFilter_BINARY);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("GENERIC_RW App: Error Registering Events, RC = 0x%08lX\n",
                             (unsigned long)status);
        return ( status );
    }

    /*
    ** Initialize housekeeping packet (clear user data area).
    */
    CFE_SB_InitMsg(&GENERIC_RW_AppData.HkTlm.TlmHeader,
                   GENERIC_RW_APP_HK_TLM_MID,
                   sizeof(GENERIC_RW_AppData.HkTlm),
                   true);

    /* Connect to the UART */
    status = uart_init_port(&RW_UART);
    if(status != CFE_SUCCESS)
    {
    	CFE_ES_WriteToSysLog("GENERIC_RW App: UART port initialization error!\n");
    }    
    

    /*
    ** Create Software Bus message pipe.
    */
    status = CFE_SB_CreatePipe(&GENERIC_RW_AppData.CommandPipe,
                               GENERIC_RW_AppData.PipeDepth,
                               GENERIC_RW_AppData.PipeName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("GENERIC_RW App: Error creating pipe, RC = 0x%08lX\n",
                             (unsigned long)status);
        return ( status );
    }

    /*
    ** Subscribe to Housekeeping request commands
    */
    status = CFE_SB_Subscribe(GENERIC_RW_APP_SEND_HK_MID,
                              GENERIC_RW_AppData.CommandPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("GENERIC_RW App: Error Subscribing to HK request, RC = 0x%08lX\n",
                             (unsigned long)status);
        return ( status );
    }

    /*
    ** Subscribe to ground command packets
    */
    status = CFE_SB_Subscribe(GENERIC_RW_APP_CMD_MID,
                              GENERIC_RW_AppData.CommandPipe);
    if (status != CFE_SUCCESS )
    {
        CFE_ES_WriteToSysLog("GENERIC_RW App: Error Subscribing to Command, RC = 0x%08lX\n",
                             (unsigned long)status);

        return ( status );
    }

    CFE_EVS_SendEvent (GENERIC_RW_STARTUP_INF_EID,
                       CFE_EVS_EventType_INFORMATION,
                       "GENERIC_RW App Initialized. Version %d.%d.%d.%d",
                       GENERIC_RW_APP_MAJOR_VERSION,
                       GENERIC_RW_APP_MINOR_VERSION,
                       GENERIC_RW_APP_REVISION,
                       GENERIC_RW_APP_MISSION_REV);

    return ( CFE_SUCCESS );

} /* End of GENERIC_RW_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  GENERIC_RW_ProcessCommandPacket                                        */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the GENERIC_RW    */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void GENERIC_RW_ProcessCommandPacket( CFE_SB_MsgPtr_t Msg )
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(Msg);

    switch (MsgId)
    {
        case GENERIC_RW_APP_CMD_MID:
            GENERIC_RW_ProcessGroundCommand(Msg);
            break;

        case GENERIC_RW_APP_SEND_HK_MID:
            GENERIC_RW_ReportHousekeeping();
            break;

        default:
            CFE_EVS_SendEvent(GENERIC_RW_INVALID_MSGID_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "GENERIC_RW: invalid command packet,MID = 0x%x",
                              MsgId);
            break;
    }

    return;

} /* End GENERIC_RW_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* GENERIC_RW_ProcessGroundCommand() -- GENERIC_RW ground commands                    */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void GENERIC_RW_ProcessGroundCommand( CFE_SB_MsgPtr_t Msg )
{
    uint16 CommandCode;
    CommandCode = CFE_SB_GetCmdCode(Msg);

    /*
    ** Process "known" GENERIC_RW app ground commands
    */
    switch (CommandCode)
    {
        case GENERIC_RW_APP_NOOP_CC:
            if (GENERIC_RW_VerifyCmdLength(Msg, sizeof(GENERIC_RW_Noop_t)))
            {
                GENERIC_RW_Noop((GENERIC_RW_Noop_t *)Msg);
            }

            break;

        case GENERIC_RW_APP_RESET_COUNTERS_CC:
            if (GENERIC_RW_VerifyCmdLength(Msg, sizeof(GENERIC_RW_ResetCounters_t)))
            {
                GENERIC_RW_ResetCounters((GENERIC_RW_ResetCounters_t *)Msg);
            }

            break;

		/* Request Generic Reaction Wheel data 		*/
		case GENERIC_RW_APP_REQ_DATA_CC:
            if (GENERIC_RW_VerifyCmdLength(Msg, sizeof(GENERIC_RW_ResetCounters_t)))
            {
                GENERIC_RW_Current_Momentum((GENERIC_RW_Noop_t *)Msg);
            }
            
			break;

        /* Set Generic Reaction Wheel torque */
        case GENERIC_RW_APP_SET_TORQUE_CC:
            if (GENERIC_RW_VerifyCmdLength(Msg, sizeof(GENERIC_RW_Cmd_t)))
            {
                GENERIC_RW_Set_Torque((GENERIC_RW_Cmd_t *)Msg );
            }

            break;

        /* default case already found during FC vs length test */
        default:
            CFE_EVS_SendEvent(GENERIC_RW_COMMAND_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "Invalid ground command code: CC = %d",
                              CommandCode);
            break;
    }

    return;

} /* End of GENERIC_RW_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  GENERIC_RW_ReportHousekeeping                                          */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 GENERIC_RW_ReportHousekeeping(void)
{
    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &GENERIC_RW_AppData.HkTlm.TlmHeader);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &GENERIC_RW_AppData.HkTlm.TlmHeader);

    return CFE_SUCCESS;

} /* End of GENERIC_RW_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* GENERIC_RW_Noop -- GENERIC_RW NOOP commands                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 GENERIC_RW_Noop( const GENERIC_RW_Noop_t *Msg )
{

    GENERIC_RW_AppData.CmdCounter++;

    CFE_EVS_SendEvent(GENERIC_RW_COMMANDNOP_INF_EID,
                      CFE_EVS_EventType_INFORMATION,
                      "GENERIC_RW: NOOP command  Version %d.%d.%d.%d",
                      GENERIC_RW_APP_MAJOR_VERSION,
                      GENERIC_RW_APP_MINOR_VERSION,
                      GENERIC_RW_APP_REVISION,
                      GENERIC_RW_APP_MISSION_REV);

    return CFE_SUCCESS;

} /* End of GENERIC_RW_Noop */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  GENERIC_RW_ResetCounters                                               */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 GENERIC_RW_ResetCounters( const GENERIC_RW_ResetCounters_t *Msg )
{

    GENERIC_RW_AppData.CmdCounter = 0;
    GENERIC_RW_AppData.ErrCounter = 0;

    CFE_EVS_SendEvent(GENERIC_RW_COMMANDRST_INF_EID,
                      CFE_EVS_EventType_INFORMATION,
                      "GENERIC_RW: RESET command");

    return CFE_SUCCESS;

} /* End of GENERIC_RW_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* GENERIC_RW_Noop -- GENERIC_RW Current Momentum command                         */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 GENERIC_RW_Current_Momentum( const GENERIC_RW_Noop_t *Msg )
{
    int32_t status;
    double momentum;

    GENERIC_RW_AppData.CmdCounter++;
    CFE_EVS_SendEvent(GENERIC_RW_CMD_REQ_DATA_EID, CFE_EVS_DEBUG,"Request Generic Reaction Wheel Data");

    /* Read data from the UART */
    status = GetCurrentMomentum(&momentum);
    //OS_printf("GENERIC_RW: GetCurrentMomentum:  status=%d, momentum=%f\n", status, momentum);
    if (status > 0) {
        GENERIC_RW_AppData.HkTlm.Payload.data.momentum = momentum;
    } else {
        CFE_EVS_SendEvent(GENERIC_RW_CMD_REQ_DATA_EID, CFE_EVS_ERROR,"Request Generic Reaction Wheel Data - Error reading momentum");
    }

    /* publish the HK message which includes reaction wheel data */
    GENERIC_RW_ReportHousekeeping();

    return CFE_SUCCESS;
} /* End of GENERIC_RW_Current_Momentum */

/************************************************************************
** Get current momentum data from the UART
*************************************************************************/
static int32_t GetCurrentMomentum(double *momentum)
{
    uint8_t *DataBuffer;
    int32 DataLen;
    /* todo - fix the 1024 hard coded number */
    DataBuffer = (uint8_t *)malloc((1024) * sizeof(uint8_t));
    char *reply;

    char *request = "CURRENT_MOMENTUM";
    int32_t status = uart_write_port(&RW_UART, (uint8_t*)request, strlen(request));
    if (status < 0) {
        CFE_EVS_SendEvent(GENERIC_RW_CMD_REQ_DATA_EID, CFE_EVS_ERROR, "GetCurrentMomentum: Error writing to UART=%d\n", status);
    }
    /* check how many bytes are waiting on the uart */
    DataLen = uart_bytes_available(&RW_UART);
    if (DataLen > 0)
    {
        /* grab the bytes */
        status = uart_read_port(&RW_UART, DataBuffer, DataLen);
        if (status < 0) {
            CFE_EVS_SendEvent(GENERIC_RW_CMD_REQ_DATA_EID, CFE_EVS_ERROR, "GetCurrentMomentum: Error reading from UART=%d\n", status);
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

    /* Cleanup the data buffer once finished with the data */
    free(DataBuffer);

    return status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* GENERIC_RW_Noop -- GENERIC_RW Set Torque command                         */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 GENERIC_RW_Set_Torque( const GENERIC_RW_Cmd_t *Msg )
{
    int32_t status;
    double torque;
    char request[22];

    GENERIC_RW_AppData.CmdCounter++;
    CFE_EVS_SendEvent(GENERIC_RW_CMD_SET_TORQUE_EID, CFE_EVS_DEBUG,"Set Generic Reaction Wheel Torque");

    GENERIC_RW_Cmd_t *cmd;
    cmd = (GENERIC_RW_Cmd_t*)Msg;
    CFE_EVS_SendEvent(GENERIC_RW_CMD_SET_TORQUE_EID, CFE_EVS_INFORMATION, 
                    "Generic Reaction Wheel: Info, Set Torque Command received (%d * 10^-4 N-m)", cmd->data);
    torque = cmd->data;
    torque /= 10000.0; // units are 10^-4 Newton-meters (so we don't have to send floats in the command)

    sprintf(request, "SET_TORQUE=%10.4f", torque);
    status = uart_write_port(&RW_UART, (uint8_t*)request, strlen(request));
    //OS_printf("Generic Reaction Wheel: Sending command:%s\n", request);
    if (status < 0) {
        CFE_EVS_SendEvent(GENERIC_RW_CMD_SET_TORQUE_EID, CFE_EVS_ERROR, "Generic Reaction Wheel: Error writing to UART=%d\n", status);
    } else {
        /* Read the reply */
        uint8_t *DataBuffer;
        int32 DataLen;
        /* todo - fix the 1024 hard coded number */
        DataBuffer = (uint8_t *)malloc((1024) * sizeof(uint8_t));
        /* check how many bytes are waiting on the uart */
        DataLen = uart_bytes_available(&RW_UART);
        if (DataLen > 0)
        {
            uart_read_port(&RW_UART, DataBuffer, DataLen);
            DataBuffer[DataLen] = 0; // Ensure null termination
            //OS_printf("Generic Reaction Wheel: Response on UART=%s\n", (char *)DataBuffer);
        }
    }

    return CFE_SUCCESS;
} /* End of GENERIC_RW_Set_Torque */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* GENERIC_RW_VerifyCmdLength() -- Verify command packet length                   */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool GENERIC_RW_VerifyCmdLength( CFE_SB_MsgPtr_t Msg, uint16 ExpectedLength )
{
    bool result = true;

    uint16 ActualLength = CFE_SB_GetTotalMsgLength(Msg);

    /*
    ** Verify the command packet length.
    */
    if (ExpectedLength != ActualLength)
    {
        CFE_SB_MsgId_t MessageID   = CFE_SB_GetMsgId(Msg);
        uint16         CommandCode = CFE_SB_GetCmdCode(Msg);

        CFE_EVS_SendEvent(GENERIC_RW_LEN_ERR_EID,
                          CFE_EVS_EventType_ERROR,
                          "Invalid Msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
                          MessageID,
                          CommandCode,
                          ActualLength,
                          ExpectedLength);

        result = false;

        GENERIC_RW_AppData.ErrCounter++;
    }

    return( result );

} /* End of GENERIC_RW_VerifyCmdLength() */
