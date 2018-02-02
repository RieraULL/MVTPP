#include "iolib.hpp"

namespace iolib{
//-----------------------------------------------------------------------
// LIBItem
//-----------------------------------------------------------------------

LIBItem::LIBItem(int			nTk,
			const char**  			TkLst,
			int						nCfg, 
			const bool**  				CfgLst):
			nTokens_(nTk),
			Tokens_(TkLst),
			nConfigurations_(nCfg),
			Configurations_(CfgLst)
{}

LIBItem::~LIBItem(void)
{}

bool LIBItem::Succed(void)
{
	bool Encontrado=false;

	for(int j=0;(j<nConfigurations_)&&(!Encontrado);j++){
		bool hecho=true;
		for(int i=0;(i<nTokens_)&&(hecho);i++){
			if (Done_[i] !=Configurations_[j][i])
				hecho=false;
		}
		if (hecho) Encontrado=true;
	}

	return Encontrado;
}

bool LIBItem::Read(std::string s,istream& is)
{
	int inx=Token(s);

	if (inx>=0)							// Si se detecta el token se lee
		if (Read(inx,is)){
			Done_[inx]=true;
			return true;
		}
		else{
			cerr << "ERROR LEYENDO TOKEN "<<inx;
			exit(1);
		}
	else
		return false;
}

ostream& LIBItem::Write(ostream& os)
{
	for(int i=0;i<nTokens_;i++){
		Write(i,os);
	}

	return os;
}

int LIBItem::Token(std::string s)
{
	char seps[]   = " , \n:";
	char* token;
	token=strtok( (char*)(s.data()), seps );

	if (!token) return -1;
	for(int i=0;i<nTokens_;i++){
		if (!Done_[i]){
			// Eliminar spacios
			if(strcmp(token,Tokens_[i])==0)
				return i;
		}
	}
	return -1;
}

void LIBItem::Inicializa(void)
{	
		for(int i=0;i<nTokens_;i++)
			Done_[i] =false;
}


std::string LIBItem::FormatedString(const char* s)
{
	char aux[512];
	sprintf(aux,"%-22s",s);

	return std::string(aux);
}


//-----------------------------------------------------------------------
// VEHICLES
//-----------------------------------------------------------------------

const char* VEHICLE_TOKENS []={			"N_VEHICLES",
										"VEHICLE_SECTION"};

VEHICLES_LIBRead	 VEHICLE_READ []={	&VEHICLES::ReadNVehicles,
										&VEHICLES::ReadVehicleSection};

VEHICLES_LIBWrite VEHICLE_WRITE []={		&VEHICLES::WriteNVehicles,
										&VEHICLES::WriteVehicleSection};

//									N_VEHICLES	VEHICLE_SECTION
const bool VEHICLE_CFG [][2]={
								{	true,		true	},
														};

VEHICLES::VEHICLES(void):
			LIBItem(2,
			VEHICLE_TOKENS,
			1, 
			(const bool**)VEHICLE_CFG),
			ReadList_(VEHICLE_READ),
			WriteList_(VEHICLE_WRITE)
{
	for(int i=0;i<nTokens_;i++)
		Done_.push_back(false);
}

VEHICLES::~VEHICLES(void)
{}
 
bool VEHICLES::ReadNVehicles(istream& is)
{
	is >> nVehicles_;
	return true;
}

bool VEHICLES::ReadVehicleSection(istream& is)
{
	int aux;
	double aux2;

	for(int i=0;i<nVehicles_;i++){
		is >> aux;
		CapVehicles_.push_back(aux);
		is >> aux2;
		FCost_.push_back(aux2);
		is >> aux2;
		Cost_.push_back(aux2);
        is >> aux;
        Avail_.push_back(aux);
	}

	return true;
}

bool VEHICLES::WriteNVehicles(ostream& os)
{
	os << FormatedString(VEHICLE_TOKENS[0]) << " : " << nVehicles_<< endl;
	return true;
}

bool VEHICLES::WriteVehicleSection(ostream& os)
{
	os << FormatedString(VEHICLE_TOKENS[1]) << " : "<< endl;
	for(int i=0;i<nVehicles_;i++){
		char aux[512];
		sprintf(aux," %5d %5.1lf %5.1lf %5d",CapVehicles_[i],FCost_[i],Cost_[i],Avail_[i]);
		os << aux<< endl;
	}

	return true;
}

bool VEHICLES::Read(int i,istream& is)
{
	return (this->*VEHICLE_READ[i])(is);
}

ostream& VEHICLES::Write(int i,ostream& os)
{
	(this->*VEHICLE_WRITE[i])(os);

	return os;
}

void VEHICLES::nVehicles(int n) 
{
		nVehicles_=n;
		CapVehicles_.resize(n);
		FCost_.resize(n);
		Cost_.resize(n);

}

//-----------------------------------------------------------------------
// HEAD
//-----------------------------------------------------------------------

const char* HEAD_TOKENS []={			"NAME",
										"TYPE",
										"COMMENT"};

HEAD_LIBRead	 HEAD_READ []={		&HEAD::ReadName,
										&HEAD::ReadType,
										&HEAD::ReadComment};

HEAD_LIBWrite HEAD_WRITE []={			&HEAD::WriteName,
										&HEAD::WriteType,
										&HEAD::WriteComment};

//									NAME	TYPE	COMMENT
const bool HEAD_CFG [][3]={
								{	true,		true,	true	},
								{	true,		true,	false	}
																	};

HEAD::HEAD(void):
			LIBItem(3,
			HEAD_TOKENS,
			2, 
			(const bool**)HEAD_CFG),
			ReadList_(HEAD_READ),
			WriteList_(HEAD_WRITE)
{
	for(int i=0;i<nTokens_;i++)
		Done_.push_back(false);
}

HEAD::~HEAD(void)
{}
 
bool HEAD::ReadName(istream& is)
{
	is >> Name_;
	return true;
}

bool HEAD::ReadType(istream& is)
{
	is >> Type_;
	return true;
}

bool HEAD::ReadComment(istream& is)
{
	is >> Comment_;
	return true;
}



bool HEAD::WriteName(ostream& os)
{
	
	os <<FormatedString(HEAD_TOKENS [0])<< " : "<< Name_<< endl;
	return true;
}

bool HEAD::WriteType(ostream& os)
{
	os << FormatedString(HEAD_TOKENS [1])<< " : "<<Type_<< endl;
	return true;
}

bool HEAD::WriteComment(ostream& os)
{
	os <<FormatedString(HEAD_TOKENS [2])<< " : "<< Comment_<< endl;
	return true;
}



bool HEAD::Read(int i,istream& is)
{
	return (this->*HEAD_READ[i])(is);
}

ostream& HEAD::Write(int i,ostream& os)
{
	(this->*HEAD_WRITE[i])(os);

	return os;
}



//-----------------------------------------------------------------------
// Class GRAPHI
// 
//-----------------------------------------------------------------------

const char *EdgeWeightTypeString[]={    "EXPLICIT",
										"EUC_2D",
										"MAX_2D",
										"MAN_2D",
										"CEIL_2D"};

const char* EdgeWeightFormatString []={ "FUNCTION",
										"FULL_MATRIX",
										"UPPER_ROW",
										"UPPER_DIAG_ROW",
										"LOWER_ROW",
										"LOWER_DIAG_ROW",
										"UPPER_COL",
										"UPPER_DIAG_COL",
										"LOWER_COL",
										"LOWER_DIAG_COL"};

const char* EdgeDataFormatString []={	"EDGE_LIST",
										"ADJ_LIST"};

const char* NodeCoordTypeString[]={		"TWOD_COORDS",
										"NO_COORDS"};

const char* DisplayDataTypeString[]={	"COORD_DISPLAY",
										"TWOD_DISPLAY",
										"NO_DISPLAY"};


const char* GRAPHI_TOKENS []={			"DIMENSION",
										"EDGE_WEIGHT_TYPE",
										"EDGE_WEIGHT_FORMAT",
										"EDGE_DATA_FORMAT",
										"NODE_COORD_TYPE",
										"NODE_COORD_SECTION",
										"EDGE_DATA_SECTION",
										"EDGE_WEIGHT_SECTION",
										"DEPOT_SECTION",
										"FIXED_EDGES_SECTION",
										"DISPLAY_DATA_TYPE",
										"DISPLAY_NODE_SECTION"
										};




GRAPHI_LIBRead	 GRAPHI_READ []={		&GRAPHI::ReadDimension,
										&GRAPHI::ReadEDGE_WEIGHT_TYPE,
										&GRAPHI::ReadEDGE_WEIGHT_FORMAT,
										&GRAPHI::ReadEDGE_DATA_FORMAT,
										&GRAPHI::ReadNODE_COORD_TYPE,									
										&GRAPHI::ReadNODE_COORD_SECTION,
										&GRAPHI::ReadEDGE_DATA_SECTION,
										&GRAPHI::ReadEDGE_WEIGHT_SECTION,
										&GRAPHI::ReadDEPOT_SECTION,
										&GRAPHI::ReadFIXED_EDGES_SECTION,
										&GRAPHI::ReadDISPLAY_DATA_TYPE,
										&GRAPHI::ReadDISPLAY_NODE_SECTION
										};




GRAPHI_LIBWrite GRAPHI_WRITE []={		&GRAPHI::WriteDimension,
										&GRAPHI::WriteEDGE_WEIGHT_TYPE,
										&GRAPHI::WriteEDGE_WEIGHT_FORMAT,
										&GRAPHI::WriteEDGE_DATA_FORMAT,
										&GRAPHI::WriteNODE_COORD_TYPE,										
										&GRAPHI::WriteNODE_COORD_SECTION,
										&GRAPHI::WriteEDGE_DATA_SECTION,
										&GRAPHI::WriteEDGE_WEIGHT_SECTION,
										&GRAPHI::WriteDEPOT_SECTION,
										&GRAPHI::WriteFIXED_EDGES_SECTION,
										&GRAPHI::WriteDISPLAY_DATA_TYPE,
										&GRAPHI::WriteDISPLAY_NODE_SECTION
										};

//									NAME	TYPE	COMMENT
const bool GRAPHI_CFG [][NGTOKENS]={
								{	true,		true,	true	},
								{	true,		true,	false	}
																	};


GRAPHI::GRAPHI(void):
			LIBItem(NGTOKENS,
			GRAPHI_TOKENS,
			2, 
			(const bool**)GRAPHI_CFG),
			ReadList_(GRAPHI_READ),
			WriteList_(GRAPHI_WRITE),
			Distance_(NULL),
			EdgeWeightType_(NO_AVAILABLE),   
			EdgeWeightFormat_(NO_AVAILABLE), 
			EdgeDataFormat_(NO_AVAILABLE),    
			NodeCoordType_(NO_AVAILABLE),     
  			DisplayDataType_(NO_AVAILABLE)
{
	for(int i=0;i<nTokens_;i++)
		Done_.push_back(false);
}

GRAPHI::~GRAPHI(void)
{}
 

int GRAPHI::Token(std::string s,const char** lista,int l)
{
	char seps[]   = " , \n:";
	char *token=  strtok( (char*)(s.data()), seps );

	for(int i=0;i<l;i++)
		if(strcmp(token,lista[i])==0)
			return i;
		
	cerr << "ERROR leyendo Token "<<s<<endl;
	return -1;
}


bool GRAPHI::ReadDimension(istream& is)
{
	is >> Dimension_;

	return true;
}

bool GRAPHI::ReadEDGE_WEIGHT_TYPE(istream& is)
{
	std::string aux;

	is >> aux;
	EdgeWeightType_=Token(aux,EdgeWeightTypeString,EWT_SIZE);
	return true;
}

bool GRAPHI::ReadEDGE_WEIGHT_FORMAT(istream& is)
{
	std::string aux;

	is >> aux;
	EdgeWeightFormat_=Token(aux,EdgeWeightFormatString,EWF_SIZE);

	return true;
}

bool GRAPHI::ReadEDGE_DATA_FORMAT(istream& is)
{
	std::string aux;

	is >> aux;
	EdgeDataFormat_=Token(aux,EdgeDataFormatString,EDF_SIZE);

	return true;
}

bool GRAPHI::ReadNODE_COORD_TYPE(istream& is)
{
	std::string aux;

	is >> aux;
	NodeCoordType_=Token(aux,NodeCoordTypeString,NCT_SIZE);

	return true;
}

bool GRAPHI::ReadNODE_COORD_SECTION(istream& is)
{
	pair<double,double> aux;
	int id;

	for(int i=0;i<Dimension_;i++){
		is >> id;

		is >> aux.first;
		is >> aux.second;

		NodeIds_.push_back(id);
		NodeCoords_.push_back(aux);
	}

	return true;
}

bool GRAPHI::ReadEDGE_DATA_SECTION(istream& is)
{
	return true;
}

bool GRAPHI::ReadEDGE_WEIGHT_SECTION(istream& is)
{
	return true;
}

bool GRAPHI::ReadDEPOT_SECTION(istream& is)
{
	is >> DepotId_;

	return true;
}

bool GRAPHI::ReadDISPLAY_NODE_SECTION(istream&is )
{
	pair<double,double> aux;
	int id;

	for(int i=0;i<Dimension_;i++){
		is >> id;

		is >> aux.first;
		is >> aux.second;

		NodeIdDisplay_.push_back(id);
		NodeCoordsDisplay_.push_back(aux);
	}

	return true;
}

bool GRAPHI::ReadFIXED_EDGES_SECTION(istream& is)
{
	return true;
}

bool GRAPHI::ReadDISPLAY_DATA_TYPE(istream& is)
{
	std::string aux;

	is >> aux;
	DisplayDataType_=Token(aux,DisplayDataTypeString,DDT_SIZE);

	return true;
}



bool GRAPHI::WriteEDGE_WEIGHT_TYPE(ostream& os)
{
	if (EdgeWeightType_!=NO_AVAILABLE){
		os << FormatedString(GRAPHI_TOKENS [EDGE_WEIGHT_TYPE_]) << " : "<< EdgeWeightTypeString[EdgeWeightType_] <<endl; 
		return true;
	} else return false;
	
}

bool GRAPHI::WriteEDGE_WEIGHT_FORMAT(ostream& os)
{
	if (EdgeWeightFormat_!=NO_AVAILABLE){
		os << FormatedString(GRAPHI_TOKENS [EDGE_WEIGHT_FORMAT_]) << " : "<< EdgeWeightFormatString[EdgeWeightFormat_] <<endl; 
		return true;
	} else return false;
}

bool GRAPHI::WriteEDGE_DATA_FORMAT(ostream& os)
{
	if (EdgeDataFormat_!=NO_AVAILABLE){
		os << FormatedString(GRAPHI_TOKENS [EDGE_DATA_FORMAT_]) << " : "<< EdgeDataFormatString[EdgeDataFormat_] <<endl; 
		return true;
	} else return false;
	
}

bool GRAPHI::WriteNODE_COORD_TYPE(ostream& os)
{
	if (NodeCoordType_!=NO_AVAILABLE){
		os << FormatedString(GRAPHI_TOKENS [NODE_COORD_TYPE_]) << " : "<< NodeCoordTypeString[NodeCoordType_] <<endl; 
		return true;
	} else return false;

}

bool GRAPHI::WriteNODE_COORD_SECTION(ostream& os)
{
	if ((NodeCoordType_!=NO_AVAILABLE)||(NodeCoordType_!=NO_COORDS)){
		os << FormatedString(GRAPHI_TOKENS [NODE_COORD_SECTION_]) << " : "<< endl;
		for(int i=0;i< Dimension_;i++){
			char aux[512];
			sprintf(aux," %5d %5.0f %5.0f ",NodeIds_[i],NodeCoords_[i].first,NodeCoords_[i].second);
			os << aux << endl;
			//os << NodeIds_[i] <<" "<< NodeCoords_[i].first <<" " << NodeCoords_[i].second  << endl;
		} 
		return true;
	} else return false;
	
}

bool GRAPHI::WriteEDGE_DATA_SECTION(ostream&)
{
	return true;
}

bool GRAPHI::WriteEDGE_WEIGHT_SECTION(ostream&)
{
	return true;
}
bool GRAPHI::WriteDEPOT_SECTION(ostream& os)
{
	os << FormatedString(GRAPHI_TOKENS [DEPOT_SECTION_]) << " : "<< endl;
	os << DepotId_ << endl;
	return true;
}
bool GRAPHI::WriteFIXED_EDGES_SECTION(ostream&)
{
	return true;
}

bool GRAPHI::WriteDimension(ostream& os)
{
	os << FormatedString(GRAPHI_TOKENS [DIMENSION_]) << " : "<< Dimension_ <<endl; 
	return true;
}

bool GRAPHI::WriteDISPLAY_NODE_SECTION(ostream& os)
{
	if (NodeCoordsDisplay_.size()!=0){
		os << FormatedString(GRAPHI_TOKENS [DISPLAY_NODE_SECTION_]) << " : "<< endl;
		for(int i=0;i< Dimension_;i++){
			char aux[512];
			sprintf(aux," %5d %5.0f %5.0f ",NodeIdDisplay_[i],NodeCoordsDisplay_[i].first,NodeCoordsDisplay_[i].second);
			os << aux << endl;
			//os << NodeIdDisplay_[i] <<" "<< NodeCoordsDisplay_[i].first <<" " << NodeCoordsDisplay_[i].second  << endl;
		}
	}
	return true;
}

bool GRAPHI::WriteDISPLAY_DATA_TYPE(ostream& os)
{
	if (DisplayDataType_!=NO_AVAILABLE){
		os << FormatedString(GRAPHI_TOKENS [DISPLAY_DATA_TYPE_]) << " : "<< DisplayDataTypeString[DisplayDataType_] <<endl; 
	}
	return true;
}



void GRAPHI::ActualizaDistancia(void)
{
	if (EdgeWeightType_==EUC_2D)
		Distance_=&GRAPHI::euc_2d;
	else if (EdgeWeightType_==EXPLICIT){
		if (EdgeWeightFormat_==FULL_MATRIX)
			Distance_=&GRAPHI::full_matrix;
		else if (EdgeWeightFormat_==UPPER_ROW)
			Distance_=&GRAPHI::triangular_Upper_Row_matrix;
		else if (EdgeWeightFormat_==LOWER_ROW1)
                 Distance_=&GRAPHI::triangular_Lower_Row_Diag_matrix;
		else {
			cerr << "ERROR!!!"<<endl;
			exit(1);
		}
	}
}

bool GRAPHI::Read(int i,istream& is)
{
	return (this->*GRAPHI_READ[i])(is);

	if (i==NGTOKENS-1) // Si ha llegado al final 
		// Post Procesamiento
		{
			ActualizaDistancia();
		}
}

ostream& GRAPHI::Write(int i,ostream& os)
{
	(this->*GRAPHI_WRITE[i])(os);

	return os;
}


int GRAPHI::euc_2d(int i,int j)
{
  double xd=NodeCoords_[i].first-NodeCoords_[j].first;
  double yd=NodeCoords_[i].second-NodeCoords_[j].second;
  
  double co=sqrt(xd*xd+yd*yd);
  return int(co);
}
 
int GRAPHI::explicito(int i,int j)
{
  if (EdgeWeightFormat_==FULL_MATRIX)
    return full_matrix(i,j);
  else
    if (EdgeWeightFormat_==UPPER_ROW)
      return triangular_Upper_Row_matrix(i,j);
    else if (EdgeWeightFormat_==LOWER_DIAG_ROW)
      return triangular_Lower_Row_Diag_matrix(i,j);
    else return 1;
}

int GRAPHI::full_matrix(int i,int j)
{
  return EdgeWeight_[ELEMENTO_MATRIZ(i,j,Dimension_)];
} 

int GRAPHI::triangular_Upper_Row_matrix(int i,int j)
{ 
  if (i==j) return 0;
  else if (i<j)  
    return EdgeWeight_[ELEMENTO_MATRIZ_TRIANGULAR(i,j,Dimension_)];  
  else   
      return EdgeWeight_[ELEMENTO_MATRIZ_TRIANGULAR(j,i,Dimension_)];
}

int GRAPHI::triangular_Lower_Row_Diag_matrix(int i,int j)
{ 
  return EdgeWeight_[ELEMENTO_MATRIZ_TRIANGULAR_INFERIOR_DIAG(i,j)]; 
}

void GRAPHI::Dimension(int dim){
	Dimension_=dim;
	NodeIds_.resize(Dimension_);
	NodeCoords_.resize(Dimension_);
	NodeIdDisplay_.resize(Dimension_);
	NodeCoordsDisplay_.resize(Dimension_);
}


//-----------------------------------------------------------------------
// Class USERS
// 
//-----------------------------------------------------------------------



const char* USERS_TOKENS []={			"N_USERS",
										"USERS_SECTION",
										"USER_RATIO",};




USERS_LIBRead	 USERS_READ []={		&USERS::ReadNUsers,
										&USERS::ReadUsersSection,
										&USERS::ReadRATIO_SECTION
										};




USERS_LIBWrite USERS_WRITE []={			&USERS::WriteNUsers,
										&USERS::WriteUsersSection,
										&USERS::WriteRATIO_SECTION
										};

//									NAME	TYPE	COMMENT
const bool USERS_CFG [][NUTOKENS]={
								{	true,		true,	true	},
								{	true,		true,	true 	}
														};


USERS::USERS(void):
			LIBItem(NUTOKENS,
			USERS_TOKENS,
			2, 
			(const bool**)USERS_CFG),
			ReadList_(USERS_READ),
			WriteList_(USERS_WRITE),
			Ratio_(INFINITUM)
{
	for(int i=0;i<nTokens_;i++)
		Done_.push_back(false);
}

USERS::~USERS(void)
{}
 

int USERS::Token(std::string s,const char** lista,int l)
{
	char seps[]   = " , \n:";
	char *token=  strtok( (char*)(s.data()), seps );


	for(int i=0;i<l;i++)
		if(strcmp(token,lista[i])==0)
			return i;
		
	cerr << "ERROR leyendo Token "<<s<<endl;
	return -1;
}


bool USERS::ReadRATIO_SECTION(istream& is)
{
	is >> Ratio_;

	return true;
}

bool USERS::WriteRATIO_SECTION(ostream& os)
{
	if (Ratio_<INFINITUM){
		os << FormatedString(USERS_TOKENS [RATIO_SECTION]) << " : "<< Ratio_ <<endl; 
	}
	return true;
}

bool USERS::ReadNUsers(istream& is)
{
	is >> nUsers_;
	return true;
}

bool USERS::WriteNUsers(ostream& os)
{

	os << FormatedString(USERS_TOKENS [0])<< " : "<< nUsers_ << endl;
	return true;
}

bool USERS::ReadUsersSection(istream& is)
{
	int aux;

	for(int i=0;i<nUsers_;i++){
		is >> aux;
		UsersId_.push_back(aux);

		double x,y;

		is >> x;
		is >> y;

		Coords_.push_back(pair<double,double> (x,y));
	}

	return true;
}


bool USERS::WriteUsersSection(ostream& os)
{
	os << FormatedString(USERS_TOKENS [1])<< " : "<< endl;
	for(int i=0;i<nUsers_;i++){
		char aux[512];
		sprintf(aux," %5d %5.0f %5.0f",UsersId_[i],Coords_[i].first,Coords_[i].second);
		os << aux<< std::endl;
		//os << UsersId_[i] << endl;
	}

	return true;
}


bool USERS::Read(int i,istream& is)
{
	return (this->*USERS_READ[i])(is);
}


ostream& USERS::Write(int i,ostream& os)
{
	(this->*USERS_WRITE[i])(os);

	return os;
}

void USERS::nUsers(int n){
		nUsers_=n;
		UsersId_.resize(nUsers_);
}


//-----------------------------------------------------------------------
// InstanceLIB
//-----------------------------------------------------------------------

InstanceLIB::InstanceLIB(void)
{}

InstanceLIB::~InstanceLIB(void)
{}

void InstanceLIB::clean(std::string& s)
{
	if (s.size()<=0) return;
	
	unsigned base=0;
	unsigned longi=0;
	
	while((base<s.size())&&(!isalpha(s[base])))
		base++;
					
	if(base>=s.size()) return;
	
	while((base+longi<s.size())&&((isalpha(s[base+longi]))||(s[base+longi]=='_')))
		longi++;
	
	string s1= s.substr(base,longi);
	
	s=s1;
}

bool InstanceLIB::ExtractToken(std::string& s,istream& is)
{	
	getline(is,s, ':');
	clean(s);
	
	char str[1024];
	strcpy(str,s.data());

	if (!strstr(str,"EOF")) {

		char seps[]   = " , \n";

		char *token = strtok( str, seps );
        //std::cout << str << std::endl;
		if (token) 
			s=string(token);
			

		return true;
	} else return false;
}


void InstanceLIB::Explore(std::string s,istream& is)
{
	int sz=Components_.size();
	bool encontrado=false;

	for (int i=0;(i<sz)&&(!encontrado);i++)
			encontrado=Components_[i]->Read(s,is);
}


bool InstanceLIB::Read(istream& is)
{
	std::string s;

	Inicializa();
	while (ExtractToken(s,is)){
		Explore(s,is);
	}

	return true;
}

void InstanceLIB::Inicializa(void)
{	
	int sz=Components_.size();

	for (int i=0;i<sz;i++)
		Components_[i]->Inicializa();
}


void InstanceLIB::Write(ostream& os)
{
	int sz=Components_.size();

	for (int i=0;i<sz;i++){
			Components_[i]->Write(os);
			os << endl;
	}

	os << "EOF"<<endl;
}





//-----------------------------------------------------------------------
// GCVRPLIB
//-----------------------------------------------------------------------




GCVRPLIB::GCVRPLIB(void)
{
	LIBItem* aux= new HEAD();
	Components_.push_back(aux);

	aux= new VEHICLES();
	Components_.push_back(aux);

	aux= new USERS();
	Components_.push_back(aux);

	aux= new GRAPHI();
	Components_.push_back(aux);
}

GCVRPLIB::~GCVRPLIB(void)
{
	vector <LIBItem*>::iterator Iter;

	for ( Iter = Components_.begin() ; Iter != Components_.end() ; Iter++ )
		delete *Iter;
}



}
