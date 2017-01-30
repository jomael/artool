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

#ifndef ARTFUNCTIONOID_H
#define ARTFUNCTIONOID_H

#include <vector>

#include "ARTerror.h"
#include "ARTlink.h"


/** \addtogroup ARTimpl ART implementation
 * @{
 */

//forward declarations of classes defined in interface.h, because this file is inserted there before the declarations come.
namespace ART{
  class DataContainer;
  class Element;		
}
//Basisklasse eines Funktionoides / base class of a functionoid

/*
	Deutsch:
	******
Die folgenden Funktionsobjekte (Funktionoide oder Funktoren) sind das Gleiche, was Parserformeln fnr 
DataContainer sind: Berechnungsanleitungen. Sie stellen den DataContainern Funktionen zum 
errechnen ihres Wertes zur Verfngung. Beim Erstellen eines DataContainers wird dem Konstruktor
gleichzeitig ein zeiger auf eines der folgenden Objekte mitgegeben. Der Datencontainer schreibt seine 
Adresse in das Feld out_ des Funktionoides.

Dem Konstruktor eines jeden Funktionoides werden als Parameter zeiger auf die Datenkontainer nbergeben, 
die fnr den Rechenvorgang ben�tigt werden. Dies kann als eingefrorener Funktionsaufruf betrachtet werden,
denn erst wenn der Datenkontainer, zu welchem der Funktionoid geh�rt, ausgewertet werden soll, wird die
Kernfunktion des Funktionoides (ApplyFunction) aufgerufen. Diese Funktion weiss, auf welche Weise der 
Ausgabewert (also der Wert des in out_ angebundenen Datenkontainers) aus den Parametern berechnet 
wird.

Fnr die Berechnung eines bestimmten Wertes reicht es den Datenkontainer, der diesen Wert enthSlt, 
abzufragen. Durch Aufrufe von Funktionoidkernfunktionen (innerhalb von DataContainer-Objekten) 
und Parserevaluierungen (innerhalb von ARTdataCointainer-Objekten) werden erst alle Werte berechnet, die 
fnr den abgefragten Wert eine Rolle spielen, dann wird der abgefragte Wert berechnet.

Die Datenkontainer sind ein Netzwerk von AbhSngigkeiten. Wird ein Datenkontainer ungnltig, werden auch 
alle von ihm abhSngigen ungnltig. Ein Datenkontainer behSlt seinen Wert, bis er neu berechnet werden muss. 

	English:
	******* */
/**
The function objects (functionoids or functors) derived from this base class are the same as formulas in DataContainers:
instructions for calculation. They provide functions for calculation to DataContainers. When
creating an DataContainer, the constructor gets a pointer to one of these functionoids. The
dataContainer writes its address to the functionoid's field out_. 

Pointers to dataContainers needed for calculation are passed to the constructor of the functionoid. This can
be regarded as a deep-frozen function call, because only when the data container to which the functionoid
belongs is evaluated, the core function of the functionoid (ApplyFunction) is called. This function uses the
dataContainers passed as arguments in the constructor to calculate the value of the output (ie. the data-
Container specified by out_)

To calculate a value represented by a dataContainer, only the dataContainer's GetValue function (or
similar functions) have to be called. First all dependencies are calcualted by calls to functionoids' 
ApplyFunction or by evaluation of parser strings. Then the queried value itself is calculated.

The dataContainers are a network of dependencies. If one dataContainer is invalidated, all dataContainers
depending on it are invalidated as well. A dataContainer stores its value until it has to be recalculated.
*/
namespace ART{
class Functionoid
{   
public:
	//Jeder Funktionoid muss eine Funktion zur Berechnung seines Ausgabe-Datenkontainers zur Verfngung stellen
	/**
	 * Every functionoid must provide a function to calculate the value of its output data container. 
	 * The calculation takes place in this very function. Note that you have to call
	 * DataContainer::progressIndicator.Continue() frequently to communicate the status of calculation
	 * to the user and to check if the user wishes to abort. If you are using a loop in your function,
	 * call the function in every iteration. Note that the number of times this function is called here
	 * must be exactly the same number returned by GetIterationNumber for evaluation cost estimation
	 * to work correctly.
	 * Your loop could look somewhat like this:
	 * @code
	 * bool carryon = true;	 
	 * for (f=fmin + fstep*(fmin==0.0), j = 0; (j<flen) && carryon; f+=fstep, j++)
	 * {                    	 
	 *   carryon = DataContainer::progressIndicator.Continue(out_->GetComplexity(),out_->GetVarName());
	 *   ...
	 * }
	 * @endcode
	 *            	 
	 */             
	virtual void ApplyFunction() = 0;

