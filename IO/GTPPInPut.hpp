//
// File:   GTPPInPut.h
// Author: jriera
//
// Created on 29 de enero de 2008, 11:57
//


#ifndef _GTPPINPUT_HXX
#define _GTPPINPUT_HXX

#include <vector>
#include <set>
#include <map>
#include <string>

#include <iostream>

#include <TFeasibleSolution.hpp>

namespace GTPP {



/** The vehicleDescription class describes the features of each type of vehicle. Consist of three items for each type of vehicle:
   <ul>
     <li> Capacity of the vehicle. That is, maximum number of users able to be transported.
     <li> Route cost. That is, cost per each unit of distance.
     <li> Fixed cost. That is, the cost of using this type of vehicle.
     <li> Availability. Number of vehicles available for this type.
   </ul>
*/

class vehicleDescription {
public:
    /** Capacity of vehicle */
    int capacity_;
    /** Routing cost weight */
    double cost_;
    /** Fixed cost 		*/
    double fixedCost_;
    /** Availability of this type of vehicle */
    int availability_;
public:

    /**@name Constructor and destructor
     */
    /*@{*/
    vehicleDescription(void):
            capacity_(0),
            cost_(0.0),
            fixedCost_(0.0),
            availability_(0) {}

    vehicleDescription(unsigned int capacity,double cost,double fcost, unsigned int availability):
            capacity_(capacity),
            cost_(cost),
            fixedCost_(fcost),
            availability_(availability) {}

    virtual ~vehicleDescription(void) {}
    /*@}*/
};


/** The GCTPPInPut class describes an InPut of a Generalized Traveling Purchaser Problem. Each intance InPut consist of :
   <ul>
     <li> Number of nodes or potential bus stops.
     <li> Number of users.
     <li> Vector describing the routing cost. Formulae to access to routing cost between node \f i \f and \f$ j \f$ is \f (i \times nNodes)+j \f.
     <li> Vector describing the assignment cost. Formulae to access assignment cost between user \f$ u \f$ and node \f$ i \f$ is \f$ ((i-1) \times nNodes) + u \f$.
     <li> Vector containig the description of the types of vehicles avalilable for this specific instance.
   </ul>
*/


class GTPPInPut {

public:
    /** Number of Nodes     */
    int nNodes_;
    /** Number of Users     */
    int nUsers_;

    /** Routing Cost        */
    std::vector<double> routingCost_;


    /** Assignment Cost     */
    GTPP::AssingmentCostMatrix assignmentCost_;

    /** Availability_ of stop for each user */
    GTPP::UserAvailabilityMatrix userAvailability_;

    /** Assignment Cost     */
    GTPP::SparseMatrix userSparseMatrix_;

    /** vehicles		*/
    int vehicleCapacity_;

    /** number of vehicles*/
    int m_;

    /** bound on the length of the route */
    int routeBound_;


public:
    /**@name Constructor, Destructor */
    /*@{*/
    GTPPInPut(void):
            nNodes_(0),
            nUsers_(0),
            routingCost_(0),
            assignmentCost_(0,0),
            userAvailability_(0,0),
            vehicleCapacity_(0),
            routeBound_(0){}

    virtual ~GTPPInPut(void) {}
    /*@}*/

    /**@name Methods to get/set parameters on the fly */
    /*@{*/


