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
*  Copyright (C) 2011 by the authors and their organisations:             *
*    Alistair Braden            1)                                        *
*    Wilfried Kausel            2)         kausel(at)mdw.ac.at            *
*    Delphine Cadefaux          2)                                        *
*    Vasileios Chatziioannou    2)                                        *
*    Sadjad Siddiq              2)                                        *
*                                                                         *
*    1) School of Physics and Astronomy, Univ. of Edinburgh, GB           *
        (http://www.ph.ed.ac.uk/)                                         *
*    2) Inst. of Music Acoustics, Univ. of Music, Vienna, AT              *
        (http://iwk.mdw.ac.at)                                            *
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

#ifndef ARTLINK_CPP
#define ARTLINK_CPP

//Vereinbarungen 2wischen Hornelement und ART-Interface

#include "ARTerror.h"
#include "ARTlink.h"
#include <iostream>

using namespace ART;
T_ART_Cmplx* T_ART_Cmplx::copy()
{
	T_ART_Cmplx* nc = new T_ART_Cmplx;
	nc->re = re;
	nc->im = im;
	return nc;
}

T_ART_Tripl* T_ART_Tripl::copy()
{
	T_ART_Tripl* nt = new T_ART_Tripl;
	nt->f  = f;
	nt->re = re;
	nt->im = im;
	return nt;
}

T_ART_Matrix* T_ART_Matrix::copy()
{
	T_ART_Matrix* nm = new T_ART_Matrix;
	nm->a11 = a11;
	nm->a12 = a12;
	nm->a21 = a21;
	nm->a22 = a22;
	return nm;
}

ARTvariant::ARTvariant() //default constructor 
{
	len = -1;
	typ = C_ART_undef;
	val = new T_ART_Var;
}

ARTvariant::ARTvariant(T_ART_Type t, int l) //constructor where type is specified
{
	len = -1;
	typ = t;
	val = new T_ART_Var;
	switch(typ)
	{
		case C_ART_str: val->s = NULL; break;
		case C_ART_matx: val->mx = NULL; break;
		case C_ART_nint: val->ni = NULL; break;
		case C_ART_nflo: val->nf = NULL; break;
		case C_ART_ndbl: val->nd = NULL; break;
		case C_ART_ncpx: val->nc = NULL; break;
		case C_ART_ntri: val->nt = NULL; break;
		case C_ART_nmat: val->nm = NULL; break;
		case C_ART_nmatx: val->nmx = NULL; break;
		case C_ART_nstr:  val->ns = NULL; break;
		case C_ART_na: val->na = NULL; break;
		// suppress warning for unhandled enums
		default: break;
	}
	if (l > 0) SetArrayLength(l);
}

ARTvariant::ARTvariant(const int i) 
{
	len = -1;
	typ = C_ART_int;
	val = new T_ART_Var;
	val->i = i;
}

ARTvariant::ARTvariant(const double d) 
{
	len = -1;
	typ = C_ART_dbl;
	val = new T_ART_Var;
	val->d = d;
}

ARTvariant::ARTvariant(const float f)  
{
	len = -1;
	typ = C_ART_flo;
	val = new T_ART_Var;
	val->f = f;
}

ARTvariant::ARTvariant(const char* s)  
{
	len = -1;
	typ = C_ART_str;
	val = new T_ART_Var;
	val->s = (char*)malloc ( strlen(s)+1  ); //+1 for 0 character
	strcpy(val->s, s); 
}

ARTvariant::ARTvariant(const char* s1, const char* s2)  
{
	len = -1; //will be changed in SetArrayLength
	typ = C_ART_nstr;
	val = new T_ART_Var;
	SetArrayLength(2);
	val->ns[0] =  (char*)malloc ( strlen(s1)+1  );
	strcpy(val->ns[0], s1); 
	val->ns[1] =  (char*)malloc ( strlen(s2)+1  );
	strcpy(val->ns[1], s2); 
}

ARTvariant::ARTvariant(const char* s1, const char* s2, const char* s3)  
{
	len = -1; //will be changed in SetArrayLength
	typ = C_ART_nstr;
	val = new T_ART_Var;
	SetArrayLength(3);
	val->ns[0] = (char*)malloc ( strlen(s1)+1  );
	strcpy(val->ns[0], s1); 
	val->ns[1] = (char*)malloc ( strlen(s2)+1  );
	strcpy(val->ns[1], s2); 
	val->ns[2] = (char*)malloc ( strlen(s3)+1  );
	strcpy(val->ns[2], s3); 
}

ARTvariant::ARTvariant(const char* s1, const char* s2, const char* s3, const char* s4)  
{
	len = -1; //will be changed in SetArrayLength
	typ = C_ART_nstr;
	val = new T_ART_Var;
	SetArrayLength(4);
	val->ns[0] = (char*)malloc ( strlen(s1)+1  );
	strcpy(val->ns[0], s1); 
	val->ns[1] = (char*)malloc ( strlen(s2)+1  );
	strcpy(val->ns[1], s2); 
	val->ns[2] = (char*)malloc ( strlen(s3)+1  );
	strcpy(val->ns[2], s3); 
	val->ns[3] = (char*)malloc ( strlen(s4)+1  );
	strcpy(val->ns[3], s4); 
}

ARTvariant::ARTvariant(const ARTvariant& orig) //copy constructor 
{
	typ = orig.typ;
	len = orig.len;
	val = new T_ART_Var;
	 
	int i;

    switch(typ)
	{
		//simple types: copy orig.value
		case C_ART_int: val->i = orig.val->i; break;
		case C_ART_flo: val->f = orig.val->f; break;
		case C_ART_dbl: val->d = orig.val->d; break;
		//pointers to structures: copy structure
		case C_ART_cpx: val->c = *orig.val->c.copy(); break;
		case C_ART_tri: val->t = *orig.val->t.copy(); break;
		case C_ART_mat: val->m = *orig.val->m.copy(); break;
		case C_ART_matx: val->mx = new Matrix(*orig.val->mx); break;
		
		//std::string (C-style)
		case C_ART_str:
				{
					//first make sure that the memory block pointed to by orig.val->s is big enough to hold s
					val->s = (char*)malloc (strlen(orig.val->s)+1  ); //+1 for 0 character
					strcpy(val->s, orig.val->s); 
				}
				break;
		//array of std::strings
		case C_ART_nstr:
			val->ns = (char**)malloc ( len * sizeof(char*)  );
			for (i=0; i<len; i++)
			{
				//first make sure that the memory block pointed to by orig.val->s is big enough to hold s
				val->ns[i] = (char*)malloc ( strlen(orig.val->ns[i])+1  ); //+1 for 0 character
				strcpy(val->ns[i], orig.val->ns[i]); 
			}
			break;

		//arrays of simple types
		case C_ART_nint: 
				{
					val->ni = (int*)malloc ( len * sizeof(int)  );
					for (i=0; i<len; i++) val->ni[i] = orig.val->ni[i];
				}
				break;

		case C_ART_nflo: 
				{
					val->nf = (float*)malloc ( len * sizeof(float)  );
					for (i=0; i<len; i++) val->nf[i] = orig.val->nf[i];
				}
				break;

		case C_ART_ndbl: 
				{
					val->nd = (double*)malloc ( len * sizeof(double)  );
					for (i=0; i<len; i++) val->nd[i] = orig.val->nd[i];
				}
				break;

		//arrays of structures - the structs are no pointers and can be copied with =
        case C_ART_ncpx:
				{
					val->nc = (T_ART_Cmplx*)malloc ( len * sizeof(T_ART_Cmplx)  );
					for (i=0; i<len; i++) val->nc[i] = orig.val->nc[i];
				}
				break;

        case C_ART_ntri:
				{
					val->nt = (T_ART_Tripl*)malloc ( len * sizeof(T_ART_Tripl)  );
					for (i=0; i<len; i++) val->nt[i] = orig.val->nt[i];
				}
				break;

        case C_ART_nmat:
				{
					val->nm = (T_ART_Matrix*)malloc ( len * sizeof(T_ART_Matrix)  );
					for (i=0; i<len; i++) val->nm[i] = orig.val->nm[i];
				}
				break;

        case C_ART_nmatx:
				{
					val->nmx = (Matrix**)malloc ( len * sizeof(Matrix*)  );
					for (i=0; i<len; i++) val->nmx[i] = new Matrix(*orig.val->nmx[i]);
				}
				break;

		// in case the current variant is just a container for
		// multiple variant objects, copy memory pointer
        case C_ART_na:
				val->na = orig.val->na;
				break;

        case C_ART_undef: break; //do nothing if undefined

		default: throw ARTerror("ARTvariant (copy constructor)", "The specified type is unknown."); 
	}//end switch
}

ARTvariant::~ARTvariant()
{
	int i;
	//free memory occupied if type is array or struct
    switch(typ)
	{
		case C_ART_str: free(val->s); val->s = NULL; break;
		case C_ART_matx: delete val->mx;  val->mx = NULL; break;
		case C_ART_nint: free(val->ni); val->ni = NULL; break;
		case C_ART_nflo: free(val->nf); val->nf = NULL; break;
		case C_ART_ndbl: free(val->nd); val->nd = NULL; break;
		case C_ART_ncpx: free(val->nc); val->nc = NULL; break;
		case C_ART_ntri: free(val->nt); val->nt = NULL; break;
		case C_ART_nmat: free(val->nm); val->nm = NULL; break;
		case C_ART_nstr:
			for (i=0; i<len; i++) {free(val->ns[i]);  val->ns[i] = NULL;}
			free(val->ns); val->ns = NULL;
			break;

		case C_ART_nmatx:
			for (i=0; i<len; i++) {delete val->nmx[i];  val->nmx[i] = NULL;}
			free(val->nmx); val->nmx = NULL;
			break;

		// nothing to do in case of arrays, memory management
		// is done somewhere else
		case C_ART_na:
			break;
 

 		case C_ART_undef: break; //do nothing if undefined
		//and nothing if any of the simple types...
 		default: break;
//		default: throw ARTerror("ARTvariant (destructor)", "The specified type is unknown."); 
	}//end switch

	len = -1;
	typ = C_ART_undef;
	delete val;
	val = NULL;
}

void ARTvariant::SetArrayLength(int l)
{
	//Debug
	if (l <= 0) throw ARTerror("ARTvariant::SetArrayLength", "Specified length is smaller than 1.");
	if (len == -1) //no array allocated; pointers contain undefined value, so realloc is not possible
	{
		int i;
		len = l;
		switch(typ)
		{
			//array of *char
			case C_ART_nstr: val->ns = (char**) malloc ( len * sizeof(char*)  ); for (i=0;i<len;i++) val->ns[i]=NULL; break;
			case C_ART_nint: val->ni =  (int*)malloc ( len * sizeof(int)  ); break;
			case C_ART_nflo: val->nf = (float*)malloc ( len * sizeof(float)  ); break;
			case C_ART_ndbl: val->nd = (double*)malloc ( len * sizeof(double)  ); break;
			case C_ART_ncpx: val->nc = (T_ART_Cmplx*)malloc ( len * sizeof(T_ART_Cmplx)  ); break;
			case C_ART_ntri: val->nt = (T_ART_Tripl*)malloc ( len * sizeof(T_ART_Tripl)  ); break;
			case C_ART_nmat: val->nm = (T_ART_Matrix*)malloc ( len * sizeof(T_ART_Matrix)  ); break;
			case C_ART_nmatx: val->nmx = (Matrix**)malloc ( len * sizeof(Matrix*)  ); for (i=0;i<len;i++) val->nmx[i]=NULL; break;
			// in case of art variant array do nothing
			case C_ART_na: break;

			//non-array types and *char: do nothing.
			case C_ART_int:
			case C_ART_flo:
			case C_ART_dbl:
			case C_ART_cpx:
			case C_ART_tri:
			case C_ART_mat:
			case C_ART_matx:
			case C_ART_str:
			case C_ART_undef: break; //and also do nothing if undefined

			default: throw ARTerror("ARTvariant (copy constructor)", "The specified type is unknown."); 
		}//end switch
	}
	else //if len != -1 pointers are set to len elements
	{
		int i;
		int oldlen = len; if (oldlen < 0) oldlen = 0;
		len = l;
		switch(typ)
		{
			//array of std::strings
			case C_ART_nstr: val->ns = (char**) realloc ( val->ns, len * sizeof(char*)  ); for (i=0;i<len;i++) val->ns[i]=NULL; break;
			case C_ART_nint: val->ni =  (int*)realloc ( val->ni, len * sizeof(int)  ); break;
			case C_ART_nflo: val->nf = (float*)realloc ( val->nf, len * sizeof(float)  ); break;
			case C_ART_ndbl: val->nd = (double*)realloc ( val->nd, len * sizeof(double)  ); break;
			case C_ART_ncpx: val->nc = (T_ART_Cmplx*)realloc ( val->nc, len * sizeof(T_ART_Cmplx)  ); break;
			case C_ART_ntri: val->nt = (T_ART_Tripl*)realloc ( val->nt, len * sizeof(T_ART_Tripl)  ); break;
			case C_ART_nmat: val->nm = (T_ART_Matrix*)realloc ( val->nm, len * sizeof(T_ART_Matrix)  ); break;
			// in case of art variant array do nothing
			case C_ART_na: break;

			//non-array types and std::strings: do nothing.
			case C_ART_int:
			case C_ART_flo:
			case C_ART_dbl:
			case C_ART_cpx:
			case C_ART_tri:
			case C_ART_mat:
			case C_ART_str:
			case C_ART_undef: break; //and also do nothing if undefined

			default: throw ARTerror("ARTvariant (copy constructor)", "The specified type is unknown."); 
		}//end switch
	}
}

ARTvariant* ARTvariant::clone() 
{
	return new ARTvariant(*this);
}


void ARTvariant::SetVal(const int i, int ind) 
{
	switch(typ)
	{
		case C_ART_int: val->i = i; break;
		case C_ART_flo: val->f = i; break;
		case C_ART_dbl: val->d = i; break;
		case C_ART_cpx: 
			val->c.re = i; 
			val->c.im = 0;
			break;

		case C_ART_nint:
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else val->ni[ind] =  i;
			break;

		case C_ART_nflo: 
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else val->nf[ind] =  i;
			break;

		case C_ART_ndbl: 
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else val->nd[ind] =  i;
			break;

		case C_ART_ncpx: 
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else 
			{ 
				val->nc[ind].re =  i;
				val->nc[ind].im =  0;
			}
			break;

		case C_ART_na:
			/*if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else
			{
				if (val->na != NULL)
				{
					val->na[ind].SetVal(i);
				}
				else
				{
					throw ARTerror("ARTvariant::SetVal", "Array of ARTvariant objects has not been initialized!");
				}
			}*/
			break;

		default: throw  ARTerror("ARTvariant::SetVal", "An integer value can not be written to an ARTvariant of type %s1.", this->GetTypeString());
	}

}

void ARTvariant::SetVal(const double d, int ind) 
{
	switch(typ)
	{
		case C_ART_int: val->i = (int)d; break;
		case C_ART_flo: val->f = d; break;
		case C_ART_dbl: val->d = d; break;
		case C_ART_cpx: 
			val->c.re = d; 
			val->c.im = 0;
			break;

		case C_ART_nint:
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else val->ni[ind] =  (int)d;
			break;

		case C_ART_nflo: 
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else val->nf[ind] =  d;
			break;

		case C_ART_ndbl: 
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else val->nd[ind] =  d;
			break;

		case C_ART_ncpx: 
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else 
			{ 
				val->nc[ind].re =  d;
				val->nc[ind].im =  0;
			}
			break;

		case C_ART_na:
			/*if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else
			{
				if (val->na != NULL)
				{
					val->na[ind].SetVal(d);
				}
				else
				{
					throw ARTerror("ARTvariant::SetVal", "Array of ARTvariant objects has not been initialized!");
				}
			}*/
			break;

		default: throw  ARTerror("ARTvariant::SetVal", "A double value can not be written to an ARTvariant of type %s1.", this->GetTypeString());
	}
}

void ARTvariant::SetVal(const float f, int ind)  
{
	switch(typ)
	{
		case C_ART_int: val->i = (int)f; break;
		case C_ART_flo: val->f = f; break;
		case C_ART_dbl: val->d = f; break;
		case C_ART_cpx: 
			val->c.re = f; 
			val->c.im = 0;
			break;

		case C_ART_nint:
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else val->ni[ind] =  (int)f;
			break;

		case C_ART_nflo: 
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else val->nf[ind] =  f;
			break;

		case C_ART_ndbl: 
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else val->nd[ind] =  f;
			break;

		case C_ART_ncpx: 
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else 
			{ 
				val->nc[ind].re =  f;
				val->nc[ind].im =  0;
			}
			break;

		case C_ART_na:
			/*if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else
			{
				if (val->na != NULL)
				{
					val->na[ind].SetVal(f);
				}
				else
				{
					throw ARTerror("ARTvariant::SetVal", "Array of ARTvariant objects has not been initialized!");
				}
			}*/
			break;

		default: throw  ARTerror("ARTvariant::SetVal", "A float value can not be written to an ARTvariant of type %s1.", this->GetTypeString());
	}

}

void ARTvariant::SetVal(std::complex<double> c, int ind) 
{
	this->SetVal(c.real(), c.imag(), ind);
}

void ARTvariant::SetVal(const double re, const double im, int ind)
{
	switch(typ)
	{
		case C_ART_int: 
			if (((int)im) != 0) throw ARTerror("ARTvariant::SetVal", "A complex value can not be passed to an ARTvariant of type %s1.", this->GetTypeString());
			val->i = (int)re; 
			break;
		case C_ART_flo: 
			if (im != 0) throw ARTerror("ARTvariant::SetVal", "A complex value can not be passed to an ARTvariant of type %s1.", this->GetTypeString());
			val->f = re; 
			break;
		case C_ART_dbl: 
			if (im != 0) throw ARTerror("ARTvariant::SetVal", "A complex value can not be passed to an ARTvariant of type %s1.", this->GetTypeString());
			val->d = re; 
			break;
		case C_ART_cpx: 
			val->c.re = re; 
			val->c.im = im;
			break;

		case C_ART_nint:
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			if (im != 0) throw ARTerror("ARTvariant::SetVal", "A complex value can not be passed to an ARTvariant of type %s1.", this->GetTypeString());
			else val->ni[ind] =  (int)re;
			break;

		case C_ART_nflo: 
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			if (im != 0) throw ARTerror("ARTvariant::SetVal", "A complex value can not be passed to an ARTvariant of type %s1.", this->GetTypeString());
			else val->nf[ind] =  re;
			break;

		case C_ART_ndbl: 
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			if (im != 0) throw ARTerror("ARTvariant::SetVal", "A complex value can not be passed to an ARTvariant of type %s1.", this->GetTypeString());
			else val->nd[ind] =  re;
			break;

		case C_ART_ncpx: 
			if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else 
			{ 
				val->nc[ind].re =  re;
				val->nc[ind].im =  im;
			}
			break;

		case C_ART_na:
			/*if (len <= ind) throw ARTerror("ARTvariant::SetVal", "Index is out of bounds.");
			else
			{
				if (val->na != NULL)
				{
					val->na[ind].SetVal(re, im);
				}
				else
				{
					throw ARTerror("ARTvariant::SetVal", "Array of ARTvariant objects has not been initialized!");
				}

			}*/
			break;

		default: throw ARTerror("ARTvariant::SetVal", "A complex value can not be written to an ARTvariant of type %s1.", this->GetTypeString());
	}
}

void ARTvariant::SetVal(const char* s)  
{
	if (typ == C_ART_str)
	{
		//get rid of the old string
		free(val->s); val->s = NULL;
		//first make sure that the memory block pointed to by val->s is big enough to hold a_szVal
		val->s = (char*)malloc (strlen(s)+1  ); //+1 for 0 character
		strcpy(val->s, s); 
	}
	else throw ARTerror("ARTvariant::SetVal", "A string can not be written to an ARTvariant of type %s1.", this->GetTypeString());
}

void ARTvariant::SetVal(const char* s, int ind)  
{
	if (typ == C_ART_nstr)
	{
		if (len <= ind) throw ARTerror("ARTvariant::SetVal", "The index is out of bounds.", this->GetTypeString());
		//get rid of the old string
		free(val->ns[ind]); val->ns[ind] = NULL;
		//first make sure that the memory block pointed to by val->s is big enough to hold a_szVal
		val->ns[ind] = (char*)malloc (strlen(s)+1  ); //+1 for 0 character
		strcpy(val->ns[ind], s); 
	}
	/*else if (typ == C_ART_na)
	{
		if (len <= ind) throw ARTerror("ARTvariant::SetVal", "The index is out of bounds.");
		if (val->na != NULL) {
			val->na[ind].SetVal(s);
		}
		else
		{
			throw ARTerror("ARTvariant::SetVal", "Array of ARTvariant objects has not been initialized!");
		}
	}*/
	else throw ARTerror("ARTvariant::SetVal", "A string with index can not be written to an ARTvariant of type %s1.", this->GetTypeString());
}


void ARTvariant::SetType(T_ART_Type t, int l)
{
	if (!val) throw ARTerror("ARTvariant::SetType", "The field 'val' of the ARTvariant is NULL. (ARTvariant is not properly initialized.)");
	//free memory occupied if type is array or struct
	if (len>0)
	{
		int i;
		switch(typ)
		{
			default: break;
			case C_ART_str: free(val->s); val->s = NULL; break; //but usually len is -1, that's why we also have the else clause below
			case C_ART_nint: free(val->ni); val->ni = NULL; break;
			case C_ART_nflo: free(val->nf); val->nf = NULL; break;
			case C_ART_ndbl: free(val->nd); val->nd = NULL; break;
			case C_ART_ncpx: free(val->nc); val->nc = NULL; break;
			case C_ART_ntri: free(val->nt); val->nt = NULL; break;
			case C_ART_nmat: free(val->nm); val->nm = NULL; break;
			case C_ART_nstr:
				for (i=0; i<len; i++) {free(val->ns[i]); val->ns[i] = NULL;}
				free(val->ns); val->ns = NULL; 
				break;
			// only set pointer to null, memory management is done by
			// other instances
			case C_ART_na: val->na = NULL; break;
		}//end switch
	}
	else
		if (typ==C_ART_str) {free(val->s); val->s = NULL;}

	//remember that the memory was cleared
	len = -1;
	//change type and set length
	typ = t;
	switch(typ)
	{
		case C_ART_str: val->s = NULL; break;
		case C_ART_nint: val->ni = NULL; break;
		case C_ART_nflo: val->nf = NULL; break;
		case C_ART_ndbl: val->nd = NULL; break;
		case C_ART_ncpx: val->nc = NULL; break;
		case C_ART_ntri: val->nt = NULL; break;
		case C_ART_nmat: val->nm = NULL; break;
		case C_ART_nstr:  val->ns = NULL; break;
		case C_ART_na: val->na = NULL; break;
		default: break;
	}
	if (l > 0) SetArrayLength(l);
}

std::string ARTvariant::GetTypeString()
{
	switch (typ)
	{
		case C_ART_str: return "C_ART_str";
		case C_ART_enum: return "C_ART_enum";
		case C_ART_int: return "C_ART_int";
		case C_ART_dbl: return "C_ART_dbl";
		case C_ART_flo: return "C_ART_flo";
		case C_ART_cpx: return "C_ART_cpx";
		case C_ART_tri: return "C_ART_tri";
		case C_ART_mat: return "C_ART_mat";
		case C_ART_nstr: return "C_ART_nstr";
		case C_ART_nint: return "C_ART_nint";
		case C_ART_nflo: return "C_ART_nflo";
		case C_ART_ndbl: return "C_ART_ndbl";
		case C_ART_ncpx: return "C_ART_ncpx";
		case C_ART_ntri: return "C_ART_ntri";
		case C_ART_nmat: return "C_ART_nmat";
		case C_ART_na: return "C_ART_na";
		case C_ART_undef: return "C_ART_undef";
		default: return "(unknown type)";
	}
}

bool ARTvariant::IsEqual(ARTvariant* other)
{
	//first check if length and type are similar
	if (typ != other->typ) return false;
	if (len != other->len) return false;

	switch(typ)
	{
		case C_ART_enum: if (val->e != other->val->e) return false; break;
		case C_ART_int: if (val->i != other->val->i) return false; break;
		case C_ART_flo: if (val->f != other->val->f) return false; break;
		case C_ART_dbl: if (val->d != other->val->d) return false; break;

		case C_ART_str: 
			if ((val->s == NULL) && (other->val->s == NULL)) return true;
			if ((val->s == NULL) && (other->val->s != NULL)) return false;
			if ((val->s != NULL) && (other->val->s == NULL)) return false;
			if (0!=strcmp(val->s, other->val->s)) return false; 
			break;
		case C_ART_cpx: 
			if (0!=memcmp(&val->c, &other->val->c, sizeof(T_ART_Cmplx))) return false; 
			break;
		case C_ART_tri: 
			if (0!=memcmp(&val->t, &other->val->t, sizeof(T_ART_Tripl))) return false; 
			break;
		case C_ART_mat: 
			if (0!=memcmp(&val->m, &other->val->m, sizeof(T_ART_Matrix))) return false; 
			break;
		case C_ART_nstr: 
			int i;
			if ((val->ns == NULL) && (other->val->ns == NULL)) return true;
			if ((val->ns == NULL) && (other->val->ns != NULL)) return false;
			if ((val->ns != NULL) && (other->val->ns == NULL)) return false;
			for (i=0; i < len; i++)
			{
				if ((val->ns[i] == NULL) && (other->val->ns[i] == NULL)) return true;
				if ((val->ns[i] == NULL) && (other->val->ns[i] != NULL)) return false;
				if ((val->ns[i] != NULL) && (other->val->ns[i] == NULL)) return false;
				if (0!=strcmp(val->ns[i], other->val->ns[i])) return false; 
			}
			break;
		case C_ART_nint: 
			if ((val->ni == NULL) && (other->val->ni == NULL)) return true;
			if ((val->ni == NULL) && (other->val->ni != NULL)) return false;
			if ((val->ni != NULL) && (other->val->ni == NULL)) return false;
			if (0!=memcmp(val->ni, other->val->ni, len*sizeof(int))) return false; 
			break;
		case C_ART_nflo: 
			if ((val->nf == NULL) && (other->val->nf == NULL)) return true;
			if ((val->nf == NULL) && (other->val->nf != NULL)) return false;
			if ((val->nf != NULL) && (other->val->nf == NULL)) return false;
			if (0!=memcmp(val->nf, other->val->nf, len*sizeof(float))) return false; 
			break;
		case C_ART_ndbl: 
			if ((val->nd == NULL) && (other->val->nd == NULL)) return true;
			if ((val->nd == NULL) && (other->val->nd != NULL)) return false;
			if ((val->nd != NULL) && (other->val->nd == NULL)) return false;
			if (0!=memcmp(val->nd, other->val->nd, len*sizeof(double))) return false; 
			break;
		case C_ART_ncpx: 
			if ((val->nc == NULL) && (other->val->nc == NULL)) return true;
			if ((val->nc == NULL) && (other->val->nc != NULL)) return false;
			if ((val->nc != NULL) && (other->val->nc == NULL)) return false;
			if (0!=memcmp(val->nc, other->val->nc, len*sizeof(T_ART_Cmplx))) return false; 
			break;
		case C_ART_ntri: 
			if ((val->nt == NULL) && (other->val->nt == NULL)) return true;
			if ((val->nt == NULL) && (other->val->nt != NULL)) return false;
			if ((val->nt != NULL) && (other->val->nt == NULL)) return false;
			if (0!=memcmp(val->nt, other->val->nt, len*sizeof(T_ART_Tripl))) return false; 
			break;
		case C_ART_nmat: 
			if ((val->nm == NULL) && (other->val->nm == NULL)) return true;
			if ((val->nm == NULL) && (other->val->nm != NULL)) return false;
			if ((val->nm != NULL) && (other->val->nm == NULL)) return false;
			if (0!=memcmp(val->nm, other->val->nm, len*sizeof(T_ART_Matrix))) return false; 
			break;
		case C_ART_na:
			// only compare whether pointer of arrays are equal
			if ((val->na == NULL) && (other->val->na == NULL)) return true;
			if ((val->na == NULL) && (other->val->na != NULL)) return false;
			if ((val->na != NULL) && (other->val->na == NULL)) return false;
			/* for (i = 0; i < len; i++) {
				if ( (val->na[i].IsEqual( &(other->val->na[i]) )) == false ) {
					return false;
				}
			}*/
			return false;
			break;
		case C_ART_undef: break; //no comparison if undefined
		default: throw ARTerror("ARTvariant::IsEqual", "The specified type is unknown."); 
	}
	return true;
}

bool ARTvariant::IsEqual(const char* other) {
	if (typ != C_ART_str) {
		return false;
	}
	if ((val->s == NULL) && (other == NULL)) {
		return true;
	}
	if ((val->s == NULL) && (other != NULL)) {
		return false;
	}
	if ((val->s != NULL) && (other == NULL)) {
		return false;
	}
	return (0==strcmp(val->s, other));
}



#endif

