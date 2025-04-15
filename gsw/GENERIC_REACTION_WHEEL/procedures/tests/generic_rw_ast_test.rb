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

  generic_rw0_sim_disable()
  generic_rw1_sim_disable()
  generic_rw2_sim_disable()

  get_GENERIC_REACTION_WHEEL_data()
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT  == #{cmd_cnt}")
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT >= #{cmd_err_cnt}")

  generic_rw0_sim_enable()
  generic_rw1_sim_enable()
  generic_rw2_sim_enable()

  confirm_GENERIC_REACTION_WHEEL_data()


end 


puts "End of RW AST Test Script"
