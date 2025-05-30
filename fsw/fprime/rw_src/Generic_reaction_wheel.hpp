// ======================================================================
// \title  Generic_reaction_wheel.hpp
// \author jstar
// \brief  hpp file for Generic_reaction_wheel component implementation class
// ======================================================================

#ifndef Components_Generic_reaction_wheel_HPP
#define Components_Generic_reaction_wheel_HPP

#include "rw_src/Generic_reaction_wheelComponentAc.hpp"

namespace Components {

  class Generic_reaction_wheel :
    public Generic_reaction_wheelComponentBase
  {

    public:

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

      //! Handler implementation for command GET_MOMENTUM
      //!
      //! Gets the Momentum Values from all 3 reaction wheels
      void GET_MOMENTUM_cmdHandler(
        FwOpcodeType opCode, 
        U32 cmdSeq
      ) override;
      
      //! Handler implementation for command SET_TORQUE
      //!
      //! Sets the torque of a given reaction wheel. Takes input of reaction wheel number (0-2) and torque
      void SET_TORQUE_cmdHandler(
        FwOpcodeType opCode, //!< The opcode
        U32 cmdSeq, //!< The command sequence number
        const I16 wheel_num, //!< Reaction Wheel Number to set torque of
        const F64 torque //!< Torque to set reaction wheel to
      ) override;

  };

}

#endif
