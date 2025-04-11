# Library for GENERIC_REACTION_WHEEL Target
require 'cosmos'
require 'cosmos/script'

#
# Definitions
#
GENERIC_REACTION_WHEEL_CMD_SLEEP = 0.25
GENERIC_REACTION_WHEEL_RESPONSE_TIMEOUT = 5
GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT = 1
GENERIC_REACTION_WHEEL_DEVICE_LOOP_COUNT = 5

#
# Functions
#


def get_GENERIC_REACTION_WHEEL_data()
    cmd("GENERIC_REACTION_WHEEL GENERIC_RW_REQ_DATA_CC")
    wait_check_packet("GENERIC_REACTION_WHEEL", "GENERIC_REACTION_WHEEL_DATA_TLM", 1, GENERIC_REACTION_WHEEL_RESPONSE_TIMEOUT)
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



def safe_GENERIC_REACTION_WHEEL()
    get_GENERIC_REACTION_WHEEL_data()
    # GENERIC_REACTION_WHEEL_cmd()
end

def confirm_GENERIC_REACTION_WHEEL_data()
    cmd_cnt = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
    cmd_err_cnt = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")
    
    get_GENERIC_REACTION_WHEEL_data()
    # Note these checks assume default simulator configuration
    

    get_GENERIC_REACTION_WHEEL_data()
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT >= #{cmd_cnt}")
    check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT == #{cmd_err_cnt}")
end

def confirm_GENERIC_REACTION_WHEEL_data_loop()
    GENERIC_REACTION_WHEEL_DEVICE_LOOP_COUNT.times do |n|
        confirm_GENERIC_REACTION_WHEEL_data()
    end
end

#
# Simulator Functions
#
def GENERIC_REACTION_WHEEL_prepare_ast()
    # Get to known state
    safe_GENERIC_REACTION_WHEEL()

    # Confirm data
    confirm_GENERIC_REACTION_WHEEL_data_loop()
end

