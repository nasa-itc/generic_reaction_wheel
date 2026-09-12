# Library for GENERIC_REACTION_WHEEL Target
import sys
import glob

for p in glob.glob('/gems/gems/openc3-cosmos-nos3-*/targets/GENERIC_REACTION_WHEEL/scripts'):
    if p not in sys.path:
        sys.path.append(p)

try:
    from openc3.script import cmd, tlm, check, wait_check, wait_check_packet
    import time
except ImportError:
    pass

#
# Definitions
#
GENERIC_REACTION_WHEEL_CMD_SLEEP = 0.25
GENERIC_REACTION_WHEEL_RESPONSE_TIMEOUT = 10
GENERIC_REACTION_WHEEL_TORQUE_RESPONSE_SLEEP = 5
GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT = 1
GENERIC_REACTION_WHEEL_DEVICE_LOOP_COUNT = 1
GENERIC_REACTION_WHEEL_MAX_MOMENTUM_NM = 0.01

#
# Functions
#
def get_GENERIC_REACTION_WHEEL_data():
    cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_REQ_DATA_CC")
    wait_check_packet("GENERIC_REACTION_WHEEL_DEBUG", "GENRW_HK_TLM_T", 1, GENERIC_REACTION_WHEEL_RESPONSE_TIMEOUT)
    time.sleep(GENERIC_REACTION_WHEEL_CMD_SLEEP)

def GENERIC_REACTION_WHEEL_cmd(command_string):
    count = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT") + 1

    if (count == 256):
        count = 0

    cmd(command_string)
    get_GENERIC_REACTION_WHEEL_data()
    current = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT")
    if (current != count):
        # Try again
        cmd(command_string)
        get_GENERIC_REACTION_WHEEL_data()
        current = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT")
        if (current != count):
            # Third times the charm
            cmd(command_string)
            get_GENERIC_REACTION_WHEEL_data()
            current = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT")
            
    check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T COMMAND_COUNT >= {count}")

def turn_off_RWS():
    #Turning off RW's
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 0, TORQUE 0")
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 1, TORQUE 0")
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 2, TORQUE 0")


def safe_GENERIC_REACTION_WHEEL():
    get_GENERIC_REACTION_WHEEL_data()
    #Turn off RWs, set to 0
    turn_off_RWS()

def confirm_GENERIC_REACTION_WHEEL_data():
    cmd_err_cnt = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T ERROR_COUNT")
       
    # Checking RW 0 Positive Direction
    rw0_momentum_init = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_0")
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 0, TORQUE 10")
    time.sleep(GENERIC_REACTION_WHEEL_TORQUE_RESPONSE_SLEEP)
    get_GENERIC_REACTION_WHEEL_data()
    if (rw0_momentum_init >= GENERIC_REACTION_WHEEL_MAX_MOMENTUM_NM):
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_0 >= {rw0_momentum_init}", 5)
    else:
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_0 > {rw0_momentum_init}", 5)

    
    rw0_momentum = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_0")
    print(f"Reaction Wheel 0 Momentum (N m): {rw0_momentum}")
    
    # Checking RW 1 Positive Direction
    rw1_momentum_init = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_1")
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 1, TORQUE 10")
    time.sleep(GENERIC_REACTION_WHEEL_TORQUE_RESPONSE_SLEEP)
    get_GENERIC_REACTION_WHEEL_data()
    if (rw1_momentum_init >= GENERIC_REACTION_WHEEL_MAX_MOMENTUM_NM):
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_1 >= {rw1_momentum_init}", 5)
    else:
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_1 > {rw1_momentum_init}", 5)
    
    rw1_momentum = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_1")
    print(f"Reaction Wheel 1 Momentum (N m): {rw1_momentum}")
     
    # Checking RW 2 Positive Direction
    rw2_momentum_init = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_2")
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 2, TORQUE 10")
    time.sleep(GENERIC_REACTION_WHEEL_TORQUE_RESPONSE_SLEEP)
    get_GENERIC_REACTION_WHEEL_data()
    if (rw2_momentum_init >= GENERIC_REACTION_WHEEL_MAX_MOMENTUM_NM):
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_2 >= {rw2_momentum_init}", 5)
    else:
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_2 > {rw2_momentum_init}", 5)
    
    rw2_momentum = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_2")
    print(f"Reaction Wheel 2 Momentum (N m): {rw2_momentum}")

    # Checking RW 0 Negative Direction
    rw0_momentum_init = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_0")
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 0, TORQUE -10")
    time.sleep(GENERIC_REACTION_WHEEL_TORQUE_RESPONSE_SLEEP)
    get_GENERIC_REACTION_WHEEL_data()
    if (rw0_momentum_init <= (-1 * GENERIC_REACTION_WHEEL_MAX_MOMENTUM_NM)):
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_0 <= {rw0_momentum_init}", 5)
    else:
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_0 < {rw0_momentum_init}", 5)
    
    rw0_momentum = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_0")
    print(f"Reaction Wheel 0 Momentum (N m): {rw0_momentum}")
    
    # Checking RW 1 Negative Direction
    rw1_momentum_init = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_1")
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 1, TORQUE -10")
    time.sleep(GENERIC_REACTION_WHEEL_TORQUE_RESPONSE_SLEEP)
    get_GENERIC_REACTION_WHEEL_data()
    if (rw1_momentum_init <= (-1 * GENERIC_REACTION_WHEEL_MAX_MOMENTUM_NM)):
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_1 <= {rw1_momentum_init}", 5)
    else:
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_1 < {rw1_momentum_init}", 5)
    
    rw1_momentum = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_1")
    print(f"Reaction Wheel 1 Momentum (N m): {rw1_momentum}")
        
    # Checking RW 2 Negative Direction
    rw2_momentum_init = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_2")
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 2, TORQUE -10")
    time.sleep(GENERIC_REACTION_WHEEL_TORQUE_RESPONSE_SLEEP)
    get_GENERIC_REACTION_WHEEL_data()
    if (rw2_momentum_init <= (-1 * GENERIC_REACTION_WHEEL_MAX_MOMENTUM_NM)):
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_2 <= {rw2_momentum_init}", 5)
    else:
        wait_check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_2 < {rw2_momentum_init}", 5)
    
    rw2_momentum = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T MOMENTUM_NMS_2")
    print(f"Reaction Wheel 2 Momentum (N m): {rw2_momentum}")

    # Confirm no errors
    get_GENERIC_REACTION_WHEEL_data()
    check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T ERROR_COUNT == {cmd_err_cnt}")