    void buildReduce(GTPPInPut& I,std::vector<int>& mapping)
    {
        std::set<int> useful;
        // Establecer los vértices que tienen asignación potencial
        for(int u=0;u<nUsers_;u++)
            for(int i=1;i<nNodes_;i++)
                if (available(u,i))
                    useful.insert(i);      
                   

         const int new_nNodes=useful.size()+1;
         const int new_nUsers=nUsers_;
         const int new_vehicleCapacity=vehicleCapacity_;

         std::vector<double>& new_routingCost=I.routingCost_;
        
         mapping.push_back(0);

         std::set<int>::iterator it;

         for(it=useful.begin();it!=useful.end();++it)
             mapping.push_back(*it);

         new_routingCost.resize(new_nNodes*new_nNodes);

         for(int i=0;i<new_nNodes;i++)
             for(int j=0;j<new_nNodes;j++)
                 new_routingCost[i*new_nNodes+j]=routingCost_[mapping[i]*nNodes_+mapping[j]];

         GTPP::AssingmentCostMatrix& new_assignmentCost=I.assignmentCost_;
         new_assignmentCost.resize((new_nNodes-1)*new_nUsers);
         new_assignmentCost.nNodes_=new_nNodes;
         new_assignmentCost.nUsers_=new_nUsers;

         for(int u=0;u<new_nUsers;u++)
            for(int i=1;i<new_nNodes;i++){
                
                const int pos =   (mapping[i]-1)*nUsers_+u;
                const int new_pos=(i-1)* new_nUsers+u;
                

                new_assignmentCost[new_pos]=assignmentCost_[pos];
            }


        GTPP::UserAvailabilityMatrix& new_userAvailability=I.userAvailability_;
        new_userAvailability.resize((new_nNodes-1)*new_nUsers);

        new_userAvailability.nNodes_=new_nNodes;
        new_userAvailability.nUsers_=new_nUsers;

        for(int u=0;u<new_nUsers;u++)
            for(int i=1;i<new_nNodes;i++){
                const int pos =   (mapping[i]-1)*nUsers_+u;
                const int new_pos=(i-1)* new_nUsers+u;

                new_userAvailability[new_pos]=userAvailability_[pos];
           
        }


        GTPP::SparseMatrix& new_userSparseMatrix=I.userSparseMatrix_;
  
        new_userSparseMatrix.set(new_assignmentCost,new_userAvailability);
        
        I.nNodes_=new_nNodes;
        I.nUsers_=new_nUsers;        
        I.vehicleCapacity_=new_vehicleCapacity;            

    }

    bool available(unsigned int u,unsigned int i) {
        if (i==0) return true;
        const int pos = (i-1)*nUsers_+u;
        return userAvailability_[pos];
    }

    /** Gets the assignment cost of user u to node i*/
    inline const double assignmentCost(unsigned int u,unsigned int i) {
        if (i==0)
            return 1E5;

        const int pos = (i-1)*nUsers_+u;

        if (userAvailability_[pos])
            return assignmentCost_[pos]; //OJO USER
        else {
            std::cout << "Assignment not available: <"<<u<<" -> "<<i<<">"<< std::endl;
            exit(0);
            return 999999;
        }

    }


    const int assignmentPos(int u, int i) {
        if (i==0)
            return (nNodes_*nUsers_)+u;

        const int pos= (i-1)*nUsers_+u;
        if (userAvailability_[pos])
            return pos; //OJO USER
        else {
            return pos;
        }


    }

    inline const int arcPos(int i,int j) {
        return i*nNodes_+j;
    }

    /** Gets the assignment cost of an assignment as*/
    const double assignmentCost(Tassignment_ as) {

	if (as.second==0) return 1E-5;

        const int pos=(as.second-1)*nUsers_+as.first;

        if (userAvailability_[pos])
            return assignmentCost_[pos]; //OJO USER
        else {
            std::cout << "Assignment not available: <"<<pos<<">"<< std::endl;
            exit(0);
            return 999999;
        }
    }

    /** Gets the routing cost between i and j*/
    inline const double routingCost(unsigned int i,unsigned int j)  {
        return routingCost_[i*nNodes_+j];
    }

    /** Gets the number of nodes */
    inline const int nNodes(void) {
        return nNodes_;
    }

    /** Gets the number of users*/
    inline const int nUsers(void) {
        return nUsers_;
    }

    inline const int routeBound(void){
        return routeBound_;
    }

    /** Gets the number of vehicles*/
//        inline const int nVehicles(void) {
//                return vehicles_.size();
//        }

    const int AssignmentPotentiality(int node) {
        int counter=0;

        for (int u=0;u<nUsers_;u++)
            if (userAvailability_[assignmentPos(u,node)])
                counter++;

        return counter;
    }

