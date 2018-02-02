 #ifndef GCVRPIO_HPP_
 #define GCVRPIO_HPP_


 #include <map>
 #include <iostream>
 #include <vector>
 #include <algorithm>
 #include <utility>
 #include <math.h>

#include <commonTypes.hpp>

#include "TAssignment.hpp"
#include "TCycle.hpp"


typedef std::vector<GTPP::TCycle> TVectorOfCycles1;
typedef std::vector<GTPP::TAssignment> TVectorOfAssignment1;



 #define DRW_EPS 1E-6
 #define WEIGHT_FUNC(w) 10-(int)((w)*10)
 
 using namespace std;
 
 namespace GCVRP
 {
 enum { TYPE_A, TYPE_B, USERS};
 
 class GCVRPIO
 {
 protected:
     int nNodes_;
     int nUsers_;

 public:
         GCVRPIO(int nNodes,int nUsers):
     nNodes_(nNodes), nUsers_(nUsers){}

     virtual ~GCVRPIO(void) {}

     inline int nNodes(void){return nNodes_;}
     inline int nUsers(void){return nUsers_;}
 
         ostream& WriteDOTGraph(ostream& os,
                                std::vector<double>& NodeBweight,
                                std::vector<double>& NodeAweight,
                                std::vector<double>& AssWeight,
                                std::vector<double>& ConWeight);
 
//         ostream&  WriteDOTCycle(ostream& os,TCycle& C,TAssignment& As);
 
         ostream& WriteDOTGraph(ostream& os,
                                std::map<int,double>& Edgeweight,
                                std::map<int,double>& Assweight);

         ostream& WriteDOTGraph(ostream& os,
                                TVectorOfCycles1& C,
                                TVectorOfAssignment1& A,
                                 std::vector<pair<int, int> >& coordN,
                                 std::vector<pair<int, int> >& coordU);

        ostream& WriteDOTGraph(ostream& os,
                                TVectorOfCycles1& C,
                                TVectorOfAssignment1& A);
 
        ostream& WriteDOTGraph(ostream& os,
                                 std::vector<double>& Edgeweight,
                                 std::vector<double>& Nodeweight,
                                 std::vector<double>& Assweight,
                                 GTPP::UserAvailabilityMatrix& avail);

         ostream& WriteDOTGraph(ostream& os,
                                 std::vector<double>& Edgeweight,
                                 std::vector<double>& Nodeweight,
                                 std::vector<double>& Assweight);

         ostream& WriteDOTGraph(ostream& os, const double* xyz,
                                 std::vector<double>& Edgeweight,
                                 std::vector<double>& Nodeweight,
                                 std::vector<double>& Assweight);

         ostream& WriteDOTGraph(ostream& os,
                                 std::vector<double>& Edgeweight,
                                 std::vector<double>& Nodeweight,
                                 std::vector<double>& Assweight,
                                 std::vector<pair<int, int> >& coordN,
                                 std::vector<pair<int, int> >& coordU);
         
 
 protected:
 
         ostream& WriteDOTAssignment(ostream& os,std::vector<double>& Assweight);
         ostream& WriteDOTAssignment(ostream& os, std::vector<double>& Assweight,GTPP::UserAvailabilityMatrix& avail);
         ostream& WriteDOTEdges(ostream& os,std::vector<double>& Edgeweight);
         ostream& WriteDOTEdges(ostream& os,std::vector<double>& Edgeweight,std::vector<double>& cost);
         ostream& WriteDOTEdges(ostream& os,std::map<int,double>& Edgeweight);
         ostream& WriteDOTEdges(ostream& os, TVectorOfCycles1& C);
         ostream& WriteDOTAssignment(ostream& os,TVectorOfAssignment1& Assweight);
         ostream& WriteDOTAssignment(ostream& os, std::vector<double>& Assweight,std::vector<double>& cost);
 
         ostream& WriteDOTCycleNodes(std::ostream& os,std::vector<double>&);
         ostream& WriteDOTCycleNodes(std::ostream& os,std::vector<double>& Nodesweight,std::vector<double>& cost);
         ostream& WriteDOTCycleNodes(std::ostream& os);
         ostream& WriteDOTCycleNodes(std::ostream& os,std::vector<pair<int, int> >& coordN);
//         ostream& WriteDOTCycleAssignment (ostream& os,TAssignment& As);
//         ostream& WriteDOTCycle(ostream& os,TCycle& C);
 
         ostream& WriteDOTNodesTypeB(std::ostream& os,int k,std::vector<double>& weight);
         ostream& WriteDOTNodesTypeA(std::ostream& os,int k,std::vector<double>& weight);
         ostream& WriteDOTUsers(ostream& os);
         ostream& WriteDOTUsers(ostream& os,std::vector<pair<int, int> >& coordU);
         ostream& WriteDOTUsers2(ostream& os);
         ostream& WriteDOTAssignment(std::ostream& os,int k,std::vector<double>& weight);
         ostream& WriteDOTEdges(std::ostream& os,int k,std::vector<double>& weight);
 
         ostream& WriteDOTNodesTypeB(std::ostream& os,std::vector<double>& weight);
         ostream& WriteDOTNodesTypeA(std::ostream& os,std::vector<double>& weight);
         //ostream& WriteDOTAssignment(std::ostream& os,std::vector<double>& weight);
         ostream& WriteDOTEdges2(std::ostream& os,std::vector<double>& weight);
 
 };
 
 }
 
 #endif

