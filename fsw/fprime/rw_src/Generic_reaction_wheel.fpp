module Components {
    @ reaction wheel device control and monitoring
    active component Generic_reaction_wheel {

        @ Command to Request Momentum from all wheels
        async command GET_MOMENTUM(
        )

        @ Command to Set Reaction Wheel Torque
        async command SET_TORQUE(
            wheel_num: I16 @< Reaction Wheel Number (0-2) to set torque of
            torque: F64 @< Torque to set reaction wheel to
        )

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