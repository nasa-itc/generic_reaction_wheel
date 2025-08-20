module Components {
    @ reaction wheel device control and monitoring
    active component Generic_reaction_wheel {

        @ RW output port
        output port RWout: RWDataPort

        @ Periodic Data RW
        async input port updateData: Svc.Sched

        @ Periodic Tlm RW
        async input port updateTlm: Svc.Sched

        @ RW input port
        async input port RWin: RWOUTDataPort
        
        @ Component Enable State
        enum ActiveState {
            DISABLED @< DISABLED
            ENABLED @< ENABLED
        }

        enum wheelNums {
            RW0 @< Reaction Wheel 0
            RW1 @< Reaction Wheel 1
            RW2 @< Reaction Wheel 2
        }

        @ Command to Request Momentum from all wheels
        async command REQUEST_DATA(
        )

        @ Command to Set Reaction Wheel Torque
        async command SET_TORQUE(
            wheel_num: wheelNums @< Reaction Wheel Number (0-2) to set torque of
            torque: F64 @< Torque to set reaction wheel to in 10^-4 Nm
        )

        @ NOOP Command
        async command NOOP(
        )

        @ Enable Cmd
        async command ENABLE(
            wheel_num: wheelNums @< RW Numbers (0-2)
        )

        @ Disable Cmd
        async command DISABLE(
            wheel_num: wheelNums @< RW Numbers (0-2)
        )

        @ Reset Counters Cmd
        async command RESET_COUNTERS()

        @ event with maximum greeting length of 30 characters
        event TELEM(
            log_info: string size 60 @< 
        ) severity activity high format "Generic_reaction_wheel: {}"

        @ Momentum of Reaction Wheel 0
        telemetry RW0_Data: F64

        @ Momentum of Reaction Wheel 1
        telemetry RW1_Data: F64

        @ Momentum of Reaction Wheel 2
        telemetry RW2_Data: F64

        @ Command Count
        telemetry CommandCount: U32

        @ Command Error Count
        telemetry CommandErrorCount: U32

        @ Device Count
        telemetry DeviceCountRW0: U32

        @ Device Error Count
        telemetry DeviceErrorCountRW0: U32

        @ Device Enabled
        telemetry DeviceEnabledRW0: ActiveState

        @ Device Count
        telemetry DeviceCountRW1: U32

        @ Device Error Count
        telemetry DeviceErrorCountRW1: U32

        @ Device Enabled
        telemetry DeviceEnabledRW1: ActiveState

        @ Device Count
        telemetry DeviceCountRW2: U32

        @ Device Error Count
        telemetry DeviceErrorCountRW2: U32

        @ Device Enabled
        telemetry DeviceEnabledRW2: ActiveState

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        # @ Example async command
        # async command COMMAND_NAME(param_name: U32)

        # @ Example telemetry counter
        # telemetry ExampleCounter: U64

        # @ Example event
        # event ExampleStateEvent(example_state: Fw.On) severity activity high id 0 format "State set to {}"

        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        # @ Example parameter
        # param PARAMETER_NAME: U32

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}