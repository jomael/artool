/**************************************************************************
*                                                                         *
*                   Acoustic Research Tool (ART)                          *
*                                                                         *
*   A Simulation Framework with Modelling Library for Acoustic Systems    *
*                                                                         *
*         Project of the Workgroup 2 of the Technical Committee           *
*      Musical Acoustics of the European Acoustics Association EAA)       *
*                                                                         *
*   http://www.eaa-fenestra.org/technical-committees/ma/workgroups/wg2    *
*                                                                         *
*  Copyright (C) 2013 by the authors and their organisations:             *
*    Alistair Braden            1)                                        *
*    Wilfried Kausel            2)         kausel(at)mdw.ac.at            *
*    Delphine Cadefaux          2)                                        *
*    Vasileios Chatziioannou    2)                                        *
*    Sadjad Siddiq              2)                                        *
*    Clemens Geyer              2)                                        *
*                                                                         *
*    1) School of Physics and Astronomy, Univ. of Edinburgh, GB           *
*       (http://www.ph.ed.ac.uk/)                                         *
*    2) Inst. of Music Acoustics, Univ. of Music, Vienna, AT              *
*       (http://iwk.mdw.ac.at)                                            *
*                                                                         *
*  This program is free software: you can redistribute it and/or modify   *
*  it under the terms of the GNU General Public License as published by   *
*  the Free Software Foundation, either version 3 of the License, or      *
*  any later version.                                                     *
*                                                                         *
*  This program is distributed in the hope that it will be useful,        *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*  GNU General Public License for more details.                           *
*                                                                         *
*  You should have received a copy of the GNU General Public License      *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
*                                                                         *
***************************************************************************/

#ifndef ARTLINK_H
#define ARTLINK_H

//Vereinbarungen zwischen Hornelement und ART-Interface
#ifdef __cplusplus
#include <string>
#include <complex>
#include "matrix.h"
#endif
/** \addtogroup ARTimpl
 * @{
 */


//Datentypen:
typedef enum	
{
	C_ART_str,		///< string type
	C_ART_enum, 	///< enum type
	C_ART_int, 		///< integer type
	C_ART_dbl, 		///< double type
	C_ART_flo, 		///< float type
	C_ART_cpx, 		///< T_ART_Cmplx type
	C_ART_tri, 		///< T_ART_Tripl type
	C_ART_mat,		///< T_ART_Matrix type
	C_ART_matx,		///< math::matrix< std::complex<double> >* type
	C_ART_nstr,		///< string array type
	C_ART_nint,		///< integer array type
	C_ART_nflo,		///< float array type
	C_ART_ndbl,		///< double array type
	C_ART_ncpx,		///< T_ART_Cmplx array type
	C_ART_ntri,		///< T_ART_Tripl array type
	C_ART_nmat,		///< T_ART_Matrix array type
	C_ART_nmatx,	///< math::matrix< std::complex<double> >* array type
	C_ART_na,		///< ART::ARTvariant array type (container for other ART::ARTvariant objects)
	C_ART_undef  	///< uninitialized type
					} T_ART_Type;


/**
 * This structure represents a complex number.
 */  
#ifdef __cplusplus
struct T_ART_Cmplx	
{
	double re; 
	double im;
	T_ART_Cmplx* copy();
	//T_ART_Cmplx(const T_ART_Cmplx& c) {} //empty constructor... will have problems when doing smth. like "T_ART_Cmplx()", but strangely not when I add a field "double f;" ... WHY??????
};
#else
typedef struct
{
	double re;
	double im;
} T_ART_Cmplx;
#endif /* __cplusplus */

/**
 * In this structure a frequency and a complex number can be stored. 
 */
#ifdef __cplusplus
struct T_ART_Tripl	
{
	double f; 
	double re; 
	double im;
	T_ART_Tripl* copy();
};
#else
typedef struct
{
	double f;
	double re;
	double im;
} T_ART_Tripl;
#endif /* __cplusplus */

/**
 * This structure represents a 2x2 matrix.
 */  
#ifdef __cplusplus
struct T_ART_Matrix
{
	T_ART_Cmplx a11; 
	T_ART_Cmplx a12; 
	T_ART_Cmplx a21; 
	T_ART_Cmplx a22;

	T_ART_Matrix* copy();
//	double** a; 
};
#else
typedef struct
{
	T_ART_Cmplx a11;
	T_ART_Cmplx a12;
	T_ART_Cmplx a21;
	T_ART_Cmplx a22;
} T_ART_Matrix;
#endif /* __cplusplus */

#ifdef __cplusplus
typedef math::matrix< std::complex<double> > Matrix;

namespace ART{// cbg: forward declaration
struct ARTvariant;
}
/**
 * This structure provides a field, where values or arrays of different types can be 
 * stored. Since it is an union only a value of one type can be stored at once. 
 * ARTvariant:s implement interfaces for T_ART_Var:s and should be favored.
 * @see ARTvariant 
 */ 
typedef union
{
	char*			s; //bei union kann kein string verwendet werden, weil dieser copy constr. enthält
	int				e;
	int				i;
	double			d;	//war f!
	float			f; 
	T_ART_Cmplx c; 
	T_ART_Tripl t;
	T_ART_Matrix m;
	math::matrix< std::complex<double> >* mx;
	char**			ns;
	int*			ni;
	double*			nd;
	float*			nf;
	T_ART_Cmplx*	nc;
	T_ART_Tripl*	nt;
	T_ART_Matrix*	nm;
	void*		na;
	math::matrix< std::complex<double> >** nmx;
	//pointers are not seperate types but nX types with len = 1 
} T_ART_Var;
namespace ART{
/**
 * An instance of this structure can hold variables or arrays of different type. The 
 * structure keeps track of the type of variable and the array length. Use the 
 * functions provided to read or write to an ARTvariant structure safely.
*/

struct ARTvariant
{
	int len;			/**< length of arrays */
	T_ART_Type typ;		/**< data type */
	T_ART_Var*  val;		
	ARTvariant(); /**< default constructor */ 
	ARTvariant(T_ART_Type t, int l=-1); /**< constructor where type and length is specified */
	ARTvariant(const int i); 
	ARTvariant(const double d); 
	ARTvariant(const float f);  
	ARTvariant(const char* s);  
	ARTvariant(const char* s1, const char* s2);  
	ARTvariant(const char* s1, const char* s2, const char* s3);  
	ARTvariant(const char* s1, const char* s2, const char* s3, const char* s4);  
	ARTvariant(const ARTvariant& orig); /**< copy constructor */ 
	virtual void SetVal(const int i, const int ind = 0);
	virtual void SetVal(const double d, const int ind = 0);
	virtual void SetVal(const float f, const int ind = 0);
	virtual void SetVal(std::complex<double>, const int ind = 0);
	virtual void SetVal(const double re, const double im, const int ind = 0);
	virtual void SetVal(const char* s);
	virtual void SetVal(const char* s, int ind);
	virtual void SetType(T_ART_Type t, int l=-1);
	virtual std::string GetTypeString();
	virtual bool IsEqual(ARTvariant* other);
	virtual bool IsEqual(const char* s);
	virtual ~ARTvariant();
	virtual void SetArrayLength(int l);
	ARTvariant* clone();
} ;
}
#endif

//**************************************************************************************************************
/*
typedef struct {
	int len;
	T_ART_Type typ;
	T_ART_Var val;
} ARTresult;
*/
//**************************************************************************************************************

//define CalculationFunc as pointer to a func. of the form "void XXX();"
//typedef void (*CalculationFunc) ();

//**************************************************************************************************************


/** 
 * @}
 */

#endif
