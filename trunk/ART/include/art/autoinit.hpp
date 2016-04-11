/***************************************************************************
*                                                                          *
*              Toolset for Analysis and Prototyping (TAP)                  *
*                             Version 2.5                                  *
*                                                                          *
* A Set of Signal Processing Commandline Tools which communicate via Pipes *
*                                                                          *
*                                                                          *
*  Copyright (C) 2011 by the authors:                                      *
*    Herbert Nachtnebel         1)      herbert.nachtnebel(at)oeaw.ac.at   *
*    Wilfried Kausel            2)      kausel(at)mdw.ac.at                *
*                                                                          *
*    1) Inst. f. Integrated Sensor Systems, OEAW, Wiener Neustadt, AT      *
*       (http://www.iiss.oeaw.ac.at)                                       * 
*    2) Inst. of Music Acoustics, Univ. of Music, Vienna, AT               *
*       (http://iwk.mdw.ac.at)                                             *
*                                                                          *
*  This program is free software: you can redistribute it and/or modify    *
*  it under the terms of the GNU General Public License as published by    *
*  the Free Software Foundation, either version 3 of the License, or       *
*  any later version.                                                      *
*                                                                          *
*  This program is distributed in the hope that it will be useful,         *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*  GNU General Public License for more details.                            *
*                                                                          *
*  You should have received a copy of the GNU General Public License       *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
*                                                                          *
****************************************************************************/
/*  Change log:                                                                                   */
/*                                                                                                */
/*     Date     Rev  Ver                            Description                                   */
/*  ----------- --- ----- ----------------------------------------------------------------------  */
/*  09-May-1999 BAR       Started implementation.                                                 */
/**************************************************************************************************/
/*  Description:                                                                                  */
/*                                                                                                */
/*                                                                                                */
/*                                                                                                */
/*                                                                                                */
/*                                                                                                */
/**************************************************************************************************/
#ifndef AUTOINIT_INCLUDED
#define AUTOINIT_INCLUDED


/**************************************************************************************************/
/*               Inline Functions, Forwarded Function Definitions, Template Functions             */
/**************************************************************************************************/
#ifndef DELAYPOINTER_DEFINED
#define DELAYPOINTER_DEFINED
template<class T>
    inline T* DelayPointer (T* p)                     // useful template for bilding linked lists
    {
        static T* head = 0;                           // initialized only at program startup
        T* o = head; head = p; return o;              // exchange given pointer with stored one
    } /* DelayPointer */
#endif


/**************************************************************************************************/
/*                                       Class Definitions                                        */
/**************************************************************************************************/
class AutoPgmInit
{
	static int initcount;
	static AutoPgmInit* first;

	AutoPgmInit* next;

	int drive_initialization (int argc, char** argv, char** envp);
	int drive_termination ();

protected:
	virtual int auto_init (int argc, char** argv, char** envp) = 0;
	virtual int auto_term () { return 0; }

public:
	AutoPgmInit() { next = DelayPointer (this); }
	virtual ~AutoPgmInit() {}

	static int init (int argc, char** argv, char** envp);
	static int term ();
};


/**************************************************************************************************/
/* The following definitions may be included only once into your project!                         */
/*                                                                                                */
/* You should include it into the source file were your main procedure is declared.               */
/* To include it, define MAIN before the statement, which includes this source file.              */
/**************************************************************************************************/
#ifdef MAIN


/**************************************************************************************************/
/*                                        Static Objects                                          */
/**************************************************************************************************/
AutoPgmInit* AutoPgmInit::first;
int AutoPgmInit::initcount;


/**************************************************************************************************/
/*                                           Functions                                            */
/**************************************************************************************************/
int AutoPgmInit::init (int argc, char** argv, char** envp)
{
	initcount = 0;
	first = DelayPointer ((AutoPgmInit*) 0);
	return first ? first->drive_initialization (argc, argv, envp) : 0;
}

int AutoPgmInit::drive_initialization (int argc, char** argv, char** envp)
{
	int retval = 0;

	if (next) 
	{
		retval = next->drive_initialization (argc, argv, envp);
		if (retval) return retval;
	}

	retval = auto_init (argc, argv, envp);
	if (retval) return retval;

	initcount++;
	return retval;
}

int AutoPgmInit::term ()
{
	return first ? first->drive_termination () : 0;
}

int AutoPgmInit::drive_termination ()
{
	int retval = 0;

	if (initcount-- > 0)
	{
		int rc = auto_term ();
		if (rc) retval = rc;
	}

	if (next)
	{
		int rc = next->drive_termination ();
		if (rc) retval = rc;
	}

	return retval;
}

#endif /* MAIN */
#endif /* AUTOINIT_INCLUDED */
