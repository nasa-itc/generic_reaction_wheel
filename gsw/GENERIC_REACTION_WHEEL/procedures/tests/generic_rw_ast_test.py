import sys
import glob
import time

for p in glob.glob('/gems/gems/openc3-cosmos-nos3-*/targets/GENERIC_REACTION_WHEEL/scripts'):
    if p not in sys.path:
        sys.path.append(p)

from openc3.script import cmd, tlm, check, wait_check

try:
    from nos3.generic_reaction_wheel_lib import *
except ImportError:
    pass


def run_generic_reaction_wheel_ast_test():
    ##
    ## Hardware failure
    ##

    for n in range(GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT):
        # Prepare
        GENERIC_REACTION_WHEEL_prepare_ast()

        cmd_cnt = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT")
        cmd_err_cnt = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T ERROR_COUNT")
        dev_err_cnt_RW0 = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0")
        dev_err_cnt_RW1 = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW1")
        dev_err_cnt_RW2 = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW2")

        generic_rw0_sim_disable()
        generic_rw1_sim_disable()
        generic_rw2_sim_disable()

        time.sleep(1)

        # 3 RW commands, 1 for each wheel
        cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 0, TORQUE 0")
        cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 1, TORQUE 0")
        cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 2, TORQUE 0")

        get_GENERIC_REACTION_WHEEL_data()
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT  == {cmd_cnt}", 5)
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T ERROR_COUNT >= {cmd_err_cnt}", 5)
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0 >= {dev_err_cnt_RW0}", 5)
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW1 >= {dev_err_cnt_RW1}", 5)
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW2 >= {dev_err_cnt_RW2}", 5)

        generic_rw0_sim_enable()
        generic_rw1_sim_enable()
        generic_rw2_sim_enable()
        time.sleep(GENERIC_REACTION_WHEEL_TORQUE_RESPONSE_SLEEP)

        confirm_GENERIC_REACTION_WHEEL_data()
        
    print("End of RW AST Test Script")