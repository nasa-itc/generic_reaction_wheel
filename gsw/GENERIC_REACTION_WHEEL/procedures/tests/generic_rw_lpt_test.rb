require 'cosmos'
require 'cosmos/script'
require "cfs_lib.rb"


# initial_command_count = tlm("SAMPLE SAMPLE_HK_TLM CMD_COUNT")
# initial_error_count = tlm("SAMPLE SAMPLE_HK_TLM CMD_ERR_COUNT")
# initial_device_count = tlm("SAMPLE SAMPLE_HK_TLM DEVICE_COUNT")
# initial_device_error_count = tlm("SAMPLE SAMPLE_HK_TLM DEVICE_ERR_COUNT")
# cmd("SAMPLE SAMPLE_NOOP_CC")
# wait_check("SAMPLE SAMPLE_HK_TLM CMD_COUNT > #{initial_command_count}", 30)
# wait_check("SAMPLE SAMPLE_HK_TLM CMD_ERR_COUNT == #{initial_error_count}", 30)
# wait_check("SAMPLE SAMPLE_HK_TLM DEVICE_COUNT == #{initial_device_count}", 30)
# wait_check("SAMPLE SAMPLE_HK_TLM DEVICE_ERR_COUNT == #{initial_device_error_count}", 30)

# sleep(5)

# initial_command_count = tlm("SAMPLE SAMPLE_HK_TLM CMD_COUNT")
# initial_error_count = tlm("SAMPLE SAMPLE_HK_TLM CMD_ERR_COUNT")
# initial_device_count = tlm("SAMPLE SAMPLE_HK_TLM DEVICE_COUNT")
# initial_device_error_count = tlm("SAMPLE SAMPLE_HK_TLM DEVICE_ERR_COUNT")
# cmd("SAMPLE SAMPLE_RST_COUNTERS_CC")
# wait_check("SAMPLE SAMPLE_HK_TLM CMD_COUNT == 0", 30)
# wait_check("SAMPLE SAMPLE_HK_TLM CMD_ERR_COUNT == 0", 30)
# wait_check("SAMPLE SAMPLE_HK_TLM DEVICE_COUNT == 0", 30)
# wait_check("SAMPLE SAMPLE_HK_TLM DEVICE_ERR_COUNT == 0", 30)