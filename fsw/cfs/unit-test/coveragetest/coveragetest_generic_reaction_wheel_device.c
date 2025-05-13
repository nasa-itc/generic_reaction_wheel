#include "generic_reaction_wheel_app_coveragetest_common.h"

void Test_GetCurrentMomentum(void)
{
    uart_info_t device;
    double momentum;
    int32_t status;

    /* Test successful case */
    UT_SetDefaultReturnValue(UT_KEY(uart_write_port), strlen("CURRENT_MOMENTUM"));
    UT_SetDefaultReturnValue(UT_KEY(uart_bytes_available), 25);
    UT_SetDefaultReturnValue(UT_KEY(uart_read_port), 25);
    status = GetCurrentMomentum(&device, &momentum);
    UtAssert_True(status == 25, "GetCurrentMomentum successful case returned %ld", (long)status);

    /* Test write error */
    UT_SetDeferredRetcode(UT_KEY(uart_write_port), 1, -1);
    status = GetCurrentMomentum(&device, &momentum);
    UtAssert_True(status == -1, "GetCurrentMomentum write error returned %ld", (long)status);

    /* Test read error */
    UT_SetDefaultReturnValue(UT_KEY(uart_write_port), strlen("CURRENT_MOMENTUM"));
    UT_SetDefaultReturnValue(UT_KEY(uart_bytes_available), 25);
    UT_SetDeferredRetcode(UT_KEY(uart_read_port), 1, -1);
    status = GetCurrentMomentum(&device, &momentum);
    UtAssert_True(status == -1, "GetCurrentMomentum read error returned %ld", (long)status);

    /* Test invalid response format */
    UT_SetDefaultReturnValue(UT_KEY(uart_write_port), strlen("CURRENT_MOMENTUM"));
    UT_SetDefaultReturnValue(UT_KEY(uart_bytes_available), 25);
    UT_SetDefaultReturnValue(UT_KEY(uart_read_port), 25);
    UT_SetDataBuffer(UT_KEY(uart_read_port), "INVALID_RESPONSE=1.234", 25, false);
    status = GetCurrentMomentum(&device, &momentum);
    UtAssert_True(status == 0, "GetCurrentMomentum invalid format returned %ld", (long)status);
}

void Test_SetRWTorque(void)
{
    uart_info_t device;
    double torque = 1.2345;
    int32_t status;

    /* Test successful case */
    UT_SetDefaultReturnValue(UT_KEY(uart_write_port), strlen("SET_TORQUE=    1.2345"));
    UT_SetDefaultReturnValue(UT_KEY(uart_bytes_available), 20);
    UT_SetDefaultReturnValue(UT_KEY(uart_read_port), 20);
    status = SetRWTorque(&device, torque);
    UtAssert_True(status == strlen("SET_TORQUE=    1.2345"), "SetRWTorque successful case returned %ld", (long)status);

    /* Test write error */
    UT_SetDeferredRetcode(UT_KEY(uart_write_port), 1, -1);
    status = SetRWTorque(&device, torque);
    UtAssert_True(status == -1, "SetRWTorque write error returned %ld", (long)status);

    /* Test no response */
    UT_SetDefaultReturnValue(UT_KEY(uart_write_port), strlen("SET_TORQUE=    1.2345"));
    UT_SetDefaultReturnValue(UT_KEY(uart_bytes_available), 0);
    status = SetRWTorque(&device, torque);
    UtAssert_True(status == OS_ERROR, "SetRWTorque no response returned %ld", (long)status);
}

void Test_GetCurrentMomentum_Hook(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context, va_list va) {}

/*
 * Setup function prior to every test
 */
void Generic_reaction_wheel_UT_Setup(void)
{
    UT_ResetState(0);
}

/*
 * Teardown function after every test
 */
void Generic_reaction_wheel_UT_TearDown(void) {}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    UT_SetVaHandlerFunction(UT_KEY(Test_GetCurrentMomentum), Test_GetCurrentMomentum_Hook, NULL);
    ADD_TEST(GetCurrentMomentum);
    ADD_TEST(SetRWTorque);
}