#ifndef IOLIB_HPP_
#define IOLIB_HPP_

//-----------------------------------------------------------------------
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
//-----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

namespace iolib{

#define NO_AVAILABLE -1
#define LABEL_WIDTH 20
#define INFINITUM 999999


//-----------------------------------------------------------------------
// Class LIBItem
//
// Clase correspondiente a cada uno de los super elementos de que consta
// cada fichero. Existe elementos complejos y elementos simples. Los ele-
// mentos complejos requieren de forma adicional procedimientos para trans-
// formar los datos
// 
//-----------------------------------------------------------------------


class LIBItem{
protected:

	int					nTokens_;			// Número de Tokens posibles con sus
	const char**				Tokens_;			// funciones específicas de Lectura/Escritura
	int					nConfigurations_;	// Número de configuraciones posibles con la
	const  bool**				Configurations_;	// descripción de cada una de ellas

	vector<bool>				Done_;				// Señala que la función de lectura asociada a un
							// token ha sido ejecutada con éxito

protected:

	int Token(std::string);
	inline int nToken(void){return nTokens_;}
	bool Succed(void);
	std::string FormatedString(const char*);

public:

	LIBItem(int,
			const char**,
			int, 
			const bool**);

	virtual ~LIBItem(void);

	bool Read(std::string,istream&);				// Comprueba que la cadena leída es un token de la clase
											// Si es así se llama al procedimiento específico de lectura
	ostream& Write(ostream&);
	virtual bool Read(int,istream&)=0;
	virtual ostream& Write(int,ostream&)=0;
	void Inicializa(void);
};




//-----------------------------------------------------------------------
// Class GRAPHI
// 
//-----------------------------------------------------------------------

// Elemento (i,j) en una matriz de dimension nxn 
#define ELEMENTO_MATRIZ_TRIANGULAR(i,j,n)           n*i-(i*(i+1))/2+j-(i+1)
#define ELEMENTO_MATRIZ_TRIANGULAR_INFERIOR(i,j)      ((i-1)*(i-2))/2+(j-1)
#define ELEMENTO_MATRIZ_TRIANGULAR_INFERIOR_DIAG(i,j)         (i*(i+1))/2+j 
#define ELEMENTO_MATRIZ(i,j,n)                                        i*n+j

typedef int                    TcoordItem_;
typedef pair<TcoordItem_,TcoordItem_>   Tcoord_;
typedef vector<Tcoord_>                 TcoordArray_;

typedef int                  TidItem_;
typedef vector<TidItem_>                TidArray_;

// Array Id, Coordenada X, Coordenada Y
typedef vector<int>			nodeIdArray;
typedef TcoordArray_                    nodeCoordArray;
typedef vector<pair<int,int> >          edgeDataArray;
typedef vector<int>                     edgeWeightArray;

enum GRAPHI_TOKENS_   {					DIMENSION_,
							EDGE_WEIGHT_TYPE_,
							EDGE_WEIGHT_FORMAT_,
							EDGE_DATA_FORMAT_,
							NODE_COORD_TYPE_,
							NODE_COORD_SECTION_,
							EDGE_DATA_SECTION_,
							EDGE_WEIGHT_SECTION_,
							DEPOT_SECTION_,
							FIXED_EDGES_SECTION_,
							DISPLAY_DATA_TYPE_,
							DISPLAY_NODE_SECTION_
							};

enum EDGE_WEIGHT_TYPE_   {	EXPLICIT,
							EUC_2D,
							MAX_2D,
							MAN_2D,
							CEIL_2D};

#define EWT_SIZE 5

enum EDGE_WEIGHT_FORMAT_ {	FUNCTION,
							FULL_MATRIX,
							UPPER_ROW,
							UPPER_DIAG_ROW,
							LOWER_ROW1, 
							LOWER_DIAG_ROW,
							UPPER_COL,
							UPPER_DIAG_COL,
							LOWER_COL,
							LOWER_DIAG_COL};
#define EWF_SIZE 10

enum EDGE_DATA_FORMAT_   {	EDGE_LIST,
							ADJ_LIST};
#define EDF_SIZE 2

enum NODE_COORD_TYPE_    {	TWOD_COORDS,
							NO_COORDS
							};
#define NCT_SIZE 2

enum DISPLAY_DATA_TYPE_  {	NO_DISPLAY,
							COORD_DISPLAY,
							TWOD_DISPLAY};
#define DDT_SIZE 3

#define NGTOKENS 12


class GRAPHI;

typedef		bool (GRAPHI::* GRAPHI_LIBRead )(istream&);	// Puntero a función miembro LIBItem
typedef		bool (GRAPHI::* GRAPHI_LIBWrite)(ostream&);	// Puntero a función miembro LIBItem

typedef		int (GRAPHI::* GRAPHI_Distance)(int,int);  // Función de distancia

class GRAPHI:public LIBItem {

public:
	GRAPHI_LIBRead*		ReadList_;
	GRAPHI_LIBWrite*	WriteList_;

