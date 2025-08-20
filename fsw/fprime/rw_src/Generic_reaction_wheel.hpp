// ======================================================================
// \title  Generic_reaction_wheel.hpp
// \author jstar
// \brief  hpp file for Generic_reaction_wheel component implementation class
// ======================================================================

#ifndef Components_Generic_reaction_wheel_HPP
#define Components_Generic_reaction_wheel_HPP

#include "rw_src/Generic_reaction_wheelComponentAc.hpp"
#include "rw_src/Generic_reaction_wheel_ActiveStateEnumAc.hpp"
#include "rw_src/Generic_reaction_wheel_wheelNumsEnumAc.hpp"

extern "C"{
#include "generic_reaction_wheel_device.h"
#include "libuart.h"
}

#include "nos_link.h"

#define RW_NUM 3

typedef struct
{
    uint8_t                         CommandCount;
    uint8_t                         CommandErrorCount;
    uint8_t                         DeviceCount[RW_NUM];
    uint8_t                         DeviceErrorCount[RW_NUM];
    uint8_t                         DeviceEnabled[RW_NUM];
    double                          momentum[RW_NUM];
} RW_Hk_tlm_t;
#define RW_HK_TLM_LNGTH sizeof(RW_Hk_tlm_t)

#define GENERIC_RW_DEVICE_DISABLED 0
#define GENERIC_RW_DEVICE_ENABLED  1

namespace Components {

  class Generic_reaction_wheel :
    public Generic_reaction_wheelComponentBase
  {

    public:

    uart_info_t RW_UART[RW_NUM] = {
      {.deviceString = &GENERIC_REACTION_WHEEL_1_CFG_STRING[0], .handle = GENERIC_REACTION_WHEEL_1_CFG_HANDLE, .isOpen = GENERIC_REACTION_WHEEL_1_CFG_IS_OPEN, .baud = GENERIC_REACTION_WHEEL_1_CFG_BAUDRATE_HZ},
      {.deviceString = &GENERIC_REACTION_WHEEL_2_CFG_STRING[0], .handle = GENERIC_REACTION_WHEEL_2_CFG_HANDLE, .isOpen = GENERIC_REACTION_WHEEL_2_CFG_IS_OPEN, .baud = GENERIC_REACTION_WHEEL_2_CFG_BAUDRATE_HZ},
      {.deviceString = &GENERIC_REACTION_WHEEL_3_CFG_STRING[0], .handle = GENERIC_REACTION_WHEEL_3_CFG_HANDLE, .isOpen = GENERIC_REACTION_WHEEL_3_CFG_IS_OPEN, .baud = GENERIC_REACTION_WHEEL_3_CFG_BAUDRATE_HZ},
    };

    RW_Hk_tlm_t HkTelemetryPkt;

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct Generic_reaction_wheel object
      Generic_reaction_wheel(
          const char* const compName //!< The component name
      );

      //! Destroy Generic_reaction_wheel object
      ~Generic_reaction_wheel();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      void NOOP_cmdHandler(
        FwOpcodeType opCode, 
        U32 cmdSeq
      ) override;

      void ENABLE_cmdHandler(
        FwOpcodeType opCode, 
        U32 cmdSeq,
        const Generic_reaction_wheel_wheelNums wheel_num
      ) override;

      void DISABLE_cmdHandler(
        FwOpcodeType opCode, 
        U32 cmdSeq,
        const Generic_reaction_wheel_wheelNums wheel_num
      ) override;

      void RESET_COUNTERS_cmdHandler(
        FwOpcodeType opCode, 
        U32 cmdSeq
      ) override;

      void REQUEST_DATA_cmdHandler(
        FwOpcodeType opCode, 
        U32 cmdSeq
      ) override;
      
      void SET_TORQUE_cmdHandler(
        FwOpcodeType opCode, //!< The opcode
        U32 cmdSeq, //!< The command sequence number
        const Generic_reaction_wheel_wheelNums wheel_num, //!< Reaction Wheel Number to set torque of
        const F64 torque //!< Torque to set reaction wheel to
      ) override;

      void updateData_handler(
        const NATIVE_INT_TYPE portNum, //!< The port number
        NATIVE_UINT_TYPE context //!< The call order
      ) override;

      void updateTlm_handler(
        const NATIVE_INT_TYPE portNum, //!< The port number
        NATIVE_UINT_TYPE context //!< The call order
      ) override;

      void RWin_handler(
        NATIVE_INT_TYPE portNum,
        F64 Torque0,
        F64 Torque1,
        F64 Torque2
      ) override;
      
      inline Generic_reaction_wheel_ActiveState get_active_state(uint8_t DeviceEnabled);

  };

}

#endif
