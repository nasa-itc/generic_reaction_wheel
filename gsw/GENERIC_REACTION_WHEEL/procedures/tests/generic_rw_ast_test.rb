require 'cosmos'
require 'cosmos/script'
require "generic_reaction_wheel_lib.rb"

##
## Hardware failure
##

GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT.times do |n|

  #prepare
  GENERIC_REACTION_WHEEL_prepare_ast()

  cmd_cnt = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
  cmd_err_cnt = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")
  dev_err_cnt_RW0 = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0")
  dev_err_cnt_RW1 = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW1")
  dev_err_cnt_RW2 = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW2")

  generic_rw0_sim_disable()
  generic_rw1_sim_disable()
  generic_rw2_sim_disable()

  turn_off_RWS()

  get_GENERIC_REACTION_WHEEL_data()
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT  == #{cmd_cnt}")
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT >= #{cmd_err_cnt}")

  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW0 >= #{dev_err_cnt_RW0}")
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW1 >= #{dev_err_cnt_RW1}")
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T DEVICE_ERR_COUNT_RW2 >= #{dev_err_cnt_RW2}")

  generic_rw0_sim_enable()
  generic_rw1_sim_enable()
  generic_rw2_sim_enable()

  confirm_GENERIC_REACTION_WHEEL_data()


end 


puts "End of RW AST Test Script"