    bool checkUserReach(void) {
        // For each user check that reach at least one stop
        bool feasible=true;

        for (int u=0;(u<nUsers_)&&(feasible);u++) {
            int reachable=0;
            for (int i=1;i<nNodes_;i++) {
                if (userAvailability_[assignmentPos(u,i)])
                    reachable++;
            }
            if (reachable<1)
                feasible=false;
        }

        return feasible;
    }

    bool checkUserCapacity(void) {
        bool feasible=true;

        for (int i=1;(i<nNodes_)&&(feasible);i++) {
            int cap=0;
            for (int u=0;(u<nUsers_)&&(feasible);u++) {
                if (userAvailability_[assignmentPos(u,i)])
                    cap++;
            }
            if (cap>vehicleCapacity_)
                feasible=false;
        }

        return feasible;
    }

//        bool checkFeasibility(void)
//        {
//            // For each user check that reach at least one stop
//            if (!checkUserReach())
//                return false;
//            else if (!checkUserCapacity())
//                return false;
//            else return true;
//        }

    /** Gets the capacity of the first type of vehicle (useful only for identical vehicles*/
    inline const unsigned int capacity(void) {
        //return vehicles_[0].capacity_;
        return vehicleCapacity_;
    }




//        void vehicleVector(  std::vector<unsigned int>& capacity,
//                             std::vector<double>& fixedCost,
//                             std::vector<double>& variableCost,
//                             std::vector<unsigned int>& availability ) {
//                std::vector<vehicleDescription>::iterator it;
//
//                capacity.clear();
//                fixedCost.clear();
//                variableCost.clear();
//                availability.clear();
//
//                capacity.reserve(vehicles_.size());
//                fixedCost.reserve(vehicles_.size());
//                variableCost.reserve(vehicles_.size());
//                availability.reserve(vehicles_.size());
//
//
//                for (it=vehicles_.begin();it!=vehicles_.end();++it) {
//                        capacity.push_back((*it).capacity_);
//                        fixedCost.push_back((*it).fixedCost_);
//                        variableCost.push_back((*it).cost_);
//                        availability.push_back((*it).availability_);
//                }
//        }


    inline const Tcost_ edgeCost(TnodeId_ i, TnodeId_ j) {
 
         return routingCost_[i*nNodes_+j];
    }

    /*@}*/

    /**@name Methods to compute the objective value of a feasible solution */
    /*@{*/

    /** Computes the cost of cycle C in a identical vehicle scenario*/
    const Tcost_ computeCycleCost(TCycle& C) {

	if (C.size()==0) return 0.0;

        Tcost_ auxC=0.0;
        const int Csz=C.size();

        auxC+=edgeCost(0,C[0]);
        //std::cout<<"(" << 0<<","<<C[0]<<") "<<Graph_.getEdgeCost(0,C[0])<<" ";

        for (int j=0;j<Csz-1;j++) {
            auxC+=edgeCost(C[j],C[j+1]); // Compute total routing cost
            //std::cout<<"(" << C[j]<<","<<C[j+1]<<") "<<Graph_.getEdgeCost(C[j],C[j+1])<<" ";
        }

        auxC+=edgeCost(C[Csz-1],0);

        //std::cout<<"(" << C[Csz-1]<<","<<0<<") "<<Graph_.getEdgeCost(0,C[Csz-1])<<std::endl;

        return auxC;
    }

    const Tcost_ computeCycleCost2(TCycle& C) {

	if (C.size()==0) return 0.0;

        Tcost_ auxC=0.0;
        const int Csz=C.size();

        //auxC+=edgeCost(0,C[0]);
        //std::cout<<"(" << 0<<","<<C[0]<<") "<<Graph_.getEdgeCost(0,C[0])<<" ";

        for (int j=0;j<Csz-1;j++) {
            auxC+=edgeCost(C[j],C[j+1]); // Compute total routing cost
            //std::cout<<"(" << C[j]<<","<<C[j+1]<<") "<<Graph_.getEdgeCost(C[j],C[j+1])<<" ";
        }


        auxC+=edgeCost(C[Csz-1],C[0]);

        //std::cout<<"(" << C[Csz-1]<<","<<0<<") "<<Graph_.getEdgeCost(0,C[Csz-1])<<std::endl;

        return auxC;
    }