	//Jeder Funktionoid muss sich bei Datenkontainern, von denen sein Ausgabewert abhSngt, anmelden.
	///Every functionoid must register with data containers, on which its output value depends. override only if special dependencies have to be established.
	virtual void SetDependencies(); 

	virtual ~Functionoid() = 0;

	//fnr einen gegebenen 2eiger vom Typ Functionoid (oder subtyp), muss der Funktionoid entscheiden, ob der 2eiger die gleiche Funktion reprSsentiert
	///Given a pointer of type Functionoid (or subtypes), the functionoid needs to decide if the pointer represents the same function.
	virtual bool IsSameFunctionoid(Functionoid* f) = 0; //{return false;}

  ///This function should determine the number of iterations as exactly as possible according to the current state of all dependencies so that the evaluation cost of this data container can be estimated. (The user should take care to keep data containers like frequency grids or other determining the number of iterations valid before estimating the evaluation cost!) This number should be exactly the same as the number of times the function DataContainer::progressIndicator.Continue is called in the functionoid's applyFunction. 
	virtual int GetIterationNumber()= 0; //{return false;}

	//Der Ausgabedatenkontainer, dem der Funktionoid im Konstruktor nbergeben wird, setzt den zeiger out_ auf sich.
	///The dataContainer, to which this functionoid is passed as an argument in the constructor, automatically sets the pointer out_ on itself.
	void SetOutput(ART::DataContainer* out) {out_ = out;}

protected:
	ART::DataContainer* out_; ///set by SetOutput() in constructor of data container	
	vector<ART::DataContainer*> in_; ///pointers to input arguments, this vector is used to register dependencies.
	//a second vector for input arguments that are objects like hosts ??
};


//functionoid as wrapper for multimodeRadiationImpedance
class ARTmmRadImpFunc : public Functionoid
{
public:
 ARTmmRadImpFunc(ART::Element* host, ART::DataContainer* frequencies, ART::DataContainer* modes, bool hasBends)
	: host_(host), hasBends_(hasBends), frequencies_(frequencies), modes_(modes)
	{
		if (!host) throw ARTerror("ARTmmRadImpFunc(Constructor)", "Argument '%s1' is NULL.","host");
		if (!frequencies) throw ARTerror("ARTmmRadImpFunc(Constructor)", "Argument '%s1' is NULL.","host");
		if (!modes) throw ARTerror("ARTmmRadImpFunc(Constructor)", "Argument '%s1' is NULL.","host");
		in_.push_back(frequencies);
		in_.push_back(modes);
	}

	virtual bool IsSameFunctionoid(Functionoid* f);
	virtual void SetDependencies(); 
	virtual void ApplyFunction();
	virtual int GetIterationNumber();

private:
	ART::Element* host_;
	bool hasBends_;
	ART::DataContainer* frequencies_;
	ART::DataContainer* modes_;
};

//functionoid as wrapper for frq-Grid
class ARTfrqGridFunc : public Functionoid
{
public:
 ARTfrqGridFunc(ART::DataContainer* lowfrq, ART::DataContainer* highfrq, ART::DataContainer* frqStep)
		: lowfrq_(lowfrq), highfrq_(highfrq), frqStep_(frqStep)
	{
		if (!lowfrq_) throw ARTerror("ARTfrqGridFunc(constructor)", "The argument 'lowfrq' is NULL.");
		if (!highfrq_) throw ARTerror("ARTfrqGridFunc(constructor)", "The argument 'highfrq' is NULL.");
		if (!frqStep_) throw ARTerror("ARTfrqGridFunc(constructor)", "The argument 'frqStep' is NULL.");
		in_.push_back(lowfrq);
		in_.push_back(highfrq);
		in_.push_back(frqStep);
	}

	virtual bool IsSameFunctionoid(Functionoid* f);
	//virtual void SetDependencies();
	virtual void ApplyFunction();
	virtual int GetIterationNumber();

private:
	ART::DataContainer* lowfrq_;
	ART::DataContainer* highfrq_;
	ART::DataContainer* frqStep_;
};

//functionoid as wrapper for wfrq-Grid
class ARTwfrqGridFunc : public Functionoid
{
public:
 ARTwfrqGridFunc(ART::DataContainer* frqGrid)
	: frequencies_(frqGrid)
	{
		if (!frqGrid) throw ARTerror("ARTwfrqGridFunc(Constructor)", "Argument '%s1' is NULL.","frqGrid");
		in_.push_back(frqGrid);
	}

	virtual bool IsSameFunctionoid(Functionoid* f);
	//virtual void SetDependencies();
	virtual void ApplyFunction();
	virtual int GetIterationNumber();

private:
	ART::DataContainer* frequencies_;
};
}

/**
 * @}
 */


#endif	
