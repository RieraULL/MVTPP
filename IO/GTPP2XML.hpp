
#ifndef _GTPP2XM_HPP
#define _GTPP2XM_HPP


#include <GTPPInPut.hpp>
#include "GTPPXMLInPut.hpp"
#include <iostream>
#include <vector>


using namespace GTPP;

namespace GTPPXML{

int XML2GTPP(std::istream&,GTPPInPutFull&,int);

int GTPP2XML(GTPPInPutFull&);
}


#endif
