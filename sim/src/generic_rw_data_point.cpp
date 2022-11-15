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

#include <generic_rw_data_point.hpp>

#include <ItcLogger/Logger.hpp>

namespace Nos3
{
    extern ItcLogger::Logger *sim_logger;

    //GenericRWDataPoint::GenericRWDataPoint(void) : SimIDataPoint()
    //{
    //    sim_logger->trace("GenericRWDataPoint::GenericRWDataPoint:  Constructor executed");
    //}

    GenericRWDataPoint::GenericRWDataPoint(int16_t spacecraft, int16_t reactionwheel, const boost::shared_ptr<Sim42DataPoint> dp) : 
        _sc(spacecraft), _reactionwheel(reactionwheel), _dp(*dp), _not_parsed(true) 
    {
        sim_logger->trace("GPSSimDataPoint::GPSSimDataPoint:  Created instance using _sc=%d, _gps=%d, _dp=%s", 
            _sc, _reactionwheel, _dp.to_string().c_str());
    }

    GenericRWDataPoint::~GenericRWDataPoint(void)
    {
        sim_logger->trace("GenericRWDataPoint::~GenericRWDataPoint:  Destructor executed");
    }

    void GenericRWDataPoint::do_parsing(void) const
    {
        std::ostringstream MatchString;
        MatchString << "SC[" << _sc << "].AC.Whl[" << _reactionwheel << "].";
        size_t MSsize = MatchString.str().size();

        _not_parsed = false;
        
        std::vector<std::string> lines = _dp.get_lines();
        
        try {
            for (int i = 0; i < lines.size(); i++) {
                if (lines[i].compare(0, MSsize, MatchString.str()) == 0) { // e.g. SC[0].AC.Whl[0]
                    sim_logger->trace("GenericRWDataPoint::do_parsing:  Found a string with the correct prefix = %s.  String:  %s", MatchString.str().c_str(), lines[i].c_str());
                    // Whl. H (momentum)
                    if (lines[i].compare(MSsize, 2, "H ") == 0) {
                        size_t found = lines[i].find_first_of("=");
                        _momentum = std::stod(lines[i].substr(found+1, lines[i].size() - found - 1));
                        sim_logger->trace("GenericRWDataPoint::do_parsing:  Parsed Momentum.  = found at %d, rhs=%s, _momentum=%f", 
                            found, lines[i].substr(found+1, lines[i].size() - found - 1).c_str(), _momentum);
                    }
                }
            }
        } catch(const std::exception& e) {
            sim_logger->error("GenericRWDataPoint::do_parsing:  Parsing exception:  %s", e.what());
        }
                
        sim_logger->trace("GenericRWDataPoint::do_parsing:  Parsed data point:\n%s", to_string().c_str());

    }

    std::string GenericRWDataPoint::to_string(void) const
    {
        parse_data_point();
        
        std::stringstream ss;

        ss << std::fixed << std::setfill(' ');
        ss << "Generic Reaction Wheel Data Point: ";
        ss << std::setprecision(std::numeric_limits<double>::digits10); // Full double precision
        ss << " Momentum: " << _momentum;

        return ss.str();
    }
}
