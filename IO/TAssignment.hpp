#ifndef TASSIGNMENT_H_
#define TASSIGNMENT_H_

#include <vector>
#include <commonTypes.hpp>

namespace GTPP
{

/** The TAssignment class describes a vector of assignments to the nodes of a specific cycle
*/

class TAssignment: public  std::vector<Tassignment_>
{

public:
        /**@name Constructor and destructor
             */
        /*@{*/
        TAssignment(void):
                        std::vector<Tassignment_> () {}

//        TAssignment(int sz,const int as[][2]):
//                        std::vector<Tassignment_> (sz) {
//
//                resize(sz);
//
//                for (int i=0;i<sz;i++) {
//                        (*this)[i]=Tassignment_(as[i][0],as[i][1]);
//                }
//        }

        TAssignment(const TAssignment& C)
        {
            reserve(C.size());
            vector<Tassignment_>::operator= ((vector<Tassignment_>)(C));
        }


        virtual ~TAssignment(void) {}

        /*@}*/

	/** Shows a vector of assignments (useful debuggin)*/
        void write(ostream& os) {
                std::vector<Tassignment_>::iterator it;

                os << "[";
                for (it=begin();it!=end();++it) {
                        os << "("<<(*it).first<<","<<(*it).second<<")";
                }
                os << "] ";
        }


        int demand(int n) {

            int d=0;
            std::vector<Tassignment_>::iterator it;

            for (it=begin();it!=end();++it) {
               if (n==(*it).second)
                   d++;
            }

            return d;
        }

        void removeNode(int node) {
            bool borrado = false;
            do {

                borrado = false;
                TAssignment::iterator itA = begin();

                while ((itA != end()) && (!borrado))
                    if ((*itA).second == node) {
                        erase(itA);
                        //                                        std::cout << "Borrando: " << (*itA).first << " -> " << (*itA).second << std::endl;
                        borrado = true;

                    } else
                        ++itA;
            } while (borrado);
        }

        void getUsers(std::set<int>& users)
        {
            users.clear();

            TAssignment::iterator itA = begin();

            for(itA=begin();itA!=end();++itA)
                users.insert((*itA).first);
            
        }
};
}


#endif

