#ifndef TCYCLE_H_
#define TCYCLE_H_

#include <math.h>

#include <vector>
#include <algorithm>
#include <iostream>

//#include <iostream> // only debugging
using namespace std;


#include <commonTypes.hpp>


namespace GTPP
{

/** The TCycle class describes a cycle as a sequence of nodes. Note that the origin (node 0) has been omitted. It is supposed that the cycle begins and ends at 0.
*/

class TCycle:public vector<TnodeId_>
{
public:
        /**@name Constructor and destructor
          */
        /*@{*/
        TCycle(void):
                        vector<TnodeId_>() {}

        virtual ~TCycle(void) {}

//        TCycle(int sz,const int c[]):
//                        vector<TnodeId_>(sz) {
//
//                resize(sz);
//
//                for (int i=0;i<sz;i++)
//                        (*this)[i]=c[i];
//        }

        TCycle(const TCycle& C)
        {
            const int sz=C.size();
            resize(sz);
            
            for(int i=0;i<sz;i++)
                operator[](i)=C[i];
           
        }

        /*@}*/

        void Reverse(void) {
                Tsize_ sz=size();

                Tinx_ nIter=floor(sz/2);

                for (Tinx_ l=0;l<nIter;l++) {
                    std::swap(operator[](l),operator[](sz-1-l));
                }

        }


        /** Interchange two arcs a(s(a)-> t(a)) and b(s(b) t(b)) (whose target vertices iterator are ta and tb respectively)
         with (s(a)->s(b)) and (t(a)->t(b))
         reversing arcs between t(a) s(b) */ 
        void CyclicReverse(Tinx_ i,Tinx_ j) {
                Tsize_ sz=size();
                j=j>i?j:j+size();

                Tinx_ nIter=(int)ceil(((double)(j-i))/2);

                //std::cout << nIter<< std::endl;

                for (Tinx_ l=0;l<nIter;l++) {
                    //std::cout << "INTERCAMBIO: "<<(i+l)%sz << " y "<< (sz+(j-l))%sz<< " ###### "<< sz << std::endl;
                    std::swap(operator[]((i+l)%sz),operator[]((sz+(j-l))%sz));
                }

        }

        void CyclicReverse2(Tinx_ i,Tinx_ j) {
                Tsize_ sz=size();
                j=j>i?j:j+size();

                Tinx_ nIter=(int)ceil(((double)(j-i))/2);

                std::cout << nIter<< std::endl;

                for (Tinx_ l=0;l<nIter;l++) {
                    std::cout << "INTERCAMBIO: "<<(i+l)%sz << " y "<< (sz+(j-l))%sz << std::endl;
                    std::swap(operator[]((i+l)%sz),operator[]((sz+(j-l))%sz));
                }

        }


	/** Shows this specific cycle */
        void write(ostream& os) {
                vector<TnodeId_>::iterator it;

                os << "{";
                for (it=begin();it!=end();++it) {
                        os << " "<<*it;
                }
                os << "} ";
        }


	/** Remove origin if it is included*/
        void removeDepot(void) {
                vector<TnodeId_>::iterator it=begin();
                bool found=false;

                for (;(it!=end())&&(!found);++it) {
                        if ((*it)==0) {
                                erase(it);
                                found=true;
                        }
                }
        }


	/** Remove origin if it is included*/
        void removeItem(int n) {
                vector<TnodeId_>::iterator it=begin();
                bool found=false;

                for (;(it!=end())&&(!found);++it) {
                        if ((*it)==n) {
                                erase(it);
                                found=true;
                        }
                }
        }

        void removeDepot2(void){

            std::vector<TnodeId_> aux;
            vector<TnodeId_>::iterator it=begin();
            int pos=-1;
            const int sz=size();

            for (int i=0;it!=end();++it,i++) {
                aux.push_back(*it);
                if ((*it)==0)
                    pos=i;
            }
           

            clear();           
            resize(sz-1);

            for(int i=0;i<sz-1;i++){
                vector<TnodeId_>::operator[](i)=aux[(pos+i+1)%sz];               
            }

        }

	/** Add a node after pos pos*/
        void posInsert(TnodeId_ n,Tinx_ pos) {
                if ((pos>size()-1)||(empty()))
                        push_back(n);
                else {
                        vector<TnodeId_>::iterator it=begin();
                        Tinx_ i=0;

                        for (;(it!=end())&&(i<pos);) {
                                ++i;
                                ++it;
                        }

//	    cout<< "Inserting "<<  i<< endl;
                        insert(it,n);
                }
        }

};




}

#endif /*TCYCLE_H_*/
