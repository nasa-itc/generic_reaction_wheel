#include <generic_rw_shmem_data_provider.hpp>

namespace Nos3
{
    REGISTER_DATA_PROVIDER(GenericRWShmemDataProvider,"GENERIC_RW_SHMEM_PROVIDER");

    extern ItcLogger::Logger *sim_logger;

    GenericRWShmemDataProvider::GenericRWShmemDataProvider(const boost::property_tree::ptree& config) : SimData42SocketProvider(config)
    {
        sim_logger->trace("GenericRWShmemDataProvider::GenericRWShmemDataProvider:  Constructor executed");
        const std::string shm_name = config.get("simulator.hardware-model.data-provider.shared-memory-name", "Blackboard");
        const size_t shm_size = sizeof(BlackboardData);
        bip::shared_memory_object shm(bip::open_or_create, shm_name.c_str(), bip::read_write);
        shm.truncate(shm_size);
        bip::mapped_region shm_region(shm, bip::read_write);
        _shm_region = std::move(shm_region); // don't let this go out of scope/get destroyed
        _blackboard_data = static_cast<BlackboardData*>(_shm_region.get_address());
        _sc = config.get("simulator.hardware-model.data-provider.spacecraft", 0);
        _reactionwheel = config.get("simulator.hardware-model.data-provider.reactionwheel", 0);
    }

    boost::shared_ptr<SimIDataPoint> GenericRWShmemDataProvider::get_data_point(void) const
    {
        sim_logger->trace("GenericRWShmemDataProvider::get_data_point:  Executed");

        /* Get the 42 data */
        boost::shared_ptr<GenericRWDataPoint> dp;
        {
            dp = boost::shared_ptr<GenericRWDataPoint>(
                new GenericRWDataPoint(_sc, _reactionwheel, _blackboard_data->WhlH[_reactionwheel])
            );
        }
        return dp;
    }
}
