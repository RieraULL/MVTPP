#ifndef TFEASIBLESOLUTION_HPP_
#define TFEASIBLESOLUTION_HPP_

#include <vector>

using namespace std;

#include "TCycle.hpp"
#include "TAssignment.hpp"
#include "commonTypes.hpp"


namespace GTPP {

    /** The TFeasibleSolution class describes a feasible solution of a GTPP. A feasible solution consists of three elements for each vehicle:
       <ul>
         <li> Vehicle identification showing the type of the vehicle associated to this item
         <li> A cycle @see TCycle.
         <li> An assignment @see TAssignment.
       </ul>
     */

    class TFeasibleSolution {
    public:

        /** Vector of vehicle identifications*/
        std::vector<TvehicleId_> vehicleSet_;
        /** Vector of cycles*/
        std::vector<TCycle> cycleSet_;
        /** Vector of assignments*/
        std::vector<TAssignment> assignmentSet_;

    public:
        /**@name Constructor and destructor
         */

        /*@{*/

        TFeasibleSolution(void) :
        vehicleSet_(),
        cycleSet_(),
        assignmentSet_() {
        }

        TFeasibleSolution(std::vector<TCycle> C,std::vector<TAssignment> A,std::vector<TvehicleId_> V) :
        vehicleSet_(),
        cycleSet_(),
        assignmentSet_() {

//            vehicleSet_.reserve(V.size());
//            cycleSet_.reserve(C.size());
//            assignmentSet_.reserve(A.size());
//
//            vehicleSet_ = V;
//            cycleSet_ = C;
//            assignmentSet_ = A;

            vehicleSet_.resize(V.size());
            cycleSet_.resize(C.size());
            assignmentSet_.resize(A.size());

//            std::cout << "vs: "<< vehicleSet_.size()<< std::endl;
//            std::cout << "cs: "<< cycleSet_.size()<< std::endl;
//            std::cout << "as: "<< assignmentSet_.size()<< std::endl;

            for(int i=0;i<vehicleSet_.size();i++)
                vehicleSet_[i] = V[i];

            for(int i=0;i<cycleSet_.size();i++)
                cycleSet_[i] = C[i];

            for(int i=0;i<assignmentSet_.size();i++)
                assignmentSet_[i] = A[i];


        }

        virtual ~TFeasibleSolution(void) {
        }

        TFeasibleSolution(const TFeasibleSolution& S) :
        vehicleSet_(),
        cycleSet_(),
        assignmentSet_() {


            vehicleSet_.resize(S.vehicleSet_.size());
            cycleSet_.resize(S.cycleSet_.size());
            assignmentSet_.resize(S.assignmentSet_.size());

//            std::cout << "vs: "<< vehicleSet_.size()<< std::endl;
//            std::cout << "cs: "<< cycleSet_.size()<< std::endl;
//            std::cout << "as: "<< assignmentSet_.size()<< std::endl;

            for(int i=0;i<vehicleSet_.size();i++)
                vehicleSet_[i] = S.vehicleSet_[i];

            for(int i=0;i<cycleSet_.size();i++)
                cycleSet_[i] = S.cycleSet_[i];

            for(int i=0;i<assignmentSet_.size();i++)
                assignmentSet_[i] = S.assignmentSet_[i];



        }

        /*@}*/

        void getInvolvedNodes(std::set<int>& nodes)
        {
            nodes.clear();

            const int sz=cycleSet_.size();

            for(int i=0;i<sz;i++){
                TCycle& C=cycleSet_[i];

                const int csz=C.size();

                for(int j=0;j<csz;j++)
                    nodes.insert(C[j]);
            }
        }


