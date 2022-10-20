/* Copyright (C) 2016 - 2016 National Aeronautics and Space Administration. All Foreign Rights are Reserved to the U.S. Government.

   This software is provided "as is" without any warranty of any, kind either express, implied, or statutory, including, but not
   limited to, any warranty that the software will conform to, specifications any implied warranties of merchantability, fitness
   for a particular purpose, and freedom from infringement, and any warranty that the documentation will conform to the program, or
   any warranty that the software will be error free.

   In no event shall NASA be liable for any damages, including, but not limited to direct, indirect, special or consequential damages,
   arising out of, resulting from, or in any way connected with the software or its documentation.  Whether or not based upon warranty,
   contract, tort or otherwise, and whether or not loss was sustained from, or arose out of the results of, or use of, the software,
   documentation or services provided hereunder

   ITC Team
   NASA IV&V
   ivv-itc@lists.nasa.gov
*/

#ifndef GENERICRW42DATAPROVIDER_HPP
#define GENERICRW42DATAPROVIDER_HPP

#include <sim_data_42socket_provider.hpp>

namespace Nos3
{
    class GenericRWData42SocketProvider : public SimData42SocketProvider
    {
    public:
        GenericRWData42SocketProvider(const boost::property_tree::ptree& config);
        ~GenericRWData42SocketProvider(void);
        boost::shared_ptr<SimIDataPoint> get_data_point(void) const;
    private:
        // Private helper methods
        // Private data
        int16_t _sc;  // Which spacecraft number to parse out of 42 data
        int16_t _reactionwheel; // Which reaction wheel number to parse out of 42 data
    };
}

#endif
