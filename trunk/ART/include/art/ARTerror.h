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

#ifndef ARTERROR_H
#define ARTERROR_H


#include <string>

/** \addtogroup ARTimpl
 * @{
 */

/**
 * For errors that make the continuation of the programme impossible an object of this 
 * class is thrown. It contains a description of the error and the name of the function 
 * where it occured. Some error messages are addressed at programmers, some at end users.
 */
struct ARTerror
{
	int test;
	std::string inFunction;
	std::string description;
	/**
	 * The constructor of an ARTerror object.
	 * @param in The name of the function the error occures in. The name should 
	 *    also specify the class.
	 * @param des The description of the error. If variable string parts should be
	 *    included in the description, use the placeholders "%s1", "%s2" and "%s3".
	 *    If the constructor is called like this
	 *    @code
	 *       ARTerror e = new ARTerror("myfunction()","The variable %s1 of element %s2 is NULL", "Length", "Cylinder1");
	 *    @endcode
	 *    The call @code e->GetErrorMessage(); @endcode will return "The variable 
	 *    Length of element Cylinder1 is NULL"             	 
	 * @param s1 The string that will replace the placeholder %s1 in the des. (optional)            	 
	 * @param s2 The string that will replace the placeholder %s2 in the des. (optional)           	 
	 * @param s3 The string that will replace the placeholder %s3 in the des. (optional)           	 
	 */   	
	ARTerror(std::string in, std::string des, std::string s1 ="", std::string s2 = "", std::string s3 = "");
	/**
	 * @returns a message with function name
	 */	 
	std::string GetErrorMessage();
	/**
	 * @returns only the error string, no function name
	 */	 
	std::string GetErrorString();
};

/**
 * An object of this class is thrown by ARTprogressIndicator when the user 
 * aborts an operation. It must be catched wherever an abortion of a calculation 
 * by the user is possible.
 * If the evaluation of a data container is aborted, the data container has to be reset
 * to its original state with the function ResetEvaluation. Do so only when you are sure
 * there are no circular dependencies in a data container, for example after evaluation as
 * in the catch blockof an ARTabort error. 
 * @see ARTprogressIndicator 
 */ 
struct ARTabort
{
	float stoppedAt;
	ARTabort(float p)
	: stoppedAt(p)
	{ }
};

/** 
 * @}
 */

#endif
