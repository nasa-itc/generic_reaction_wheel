# Library for GENERIC_REACTION_WHEEL Target
require 'cosmos'
require 'cosmos/script'

#
# Definitions
#
GENERIC_REACTION_WHEEL_CMD_SLEEP = 0.25
GENERIC_REACTION_WHEEL_RESPONSE_TIMEOUT = 10
GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT = 1
GENERIC_REACTION_WHEEL_DEVICE_LOOP_COUNT = 5

#
# Functions
#


def get_GENERIC_REACTION_WHEEL_data()
    cmd("GENERIC_REACTION_WHEEL GENERIC_RW_REQ_DATA_CC")
    wait_check_packet("GENERIC_REACTION_WHEEL", "GENRW_HK_TLM_T", 1, GENERIC_REACTION_WHEEL_RESPONSE_TIMEOUT)
    sleep(GENERIC_REACTION_WHEEL_CMD_SLEEP)
end

def GENERIC_REACTION_WHEEL_cmd(*command)
    count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT") + 1

    if (count == 256)
        count = 0
    end

    cmd(*command)
    get_GENERIC_REACTION_WHEEL_data()
    current = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
    if (current != count)
        # Try again
        cmd(*command)
        get_GENERIC_REACTION_WHEEL_data()
        current = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
        if (current != count)
            # Third times the charm
            cmd(*command)
            get_GENERIC_REACTION_WHEEL_data()
            current = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
        end
    end
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT >= #{count}")
end

def turn_off_RWS
    #Turning off RW's
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 0, TORQUE 0")
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 1, TORQUE 0")
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 2, TORQUE 0")
end


def safe_GENERIC_REACTION_WHEEL()

    get_GENERIC_REACTION_WHEEL_data()

    #Turn off RWs, set to 0
    turn_off_RWS()
 
end

def confirm_GENERIC_REACTION_WHEEL_data()
    cmd_cnt = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
    cmd_err_cnt = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")
    
    get_GENERIC_REACTION_WHEEL_data()
   
    #Checking RW 0 Positive Direction
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 0, TORQUE 50")
    sleep 10
    get_GENERIC_REACTION_WHEEL_data_loop()
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_0 > 0")
    rw0_momentum = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_0")
    puts "Reaction Wheel 0 Momentum (N m): #{rw0_momentum}"
    
    #Checking RW 1 Positive Direction
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 1, TORQUE 50")
    sleep 10
    get_GENERIC_REACTION_WHEEL_data_loop()
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_1 > 0")
    rw1_momentum = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_1")
    puts "Reaction Wheel 1 Momentum (N m): #{rw1_momentum}"
     
    #Checking RW 2 Positive Direction
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 2, TORQUE 50")
    sleep 10
    get_GENERIC_REACTION_WHEEL_data_loop()
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_2 > 0")
    rw2_momentum = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_2")
    puts "Reaction Wheel 2 Momentum (N m): #{rw2_momentum}"

    #Checking RW 0 Negative Direction
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 0, TORQUE -100")
    sleep 10
    get_GENERIC_REACTION_WHEEL_data_loop()
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_0 < 0")
    rw0_momentum = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_0")
    puts "Reaction Wheel 0 Momentum (N m): #{rw0_momentum}"
    
    #Checking RW 1 Negative Direction
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 1, TORQUE -100")
    sleep 10
    get_GENERIC_REACTION_WHEEL_data_loop()
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_1 < 0")
    rw1_momentum = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_1")
    puts "Reaction Wheel 1 Momentum (N m): #{rw1_momentum}"
        
    #Checking RW 2 Negative Direction
    GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 2, TORQUE -100")
    sleep 10
    get_GENERIC_REACTION_WHEEL_data_loop()
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_2 < 0")
    rw2_momentum = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_2")
    puts "Reaction Wheel 2 Momentum (N m): #{rw2_momentum}"


    get_GENERIC_REACTION_WHEEL_data_loop()
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT >= #{cmd_cnt}")
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT == #{cmd_err_cnt}")
end

def confirm_GENERIC_REACTION_WHEEL_data_loop()
    GENERIC_REACTION_WHEEL_DEVICE_LOOP_COUNT.times do |n|
        confirm_GENERIC_REACTION_WHEEL_data()
    end
end

def confirm_RW_device_data()

    dev_cnt_rw0 = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_COUNT_RW0")
    dev_cnt_rw1 = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_COUNT_RW1")
    dev_cnt_rw2 = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_COUNT_RW2")

    dev_err_cnt_RW0 = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0")
    dev_err_cnt_RW1 = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW1")
    dev_err_cnt_RW2 = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW2")

    turn_off_RWS()

    get_GENERIC_REACTION_WHEEL_data()

    #RW0
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_COUNT_RW0 >= #{dev_cnt_rw0}")
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0 == #{dev_err_cnt_RW0}")

    #RW1
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_COUNT_RW1 >= #{dev_cnt_rw1}")
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0 == #{dev_err_cnt_RW1}")

    #RW2
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_COUNT_RW2 >= #{dev_cnt_rw2}")
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0 == #{dev_err_cnt_RW2}")



end

def get_GENERIC_REACTION_WHEEL_data_loop()
    GENERIC_REACTION_WHEEL_DEVICE_LOOP_COUNT.times do |n|
        get_GENERIC_REACTION_WHEEL_data()
    end
end

def teardown_RW

    turn_off_RWS()
end

def enable_all_RW()

    cmd("GENERIC_REACTION_WHEEL GENERIC_RW_ENABLE_CC with WHEEL_NUMBER 0")
    cmd("GENERIC_REACTION_WHEEL GENERIC_RW_ENABLE_CC with WHEEL_NUMBER 1")
    cmd("GENERIC_REACTION_WHEEL GENERIC_RW_ENABLE_CC with WHEEL_NUMBER 2")

end

def disable_all_RW()

    cmd("GENERIC_REACTION_WHEEL GENERIC_RW_DISABLE_CC with WHEEL_NUMBER 0")
    cmd("GENERIC_REACTION_WHEEL GENERIC_RW_DISABLE_CC with WHEEL_NUMBER 1")
    cmd("GENERIC_REACTION_WHEEL GENERIC_RW_DISABLE_CC with WHEEL_NUMBER 2")
    
end

def generic_rw0_sim_disable()
    cmd("SIM_CMDBUS_BRIDGE GENERIC_RW0_DISABLE")
end

def generic_rw1_sim_disable()
    cmd("SIM_CMDBUS_BRIDGE GENERIC_RW1_DISABLE")
end

def generic_rw2_sim_disable()
    cmd("SIM_CMDBUS_BRIDGE GENERIC_RW2_DISABLE")
end


def generic_rw0_sim_enable()
    cmd("SIM_CMDBUS_BRIDGE GENERIC_RW0_ENABLE")
end

def generic_rw1_sim_enable()
    cmd("SIM_CMDBUS_BRIDGE GENERIC_RW1_ENABLE")
end

def generic_rw2_sim_enable()
    cmd("SIM_CMDBUS_BRIDGE GENERIC_RW2_ENABLE")
end

#
# Simulator Functions
#
def GENERIC_REACTION_WHEEL_prepare_ast()
    # Get to known state
    safe_GENERIC_REACTION_WHEEL()

    # Confirm data
    confirm_GENERIC_REACTION_WHEEL_data()

    teardown_RW()
end

