#ifndef VEHICLESET_HPP_
#define VEHICLESET_HPP_

#include "commonTypes.hpp"

#include <vector>

using namespace std;

namespace GTPP
{

  class THeterogeneousVehicleSet
  {
  public:

    //** Default constructor */
    THeterogeneousVehicleSet(void) {}

    //** Copy constructor */
    THeterogeneousVehicleSet(const THeterogeneousVehicleSet &g):
        Capacities_(g.Capacities_),
        FCost_(g.FCost_),
        Cost_(g.Cost_),
        Avail_(g.Avail_)
    {}

    //** Destructor */
    virtual ~THeterogeneousVehicleSet(void) {}

    //** Number of vehicles */
    inline int nVehicles(void){ return Capacities_.size();}

    /** Capacities of vehicles */
    inline int getCapacity(TvehicleId_ v){return Capacities_[v];}
    
    inline const vector<Tcapacity_>& getCapacity(void){return Capacities_;}

    /** Fixed cost for each vehicle */
    inline double getFCost(TvehicleId_ v){return FCost_[v];}

    /** Cost per distance unit */
    inline double getCost(TvehicleId_ v){return Cost_[v];}

    /** Capacities of vehicles */
    inline void setCapacity(TvehicleId_ v,Tcapacity_ q){Capacities_[v]=q;}

    /** number of vehicles */
    inline void setAvailability(TvehicleId_ v,unsigned int av){Avail_[v]=av;}

    /** Fixed cost for each vehicle */
    inline void setFCost(TvehicleId_ v,Tcost_ c){FCost_[v]=c;}

    /** Cost per distance unit */
    inline void setCost(TvehicleId_ v,Tcost_ c){Cost_[v]=c;}

    /** Availability for vehicle v*/
    inline int getAvail(TvehicleId_ v){return Avail_[v];}

    /** Init vehicles arrays */
    void initVehicles(Tsize_ nV)
    {
      Capacities_.resize(nV);
      FCost_.resize(nV);
      Cost_.resize(nV);
      Avail_.resize(nV,99999);
    }
    
  public:

    /** Capacities of vehicles */
    vector<Tcapacity_>    Capacities_;

    /** Fixed cost for each vehicle */
    vector<Tcost_> FCost_;

    /** Cost per distance unit */
    vector<Tcost_> Cost_;

    /** Number of vehicles available for each type */
    vector<unsigned int> Avail_;

  };

}

#endif
