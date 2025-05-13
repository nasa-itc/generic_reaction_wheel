/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/*
** File: coveragetest_generic_reaction_wheel_app.c
**
** Purpose:
** Coverage Unit Test cases for the GENERIC_RW Application
**
** Notes:
** This implements various test cases to exercise all code
** paths through all functions defined in the GENERIC_RW application.
**
** It is primarily focused at providing examples of the various
** stub configurations, hook functions, and wrapper calls that
** are often needed when coercing certain code paths through
** complex functions.
*/

/*
 * Includes
 */

#include "generic_reaction_wheel_app_coveragetest_common.h"
#include "ut_generic_reaction_wheel_app.h"
#include "generic_reaction_wheel_msg.h"
#include "generic_reaction_wheel_events.h"

/* External reference to app data */
extern GENERIC_RW_AppData_t GENERIC_RW_AppData;

typedef struct
{
    uint16      ExpectedEvent;
    uint32      MatchCount;
    const char *ExpectedFormat;
} UT_CheckEvent_t;

/*
 * An example hook function to check for a specific event.
 */
static int32 UT_CheckEvent_Hook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context,
                                va_list va)
{
    UT_CheckEvent_t *State = UserObj;
    uint16           EventId;
    const char      *Spec;

    /*
     * The CFE_EVS_SendEvent stub passes the EventID as the
     * first context argument.
     */
    if (Context->ArgCount > 0)
    {
        EventId = UT_Hook_GetArgValueByName(Context, "EventID", uint16);
        if (EventId == State->ExpectedEvent)
        {
            if (State->ExpectedFormat != NULL)
            {
                Spec = UT_Hook_GetArgValueByName(Context, "Spec", const char *);
                if (Spec != NULL)
                {
                    if (strcmp(Spec, State->ExpectedFormat) == 0)
                    {
                        ++State->MatchCount;
                    }
                }
            }
            else
            {
                ++State->MatchCount;
            }
        }
    }

    return 0;
}

/*
 * Helper function to set up for event checking
 * This attaches the hook function to CFE_EVS_SendEvent
 */
static void UT_CheckEvent_Setup(UT_CheckEvent_t *Evt, uint16 ExpectedEvent, const char *ExpectedFormat)
{
    memset(Evt, 0, sizeof(*Evt));
    Evt->ExpectedEvent  = ExpectedEvent;
    Evt->ExpectedFormat = ExpectedFormat;
    UT_SetVaHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_CheckEvent_Hook, Evt);
}

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/

void Test_GENERIC_RW_AppInit(void)
{
    /*
     * Test Case For:
     * int32 GENERIC_RW_AppInit( void )
     */

    /* nominal case should return CFE_SUCCESS */
    UT_TEST_FUNCTION_RC(GENERIC_RW_AppInit(), CFE_SUCCESS);

    /* trigger a failure for each of the sub-calls,
     * and confirm a write to syslog for each.
     * Note that this count accumulates, because the status
     * is _not_ reset between these test cases. */
    UT_SetDeferredRetcode(UT_KEY(CFE_EVS_Register), 1, CFE_EVS_INVALID_PARAMETER);
    UT_TEST_FUNCTION_RC(GENERIC_RW_AppInit(), CFE_EVS_INVALID_PARAMETER);
    UtAssert_True(UT_GetStubCount(UT_KEY(CFE_ES_WriteToSysLog)) == 1, "CFE_ES_WriteToSysLog() called");

    UT_SetDeferredRetcode(UT_KEY(CFE_SB_CreatePipe), 1, CFE_SB_BAD_ARGUMENT);
    UT_TEST_FUNCTION_RC(GENERIC_RW_AppInit(), CFE_SB_BAD_ARGUMENT);

    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 1, CFE_SB_BAD_ARGUMENT);
    UT_TEST_FUNCTION_RC(GENERIC_RW_AppInit(), CFE_SB_BAD_ARGUMENT);

    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 2, CFE_SB_BAD_ARGUMENT);
    UT_TEST_FUNCTION_RC(GENERIC_RW_AppInit(), CFE_SB_BAD_ARGUMENT);
}

void Test_GENERIC_RW_ReportHousekeeping(void)
{
    /*
     * Test Case For:
     * int32 GENERIC_RW_ReportHousekeeping
     */
    UT_CheckEvent_t EventTest;

    UT_CheckEvent_Setup(&EventTest, GENERIC_RW_COMMAND_ERR_EID, NULL);
    UT_TEST_FUNCTION_RC(GENERIC_RW_ReportHousekeeping(), CFE_SUCCESS);
    UtAssert_True(EventTest.MatchCount == 0, "GENERIC_RW_COMMAND_ERR_EID not generated (%u)", (unsigned int)EventTest.MatchCount);
}

void Test_GENERIC_RW_Enable(void)
{
    /*
     * Test Case For:
     * void GENERIC_RW_Enable
     */
    UT_CheckEvent_t EventTest;
    GENERIC_RW_Cmd_t Cmd;

    UT_CheckEvent_Setup(&EventTest, GENERIC_RW_COMMAND_ERR_EID, NULL);
    GENERIC_RW_Enable(&Cmd);
    UtAssert_True(EventTest.MatchCount == 0, "GENERIC_RW_COMMAND_ERR_EID not generated (%u)", (unsigned int)EventTest.MatchCount);
}

void Test_GENERIC_RW_Disable(void)
{
    /*
     * Test Case For:
     * void GENERIC_RW_Disable
     */
    UT_CheckEvent_t EventTest;
    GENERIC_RW_Cmd_t Cmd;

    UT_CheckEvent_Setup(&EventTest, GENERIC_RW_COMMAND_ERR_EID, NULL);
    GENERIC_RW_Disable(&Cmd);
    UtAssert_True(EventTest.MatchCount == 0, "GENERIC_RW_COMMAND_ERR_EID not generated (%u)", (unsigned int)EventTest.MatchCount);
}

void Generic_reaction_wheel_UT_Setup(void)
{
    UT_ResetState(0);
    memset(&GENERIC_RW_AppData, 0, sizeof(GENERIC_RW_AppData));
}

void Generic_reaction_wheel_UT_TearDown(void) {}

void UtTest_Setup(void)
{
    ADD_TEST(GENERIC_RW_AppInit);
    ADD_TEST(GENERIC_RW_ReportHousekeeping);
    ADD_TEST(GENERIC_RW_Enable);
    ADD_TEST(GENERIC_RW_Disable);
}