	GRAPHI_Distance		Distance_;

  //-----------------------------------------------------------------------
  // NODOS
  //-----------------------------------------------------------------------
  int			Dimension_;         // Número de paradas del problema+depósito+colegios+usuarios

  int			EdgeWeightType_;    // Tipo del Peso de la Arista
  int			EdgeWeightFormat_;  // Tipo del formato de la Arista
  int			EdgeDataFormat_;    // Tipo Dato de arista
  int			NodeCoordType_;     // Tipo coordenada Nodo
  int			DisplayDataType_;

  

  int			DepotId_;

  //-----------------------------------------------------------------------
  //Parte de DATOS NODOS
  //-----------------------------------------------------------------------
  //NODE_COORD_SECTION
  nodeIdArray	 NodeIds_;
  nodeCoordArray NodeCoords_;
  nodeIdArray	 NodeIdDisplay_;
  nodeCoordArray NodeCoordsDisplay_;
  
  //EDGE_DATA_SECTION
  edgeDataArray EdgeData_;

  //EDGE_WEIGH_SECTION
  edgeWeightArray EdgeWeight_;

private:
	int Token(std::string,const char**,int);
	void UpdateDistanceFunction(void);

	void ActualizaDistancia(void);

public:
	bool ReadDimension(istream&);
	bool ReadEDGE_WEIGHT_TYPE(istream&);
	bool ReadEDGE_WEIGHT_FORMAT(istream&);
	bool ReadEDGE_DATA_FORMAT(istream&);
	bool ReadNODE_COORD_TYPE(istream&);
	bool ReadNODE_COORD_SECTION(istream&);
	bool ReadEDGE_DATA_SECTION(istream&);
	bool ReadEDGE_WEIGHT_SECTION(istream&);
	bool ReadDEPOT_SECTION(istream&);
	bool ReadFIXED_EDGES_SECTION(istream&);
	bool ReadDISPLAY_DATA_TYPE(istream&);
	bool ReadDISPLAY_NODE_SECTION(istream&);

	bool WriteDimension(ostream&);
	bool WriteEDGE_WEIGHT_TYPE(ostream&);
	bool WriteEDGE_WEIGHT_FORMAT(ostream&);
	bool WriteEDGE_DATA_FORMAT(ostream&);
	bool WriteNODE_COORD_TYPE(ostream&);
	bool WriteNODE_COORD_SECTION(ostream&);
	bool WriteEDGE_DATA_SECTION(ostream&);
	bool WriteEDGE_WEIGHT_SECTION(ostream&);
	bool WriteDEPOT_SECTION(ostream&);
	bool WriteFIXED_EDGES_SECTION(ostream&);
	bool WriteDISPLAY_DATA_TYPE(ostream&);
	bool WriteDISPLAY_NODE_SECTION(ostream&);


public:

	int euc_2d(int,int);
	int explicito(int,int);
	int full_matrix(int,int);
	int triangular_Upper_Row_matrix(int,int);
	int triangular_Lower_Row_Diag_matrix(int i,int j);

public:
	GRAPHI(void);
	virtual ~GRAPHI(void);

	virtual bool Read(int,istream&);
	virtual ostream& Write(int,ostream&);