    const Tcost_ computeCycleCost(const std::vector<int>& C) {

        Tcost_ auxC=0.0;
        const int Csz=C.size();

        auxC+=edgeCost(0,C[0]);
        //std::cout<<"(" << 0<<","<<C[0]<<") "<<Graph_.getEdgeCost(0,C[0])<<" ";

        for (int j=0;j<Csz-1;j++) {
            auxC+=edgeCost(C[j],C[j+1]); // Compute total routing cost
            //std::cout<<"(" << C[j]<<","<<C[j+1]<<") "<<Graph_.getEdgeCost(C[j],C[j+1])<<" ";
        }

        auxC+=edgeCost(C[Csz-1],0);

        //std::cout<<"(" << C[Csz-1]<<","<<0<<") "<<Graph_.getEdgeCost(0,C[Csz-1])<<std::endl;

        return auxC;
    }



    /** Compute the cost of cycle C and type of vehicle k*/
    Tcost_ computeCycleCost(const TCycle& C,int k) {
        Tcost_ cost=0.0;

        //cost+= vehicles_[k].fixedCost_;	// Update fixed cost for vehicle i
        Tcost_ auxC=computeCycleCost(C);
        cost+=auxC/* *vehicles_[k].cost_*/;

        return cost;
    }


    /** Computes the cost of an assignment A in an identical vehicle scenario*/
    Tcost_ computeAssignmentCost(TAssignment& A) {
        Tcost_ auxC=0.0;
        const int Csz=A.size();

        for (int j=0;j<Csz;j++) {
            auxC+=assignmentCost(A[j].first,A[j].second); // Compute total assignment cost
//            std::cout <<"< " <<A[j].first<< " ->"<< A[j].second<< " >: "<< auxC << std::endl;
        }

        return auxC;
    }

    /** Computes the cost of a feasible solution S in an identical vehicle scenario*/
    Tcost_ computeCost(TFeasibleSolution& S) { // Compute Total cost according to a feasible solution
        Tcost_ cost=0.0;

        int sz= S.size();
        
        for (int i=0;i<sz;i++) {
            //cost+= vehicles_[S.getVehicleId(i)].fixedCost_;	// Update fixed cost for vehicle i
            const TCycle& aux=S.getCycle(i);
            Tcost_ auxC=computeCycleCost(aux);
            //std::cout<<"Ciclo: " << auxC<< std::endl;
            cost+=auxC/* *vehicles_[S.getVehicleId(i)].cost_*/;

            TAssignment& as=S.getAssignment(i);

            cost+=computeAssignmentCost(as);
            //std::cout<<"Resto: " << cost << std::endl;
        }

        return cost;
    }

    Tcost_ computeMaxCycleCost(TFeasibleSolution& S) { // Compute Total cost according to a feasible solution
        Tcost_ cost=0.0;

        int sz= S.size();

        for (int i=0;i<sz;i++) {
            //cost+= vehicles_[S.getVehicleId(i)].fixedCost_;	// Update fixed cost for vehicle i
            const TCycle& aux=S.getCycle(i);
            Tcost_ auxC=computeCycleCost(aux);
//            std::cout<<"Ciclo: " << auxC<< std::endl;
            if (auxC>cost)
                cost=auxC/* *vehicles_[S.getVehicleId(i)].cost_*/;


        }

        return cost;
    }

  Tcost_ cycleCost(const int t,const std::vector<double>& x)
  {
      double cost=routingCost(0,t);

      int source=0;
      int target=t;

      do{
          bool found=false;
          for(int i=0;(i<nNodes_)&&(!found);i++)
            if ((target!=i)&&(x[arcPos(target,i)]>0.9)){
                source=target;
                target=i;
                cost+=routingCost(source,target);
                found=true;
            }
      }while (target!=0);

      return cost;
  }