        TFeasibleSolution & operator=(const TFeasibleSolution& sol) {

            vehicleSet_.reserve(sol.vehicleSet_.size() + 1);
            cycleSet_.reserve(sol.cycleSet_.size() + 1);
            assignmentSet_.reserve(sol.assignmentSet_.size() + 1);

            vehicleSet_.resize(sol.vehicleSet_.size());
            cycleSet_.resize(sol.cycleSet_.size());
            assignmentSet_.resize(sol.assignmentSet_.size());

//            std::cout << "vs: "<< vehicleSet_.size()<< std::endl;
//            std::cout << "cs: "<< cycleSet_.size()<< std::endl;
//            std::cout << "as: "<< assignmentSet_.size()<< std::endl;

            for(int i=0;i<vehicleSet_.size();i++)
                vehicleSet_[i] = sol.vehicleSet_[i];

            for(int i=0;i<cycleSet_.size();i++)
                cycleSet_[i] = sol.cycleSet_[i];

            for(int i=0;i<assignmentSet_.size();i++)
                assignmentSet_[i] = sol.assignmentSet_[i];
            
        }

        /** Add a new set of Cycle, Assignment and Vehicle to a solution*/
        inline void insertCycle(const TCycle& C, const TAssignment& A, TvehicleId_ v) {

            vehicleSet_.reserve(vehicleSet_.size() + 10);
            cycleSet_.reserve(cycleSet_.size() + 10);
            assignmentSet_.reserve(assignmentSet_.size() + 10);


            cycleSet_.push_back(C);
            assignmentSet_.push_back(A);
            vehicleSet_.push_back(v);
        }

        /** Interchanges two assignments */
        void interchangeAssignment(std::pair<Tassignment_, Tassignment_> ap, std::pair<int,int> av) {

            TAssignment& As1 = assignmentSet_[av.first];
            TAssignment& As2 = assignmentSet_[av.second];

            TAssignment::iterator it1 = As1.begin();
            TAssignment::iterator it2 = As2.begin();

            for (; (it1 != As1.end()) && (ap.first.first != (*it1).first); ++it1);

            (*it1).first = ap.second.first;

            for (; (it2 != As2.end()) && (ap.second.first != (*it2).first); ++it2);
            (*it2).first = ap.first.first;

        }

        inline int getVehicleId(int pos) const {
            return vehicleSet_[pos];
        }

        TCycle& getCycle(int pos)  {
            return cycleSet_[pos];
        }

        TAssignment& getAssignment(int pos){
            return assignmentSet_[pos];
        }

        inline int size(void) const {
            return cycleSet_.size();
        }

        void clear(void) {
            cycleSet_.clear();
            assignmentSet_.clear();
            vehicleSet_.clear();
        }

        /** Remove those cycles without assignments (usually as result of an heuristic improvement) */
        void cleanEmptyCycles(void) {

            bool borrado = false;

//            std::cout << "-vs: "<< vehicleSet_.size()<< std::endl;
//            std::cout << "-cs: "<< cycleSet_.size()<< std::endl;
//            std::cout << "-as: "<< assignmentSet_.size()<< std::endl;


            do {

                borrado = false;
                std::vector<TvehicleId_>::iterator vsIt = vehicleSet_.begin();
                std::vector<TCycle>::iterator csIt = cycleSet_.begin();
                std::vector<TAssignment>::iterator asIt = assignmentSet_.begin();

                for (; (vsIt != vehicleSet_.end()) && (!borrado); ++vsIt, ++csIt, ++asIt) {
                    if (((*csIt).size() == 0) || ((*asIt).size() == 0)) {
                        borrado = true;

                        if (vsIt != vehicleSet_.end())
                            vehicleSet_.erase(vsIt);

                        if (csIt != cycleSet_.end())
                            cycleSet_.erase(csIt);

                        if (asIt != assignmentSet_.end())
                            assignmentSet_.erase(asIt);
                    }
                }

            } while (borrado);

//            std::cout << "--vs: "<< vehicleSet_.size()<< std::endl;
//            std::cout << "--cs: "<< cycleSet_.size()<< std::endl;
//            std::cout << "--as: "<< assignmentSet_.size()<< std::endl;

        }

        /** Shows a feasible solution */
        void write(ostream& os) {

            os << "<" << std::endl;

            for (int i = 0; i < cycleSet_.size(); i++) {
                cycleSet_[i].write(os);
                assignmentSet_[i].write(os);
                os << vehicleSet_[i] << std::endl;
            }

            os << "> " << std::endl;
        }


    };

}

#endif
