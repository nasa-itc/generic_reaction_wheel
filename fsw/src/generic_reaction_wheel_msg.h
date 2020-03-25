/*******************************************************************************
** File:
**  generic_reaction_wheel_app_msg.h
**
** Purpose:
**  Define Generic Reaction Wheel App  Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _generic_reaction_wheel_app_msg_h_
#define _generic_reaction_wheel_app_msg_h_

/*
** Generic Reaction Wheel App command codes
*/
#define GENERIC_RW_APP_NOOP_CC                 0
#define GENERIC_RW_APP_RESET_COUNTERS_CC       1
#define GENERIC_RW_APP_REQ_DATA_CC             2

/*************************************************************************/

/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];

} GENERIC_RW_NoArgsCmd_t;

/*
** The following commands all share the "NoArgs" format
**
** They are each given their own type name matching the command name, which_open_mode
** allows them to change independently in the future without changing the prototype
** of the handler function
*/
typedef GENERIC_RW_NoArgsCmd_t      GENERIC_RW_Noop_t;
typedef GENERIC_RW_NoArgsCmd_t      GENERIC_RW_ResetCounters_t;

/*************************************************************************/
/*
** Type definition (Generic Reaction Wheel App housekeeping)
*/

typedef struct
{
    double momentum;
} GENERIC_RW_Data;


typedef struct
{
    uint8              CommandErrorCounter;
    uint8              CommandCounter;
    GENERIC_RW_Data    data;
} GENERIC_RW_HkTlm_Payload_t;

typedef struct
{
    uint8                       TlmHeader[CFE_SB_TLM_HDR_SIZE];
    GENERIC_RW_HkTlm_Payload_t  Payload;

} OS_PACK GENERIC_RW_HkTlm_t;

#endif /* _generic_reaction_wheel_app_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
