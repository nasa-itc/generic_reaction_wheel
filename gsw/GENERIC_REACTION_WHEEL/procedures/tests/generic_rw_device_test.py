import sys
import glob

for p in glob.glob('/gems/gems/openc3-cosmos-nos3-*/targets/GENERIC_REACTION_WHEEL/scripts'):
    if p not in sys.path:
        sys.path.append(p)

from openc3.script import cmd, tlm, check

try:
    from nos3.generic_reaction_wheel_lib import *
except ImportError:
    pass


def run_generic_reaction_wheel_device_test():
    ##
    ## This script tests the cFS component device functionality.
    ## Currently this includes: 
    ##   Enable / disable, control hardware communications
    ##   Configuration, reconfigure sample instrument register
    ##


    ##
    ## Enable / disable, control hardware communications
    ##
    for n in range(GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT):
        # Get to known state
        safe_GENERIC_REACTION_WHEEL()
        #resetting counters
        cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_RST_COUNTERS_CC")
        
        get_GENERIC_REACTION_WHEEL_data()

        dev_err_cnt_RW0 = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0")
        dev_err_cnt_RW1 = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW1")
        dev_err_cnt_RW2 = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW2")

        # Manually command to disable when already disabled
        #disable RWs watch error counters increase:
        disable_all_RW()

        get_GENERIC_REACTION_WHEEL_data()

        cmd_cnt = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT")
        cmd_err_cnt = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T ERROR_COUNT")

        #sending 3 torque commands (value set to 0)
        cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 0, TORQUE 0")
        cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 1, TORQUE 0")
        cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 2, TORQUE 0")

        get_GENERIC_REACTION_WHEEL_data()

        check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0 == {dev_err_cnt_RW0+1}")
        check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW1 == {dev_err_cnt_RW1+1}")
        check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW2 == {dev_err_cnt_RW2+1}")

        print(f"command count is: {cmd_cnt}")
        check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT == {cmd_cnt}")
        check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T ERROR_COUNT == {cmd_err_cnt+3}") #+3 for 3 commands failed

        enable_all_RW()
        get_GENERIC_REACTION_WHEEL_data()

        #confirming normal counters
        confirm_RW_device_data()

    print("End of RW Device Test Script")