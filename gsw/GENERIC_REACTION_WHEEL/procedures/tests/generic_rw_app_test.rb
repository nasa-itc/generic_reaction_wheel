require 'cosmos'
require 'cosmos/script'
require "generic_reaction_wheel_lib.rb"

#Turning off RW's
safe_GENERIC_REACTION_WHEEL()

##
# get tlm packet for Reaction wheel
##
GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT.times do |n|
  get_GENERIC_REACTION_WHEEL_data()
end

##
## Confirm NOOP command
##
GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT.times do |n|
  GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL GENERIC_RW_NOOP_CC")
end

##
## Confirm RW Data
## 
GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT.times do |n|
  #Testing RW commands and detecting Momentum directions
  confirm_GENERIC_REACTION_WHEEL_data()
end

##
## reset counters
##
GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT.times do |n|
  GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL GENERIC_RW_NOOP_CC")
  #ADCS May be running, so saving variables before reset
  initial_command_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
  initial_error_count = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")

  cmd("GENERIC_REACTION_WHEEL GENERIC_RW_RST_COUNTERS_CC")

  get_GENERIC_REACTION_WHEEL_data()
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT  < #{initial_command_count}")
  check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT <= #{initial_error_count}")

end

##
##   Invalid ground command, confirm bad lengths and codes are rejected
##
GENERIC_REACTION_WHEEL_TEST_LOOP_COUNT.times do |n|
   GENERIC_REACTION_WHEEL_cmd("GENERIC_REACTION_WHEEL GENERIC_RW_NOOP_CC")
   # Bad length
   cmd_cnt = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT")
   cmd_err_cnt = tlm("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT")
   cmd("GENERIC_REACTION_WHEEL GENERIC_RW_NOOP_CC with CCSDS_LENGTH #{n+2}")  # Note +2 due to CCSDS already being +1
   get_GENERIC_REACTION_WHEEL_data()
   check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T COMMAND_COUNT  == #{cmd_cnt}")
   check("GENERIC_REACTION_WHEEL GENRW_HK_TLM_T ERROR_COUNT == #{cmd_err_cnt+1}")
end

puts "End of RW App Test Script"