  Tcost_ computeMaxCycleCost(const std::vector<double>& x) { // Compute Total cost according to a feasible solution

      std::vector<int> origen;

      for(int i=0;i<nNodes_;i++)
          if (x[i]>0.9)
              origen.push_back(i);

      const int sz=origen.size();

      double maxCost=0.0;

//      std::cout << sz << std::endl;

      for(int i=0;i<sz;i++){

          const double ccost=cycleCost(origen[i],x);
          if (ccost>maxCost)
              maxCost=ccost;
      }

      std::cout << maxCost << std::endl;

      return maxCost;
    }


    /** Computes the cost of a feasible solution S separting the cost of each cycle/assignment*/
    void computeCost(TFeasibleSolution& S,std::vector<double>& costs) { // Compute Total cost according to a feasible solution

        int sz= S.size();
        costs.clear();
        costs.reserve(sz);

        for (int i=0;i<sz;i++) {
            double c=0.0;
            //double c= vehicles_[S.getVehicleId(i)].fixedCost_;	// Update fixed cost for vehicle i
            Tcost_ auxC=computeCycleCost(S.getCycle(i));
            c+=auxC/* *vehicles_[S.getVehicleId(i)].cost_*/;
            c+=computeAssignmentCost(S.getAssignment(i));
//			std::cout << "CAPACIDAD: "<<S.getAssignment(i).size()<< "->" << vehicleCapacity_ << std::endl;
//             if (S.getAssignment(i).size()>vehicleCapacity_) {
//                 std::cout << "ERROR CAPACIDAD: "<<S.getAssignment(i).size()<< "->" << vehicleCapacity_ << std::endl;
//                 exit(0);
//             }

            costs.push_back(c);
        }
    }

    /*@}*/

};

class GTPPInPutFull:public GTPPInPut {
public:
    std::string	name_;
    std::string	description_;
    int             Radius_;
    int             Weight_;

    std::vector<pair<int, int> > coordN;
    std::vector<pair<int, int> > coordU;

public:
    GTPPInPutFull(void):
            GTPPInPut(),
            name_(),
            description_() {}

    virtual ~GTPPInPutFull(void) {}

    void Preprocessing(GTPPInPutFull& I)
    {
        std::vector<int> mapping;
        GTPPInPut::buildReduce(I,mapping);

        I.Radius_=Radius_;
        I.Weight_=Weight_;
        I.name_=name_;
        I.m_=m_;
        I.vehicleCapacity_=vehicleCapacity_;
        I.routeBound_=routeBound_;

//        coordN.clear();
//        coordU.clear();
//
//        const int nsz=I.nNodes_;
//        const int usz=I.nUsers_;

//        for(int i=0;i<nsz;i++)
//            I.coordN.push_back(coordN[mapping[i]]);
//
//        for(int u=0;u<usz;u++)
//            I.coordU.push_back(coordU[u]);

        }

        void readCmRSP(std::istream& is) {
            
            is >> nNodes_;
            is >> nUsers_;

            int aux;
            is >> aux;

            is >> vehicleCapacity_;
            is >> m_;

            char name[80];

            sprintf(name, "CmRSP_n%d_u%d_m%d_Q%d", nNodes_, nUsers_, m_, vehicleCapacity_);
            name_ = name;

            routingCost_.resize(nNodes_ * nNodes_);


            assignmentCost_.set(nNodes_, nUsers_);
            userAvailability_.set(nNodes_, nUsers_);


            for (int i = 0; i < nUsers_; i++) {
                assignmentCost_[i * nUsers_ + i] = 0;
                userAvailability_[i * nUsers_ + i] = true;
            }

            while (!is.eof()) {
                char c;
                is >> c;

                int i, j, cost;

                is >> i;
                is >> j;
                is >> cost;

                if (c == 'e') {
                    routingCost_[i * nNodes_ + j] = cost;
                    routingCost_[j * nNodes_ + i] = cost;
                } else
                    if (c == 'a') {
                    userAvailability_[(j - 1) * nUsers_ + i-1] = true;
                    assignmentCost_[(j - 1) * nUsers_ + i-1] = cost;
                }
            }

        userSparseMatrix_.set(assignmentCost_,userAvailability_);

        }

};

}

#endif	/* _GTPPINPUT_H */

