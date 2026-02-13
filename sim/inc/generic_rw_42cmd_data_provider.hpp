#ifndef NOS3_GENERIC_RW_42CMD_DATAPROVIDER_HPP
#define NOS3_GENERIC_RW_42CMD_DATAPROVIDER_HPP

#include <boost/property_tree/ptree.hpp>
#include <ItcLogger/Logger.hpp>
#include <sim_data_42socket_provider.hpp>

namespace Nos3
{
    /* Standard for a 42 data provider */
    class GenericRW42CmdDataProvider : public SimData42SocketProvider
    {
    public:
        /* Constructors */
        GenericRW42CmdDataProvider(const boost::property_tree::ptree& config);
        ~GenericRW42CmdDataProvider(void) {};

        void cmd_torque(int rw, double trq);

    protected:
        int16_t _sc;  /* Which spacecraft number to command in 42 */

    private:
        /* Disallow these */
        GenericRW42CmdDataProvider& operator=(const GenericRW42CmdDataProvider&) {return *this;};
    };
}

#endif
