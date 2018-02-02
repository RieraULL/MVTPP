//
// C++ Implementation: GTPP2XML
//
// Description:
//
//
// Author: Jorge Riera-Ledesma <jriera@ull.es>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "GTPP2XML.hpp"

#include <fstream>

//using namespace GTPPXMLInPut;

using namespace GTPP;

namespace GTPPXML {


int XML2GTPP(std::istream& is,GTPPInPutFull& I,int open) {
    try {
        std::auto_ptr<TPPXMLInPut_t> InPut (TPPInPutXML(is));

        I.nNodes_=InPut->nNodes();
        I.nUsers_=InPut->nUsers();
        I.name_=InPut->instanceName();

        {


            I.userAvailability_.set(I.nNodes_,I.nUsers_);
            //const int sz=I.userAvailability_.sz();

            int i=0;

            for (int u=0;u<I.nUsers_;u++)
                for (int l=1;l<I.nNodes_;l++) {
                    I.userAvailability_.push_back(InPut->availabilityMatrix()[i++]);
                }
        }

        {

            I.assignmentCost_.set(I.nNodes_,I.nUsers_);
            const int sz=I.assignmentCost_.sz();

            for (int i=0;i<sz;i++)
                //if (I.userAvailability_[i])
                I.assignmentCost_.push_back(InPut->usersMatrixCost()[i]);
            //else
            //    I.assignmentCost_.push_back(99999);
        }

        I.userSparseMatrix_.set(I.assignmentCost_,I.userAvailability_);

        {
            const int sz=I.nNodes_*I.nNodes_;
            I.routingCost_.reserve(sz);

            for (int i=0;i<sz;i++){
                const int s=i/I.nNodes_;
                const int t=i%I.nNodes_;

                if (((s==0)&&(open==1))||(s==t))
                    I.routingCost_.push_back(0);
                else
                    I.routingCost_.push_back(InPut->nodesMatrixCost()[i]);
            }
        }



//        {
//            for (int i=0;i<I.nNodes_;i++){
//                I.coordN.push_back(std::pair<int,int> (InPut->nodesCoords()[2*i],InPut->nodesCoords()[2*i+1]));
//            }
//        }
//
//        {
//            for (int i=0;i<I.nUsers_;i++){
//                I.coordU.push_back(std::pair<int,int> (InPut->usersCoords()[2*i],InPut->usersCoords()[2*i+1]));
//            }
//        }

        //I.vehicleCapacity_=InPut->vehicleCapacity();



    } catch (const xml_schema::exception& e) {
        std::cerr << e << std::endl;
        return 1;
    }

    return 0;
}

int GTPP2XML(GTPPInPutFull& I) {
    try {
        //hello_t h ("Hi");
        //std::auto_ptr<GTPPXMLInPut_t> InPut (GTPPXMLInPut(I.name_));



        TPPXMLInPut_t::nodesMatrixCost_type nmc;
        TPPXMLInPut_t::usersMatrixCost_type umc;
        TPPXMLInPut_t::availabilityMatrix_type am;
        TPPXMLInPut_t::nodesCoords_type coordN;
        TPPXMLInPut_t::usersCoords_type coordU;


        for (std::vector<double>::iterator it=I.routingCost_.begin();it!=I.routingCost_.end();++it)
            nmc.push_back(*it);

        for (std::vector<double>::iterator it=I.assignmentCost_.begin();it!=I.assignmentCost_.end();++it)
            umc.push_back(*it);

        for (std::vector<bool>::iterator it=I.userAvailability_.begin();it!=I.userAvailability_.end();++it)
            am.push_back(*it);

        for (std::vector<pair<int, int> >::iterator it=I.coordN.begin();it!=I.coordN.end();++it){
            coordN.push_back((*it).first);
            coordN.push_back((*it).second);
        }

        for (std::vector<pair<int, int> >::iterator it=I.coordU.begin();it!=I.coordU.end();++it){
            coordU.push_back((*it).first);
            coordU.push_back((*it).second);
        }


        TPPXMLInPut_t TPPInPut (I.name_,I.description_,I.nNodes_,I.nUsers_,nmc,umc,am,coordN,coordU);

        xml_schema::namespace_infomap map;

        map[""].name = "";
        map[""].schema = "GTPPXMLInPut.xsd";

        char aux2[200];

        strcpy(aux2,I.name_.c_str());
        strcat(aux2,".xml");

        std::ofstream ofs (aux2);
        //hello (ofs, h, map);

        TPPInPutXML(ofs,TPPInPut,map);

        //GTPPXMLInPut_t InPut(TPPInPut,I.capacity());

        //GTPPXMLInPut(ofs,InPut,map);


    } catch (const xml_schema::exception& e) {
        std::cerr << e << std::endl;
        return 1;
    }

    return 0;
}

}

//
// C++ Implementation: GTPP2XML
//
// Description:
//
//
// Author: Jorge Riera-Ledesma <jriera@ull.es>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