	inline int DepotId(void){return DepotId_;}
	inline void DepotId(int Id){DepotId_=Id;}
	inline int Dimension(void){return Dimension_;}
	inline int Distance(int i,int j){return (this->*Distance_)(i,j);}
	inline nodeIdArray& NodeIds(void){return NodeIds_;}
	inline int NodeId(int i){return NodeIds_[i];}
	inline double NodeCoordx(int i){return NodeCoords_[i].first;}
	inline double NodeCoordy(int i){return NodeCoords_[i].second;}
	inline bool Complete(void){return EdgeDataFormat_!=-1;}
	inline bool Asimetric(void){return EdgeWeightFormat_==FULL_MATRIX;}

	inline int& CoordsId(int i){return NodeIds_[i];}
	inline Tcoord_& Coords(int i){return NodeCoords_[i];}
	inline int& CoordsDisId(int i){return NodeIdDisplay_[i];}
	inline Tcoord_& CoordsDis(int i){return NodeCoordsDisplay_[i];}

	void Dimension(int);

	inline void ChangeNODE_COORD_TYPE(int v){NodeCoordType_=v;}
	inline void ChangeDISPLAY_DATA_TYPE(int v){DisplayDataType_=v;}
	inline void ChangeEDGE_WEIGHT_TYPE(int v){EdgeWeightType_=v;}

};

//-----------------------------------------------------------------------
// Class USERS
// 
//-----------------------------------------------------------------------

#define NUTOKENS 3

enum USERS_TOKENS_   {	N_USERS,
						USERS_SECTION,
						RATIO_SECTION};

class USERS;

typedef		bool (USERS::* USERS_LIBRead )(istream&);	// Puntero a función miembro LIBItem
typedef		bool (USERS::* USERS_LIBWrite)(ostream&);	// Puntero a función miembro LIBItem

class USERS:public LIBItem {

public:
	USERS_LIBRead*  ReadList_;
	USERS_LIBWrite* WriteList_;

	int									nUsers_;				// Número de usuarios
	vector<int>							UsersId_;				// Identificadores de los usuarios
	vector<Tcoord_ >		Coords_;					// Identificadores de los usuarios

	double								Ratio_;					// Radio de influencia de un usuario

public:
	USERS(void);
	virtual ~USERS(void);

	int Token(std::string,const char**,int);

	virtual bool Read(int,istream&);
	virtual ostream& Write(int,ostream&);

	bool ReadNUsers(istream&);
	bool ReadUsersSection(istream&);
	bool ReadRATIO_SECTION(istream&);

	bool WriteNUsers(ostream&);
    bool WriteUsersSection(ostream&);
	bool WriteRATIO_SECTION(ostream&);

	inline int nUsers(void){return nUsers_;}
	inline int& getUserId(int i){return UsersId_[i];}
	inline double getUserCoordX(int i){return Coords_[i].first;}
	inline double getUserCoordY(int i){return Coords_[i].second;}
	inline double Ratio(void){return Ratio_;}
	inline void Ratio(double r){Ratio_=r;}

	void nUsers(int);
};

//-----------------------------------------------------------------------
// Class VEHICLES
// 
//-----------------------------------------------------------------------

class VEHICLES;

typedef		bool (VEHICLES::* VEHICLES_LIBRead )(istream&);	// Puntero a función miembro LIBItem
typedef		bool (VEHICLES::* VEHICLES_LIBWrite)(ostream&);	// Puntero a función miembro LIBItem

class VEHICLES:public LIBItem {

public:
	VEHICLES_LIBRead*  ReadList_;
	VEHICLES_LIBWrite* WriteList_;

	int				     nVehicles_;			// Número de colegios destino
	vector<int>		     CapVehicles_;		// Capacidades de los vehículos
	vector<double>		 FCost_;				// Coste fijo
	vector<double>	     Cost_;				// Coste de ruta
    vector<unsigned int> Avail_;

public:
	bool ReadNVehicles(istream&);
	bool ReadVehicleSection(istream&);

	bool WriteNVehicles(ostream&);
    bool WriteVehicleSection(ostream&);

public:
	VEHICLES(void);
	virtual ~VEHICLES(void);

	virtual bool Read(int,istream&);
	virtual ostream& Write(int,ostream&);

	inline int nVehicles(void) {return nVehicles_;}
	inline int& CapVehicle(int i){return CapVehicles_[i];}
	inline void CapVehicle(int i,int v){CapVehicles_[i]=v;}
	inline double FCost(int i){return FCost_[i];}
	inline double Cost(int i){return Cost_[i];}
    inline unsigned int Avail(int i) {return Avail_[i];}

