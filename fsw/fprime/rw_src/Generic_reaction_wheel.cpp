// ======================================================================
// \title  Generic_reaction_wheel.cpp
// \author jstar
// \brief  cpp file for Generic_reaction_wheel component implementation class
// ======================================================================

/*
** Include Files
*/
#include "rw_src/Generic_reaction_wheel.hpp"
#include "FpConfig.hpp"


namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  Generic_reaction_wheel ::
    Generic_reaction_wheel(const char* const compName) :
      Generic_reaction_wheelComponentBase(compName)
  {
    uint32_t status = OS_SUCCESS;

    /* Initialize HWLIB */
    nos_init_link();

    /* Connect to the UART */
    status = uart_init_port(&RW_UART[0]);
    if(status != OS_SUCCESS)
    {
    	OS_printf("GENERIC_RW Checkout: UART 0 port initialization error!\n");
    }    
    status = uart_init_port(&RW_UART[1]);
    if(status != OS_SUCCESS)
    {
    	OS_printf("GENERIC_RW Checkout: UART 1 port initialization error!\n");
    }    
    status = uart_init_port(&RW_UART[2]);
    if(status != OS_SUCCESS)
    {
    	OS_printf("GENERIC_RW Checkout: UART 2 port initialization error!\n");
    } 

    for ( int i = 0; i < RW_NUM; i++ )
    {
        uart_close_port(&RW_UART[i]);
    }

    HkTelemetryPkt.CommandCount = 0;
    HkTelemetryPkt.CommandErrorCount = 0;

    for(int i = 0; i < RW_NUM; i++){
      HkTelemetryPkt.DeviceCount[i] = 0;
      HkTelemetryPkt.DeviceErrorCount[i] = 0;
      HkTelemetryPkt.DeviceEnabled[i] = GENERIC_RW_DEVICE_DISABLED;
    }
  }

  Generic_reaction_wheel ::
    ~Generic_reaction_wheel()
  {
    // Close the devices
    for ( int i = 0; i < RW_NUM; i++ )
    {
        uart_close_port(&RW_UART[i]);
    }

    nos_destroy_link();

    OS_printf("Cleanly exiting generic_reaction_wheel application...\n\n"); 
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void Generic_reaction_wheel :: NOOP_cmdHandler(FwOpcodeType opCode, U32 cmdSeq){
    HkTelemetryPkt.CommandCount++;

    this->log_ACTIVITY_HI_TELEM("NOOP command success!");
    OS_printf("NOOP command successful!\n");

    this->tlmWrite_CommandCount(HkTelemetryPkt.CommandCount);
    this->tlmWrite_DeviceEnabledRW0(get_active_state(HkTelemetryPkt.DeviceEnabled[0]));
    this->tlmWrite_DeviceEnabledRW1(get_active_state(HkTelemetryPkt.DeviceEnabled[1]));
    this->tlmWrite_DeviceEnabledRW2(get_active_state(HkTelemetryPkt.DeviceEnabled[2]));
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void Generic_reaction_wheel :: RESET_COUNTERS_cmdHandler(FwOpcodeType opCode, U32 cmdSeq){
    HkTelemetryPkt.CommandCount = 0;
    HkTelemetryPkt.CommandErrorCount = 0;
    HkTelemetryPkt.DeviceCount[0] = 0;
    HkTelemetryPkt.DeviceErrorCount[0] = 0;
    HkTelemetryPkt.DeviceCount[1] = 0;
    HkTelemetryPkt.DeviceErrorCount[1] = 0;
    HkTelemetryPkt.DeviceCount[2] = 0;
    HkTelemetryPkt.DeviceErrorCount[2] = 0;

    this->log_ACTIVITY_HI_TELEM("Reset Counters command successful!");
    OS_printf("Reset Counters command successful!\n");
    this->tlmWrite_CommandCount(HkTelemetryPkt.CommandCount);
    this->tlmWrite_CommandErrorCount(HkTelemetryPkt.CommandErrorCount);

    this->tlmWrite_DeviceCountRW0(HkTelemetryPkt.DeviceCount[0]);
    this->tlmWrite_DeviceErrorCountRW0(HkTelemetryPkt.DeviceErrorCount[0]);
    this->tlmWrite_DeviceEnabledRW0(get_active_state(HkTelemetryPkt.DeviceEnabled[0]));

    this->tlmWrite_DeviceCountRW1(HkTelemetryPkt.DeviceCount[1]);
    this->tlmWrite_DeviceErrorCountRW1(HkTelemetryPkt.DeviceErrorCount[1]);
    this->tlmWrite_DeviceEnabledRW1(get_active_state(HkTelemetryPkt.DeviceEnabled[1]));

    this->tlmWrite_DeviceCountRW2(HkTelemetryPkt.DeviceCount[2]);
    this->tlmWrite_DeviceErrorCountRW2(HkTelemetryPkt.DeviceErrorCount[2]);
    this->tlmWrite_DeviceEnabledRW2(get_active_state(HkTelemetryPkt.DeviceEnabled[2]));

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void Generic_reaction_wheel :: ENABLE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, const Generic_reaction_wheel_wheelNums wheel_num){

    int32_t status = OS_SUCCESS;

    if(HkTelemetryPkt.DeviceEnabled[wheel_num.e] == GENERIC_RW_DEVICE_DISABLED)
    {
      HkTelemetryPkt.CommandCount++;

      status = uart_init_port(&RW_UART[wheel_num.e]);
      if(status == OS_SUCCESS)
      {
        HkTelemetryPkt.DeviceCount[wheel_num.e]++;
        HkTelemetryPkt.DeviceEnabled[wheel_num.e] = GENERIC_RW_DEVICE_ENABLED;
        char configMsg[40];
        sprintf(configMsg, "Enabled RW%d successfully!", wheel_num.e);
        this->log_ACTIVITY_HI_TELEM(configMsg);
        OS_printf("Enabled RW%d successfully!\n", wheel_num.e);
      }
      else
      {
        HkTelemetryPkt.DeviceErrorCount[wheel_num.e]++;
        char configMsg[40];
        sprintf(configMsg, "Enable RW%d failed, uart init fail!", wheel_num.e);
        this->log_ACTIVITY_HI_TELEM(configMsg);
        OS_printf("Enable RW%d failed, uart init fail!\n", wheel_num.e);
      }
    }
    else
    {
      HkTelemetryPkt.CommandErrorCount++;
      char configMsg[40];
        sprintf(configMsg, "Enable RW%d failed, already enabled!", wheel_num.e);
        this->log_ACTIVITY_HI_TELEM(configMsg);
        OS_printf("Enable RW%d failed, already enabled!\n", wheel_num.e);
    }

    this->tlmWrite_DeviceCountRW0(HkTelemetryPkt.DeviceCount[0]);
    this->tlmWrite_DeviceErrorCountRW0(HkTelemetryPkt.DeviceErrorCount[0]);
    this->tlmWrite_DeviceEnabledRW0(get_active_state(HkTelemetryPkt.DeviceEnabled[0]));

    this->tlmWrite_DeviceCountRW1(HkTelemetryPkt.DeviceCount[1]);
    this->tlmWrite_DeviceErrorCountRW1(HkTelemetryPkt.DeviceErrorCount[1]);
    this->tlmWrite_DeviceEnabledRW1(get_active_state(HkTelemetryPkt.DeviceEnabled[1]));

    this->tlmWrite_DeviceCountRW2(HkTelemetryPkt.DeviceCount[2]);
    this->tlmWrite_DeviceErrorCountRW2(HkTelemetryPkt.DeviceErrorCount[2]);
    this->tlmWrite_DeviceEnabledRW2(get_active_state(HkTelemetryPkt.DeviceEnabled[2]));
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void Generic_reaction_wheel :: DISABLE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, const Generic_reaction_wheel_wheelNums wheel_num){
    int32_t status = OS_SUCCESS;

    if(HkTelemetryPkt.DeviceEnabled[wheel_num.e] == GENERIC_RW_DEVICE_ENABLED)
    {
      HkTelemetryPkt.CommandCount++;

      status = uart_close_port(&RW_UART[wheel_num.e]);
      if(status == OS_SUCCESS)
      {
        HkTelemetryPkt.DeviceCount[wheel_num.e]++;
        HkTelemetryPkt.DeviceEnabled[wheel_num.e] = GENERIC_RW_DEVICE_DISABLED;
        char configMsg[40];
        sprintf(configMsg, "Disabled RW%d successfully!", wheel_num.e);
        this->log_ACTIVITY_HI_TELEM(configMsg);
        OS_printf("Disabled RW%d successfully!\n", wheel_num.e);
      }
      else
      {
        HkTelemetryPkt.DeviceErrorCount[wheel_num.e]++;
        char configMsg[40];
        sprintf(configMsg, "Disable RW%d failed, uart close fail!", wheel_num.e);
        this->log_ACTIVITY_HI_TELEM(configMsg);
        OS_printf("Disable RW%d failed, uart close fail!\n", wheel_num.e);
      }
    }
    else
    {
      HkTelemetryPkt.CommandErrorCount++;
      char configMsg[40];
        sprintf(configMsg, "Disable RW%d failed, already Disabled!", wheel_num.e);
        this->log_ACTIVITY_HI_TELEM(configMsg);
        OS_printf("Disable RW%d failed, already Disabled!\n", wheel_num.e);
    }

    this->tlmWrite_DeviceCountRW0(HkTelemetryPkt.DeviceCount[0]);
    this->tlmWrite_DeviceErrorCountRW0(HkTelemetryPkt.DeviceErrorCount[0]);
    this->tlmWrite_DeviceEnabledRW0(get_active_state(HkTelemetryPkt.DeviceEnabled[0]));

    this->tlmWrite_DeviceCountRW1(HkTelemetryPkt.DeviceCount[1]);
    this->tlmWrite_DeviceErrorCountRW1(HkTelemetryPkt.DeviceErrorCount[1]);
    this->tlmWrite_DeviceEnabledRW1(get_active_state(HkTelemetryPkt.DeviceEnabled[1]));

    this->tlmWrite_DeviceCountRW2(HkTelemetryPkt.DeviceCount[2]);
    this->tlmWrite_DeviceErrorCountRW2(HkTelemetryPkt.DeviceErrorCount[2]);
    this->tlmWrite_DeviceEnabledRW2(get_active_state(HkTelemetryPkt.DeviceEnabled[2]));


    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  // GENERIC_REACTION_WHEEL_REQUEST_DATA
  void Generic_reaction_wheel :: REQUEST_DATA_cmdHandler(FwOpcodeType opCode, U32 cmdSeq)
  {

    int32_t status = OS_SUCCESS;

    for(int i = 0; i < RW_NUM; i++){
      status = GetCurrentMomentum(&RW_UART[i], &HkTelemetryPkt.momentum[i]);
      if (status < 0)
      {
        char configMsg[40];
        sprintf(configMsg, "Failed to get momentum for RW%d!", i);
        this->log_ACTIVITY_HI_TELEM(configMsg);
        OS_printf("Failed to get momentum for RW%d!\n", i);
        HkTelemetryPkt.DeviceErrorCount[i]++;
      }
      else
      {
        char configMsg[40];
        sprintf(configMsg, "Successfully got momentum for RW%d!", i);
        this->log_ACTIVITY_HI_TELEM(configMsg);
        OS_printf("Successfully got momentum for RW%d!\n", i);
        HkTelemetryPkt.DeviceCount[i]++;
      }

      switch (i)
      {
      case 0:
        this->tlmWrite_RW0_Data(HkTelemetryPkt.momentum[i]);
        break;
      case 1:
        this->tlmWrite_RW1_Data(HkTelemetryPkt.momentum[i]);
        break;
      case 2:
        this->tlmWrite_RW2_Data(HkTelemetryPkt.momentum[i]);
        break;
      
      default:
        break;
      }
    }
    
    this->tlmWrite_CommandCount(++HkTelemetryPkt.CommandCount);
    this->tlmWrite_DeviceCountRW0(HkTelemetryPkt.DeviceCount[0]);
    this->tlmWrite_DeviceErrorCountRW0(HkTelemetryPkt.DeviceErrorCount[0]);
    this->tlmWrite_DeviceCountRW1(HkTelemetryPkt.DeviceCount[1]);
    this->tlmWrite_DeviceErrorCountRW1(HkTelemetryPkt.DeviceErrorCount[1]);
    this->tlmWrite_DeviceCountRW2(HkTelemetryPkt.DeviceCount[2]);
    this->tlmWrite_DeviceErrorCountRW2(HkTelemetryPkt.DeviceErrorCount[2]);
    // Tell the fprime command system that we have completed the processing of the supplied command with OK status
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  // GENERIC_REACTION_WHEEL_Set_Torque
  void Generic_reaction_wheel :: SET_TORQUE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, const Generic_reaction_wheel_wheelNums wheel_num, const F64 torque) 
  {
    int32_t status = OS_SUCCESS;

    double scaledTorque = torque / 10000;

    if(HkTelemetryPkt.DeviceEnabled[wheel_num.e] == GENERIC_RW_DEVICE_ENABLED){

      status = SetRWTorque(&RW_UART[wheel_num.e], scaledTorque);
      if (status < 0)
      {   
        char configMsg[40];
        sprintf(configMsg, "Failed to set torque for RW%d!", wheel_num.e);
        this->log_ACTIVITY_HI_TELEM(configMsg);  
        OS_printf("GENERIC_REACTION_WHEEL_SetTorque command failed for RW %d!\n", wheel_num.e);
        HkTelemetryPkt.DeviceErrorCount[wheel_num.e]++;
        HkTelemetryPkt.CommandErrorCount++;
      }
      else
      {
        char configMsg[40];
        sprintf(configMsg, "Successfully set torque for RW%d!", wheel_num.e);
        this->log_ACTIVITY_HI_TELEM(configMsg); 
        OS_printf("RW %d torque successfully set to %lf Nm\n", wheel_num.e, scaledTorque);
        HkTelemetryPkt.DeviceCount[wheel_num.e]++;
        HkTelemetryPkt.CommandCount++;
        HkTelemetryPkt.momentum[wheel_num.e] = scaledTorque;
      }    
      
    }
    else
    {
      HkTelemetryPkt.CommandErrorCount++;
      this->log_ACTIVITY_HI_TELEM("Command Failed, Device Disabled!");  
      OS_printf("Command Failed, Device Disabled!\n");
      
    }

    this->tlmWrite_CommandCount(++HkTelemetryPkt.CommandCount);
    this->tlmWrite_DeviceCountRW0(HkTelemetryPkt.DeviceCount[0]);
    this->tlmWrite_DeviceErrorCountRW0(HkTelemetryPkt.DeviceErrorCount[0]);
    this->tlmWrite_DeviceCountRW1(HkTelemetryPkt.DeviceCount[1]);
    this->tlmWrite_DeviceErrorCountRW1(HkTelemetryPkt.DeviceErrorCount[1]);
    this->tlmWrite_DeviceCountRW2(HkTelemetryPkt.DeviceCount[2]);
    this->tlmWrite_DeviceErrorCountRW2(HkTelemetryPkt.DeviceErrorCount[2]);

    this->tlmWrite_RW0_Data(HkTelemetryPkt.momentum[0]);
    this->tlmWrite_RW1_Data(HkTelemetryPkt.momentum[1]);
    this->tlmWrite_RW2_Data(HkTelemetryPkt.momentum[2]);
      // Tell the fprime command system that we have completed the processing of the supplied command with OK status
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    
  }

  inline Generic_reaction_wheel_ActiveState Generic_reaction_wheel :: get_active_state(uint8_t DeviceEnable)
  {
    Generic_reaction_wheel_ActiveState state;

    if(DeviceEnable == GENERIC_RW_DEVICE_ENABLED)
    {
      state.e = Generic_reaction_wheel_ActiveState::ENABLED;
    }
    else
    {
      state.e = Generic_reaction_wheel_ActiveState::DISABLED;
    }

    return state;
  }

}