def confirm_GENERIC_REACTION_WHEEL_data_loop():
    for n in range(GENERIC_REACTION_WHEEL_DEVICE_LOOP_COUNT):
        confirm_GENERIC_REACTION_WHEEL_data()

def confirm_RW_device_data():
    dev_cnt_rw0 = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_COUNT_RW0")
    dev_cnt_rw1 = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_COUNT_RW1")
    dev_cnt_rw2 = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_COUNT_RW2")

    dev_err_cnt_RW0 = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0")
    dev_err_cnt_RW1 = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW1")
    dev_err_cnt_RW2 = tlm("GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW2")

    turn_off_RWS()

    get_GENERIC_REACTION_WHEEL_data()

    #RW0
    check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_COUNT_RW0 >= {dev_cnt_rw0}")
    check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0 == {dev_err_cnt_RW0}")

    ##RW1
    check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_COUNT_RW1 >= {dev_cnt_rw1}")
    check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW1 == {dev_err_cnt_RW1}") # Changed RW0 to RW1

    #RW2
    check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_COUNT_RW2 >= {dev_cnt_rw2}")
    check(f"GENERIC_REACTION_WHEEL_DEBUG GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW2 == {dev_err_cnt_RW2}") # Changed RW0 to RW2

def get_GENERIC_REACTION_WHEEL_data_loop():
    for n in range(GENERIC_REACTION_WHEEL_DEVICE_LOOP_COUNT):
        get_GENERIC_REACTION_WHEEL_data()

def teardown_RW():
    turn_off_RWS()

def enable_all_RW():
    cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_ENABLE_CC with WHEEL_NUMBER 0")
    cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_ENABLE_CC with WHEEL_NUMBER 1")
    cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_ENABLE_CC with WHEEL_NUMBER 2")

def disable_all_RW():
    cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_DISABLE_CC with WHEEL_NUMBER 0")
    cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_DISABLE_CC with WHEEL_NUMBER 1")
    cmd("GENERIC_REACTION_WHEEL_DEBUG GENERIC_RW_DISABLE_CC with WHEEL_NUMBER 2")

def generic_rw0_sim_disable():
    cmd("SIM_CMDBUS_BRIDGE GENERIC_RW0_DISABLE")

def generic_rw1_sim_disable():
    cmd("SIM_CMDBUS_BRIDGE GENERIC_RW1_DISABLE")

def generic_rw2_sim_disable():
    cmd("SIM_CMDBUS_BRIDGE GENERIC_RW2_DISABLE")

def generic_rw0_sim_enable():
    cmd("SIM_CMDBUS_BRIDGE GENERIC_RW0_ENABLE")

def generic_rw1_sim_enable():
    cmd("SIM_CMDBUS_BRIDGE GENERIC_RW1_ENABLE")

def generic_rw2_sim_enable():
    cmd("SIM_CMDBUS_BRIDGE GENERIC_RW2_ENABLE")

#
# Simulator Functions
#
def GENERIC_REACTION_WHEEL_prepare_ast():
    # Get to known state
    safe_GENERIC_REACTION_WHEEL()

    # Confirm data
    confirm_GENERIC_REACTION_WHEEL_data()

    teardown_RW()