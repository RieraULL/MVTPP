
#ifndef COMMONTYPES_HPP_
#define COMMONTYPES_HPP_

#include <utility>
#include <set>
#include <vector>
#include <map>
#include <limits.h>
#include <float.h>

#include <iostream>
//#define BRANCH_AND_CUT_ONLY 1

//#define ONLY_ROOT 1

using namespace std;

namespace GTPP {

/** Describes an user*/
typedef  int             		TuserId_;

/** Describes a node of potential stop*/
typedef  int              		TnodeId_;

/** Vehicle */
typedef  int              		TvehicleId_;
typedef  unsigned int              	Tsize_;
typedef  unsigned int              	Tcapacity_;
typedef  unsigned int              	Tdemand_;
typedef  int              		Tinx_;

typedef double                      	Tcost_;

#define _MAX_COST_ DBL_MAX
#define _EPS_      DBL_EPSILON
#define _MAX_TIME_ FLT_MAX


/** Describes a pair of nodes */
typedef std::pair<TnodeId_,TnodeId_>     	Tedge_;

/** Describes  an assignment of user to a node */
typedef std::pair<TuserId_,TnodeId_>     	Tassignment_;

typedef std::set<Tassignment_>           	TassignmentSet_;

class SymmetricCostMatrix: public std::vector<int> {
public:
    int nNodes_;
public:
    SymmetricCostMatrix ( void ) :std::vector<int>() {}
    virtual ~SymmetricCostMatrix ( void ) {}

    void set ( int nNodes ) {
        nNodes_=nNodes;
        reserve ( sz() );

    }

    inline const int pos ( int i,int j ) {


        return ( i>j ) ? ( i*i-i ) *0.5+j: ( j*j-j ) *0.5+i;

    }

    inline const int cost ( int i,int j ) {
        if ( i==j ) return 0;
        else return ( *this ) [pos ( i,j ) ];
    }

    inline const int sz ( void ) {
        return nNodes_* ( nNodes_-1 ) *0.5;
    }
};

class AssingmentCostMatrix: public std::vector<double> {
public:
    int nNodes_;
    int nUsers_;



public:
    AssingmentCostMatrix ( int nNodes,int nUsers ) :std::vector<double>() {

        set ( nNodes,nUsers );
    }

    virtual ~AssingmentCostMatrix ( void ) {}

    void set ( int nNodes,int nUsers ) {
        nNodes_=nNodes;
        nUsers_=nUsers;


        reserve ( (nNodes-1)*nUsers );

      const int sz=nUsers*(nNodes-1) ;

        for(int i=0;i<sz;i++)
            ( *this ) [i ]=1E100;


    }

    int pos ( int u,int i ) {
        const int position= (i-1)*nUsers_+u;

        return position;

    }

    inline const int cost ( int u,int i ) {
        return ( *this ) [pos ( u,i ) ];
    }

    inline const int sz ( void ) {
        return (nNodes_-1)*nUsers_ ;
    }
};


class UserAvailabilityMatrix: public std::vector<bool> {
public:
    int nNodes_;
    int nUsers_;


public:
    UserAvailabilityMatrix ( int nNodes,int nUsers ) :std::vector<bool>() {

        set ( nNodes,nUsers );
    }

    virtual ~UserAvailabilityMatrix ( void ) {}

    void set ( int nNodes,int nUsers ) {
        nNodes_=nNodes;
        nUsers_=nUsers;

        reserve ( nUsers*(nNodes-1) );

        const int sz=nUsers*(nNodes-1) ;

        for(int i=0;i<sz;i++)
            ( *this ) [i ]=false;

    }

    int pos ( int u,int i ) {
        const int position= (i-1)*nUsers_+u;

        return position;

    }

    inline const bool available ( int u,int i ) {
        return ( *this ) [pos ( u,i ) ];
    }

    inline const int sz ( void ) {
        return ( nUsers_*(nNodes_-1) );
    }
};



class SparseMatrixItem {
public:
    int row_;
    int col_;
    double val_;
public:
    SparseMatrixItem ( const int& r, const int& c, const double& v ) :
            row_ ( r ),
            col_ ( c ),
            val_ ( v ) {}

