require 'cosmos'
require 'cosmos/script'
require "generic_reaction_wheel_lib.rb"

##
## This script tests the cFS component device functionality.
## Currently this includes: 
##   Enable / disable, control hardware communications
##   Configuration, reconfigure sample instrument register
##


##
## Enable / disable, control hardware communications
##
GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT.times do |n|
  # Get to known state
  safe_GENERIC_REACTION_WHEEL()
  #resetting counters
  cmd("GENERIC_REACTION_WHEEL GENERIC_RW_RST_COUNTERS_CC")
  
  get_GENERIC_REACTION_WHEEL_data()

  dev_err_cnt_RW0 = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0")
  dev_err_cnt_RW1 = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW1")
  dev_err_cnt_RW2 = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW2")

  # Manually command to disable when already disabled
  #disable RWs watch error counters increase:
  disable_all_RW()

  get_GENERIC_REACTION_WHEEL_data()

  cmd_cnt = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
  cmd_err_cnt = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")

  #sending 3 torque commands (value set to 0)
  # turn_off_RWS()
  cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 0, TORQUE 0")
  cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 1, TORQUE 0")
  cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 2, TORQUE 0")

  get_GENERIC_REACTION_WHEEL_data()

  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0 == #{dev_err_cnt_RW0+1}")
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW1 == #{dev_err_cnt_RW1+1}")
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW2 == #{dev_err_cnt_RW2+1}")

  puts("command count is: #{cmd_cnt}")
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT == #{cmd_cnt}")
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT == #{cmd_err_cnt+3}") #+3 for 3 commands failed


  enable_all_RW()

  #confirming normal counters
  confirm_RW_device_data()
  
end
puts "End of RW Device Test Script"
