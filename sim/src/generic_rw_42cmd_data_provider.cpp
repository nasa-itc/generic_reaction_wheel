#include <generic_rw_42cmd_data_provider.hpp>

namespace Nos3
{
    extern ItcLogger::Logger *sim_logger;

    GenericRW42CmdDataProvider::GenericRW42CmdDataProvider(const boost::property_tree::ptree& config) : SimData42SocketProvider(config)
    {
        _sc = config.get("simulator.hardware-model.data-provider.spacecraft", 0);
    }

    void GenericRW42CmdDataProvider::cmd_torque(int rw, double trq)
    {
        std::stringstream ss;
        ss << "SC[" << _sc << "].Whl[" << rw << "].Tcmd = " << trq;
        sim_logger->debug("GenericRW42CmdDataProvider::cmd_torque:  buffer = %s\n", ss.str().c_str());

        send_command_to_socket(ss.str());
    }

}