    SparseMatrixItem ( const SparseMatrixItem& SMI ) :
            row_ ( SMI.row_ ),
            col_ ( SMI.col_ ),
            val_ ( SMI.val_ ) {}

    void set ( const int& r, const int& c, const double& v ) {
        row_=r;
        col_=c;
        val_=v;
    }


    virtual ~SparseMatrixItem ( void ) {}
};



class SparseMatrix: public std::vector<SparseMatrixItem> {
public:
    int nNodes_;
    int nUsers_;

    std::map<int,int,less<int> > pos_;
public:

    virtual ~SparseMatrix ( void ) {}
    SparseMatrix(void): std::vector<SparseMatrixItem>() {}

    const int sz(void) {
        return size();
    }

    const int pos(int u,int v) {

        //std::cout<< "("<<u<<","<<v<<"): "<< position(u,v)<<std::endl;

        std::map<int,int>::iterator it=pos_.find(position(u,v));

        if (it==pos_.end()) return -1;


        else return pos_[position(u,v)];
    }

    const int pos2(int as) {

        std::map<int,int>::iterator it=pos_.find(as);

        if (it==pos_.end()) return -1;
        else return pos_[as];
    }

    const int position(int u, int v) {
        return (v-1)*nUsers_+u;
    }

    void set ( AssingmentCostMatrix& acm, UserAvailabilityMatrix& uam ) {

        nNodes_=uam.nNodes_;
        nUsers_=uam.nUsers_;

        reserve(nNodes_*nUsers_);

        //std::cout << nNodes_<< "->"<< nUsers_<< std::endl;

        for ( int u = 0; u < nUsers_; u++ ) {
            for ( int v = 1; v < nNodes_; v++ ) {
                if ( uam.available ( u,v ) ) {
//                    std::cout<< "["<<u<<","<<v<<"]: "<< position(u,v) <<std::endl;
                    const int pos=position(u,v);
                    const int sz=size();

                    pos_[pos]=sz;
                    push_back ( SparseMatrixItem ( u,v,acm.cost ( u,v ) ) );
                }
            }


        }

        //std::cout << "Tamaño: "<< size()<< std::endl;
    }

    std::vector<SparseMatrixItem>::iterator& AvanceNode ( std::vector<SparseMatrixItem>::iterator& it,const int& c ) {

        ++it;
        for ( ;it!=end();++it ) {

            if ( ( *it ).col_==c )
                break;
        }

        return it;
    }

    std::vector<SparseMatrixItem>::iterator beginNode ( const int& c ) {

        std::vector<SparseMatrixItem>::iterator it=begin();

        if ( ( *it ).col_==c )
            return it;

        return AvanceNode ( it,c );
    }




    std::vector<SparseMatrixItem>::iterator& AvanceUser ( std::vector<SparseMatrixItem>::iterator& it,const int& r ) {
        ++it;
        for ( ;it!=end();++it ) {

            if ( ( *it ).row_==r )
                break;
        }

        return it;

    }

    std::vector<SparseMatrixItem>::iterator beginUser ( const int& r) {
        std::vector<SparseMatrixItem>::iterator it=begin();

        if ( ( *it ).row_==r )
            return it;

        return AvanceUser ( it,r );
    }

    const int sizeP(void) {
        return pos_.size();
    }




    void write ( std::ostream& os ) {
        std::vector<SparseMatrixItem>::iterator it=begin();
        for ( it=begin();it!=end();++it ) {
            if ( it!=end() )
                os << "("<< ( *it ).row_<<","<< ( *it ).col_<<"): "<< ( *it ).val_<<" ";
        }
        os << std::endl;

        for ( int u=1;u<=nUsers_;u++ ) {
            std::cout << "User: "<< u<< std::endl;
            for ( it=beginUser ( u );it!=end();AvanceUser ( it,u ) )
                os << "("<< ( *it ).row_<<","<< ( *it ).col_<<"): "<< ( *it ).val_<<" ";
            std::cout<< std::endl;
        }

        os << std::endl;


        for ( int v=1;v<=nNodes_;v++ ) {
            std::cout<< "Node: "<< v<< std::endl;
            for ( it=beginNode ( v );it!=end();AvanceNode ( it,v ) )
                os << "("<< ( *it ).row_<<","<< ( *it ).col_<<"): "<< ( *it ).val_<<" ";
            std::cout<< std::endl;
        }
        os << std::endl;
    }


};





class SparseMatrixArcs: public std::vector<SparseMatrixItem> {
public:
    int nNodes_;