	void nVehicles(int);
};

//-----------------------------------------------------------------------
// Class HEAD
// 
//-----------------------------------------------------------------------

class HEAD;

typedef		bool (HEAD::* HEAD_LIBRead )(istream&);	// Puntero a función miembro LIBItem
typedef		bool (HEAD::* HEAD_LIBWrite)(ostream&);	// Puntero a función miembro LIBItem

class HEAD:public LIBItem {

public:
	HEAD_LIBRead*  ReadList_;
	HEAD_LIBWrite* WriteList_;

	std::string			Name_;			// Nombre
	std::string			Type_;			// Tipo
	std::string			Comment_;		// Comentario

public:
	bool ReadName(istream&);
	bool ReadType(istream&);
	bool ReadComment(istream&);

	bool WriteName(ostream&);
	bool WriteType(ostream&);
	bool WriteComment(ostream&);

public:
	HEAD(void);
	virtual ~HEAD(void);

	virtual bool Read(int,istream&);
	virtual ostream& Write(int,ostream&);

	//inline void Name(const char* n){Name_=n;}
	inline void Name(std::string s){Name_=s;}
	inline const char* Name(void) {return Name_.c_str();}
	inline void Type(std::string s){Type_=s;}
	inline void Comment(std::string s){Comment_=s;}

};


//-----------------------------------------------------------------------
// Class InstanceLIB
// 
//-----------------------------------------------------------------------


class InstanceLIB{
protected:
	vector<LIBItem*> Components_;
public:
	InstanceLIB(void);
	virtual ~InstanceLIB(void);

	bool Read(istream&);
	void Write(ostream&);

	void Explore(std::string,istream&);

	inline LIBItem* Components(int i) {return Components_[i];}
	
private:
	void Inicializa(void);
	bool ExtractToken(std::string&,istream&);
	void clean(std::string&);

};

//-----------------------------------------------------------------------
// Class TPPMVMSLIB
// 
//-----------------------------------------------------------------------


enum INXPROC_ {				HEAD_E,
							VEHICLES_E,
							USERS_E,
							GRAPHI_E};




class GCVRPLIB: public InstanceLIB{
public:
	GCVRPLIB(void);
	~GCVRPLIB(void);

	inline HEAD*		Head(void){return (HEAD*)(Components(HEAD_E));}
	inline VEHICLES*	Vehicles(void){return (VEHICLES*)(Components(VEHICLES_E));}
	inline USERS*		Users(void){return (USERS*)(Components(USERS_E));}
	inline GRAPHI*		Graphi(void){return (GRAPHI*)(Components(GRAPHI_E));}
	
};

class GCVRPLIB2: public GCVRPLIB{
public:
	GCVRPLIB2(       
                     std::string&			Name,			
	                 std::string&			Type,			
	                 std::string&			Comment,		   
                  
                     TidArray_&	            NodeIds,
                     TcoordArray_&          NodeCoords, 

                     TidArray_&	            UserIds,
                     TcoordArray_&          UserCoords,                                                
                     double&                ratio,

                     unsigned int&           nVT,	               
	                 std::vector<int>&		CapVehicles,		
	                 std::vector<double>&		FCost,				
	                 std::vector<double>&	Cost,
                    std::vector<unsigned int>&	Avail						
)

      {
       Head()->Comment_=Comment;
       Head()->Type_=Type;
       Head()->Name_=Name;

       Vehicles()->nVehicles_=nVT;
       Vehicles()->CapVehicles_=CapVehicles;
       Vehicles()->FCost_=FCost;
       Vehicles()->Cost_=Cost;
       Vehicles()->Avail_=Avail;
      
       Graphi()->Dimension_=NodeCoords.size();
       Graphi()->NodeCoords_=NodeCoords;
       Graphi()->NodeIds_=NodeIds;

       Graphi()->EdgeWeightType_=EUC_2D;
       Graphi()->NodeCoordType_=TWOD_COORDS;
       
       Users()->nUsers_=UserIds.size();
       Users()->UsersId_=UserIds;
       Users()->Coords_=UserCoords;
       Users()->Ratio_=ratio;
}
       
	~GCVRPLIB2(void)
     {}
	
};

}

#endif /*IOLIB_HPP_*/
