 #include "GCVRPIO.hpp"
 
 
 namespace GCVRP
 {
 
 const char* DOT_HEADER =
         {"digraph G {\n\norientation = landscape;\nranksep=1.1; \nrankdir =\"LR\";\nedge[fontsize = 6.0];\n\n"
         };
 
 const char* DOT_CLOSING =
         { "}"
         };
 
 const char* DOT_NODE_STYLE[5] =
         {
                 "shape = doublecircle, width = .3, ",
                 "shape = circle, width = .3, ",
                 "shape = circle, width = .2, ",
                 "shape = circle, color=blue , label=\"\"",
                 "shape = circle, color=black, label=\"\" "
         };
 
 const char* DOT_WEIGHT[11] =
         {
                 "style=solid, color=black, ",
                 "style=solid, color=dimgray, ",
                 "style=dashed, color=black, ",
                 "style=dashed, color=dimgray,  ",
                 "style=dashed, color=gray,  ",
                 "style=dotted, color=black, ",
                 "style=dotted, color=dimgray, ",
                 "style=dotted, color=gray, ",
                 "style=dotted, color=gray, ",
                 "style=dotted, color=gray, ",
                 "style=solid, color=gray,"
         };
 
 const char* ASS_WEIGHT[11] =
         {
                 "style=solid,  color=blue3,  ",
                 "style=solid,  color=blue, ",
                 "style=dashed, color=blue3,  ",
                 "style=dashed, color=blue,  ",
                 "style=dashed, color=blue,  ",
                 "style=dotted, color=blue, ",
                 "style=dotted, color=blue, ",
                 "style=dotted, color=blue, ",
                 "style=dotted, color=blue, ",
                 "style=dotted, color=blue, ",
                 "style=inv, "
         };
 
 
 

 
 
// ostream& GCVRPIO::WriteDOTGraph(ostream& os,
//                                 std::map<int,double>& Edgeweight,
//                                 std::map<int,double>& Assweight)
// {
//
//
//
//         os << std::endl;
//         os <<DOT_HEADER;
//
//         WriteDOTEdges(os,Edgeweight);
//         WriteDOTAssignment(os,Assweight);
//         WriteDOTUsers(os);
//         WriteDOTCycleNodes(os);
//
//         os <<DOT_CLOSING;
//
//         os << "\n" << std::endl;
//         os.flush();
//
//         return os;
// }
//

ostream& GCVRPIO::WriteDOTGraph(ostream& os,
                                 std::vector<double>& Edgeweight,
                                 std::vector<double>& Nodeweight,
                                 std::vector<double>& Assweight,
                                 GTPP::UserAvailabilityMatrix& avail)
 {



         os << std::endl;
         os <<DOT_HEADER;

         WriteDOTEdges(os,Edgeweight);
         WriteDOTAssignment(os,Assweight,avail);
         WriteDOTUsers(os);
         //WriteDOTCycleNodes(os,Nodeweight);

         os <<DOT_CLOSING;

         os << "\n" << std::endl;
         os.flush();

         return os;
 }


ostream& GCVRPIO::WriteDOTGraph(ostream& os,
        const double* xyz,
        std::vector<double>& Edgeweight,
        std::vector<double>& Nodeweight,
        std::vector<double>& Assweight)
 {


         const int nEdges= nNodes_*nNodes_;
         std::vector<double> x(nEdges);

         for(int i=0;i<nEdges;i++)
             x[i]=xyz[i];


         std::vector<double> y(nNodes_);

         for(int i=0;i<nNodes_;i++)
             y[i]=xyz[i*nNodes_+i];


         std::vector<double> z((nNodes_-1)*nUsers_);

         for(int i=0;i<(nNodes_-1)*nUsers_;i++)
             z[i]=xyz[nEdges+i];

         os << std::endl;
         os <<DOT_HEADER;

         WriteDOTEdges(os,x,Edgeweight);
         WriteDOTAssignment(os,z,Assweight);
         WriteDOTUsers(os);
         WriteDOTCycleNodes(os,y,Nodeweight);

         os <<DOT_CLOSING;

         os << "\n" << std::endl;
         os.flush();

         return os;
 }




 ostream& GCVRPIO::WriteDOTGraph(ostream& os,
                                TVectorOfCycles1& C,
                                TVectorOfAssignment1& A,
                                 std::vector<pair<int, int> >& coordN,
                                 std::vector<pair<int, int> >& coordU)
 {
         os << std::endl;
         os <<DOT_HEADER;

         WriteDOTEdges(os,C);
         WriteDOTAssignment(os,A);
         WriteDOTUsers(os,coordU);
         WriteDOTCycleNodes(os,coordN);

         os <<DOT_CLOSING;

         os << "\n" << std::endl;
         os.flush();

         return os;


 }

 ostream& GCVRPIO::WriteDOTGraph(ostream& os,
                                TVectorOfCycles1& C,
                                TVectorOfAssignment1& A)
 {
         os << std::endl;
         os <<DOT_HEADER;

         WriteDOTEdges(os,C);
         WriteDOTAssignment(os,A);
         WriteDOTUsers2(os);

         os <<DOT_CLOSING;

         os << "\n" << std::endl;
         os.flush();

         return os;


 }

  ostream& GCVRPIO::WriteDOTEdges(ostream& os, TVectorOfCycles1& C) {

        for (int i = 0; i < C.size(); i++) {

            for (int j = 0; j < C[i].size()-1; j++)
                os << C[i][j] << " -> " <<  C[i][j+1] << "[style=solid, color=black,arrowsize=0.50]" << std::endl;

            os <<  C[i][C[i].size()-1] << " -> " << 0  << "[style=solid, color=black,arrowsize=0.50]" << std::endl;
        }
                
        return os;
    }

ostream& GCVRPIO::WriteDOTAssignment(ostream& os,TVectorOfAssignment1& As)
{
       for (int i = 0; i < As.size(); i++) {

            for (int j = 0; j < As[i].size(); j++)
                os << "u"<< As[i][j].first << " -> " <<  As[i][j].second << "[style=solid, color=blue,arrowsize=0.50]" << std::endl;

        }

        return os;
}


 ostream& GCVRPIO::WriteDOTGraph(ostream& os,
                                 std::vector<double>& Edgeweight,
                                 std::vector<double>& Nodeweight,
                                 std::vector<double>& Assweight,
                                 std::vector<pair<int, int> >& coordN,
                                 std::vector<pair<int, int> >& coordU)
 {


         os << std::endl;
         os <<DOT_HEADER;

         WriteDOTEdges(os,Edgeweight);
         WriteDOTAssignment(os,Assweight);
         WriteDOTUsers(os,coordU);
         WriteDOTCycleNodes(os,coordN);

         os <<DOT_CLOSING;

         os << "\n" << std::endl;
         os.flush();

         return os;
 }


 ostream& GCVRPIO::WriteDOTGraph(ostream& os,
                                 std::vector<double>& Edgeweight,
                                 std::vector<double>& Nodeweight,
                                 std::vector<double>& Assweight
                                 )
 {
 
 
 
         os << std::endl;
         os <<DOT_HEADER;
 
         WriteDOTEdges(os,Edgeweight);
         WriteDOTAssignment(os,Assweight);
         WriteDOTUsers(os);
         //WriteDOTCycleNodes(os,Nodeweight);
 
         os <<DOT_CLOSING;
 
         os << "\n" << std::endl;
         os.flush();
 
         return os;
 }




 ostream& GCVRPIO::WriteDOTAssignment(ostream& os, std::vector<double>& Assweight,GTPP::UserAvailabilityMatrix& avail) {
        std::vector<double>::iterator it;
        char id[180];

        int i = 0;
        for (it = Assweight.begin(); it != Assweight.end(); ++it) {
            const double val = *it;

            const int t = (int) (floor(i / nUsers())) + 1;
            const int u = i % nUsers();

            if (val > 1E-5) {
                sprintf(id, ", label = \"%5.2lf\"", val);
                os << "u" << u << " -> " << t << "[style=solid, color=blue" << id << ",arrowsize=0.50]" << std::endl;
            }
            else{
                if (avail.available(u,t)){
                    os << "u" << u << " -> " << t << "[style=solid, color=yellow" << ",arrowsize=0.50]" << std::endl;
                }
            }
            i++;
        }
    }

 ostream& GCVRPIO::WriteDOTAssignment(ostream& os, std::vector<double>& Assweight) {
        std::vector<double>::iterator it;
        char id[180];

        int i = 0;
        for (it = Assweight.begin(); it != Assweight.end(); ++it) {
            const double val = *it;

            if (val > 1E-5) {
                const int t = (int) (floor(i / nUsers())) + 1;
                const int u = i % nUsers();

                sprintf(id, ", label = \"%5.2lf\"", val);
                os << "u" << u << " -> " << t << "[style=solid, color=blue" /*<< id*/ << ",arrowsize=0.50]" << std::endl;
            }
            i++;
        }
    }


    ostream& GCVRPIO::WriteDOTAssignment(ostream& os, std::vector<double>& Assweight,std::vector<double>& cost) {
        std::vector<double>::iterator it;
        char id[180];

        int i = 0;
        for (it = Assweight.begin(); it != Assweight.end(); ++it) {
            const double val = *it;

            if (val > 1E-5) {
                const int t = (int) (floor(i / nUsers())) + 1;
                const int u = i % nUsers();

                sprintf(id, ", label = \"%5.2lf\"", val);
                os << "u" << u << " -> " << t << "[style=solid, color=blue" /*<< id*/ << ",arrowsize=0.50] "<<cost[i] << std::endl;
            }
            i++;
        }
    }

    ostream& GCVRPIO::WriteDOTEdges(ostream& os, std::vector<double>& Edgeweight) {
        char id[180];

        for (int i = 0; i < Edgeweight.size(); i++) {

            if (Edgeweight[i] > 1E-5) {
                const int s = (int) (floor(i / nNodes()));
                const int t = i % nNodes();
                if ((s != t)) {
                    sprintf(id, ", label = \"%5.2lf\"", Edgeweight[i]);
                    os << s << " -> " << t << "[style=solid, color=black" << id << ",arrowsize=0.50] " << std::endl;
                }
            }
        }


        return os;
    }

  ostream& GCVRPIO::WriteDOTEdges(ostream& os, std::vector<double>& Edgeweight,std::vector<double>& cost) {
        char id[180];

        for (int i = 0; i < Edgeweight.size(); i++) {

            if (Edgeweight[i] > 1E-5) {
                const int s = (int) (floor(i / nNodes()));
                const int t = i % nNodes();
                if ((s != t)) {
                    sprintf(id, ", label = \"%5.2lf\"", Edgeweight[i]);
                    os << s << " -> " << t << "[style=solid, color=black" << id << ",arrowsize=0.50] "<< cost[i] << std::endl;
                }
            }
        }


        return os;
    }


 ostream& GCVRPIO::WriteDOTUsers(ostream& os)
 {
         //const int sz=UserSet_.nUsers();
         char auxVal[180];
         char coords[180];

         for (int i=0;i<nUsers_;i++) {
                 sprintf(auxVal,"label = \"%d\"",i);
                 //sprintf(coords,", pos =\"%4.1lf,%4.1lf \"",userCoords_[i].first,userCoords_[i].second);
                 os << "u"<<i<< " ["<< DOT_NODE_STYLE[USERS]<< auxVal /* <<coords*/ <<"]; "<< std::endl;
         }

         os << std::endl;
         return os;
 }

 ostream& GCVRPIO::WriteDOTUsers(ostream& os,
                                 std::vector<pair<int, int> >& coordU)
 {
          char coords[180];
         for (int i=0;i<nUsers_;i++) {

                 sprintf(coords,", pos =\"%4d00,%4d00 \"",coordU[i].first,coordU[i].second);
                 os << "u"<<i<< " ["<< DOT_NODE_STYLE[3]<< coords <<"]; "<< std::endl;
         }

         os << std::endl;
         return os;
 }


ostream& GCVRPIO::WriteDOTUsers2(ostream& os)
 {
         //const int sz=UserSet_.nUsers();
         char auxVal[180];
         char coords[180];

         for (int i=0;i<nUsers_;i++) {

                 //sprintf(coords,", pos =\"%4.1lf,%4.1lf \"",userCoords_[i].first,userCoords_[i].second);
                 os << "u"<<i<< " ["<< DOT_NODE_STYLE[3] <<"]; "<< std::endl;
         }

         os << std::endl;
         return os;
 }


 ostream& GCVRPIO::WriteDOTEdges(ostream& os,std::map<int,double>& Edgeweight)
 {
         char id[180];
          
         std::map<int,double>::iterator it;
         
         for (it=Edgeweight.begin();it!=Edgeweight.end();++it) {
 
                 const int s= (int)(floor((*it).first/nNodes()));
                 const int t= (*it).first%nNodes();
                 if ((s != t)&&(s!=0)) {
                         sprintf(id,", label = \"%5.1lf\"",(*it).second);
                         os << s<< " -> " << t<< "[style=solid, color=gray"<<id<<"]"<< std::endl;
                 }
         }
 
 
         return os;
 }
 
 ostream& GCVRPIO::WriteDOTGraph(ostream& os,
                                 std::vector<double>& NodeBweight,
                                 std::vector<double>& NodeAweight,
                                 std::vector<double>& AssWeight,
                                 std::vector<double>& ConWeight)
 {
 
    //     for (int k=0;k<nVehicles();k++) {
 
                 os << std::endl;
                 os <<DOT_HEADER;
 
                 WriteDOTEdges2(os,ConWeight);
                 WriteDOTAssignment(os,AssWeight);
                 WriteDOTUsers(os);
                 WriteDOTNodesTypeB(os,NodeBweight);
                 WriteDOTNodesTypeA(os,NodeAweight);
 
                 os <<DOT_CLOSING;
 
                 os << "\n" << std::endl;
                 os.flush();
      //   }
         return os;
 }
 
 
 ostream& GCVRPIO::WriteDOTNodesTypeB(std::ostream& os,int k,std::vector<double>& weight)
 {
         char coords[200];
         char id[200];
         const int base=k*nNodes();
         const int sz=nNodes();
 
         for (int i=0;i<sz;i++)
                 if (weight[base+ i]>DRW_EPS) {
                         //if (weight[i]>DRW_EPS)
                         sprintf(id," label = \"%d\\n %5.3lf\"",i,weight[base+i]);
                         //else
                         //  sprintf(id," label = \"%d\"",nodeId_[i]);
                         //sprintf(coords,", pos =\"%4.1lf,%4.1lf \"",nodeCoords_[i].first,nodeCoords_[i].second);
 
                         os <<i<< " ["<< DOT_NODE_STYLE[TYPE_B]<< DOT_WEIGHT[WEIGHT_FUNC(weight[base+i])]<< id<<coords <<"]; "<< std::endl;
                 }
 
         sprintf(id," label = \"%d\"",0);
         //sprintf(coords,", pos =\"%4.1lf,%4.1lf \"",nodeCoords_[0].first,nodeCoords_[0].second);
 
         os <<0<< " ["<< DOT_NODE_STYLE[TYPE_B]<< DOT_WEIGHT[WEIGHT_FUNC(1)]<< id<<coords <<"]; "<< std::endl;
 
         return os;
 }
 
 ostream& GCVRPIO::WriteDOTNodesTypeB(std::ostream& os,std::vector<double>& weight)
 {
         char coords[200];
         char id[200];    
         const int sz=nNodes();
 
         for (int i=0;i<sz;i++)
                 if (weight[i]>DRW_EPS) {
                         //if (weight[i]>DRW_EPS)
                         sprintf(id," label = \"%d\\n %5.3lf\"",i,weight[i]);
                         //else
                         //  sprintf(id," label = \"%d\"",nodeId_[i]);
                         //sprintf(coords,", pos =\"%4.1lf,%4.1lf \"",nodeCoords_[i].first,nodeCoords_[i].second);
 
                         os <<i<< " ["<< DOT_NODE_STYLE[TYPE_B]<< DOT_WEIGHT[WEIGHT_FUNC(weight[i])]<< id<<coords <<"]; "<< std::endl;
                 }
 
         sprintf(id," label = \"%d\"",0);
         //sprintf(coords,", pos =\"%4.1lf,%4.1lf \"",nodeCoords_[0].first,nodeCoords_[0].second);
 
         os <<0<< " ["<< DOT_NODE_STYLE[TYPE_B]<< DOT_WEIGHT[WEIGHT_FUNC(1)]<< id<<coords <<"]; "<< std::endl;
 
         return os;
 }
 
 ostream& GCVRPIO::WriteDOTNodesTypeA(std::ostream& os, int k,std::vector<double>& weight)
 {
         char coords[200];
         char id[200];
 
         int base=k*nNodes();
         for (int i=1;i<nNodes()/* nodeCoords_.size()*/;i++)
                 if (weight[base+i]>DRW_EPS) {
                         sprintf(id," label = \"%d\\n %5.3lf\"",i,weight[base+i]);
                         //sprintf(coords,", pos =\"%4.1lf,%4.1lf \"",nodeCoords_[i].first,nodeCoords_[i].second);
 
                         os <<i<< " ["<< DOT_NODE_STYLE[TYPE_A]<< DOT_WEIGHT[WEIGHT_FUNC(weight[base+i])]<< id<<coords <<"]; "<< std::endl;
                 }
 
         return os;
 }
 
 ostream& GCVRPIO::WriteDOTNodesTypeA(std::ostream& os, std::vector<double>& weight)
 {
         char coords[200];
         char id[200];
 
         for (int i=1;i<nNodes_;i++)
                 if (weight[i]>DRW_EPS) {
                         sprintf(id," label = \"%d\\n %5.3lf\"",i,weight[i]);
                         //sprintf(coords,", pos =\"%4.1lf,%4.1lf \"",nodeCoords_[i].first,nodeCoords_[i].second);
 
                         os <<i<< " ["<< DOT_NODE_STYLE[TYPE_A]<< DOT_WEIGHT[WEIGHT_FUNC(weight[i])]<< id<<coords <<"]; "<< std::endl;
                 }
 
         return os;
 }
 
 
 ostream& GCVRPIO::WriteDOTAssignment(std::ostream& os,int k,std::vector<double>& weight)
 {
         char auxVal[180];
 
         int base=k*(nNodes()-1)*nUsers();
 
         for (int i=0;i<(nNodes()-1)*nUsers();i++) {
                 if (weight[base+i]>DRW_EPS) {
                         const int pos=(i % (nUsers()*(nNodes()-1)));
                         const int vehicle= (int)(floor(i / (nUsers()*(nNodes()-1))));
                         const int n=((int)floor(pos/nUsers()))+1;
                         const int u=pos % nUsers();
 
                         sprintf(auxVal,"label = \"%5.3lf\"",weight[base+i]);
                         os << "u"<<u<< " -> " << n <<  " ["<< ASS_WEIGHT[WEIGHT_FUNC(weight[base+i])]<< auxVal<< ",arrowsize=0.50]; "<< std::endl;
                 }
         }
         os << std::endl;
         return os;
 }
 
// ostream& GCVRPIO::WriteDOTAssignment(std::ostream& os,std::vector<double>& weight)
// {
//         char auxVal[180];
//
//         for (int i=0;i<(nNodes()-1)*nUsers();i++) {
//                 if (weight[i]>DRW_EPS) {
//                         const int pos=(i % (nUsers()*(nNodes()-1)));
//                         const int vehicle= (int)(floor(i / (nUsers()*(nNodes()-1))));
//                         const int n=((int)floor(pos/nUsers()))+1;
//                         const int u=pos % nUsers();
//
//                         sprintf(auxVal,"label = \"%5.3lf\"",weight[i]);
//                         os << "u"<<u<< " -> " << n <<  " ["<< ASS_WEIGHT[WEIGHT_FUNC(weight[i])]<< auxVal<< "]; "<< std::endl;
//                 }
//         }
//         os << std::endl;
//         return os;
// }
 
 
 ostream& GCVRPIO::WriteDOTCycleNodes(std::ostream& os,std::vector<double>& Nodesweight,std::vector<double>& cost)
 {
 //         char coords[200];
 //         char id[200];
 // 
 //         for (int i=1;i<nodeCoords_.size();i++) {
 // 
 //                 double val=0;
 // 
 //                 if (Nodesweight.find(i)!=Nodesweight.end())
 //                     val=Nodesweight[i];
 //              
 //                 sprintf(id," label = \"%d (%lf)\"",nodeId_[i],val);
 //                 sprintf(coords,", pos =\"%4.1lf,%4.1lf \"",nodeCoords_[i].first,nodeCoords_[i].second);
 // 
 //                 os <<i<< " ["<< DOT_NODE_STYLE[TYPE_B]<< id<<coords <<"]; "<< std::endl;
 //         }
 
         return os;
 }
 
 ostream& GCVRPIO::WriteDOTCycleNodes(std::ostream& os, std::vector<pair<int, int> >& coordN)
 {
         char coords[200];
         //char id[200];
 
         for (int i=0;i<nNodes();i++) {
 
                 //sprintf(id," label = \"%d\"",i);
                 sprintf(coords,", pos =\"%4d00,%4d00 \"",coordN[i].first,coordN[i].second);
 
                 os <<i<< " ["<< DOT_NODE_STYLE[4]<<coords <<"]; "<< std::endl;
         }
 
         return os;
 }

  ostream& GCVRPIO::WriteDOTCycleNodes(std::ostream& os)
 {
         char coords[200];
         char id[200];

         for (int i=1;i<nNodes();i++) {

                 sprintf(id," label = \"%d\"",i);
                 //sprintf(coords,", pos =\"%4.1lf,%4.1lf \"",nodeCoords_[i].first,nodeCoords_[i].second);

                 os <<i<< " ["<< DOT_NODE_STYLE[TYPE_B] /*<<coords */<<"]; "<< std::endl;
         }

         return os;
 }

 
// ostream& GCVRPIO::WriteDOTCycle(ostream& os,TCycle& C,TAssignment& As)
// {
//
//
//         os << std::endl;
//         os <<DOT_HEADER;
//
//         WriteDOTCycle(os,C);
//         WriteDOTCycleAssignment(os,As);
//         WriteDOTUsers(os);
//         WriteDOTCycleNodes(os);
//
//         os <<DOT_CLOSING;
//
//         os << "\n" << std::endl;
//
//         return os;
// }
//
// ostream& GCVRPIO::WriteDOTCycleAssignment (ostream& os,TAssignment& As)
// {
//         TAssignment::iterator a_it;
//         Tassignment_ it;
//
//         for (a_it=As.begin();a_it!=As.end();++a_it) {
//                 it=*a_it;
//                 os << "u"<<(*a_it).first<< " -> " << (*a_it).second <<  " [style=solid,  color=blue3]; "<< std::endl;
//         }
//
//         return os;
// }
//
// ostream& GCVRPIO::WriteDOTCycle(ostream& os,TCycle& C)
// {
//         TnodeId_ i;
//
//         os << 0<< " -> " << C[0]<< "[style=solid, color=black]"<< std::endl;
//
//         for (i=0;i<C.size()-1;i++)
//                 os << C[i]<< " -> " << C[i+1]<< "[style=solid, color=black]"<< std::endl;
//
//         os << C[C.size()-1] << " -> " << 0<< "[style=solid, color=black]"<< std::endl;
//
//         return os;
// }
 
 ostream& GCVRPIO::WriteDOTEdges2(std::ostream& os,std::vector<double>& weight)
 {
         char auxVal[280];
         const int nEdges=nNodes()*nNodes();
 
         for (int i=0;i<nEdges;i++) {
                 if (weight[i]>DRW_EPS) {
 
                         const int pos=i % nEdges;
                         const int vehicle= (int)(floor(i / nEdges));
                         const int s=((int)floor(pos/nNodes()));
                         const int t=pos % nNodes();
 
                         sprintf(auxVal,"label = \" %5.3lf\"",weight[i]);
 //                         std::cout << auxVal << std::endl;
 //                         std::cout << weight[i] << std::endl;
 //                         std::cout << WEIGHT_FUNC(weight[i])<< std::endl;
 //                         std::cout << DOT_WEIGHT[WEIGHT_FUNC(weight[i])] << std::endl;
                         os << s<< " -> " << t<<  " ["<< DOT_WEIGHT[WEIGHT_FUNC(weight[i])]<< auxVal<< "]; "<< std::endl;
                 }
         }
 
         os << std::endl;
         return os;
 }
 
 
 ostream& GCVRPIO::WriteDOTEdges(std::ostream& os,int k,std::vector<double>& weight)
 {
         char auxVal[180];
         const int nEdges=nNodes()*nNodes();
         const int base=k*nEdges;
 
         for (int i=0;i<nEdges;i++) {
                 if (weight[base+i]>DRW_EPS) {
 
                         const int pos=i % nEdges;
                         const int vehicle= (int)(floor(i / nEdges));
                         const int s=((int)floor(pos/nNodes()));
                         const int t=pos % nNodes();
 
                         sprintf(auxVal,"label = \" %5.3lf\"",weight[base+ i]);
                         os << s<< " -> " << t<<  " ["<< DOT_WEIGHT[WEIGHT_FUNC(weight[base+ i])]<< auxVal<< "]; "<< std::endl;
                 }
         }
 
         os << std::endl;
         return os;
 }
 

 
 

 
 }
