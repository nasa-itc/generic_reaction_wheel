require 'cosmos'
require 'cosmos/script'
require "generic_reaction_wheel_lib.rb"

##
## Hardware failure
##
SAMPLE_TEST_LOOP_COUNT.times do |n|
  # Prepare
  sample_prepare_ast()

  # Disable sim and confirm device error counts increase
  dev_cmd_cnt = tlm("SAMPLE SAMPLE_HK_TLM DEVICE_COUNT")
  dev_cmd_err_cnt = tlm("SAMPLE SAMPLE_HK_TLM DEVICE_ERR_COUNT")
  sample_sim_disable()
  check("SAMPLE SAMPLE_HK_TLM DEVICE_COUNT == #{dev_cmd_cnt}")
  check("SAMPLE SAMPLE_HK_TLM DEVICE_ERR_COUNT >= #{dev_cmd_err_cnt}")

  # Enable sim and confirm return to nominal operation
  sample_sim_enable()
  confirm_sample_data_loop()
end

GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT.times do |n|

  #prepare
  GENERIC_REACTION_WHEEL_prepare_ast()

  cmd_cnt = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
  cmd_err_cnt = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")

  generic_rw0_sim_disable()
  generic_rw1_sim_disable()
  generic_rw2_sim_disable()

  turn_off_RWS()

  get_GENERIC_REACTION_WHEEL_data()
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT  == #{cmd_cnt}")
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT >= #{cmd_err_cnt}")

  generic_rw0_sim_enable()
  generic_rw1_sim_enable()
  generic_rw2_sim_enable()

  confirm_GENERIC_REACTION_WHEEL_data()


end 


puts "End of RW AST Test Script"
