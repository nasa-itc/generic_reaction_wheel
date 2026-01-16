#ifndef NOS3_GENERIC_RW_DATAPROVIDER_HPP
#define NOS3_GENERIC_RW_DATAPROVIDER_HPP

#include <boost/property_tree/ptree.hpp>
#include <ItcLogger/Logger.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <generic_rw_data_point.hpp>
#include <sim_data_42socket_provider.hpp>
#include <blackboard_data.hpp>

namespace Nos3
{
    namespace bip = boost::interprocess;

    /* Standard for a 42 data provider */
    class GenericRWShmemDataProvider : public SimData42SocketProvider
    {
    public:
        /* Constructors */
        GenericRWShmemDataProvider(const boost::property_tree::ptree& config);

        /* Accessors */
        boost::shared_ptr<SimIDataPoint> get_data_point(void) const;

    private:
        /* Disallow these */
        ~GenericRWShmemDataProvider(void) {};
        GenericRWShmemDataProvider& operator=(const GenericRWShmemDataProvider&) {return *this;};

        int16_t _sc;  /* Which spacecraft number to parse out of 42 data */
        int16_t _reactionwheel;

        bip::mapped_region _shm_region;
        BlackboardData*    _blackboard_data;
    };
}

#endif