    std::map<int,int,less<int> > pos_;
public:
    void clear(void){
     	std::vector<SparseMatrixItem>::clear();
	pos_.clear();
    }

    virtual ~SparseMatrixArcs ( void ) {}

    SparseMatrixArcs(void):nNodes_(0), std::vector<SparseMatrixItem>() {}

    const int sz(void) {
        return size();
    }

    const int pos(int i,int j) {

        //std::cout<< "("<<u<<","<<v<<"): "<< position(u,v)<<std::endl;

        std::map<int,int>::iterator it=pos_.find(position(i,j));

        if (it==pos_.end()) return -1;


        else return pos_[position(i,j)];
    }

    const int pos(int as) {

        std::map<int,int>::iterator it=pos_.find(as);

        if (it==pos_.end()) return -1;
        else return pos_[as];
    }

    const int position(int i, int j) {
        return i*nNodes_+j;
    }


    void set(int i,int j,int pos) {

        const int posit=position(i,j);

        std::map<int,int>::iterator it=pos_.find(posit);

        if (it!=pos_.end()) {
            std::cout << "ERROR ACCEDIENDO EN MATRIZ" << std::endl;
            exit(1);
        }

        pos_[posit]=size();
        push_back ( SparseMatrixItem (i,j,pos) );
    }


    std::vector<SparseMatrixItem>::iterator& Avance_i ( std::vector<SparseMatrixItem>::iterator& it,const int& c ) {

        ++it;
        for ( ;it!=end();++it ) {

            if ( ( *it ).col_==c )
                break;
        }

        return it;
    }

    std::vector<SparseMatrixItem>::iterator begin_i ( const int& i ) {

        std::vector<SparseMatrixItem>::iterator it=begin();

        if ( ( *it ).col_==i )
            return it;

        return Avance_i ( it,i );
    }


    std::vector<SparseMatrixItem>::iterator& Avance_j ( std::vector<SparseMatrixItem>::iterator& it,const int& r ) {
        ++it;
        for ( ;it!=end();++it ) {

            if ( ( *it ).row_==r )
                break;
        }

        return it;

    }

    std::vector<SparseMatrixItem>::iterator begin_j ( const int& r) {
        std::vector<SparseMatrixItem>::iterator it=begin();

        if ( ( *it ).row_==r )
            return it;

        return Avance_j ( it,r );
    }

    const int sizeP(void) {
        return pos_.size();
    }

    void write ( std::ostream& os ) {
        std::vector<SparseMatrixItem>::iterator it=begin();
        for ( it=begin();it!=end();++it ) {
            if ( it!=end() )
                os << "("<< ( *it ).row_<<","<< ( *it ).col_<<"): "<< ( *it ).val_<<" ";
        }
        os << std::endl;

        for ( int u=1;u<=nNodes_;u++ ) {
            std::cout << "j: "<< u<< std::endl;
            for ( it=begin_j ( u );it!=end();Avance_j ( it,u ) )
                os << "("<< ( *it ).row_<<","<< ( *it ).col_<<"): "<< ( *it ).val_<<" ";
            std::cout<< std::endl;
        }

        os << std::endl;


        for ( int v=1;v<=nNodes_;v++ ) {
            std::cout<< "i: "<< v<< std::endl;
            for ( it=begin_i ( v );it!=end();Avance_i ( it,v ) )
                os << "("<< ( *it ).row_<<","<< ( *it ).col_<<"): "<< ( *it ).val_<<" ";
            std::cout<< std::endl;
        }
        os << std::endl;
    }


};


}

#endif
