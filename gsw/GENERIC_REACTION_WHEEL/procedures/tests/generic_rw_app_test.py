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


def run_generic_reaction_wheel_app_test():

    ##
    # get tlm packet for Reaction wheel
    ##
    for n in range(GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT):
        get_GENERIC_REACTION_WHEEL_data()

    ##
    ## Confirm NOOP command
    ##
    for n in range(GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT):
        GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_NOOP_CC")

    ##
    ## Confirm RW Data
    ## 
    for n in range(GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT):
        #Testing RW commands and detecting Momentum directions
        confirm_GENERIC_REACTION_WHEEL_data()

    ##
    ## reset counters
    ##
    for n in range(GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT):
        GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_NOOP_CC")
        #ADCS May be running, so saving variables before reset
        initial_command_count = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT")
        initial_error_count = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T ERROR_COUNT")

        cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_RST_COUNTERS_CC")

        get_GENERIC_REACTION_WHEEL_data()
        check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT  < {initial_command_count}")
        check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T ERROR_COUNT <= {initial_error_count}")

    ##
    ##   Invalid ground command, confirm bad lengths and codes are rejected
    ##
    for n in range(GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT):
        GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_NOOP_CC")
        # Bad length
        cmd_cnt = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT")
        cmd_err_cnt = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T ERROR_COUNT")
        cmd(f"GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_NOOP_CC with CCSDS_LENGTH {n+2}")  # Note +2 due to CCSDS already being +1
        get_GENERIC_REACTION_WHEEL_data()
        check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT  == {cmd_cnt}")
        check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T ERROR_COUNT == {cmd_err_cnt+1}")

    print("End of RW App Test Script")