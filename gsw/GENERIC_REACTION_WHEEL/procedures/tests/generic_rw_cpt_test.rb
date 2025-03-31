require 'cosmos'
require 'cosmos/script'
require "cfs_lib.rb"


#Testing NOOP
initial_command_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
initial_error_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")

cmd("GENERIC_REACTION_WHEEL GENERIC_RW_NOOP_CC")
cmd("GENERIC_REACTION_WHEEL GENERIC_RW_REQ_DATA_CC")

wait_check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT > #{initial_command_count + 1}", 30)
wait_check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT == #{initial_error_count}", 30)

sleep 3

#RW 0 Setting Torque
initial_command_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
initial_error_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")

cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 0, TORQUE 10")
cmd("GENERIC_REACTION_WHEEL GENERIC_RW_REQ_DATA_CC")
wait_check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT > #{initial_command_count + 1}", 30)
wait_check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT == #{initial_error_count}", 30)

rw0_momentum = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_0")
puts "Reaction Wheel 0 Momentum (N m): #{rw0_momentum}"

sleep 3

#RW 1 Setting Torque
initial_command_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
initial_error_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")
cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 1, TORQUE 10")
cmd("GENERIC_REACTION_WHEEL GENERIC_RW_REQ_DATA_CC")
wait_check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT > #{initial_command_count + 1}", 30)
wait_check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT == #{initial_error_count}", 30)

rw1_momentum = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_0")
puts "Reaction Wheel 1 Momentum (N m): #{rw1_momentum}"

sleep 3

#RW 2 Setting Torque
initial_command_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
initial_error_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")

cmd("GENERIC_REACTION_WHEEL GENERIC_RW_SET_TORQUE_CC with WHEEL_NUMBER 2, TORQUE 10")
cmd("GENERIC_REACTION_WHEEL GENERIC_RW_REQ_DATA_CC")
wait_check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT > #{initial_command_count + 1}", 30)
wait_check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT == #{initial_error_count}", 30)

rw2_momentum = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T MOMENTUM_NMS_0")
puts "Reaction Wheel 2 Momentum (N m): #{rw2_momentum}"

sleep 3

#Resetting counters
initial_command_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
initial_error_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")

cmd("GENERIC_REACTION_WHEEL GENERIC_RW_RST_COUNTERS_CC")
cmd("GENERIC_REACTION_WHEEL GENERIC_RW_REQ_DATA_CC")

sleep 1

reset_command_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
reset_error_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")

wait_check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT < #{initial_command_count}", 30)
wait_check_expression("reset_command_count <= initial_command_count # #{reset_command_count} <= #{initial_command_count}", 15)
wait_check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT <= #{initial_error_count}", 30)

sleep 5

puts "End of RW CPT Script"
