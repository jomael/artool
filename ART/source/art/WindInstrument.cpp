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

//	This implements a linked-list of HornElement  (including other WindInstruments). Has a static member
//	defaultList, which can be used as a template, containing the structure and size boundaries for the instrument.


// TO DO:
// Rewrite parsing routines - they are only used with (deprecated) .sct anyway, but currently leak memory as return ptr to locally-declared memory.
//

#ifndef WINDINSTRUMENT_CPP
#define WINDINSTRUMENT_CPP

#include "WindInstrument.h"

#ifdef _CONSOLE
#endif
using namespace ART;
//require some corrections. isn't working with several holes on the same branch section...
vector<int> NbHole_;
vector<WindInstrument*> HoleVector;
static WindInstrument* Hole  = NULL;


#if defined(__GNUG__)

//g++ doesn't have this function built-in but MSVCPP6 does

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukas Chmela
 * Released under GPLv3.
 * see http://www.jb.man.ac.uk/~slowe/cpp/itoa.html for further information
 */
char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

/*
char* itoa( int value, char* result, int base ) { 
		// Converts integer to char array
	if (base < 2 || base > 16) { *result = 0; return result; } // check that the base if valid
	char* out = result;
	int quotient = value;
	
	do {
		*out = "0123456789abcdef"[ std::abs( quotient % base ) ];
		++out;
		quotient /= base;
	} while ( quotient );
		
	// Only apply negative sign for base 10
	if ( value < 0 && base == 10) *out++ = '-';
	
	std::reverse( result, out );
	*out = 0;
	return result;
}
*/
#endif


//Static definitions


list<HornElement*> WindInstrument::defaultList_ = list<HornElement*>();
bool WindInstrument::hasTarget = false;
bool WindInstrument::verboseMode_ = false; 

bool WindInstrument::hasBends_ = false;
bool WindInstrument::fixedTerm_ = false;
bool WindInstrument::hasBranch_ = false;
ImpedanceCurve WindInstrument::targetImpedanceCurve = *(new ImpedanceCurve());  

//Member functions

WindInstrument::WindInstrument () {
        elementList_ = list<HornElement*>();
	impedanceCurve = ImpedanceCurve();
	iter = elementList_.begin();
#if defined(__GNUG__)   //sets default output to Matlab on GCC, but Maple on MSVCPP
	outputMode_=false;
#else
	outputMode_=true;
#endif
	verboseMode_=false;
	supressedLists_=false;
	//hasBends_=false; 
	fixedTerm_ = false;
	//hasBranch_=false;
	length_=0.0;
	type_=LISTSECTION_TYPE;
	name_="Wind Instrument";
}

WindInstrument::WindInstrument (const char filename[], const bool setDefault) {
	if (setDefault) {
		loadBase (filename); 
		setToDefault();
	} else load (filename);
	crossover(ListCrossover);
	mutator(ListMutator);
	initializer(ListInitializer);
	
	type_=LISTSECTION_TYPE;
	canModify_=setDefault;
	canSplit_=setDefault;
}


WindInstrument::WindInstrument (HornElement *c) {
	if ((*c).type()==LISTSECTION_TYPE) {
		WindInstrument* a = static_cast<WindInstrument*>(c);
		type_=LISTSECTION_TYPE;
		canModify_=(*a).canModify_;
		canSplit_=(*a).canSplit_;
		GAGenome::copy(*a);
		copyLists(&((*a).elementList_));
	} else cout << "ERROR: Trying copy constructor with incompatible HornElements\n";
}

WindInstrument::WindInstrument (const WindInstrument& a) : GAGenome(), HornElement() {
	impedanceCurve = ImpedanceCurve(); //Blank ic
	iter = elementList_.begin();
	verboseMode_ = a.verboseMode_;
	outputMode_ = a.outputMode_;
	supressedLists_ = a.supressedLists_;
	hasBends_= a.hasBends_;
	fixedTerm_ = a.fixedTerm_;
	hasBranch_ = a.hasBranch_;
	setLength (a.length());
	name_ = "Wind Instrument";
	type_ = LISTSECTION_TYPE;
	copy(a);
}

WindInstrument::WindInstrument (const bool supressedLists, const char filename[]) {
	elementList_ = list<HornElement*>();
	impedanceCurve = ImpedanceCurve();
	iter = elementList_.begin();
	verboseMode_=false;
#if defined(__GNUG__)   //sets default output to Matlab on GCC, but Maple on MSVCPP
	outputMode_=false;
#else
	outputMode_=true;
#endif
	supressedLists_=supressedLists;
	//hasBends_=false;
	fixedTerm_ = false;
	//hasBranch_=false;
	name_ = "Loaded Wind Instrument";
	length_ = 0.0;
	type_ = LISTSECTION_TYPE;
	loadElemental(filename);
}

WindInstrument::~WindInstrument () {
	cleanElements();
}

void WindInstrument::rosen (double *params) {
	// Takes parameters from Rosenbrock and sets the relevant elements' params to those given (using paramrefs for indirection)
	iter = elementList_.begin();
	const int size = getSize();
	int fiveI;

	for (int i=0; i<size; i++) {
		fiveI = 5*i;
		if (paramRefs_[0+fiveI]==LISTSECTION_TYPE) iter++; //leaves lists alone
		else if (paramRefs_[0+fiveI]==BOREJUMPSECTION_TYPE) { 
			((BoreJumpSection*)(*iter))->rIn(params[paramRefs_[1+fiveI]]); 
			((BoreJumpSection*)(*iter))->rOut(params[paramRefs_[2+fiveI]]); 
			iter++;		
		} else if (paramRefs_[0+fiveI]==CYLINDERSECTION_TYPE) { 
			(*iter)->length(params[paramRefs_[1+fiveI]]); 
			((CylinderSection*)(*iter))->radius(params[paramRefs_[2+fiveI]]); 
			iter++;		
		} else if (paramRefs_[0+fiveI]==CONESECTION_TYPE) { 
			(*iter)->length(params[paramRefs_[1+fiveI]]); 
			((ConeSection*)(*iter))->rIn(params[paramRefs_[2+fiveI]]); 
			((ConeSection*)(*iter))->rOut(params[paramRefs_[3+fiveI]]); 
			iter++;		
		} else if (paramRefs_[0+fiveI]==BESSELSECTION_TYPE) { 
			(*iter)->length(params[paramRefs_[1+fiveI]]); 
			((BesselSection*)(*iter))->rIn(params[paramRefs_[2+fiveI]]); 
			((BesselSection*)(*iter))->rOut(params[paramRefs_[3+fiveI]]); 
			((BesselSection*)(*iter))->flare(params[paramRefs_[4+fiveI]]); 
			iter++; 
		} else if (paramRefs_[0+fiveI]==CYLINDERBENDSECTION_TYPE) { 
			(*iter)->length(params[paramRefs_[1+fiveI]]); 
			((CylinderBendSection*)(*iter))->radius(params[paramRefs_[2+fiveI]]); 
			((CylinderBendSection*)(*iter))->bendRadius(params[paramRefs_[3+fiveI]]); 
			iter++; 
		} else if (paramRefs_[0+fiveI]==CONEBENDSECTION_TYPE) { 
			(*iter)->length(params[paramRefs_[1+fiveI]]); 
			((ConeBendSection*)(*iter))->rIn(params[paramRefs_[2+fiveI]]); 
			((ConeBendSection*)(*iter))->rOut(params[paramRefs_[3+fiveI]]); 
			((ConeBendSection*)(*iter))->bendRadius(params[paramRefs_[4+fiveI]]); 
			iter++; 
		} else if (paramRefs_[0+fiveI]==TERMINATIONELEMENT_TYPE) { 
			(*iter)->length(params[paramRefs_[1+fiveI]]); 
			((TerminationElement*)(*iter))->radius(params[paramRefs_[2+fiveI]]); 
			iter++;	
		}else if (paramRefs_[0+fiveI]==BRANCH_TYPE) { 
			(*iter)->length(params[paramRefs_[1+fiveI]]); 
			((Branch*)(*iter))->radius(params[paramRefs_[2+fiveI]]); 
			iter++;	
		}
	}
}

void WindInstrument::setToDefault() {
	//Sets this instrument's list to equal the static default.
	
	copyLists(&defaultList_);
	_evaluated=gaFalse;
	canModify_=true;
	canSplit_=true;
}

void WindInstrument::cleanElements() {
	//Deletes the elements pointed to by the list, then clears the list.
	
	if (elementList_.size()!=0) {
		iter=elementList_.begin();
		for (unsigned int i=0; i<elementList_.size(); i++) {
			if ((*iter)->type()==LISTSECTION_TYPE) (static_cast<WindInstrument*> (*iter))->cleanElements();
			delete (*iter); 
			iter++;
		}
	}
	elementList_.clear();
}

void WindInstrument::copyLists(list<HornElement*> *orig) {
	//Sets this list to equal given list.
	
	cleanElements();
	list<HornElement*>::iterator tempIter = (*orig).begin();
	int temptype;
	
	for (unsigned int i=0; i<(*orig).size(); i++) {
		temptype = (*tempIter)->type();
		if (temptype==BOREJUMPSECTION_TYPE) elementList_.push_back( new BoreJumpSection(*tempIter) ); 
		else if (temptype==CYLINDERSECTION_TYPE) elementList_.push_back( new CylinderSection(*tempIter) ); 
		else if (temptype==CONESECTION_TYPE) elementList_.push_back( new ConeSection(*tempIter) );
		else if (temptype==BESSELSECTION_TYPE) elementList_.push_back( new BesselSection(*tempIter) );
		else if (temptype==LISTSECTION_TYPE) elementList_.push_back( (*tempIter) ); //Note list now contains a pointer to THE SAME INSTRUMENT not a new one as with other HornElements
		else if (temptype==CYLINDERBENDSECTION_TYPE) elementList_.push_back( new CylinderBendSection(*tempIter) ); 
		else if (temptype==CONEBENDSECTION_TYPE) elementList_.push_back( new ConeBendSection(*tempIter) ); 
		else if (temptype==TERMINATIONELEMENT_TYPE) elementList_.push_back( new TerminationElement(*tempIter) ); 
		else if (temptype==BRANCH_TYPE) elementList_.push_back( new Branch(*tempIter) ); 
		tempIter++;
	}
	iter=elementList_.begin(); 
}

void WindInstrument::printList(bool indent) {
	//Prints list of elements to screen. 
	
	list<HornElement*>::iterator tempIter = elementList_.begin();

	cout << "----- size=" << elementList_.size() << endl;

	while (tempIter != elementList_.end()) { 
		if (indent) cout << "+     ";
		(*tempIter)->print();
		cout << endl;
		tempIter++;		
	}	
	cout << "------------\n";
}

HornElement* WindInstrument::getElement(int i) { //random access - deprecated
	list<HornElement*>::iterator tempIter = elementList_.begin();
	for (int j=0; j<i; j++) tempIter++; 
	return (*tempIter);
}



void WindInstrument::append (HornElement* a, bool addJumps) {
	//Appends given element onto the end of the list.
	if (a->type()==LISTSECTION_TYPE && a->rOut()==-1.0) {cout << "ERROR: Cannot append empty WindInstrument\n"; return;}		
	if (addJumps) {	//avoids discontinuities by adding in bore-jumps.
		if (this->getSize()!=0) {
			//Checks to see if a BoreJump is needed and adds it
			list<HornElement*>::iterator tempIter = elementList_.end();
			tempIter--;
			if((*tempIter)->type()!=4 && (*tempIter)->type()!=8 && (*tempIter)->type()!=7){
				double tempC = (*tempIter)->gettempC();
				double lossF = (*tempIter)->getlossF();
				double rOldOut = (*tempIter)->rOut();
				double rNewIn = a->rIn();
				if (abs(rOldOut-rNewIn) > eps) elementList_.push_back(new BoreJumpSection(rOldOut, rNewIn, tempC, lossF));
			}
		}
	}
	if (this->getSize()!=0) {
		//Checks to see if previous temperature and lossfactor can be propagated
		list<HornElement*>::iterator tempIter = elementList_.end();
		tempIter--;
		if((*tempIter)->type()==-1){tempIter--;}
		/* [SB, Ticket #4] Adding checks to ensure that 
		 * temperature and loss factor files explicitly given in the files 
		 * are not overwritten with previous values
		 */
		if (!a->gettempC()) a->settempC((*tempIter)->gettempC());
		if (!a->getlossF()) a->setlossF((*tempIter)->getlossF());
	}
	if(elementList_.size()==0){a->FirstElement(1);}
	else if(elementList_.size()!=0){a->FirstElement(0);}
	elementList_.push_back( a );
	if(a->type()>=0 && a->type()<=6){
		length_ += a->length();
	}
	if ((a->type()==CYLINDERBENDSECTION_TYPE || a->type()==CONEBENDSECTION_TYPE) && hasBends_==false) hasBends_=true;

	if (a->type()==BRANCH_TYPE && hasBranch_==false) hasBranch_=true;

	if (elementList_.size()==1) {setIterToBegin(); iter++;} // If arrays were empty, sets pointer to begin (would be NULL else)
}

void WindInstrument::erase () { //Erases element at current 
	list<HornElement*>::iterator tempIter = iter;
	tempIter--;
	elementList_.erase (iter);
	iter = tempIter;
}

void WindInstrument::erase (int i) { //Erases element at point i (random access)
	list<HornElement*>::iterator tempIter = elementList_.begin();
	for (int j=0; j<i; j++) tempIter++;
	elementList_.erase (tempIter);
}

void WindInstrument::chgtempC (const float tempC) { //Change tempC of current list element
	if (this->getSize()!=0) {
		list<HornElement*>::iterator tempIter = elementList_.end();
		tempIter--;
		(*tempIter)->settempC(tempC);
	}
}

void WindInstrument::chglossF (const float lossF) { // Change lossF of current list element
	if (this->getSize()!=0) {
		list<HornElement*>::iterator tempIter = elementList_.end();
		tempIter--;
		(*tempIter)->setlossF(lossF);
	}
}

void WindInstrument::insert (HornElement* a) { 
	//Inserts element at current iterator location
	iter = elementList_.insert(iter, a);
}

void WindInstrument::setIterToBegin() {
	iter = elementList_.begin();
}

void WindInstrument::setIterToEnd() { //Sets iterator to point to the last value (NOT to the actual end of the list)
	iter = elementList_.end();
	iter--; 
}

void WindInstrument::printDescription() const {
	//Prints verbose description to screen.

	cout << endl << "---Instrument Description---\n\n";
	cout << "Name: '" << name_.c_str();
	int size = this->getSize();
	cout << "' containing " << size << " elements" << endl;
	printList(false);
	cout << "---++++++++++++++++++++++---\n";
}

void WindInstrument::printList(const bool indent) const {
	//Prints list of elements to screen. Subsid of printDescription() and print().

	list<HornElement*>::const_iterator tempIter = elementList_.begin();
	
	double x=0;
		
	while (true) { 
		if (indent) cout << "     ";
		(*tempIter)->print();
		cout << ", x=" << x << "..";
		x+=(*tempIter)->length();
		cout << x << endl;
		tempIter++;
		if (tempIter == elementList_.end()) break; 
	}
}

int WindInstrument::write(ostream & os) const {
	//essentially a const version of printList, with a prototype compatible with the GA.
	
	list<HornElement*>::const_iterator tempIter = elementList_.begin();

	os << "----- size=" << elementList_.size() << endl;

	while (tempIter != elementList_.end()) { 
		(*tempIter)->print();
		os << endl;
		tempIter++;		
	}

	os << "------------\n";
	return 1;
}

void WindInstrument::print() const {
	// Returns a text representation to the screen. This is inherited from HornElement and matches its style.
	
	if (!supressedLists_) {
		cout << "LIST: '" << name_.c_str() << "', radius=" << rIn() << ".." << rOut() << endl;
		printList(true);
	} else {
		cout << "LIST: '" << name_.c_str() << "' (supressed), radius=" << rIn() << ".." << rOut();
	}
}

double WindInstrument::findRadius (const double x) const {
	// Returns the radius of the instrument a distance x from the origin.

	list<HornElement*>::const_iterator tempIter = elementList_.begin();
	double y=0.0, l=0.0;  //current location within instrument (seek x=y), length of current element

	while(tempIter != elementList_.end()) {
		l = (*tempIter)->length() + 0.00001;
		if (x<=(y+l)) return (*tempIter)->findRadius(x-y);
		else {
			y+=l;
			tempIter++;
		}
	}
	return 0.0;
}

void WindInstrument::compareBore (const char instrumentFile[], const char outputFile[]) const {
	//Compares bore of this to bore of given elemental file, then saves pointwise difference as Maple file
	
	if(verboseMode_) cout << "Loading target bore for comparison" << flush;
	WindInstrument target = new WindInstrument (false, instrumentFile);
	if(verboseMode_) {cout << " : done\n"; cout << "Saving bore comparison as Maple plot: " << outputFile << flush;}

	const double step = length_/500;
	ofstream file(outputFile);
	file << "plotList:=[";

	double x;

	for (x=0.0; x<(length_); x+=step) {
		file << "[" << x << ", " << findRadius(x) - target.findRadius(x) << "], ";
	}

	file << "[" << x << ", " << findRadius(x) - target.findRadius(x) << "]]:";

	file.close();
	if(verboseMode_) cout << " : done\n" << flush;
}

bool WindInstrument::check() {
	//Checks that the radius of the bore is everywhere non-decreasing (deprecated - useless for accurate descriptions of real instruments!)
	
	setIterToBegin();
	do {
		if ((*iter)->type()==BESSELSECTION_TYPE) {if((*iter)->rOut() <= (*(iter))->rIn()) return false;}
		else {if ((*iter)->rOut() < (*(iter))->rIn() && (*iter)->type()!=LISTSECTION_TYPE) return false;}
	} while (++iter != elementList_.end());

	return true;
}

void WindInstrument::elementInitialise(const double previousRout) {
	// Calls each element's initialiser.

	if(!canModify_) return;
	double tempRadius=previousRout;

	list<HornElement*>::iterator tempIter = elementList_.begin();
	do { 
		(*tempIter)->elementInitialise(tempRadius);
		tempRadius=(*tempIter)->rOut();
		tempIter++;		
	} while (tempIter != elementList_.end()) ;
}

int WindInstrument::mutate(const float pmut, const float mu, const double previousRout) {
	//Calls each elements' mutator.
	
	if (!canModify_) return 0; //Ensures that when this is not the main (i.e. based on default) list then no mutations occur.
	double tempRadius=previousRout;

	list<HornElement*>::iterator tempIter = elementList_.begin();
	int counter=0; //counts how many mutations have taken place

	do { 
		counter += (*tempIter)->mutate(pmut, mu, tempRadius);
		tempRadius=(*tempIter)->rOut();
		tempIter++;		
	} while (tempIter != elementList_.end()) ;
	_evaluated=gaFalse;
	return counter;
}


int WindInstrument::ListCrossover(const GAGenome& m, const GAGenome& d, GAGenome* s, GAGenome* b) {
	//Sexual reproduction crossover. Iterates through parents' elements, then calls dad's element's own encapsulated crossover, which modifies children 
	//appropriately. Operates on the entire list.
	
	const WindInstrument &mum = (const WindInstrument&)m;
	const WindInstrument &dad = (const WindInstrument&)d;
	WindInstrument *sis = (WindInstrument*)s;
	WindInstrument *bro = (WindInstrument*)b;
	const unsigned int dadsize = dad.elementList_.size() ;

	list<HornElement*>::iterator sisIter; 
	list<HornElement*>::iterator broIter;

	if(s) {
		if (((*sis).elementList_).size() != dadsize) (*sis).setToDefault(); 
		sisIter = (*sis).elementList_.begin();}
	if(b) {
		if (((*bro).elementList_).size() != dadsize) (*bro).setToDefault(); 
		broIter = (*bro).elementList_.begin();}
	int counter=0;
	int typeID;

	list<HornElement*>::const_iterator dadIter = dad.elementList_.begin();
	list<HornElement*>::const_iterator mumIter = mum.elementList_.begin();
	const float bias = 0.5 + ( dad.score() - mum.score() )/2; //prob of getting dad's copy of a gene.
	
	do {
		typeID=(*mumIter)->type();
		if (typeID != (*dadIter)->type()) {cout << "ERROR: Mismatched parents\n"; return 0;} 
		
		if (s && b) (*dadIter)->elementCrossover (*(*mumIter), (*sisIter), (*broIter), bias);
		else if (s) (*dadIter)->elementCrossover (*(*mumIter), (*sisIter), 0, bias);
		else if (b) (*dadIter)->elementCrossover (*(*mumIter), 0, (*broIter), bias);

		dadIter++; mumIter++; 
		if(s) sisIter++; 
		if(b) broIter++;
	} while (dadIter != dad.elementList_.end());

	return counter;
}

int WindInstrument::ListMutator(GAGenome & c, float pmut) {
	//GA calls this mutator, which then calls WindInstrument's own encapsulated self-mutator 

	WindInstrument &child = (WindInstrument&)c;
	double *params = (double *)c.userData();
	return child.mutate(pmut, params[9], -1.0);
}

void WindInstrument::ListSplit(bool modifyDefault) {
	// Called by the GA. Runs through this GAInstrument's list and splits all members that can do so.
	// If modifyDefault, it operates instead on the default List.
	
	list<HornElement*>::iterator tempIter;
	int counter=0;

	if(modifyDefault) {
		tempIter = defaultList_.begin(); 
		do { 
			cout << ".\n";
			counter += (*tempIter)->split(defaultList_, tempIter);
			tempIter++;		
		} while (tempIter != defaultList_.end()) ;
		
	} else {
		tempIter = elementList_.begin(); 
		do { 
			counter += (*tempIter)->split(elementList_, tempIter);
			tempIter++;		
		} while (tempIter != elementList_.end()) ;
	}
}


void WindInstrument::ListInitializer(GAGenome &c) {
	//GA calls this initializer, which sets given WindInstrument to its default and then calls its own initializer.
	//Net result: a new, properly-formatted instrument with random physical parameters within pre-defined boundaries (from static defaultList_).
	
	WindInstrument &child = (WindInstrument&)c;
	child.setToDefault();
	child.elementInitialise(-1.0);	
}

void WindInstrument::roughPlot(const int nModes, const double fmin, const double fmax, const int nPoints, const bool accumulate) {
	//Produces an impedance curve of this horn, within the frequency limits given.
		

	const double step = (fmax-fmin)/(double)nPoints; 
	setnModes(nModes); //Sets given nModes as static variable in HornElement, accessible by all elements.

	if(verboseMode_) cout << "Plotting rough impedance curve: nModes=" << nModes << ", f=" << fmin << ".." << fmax << ", nPoints=" << nPoints << flush;

	for (double f=fmin + step*(fmin==0.0); f<fmax; f+=step) {
		impedanceCurve.append(f, inputImpedanceMagnitude(f,1,accumulate) );
		//if(verboseMode_) cout << " frq= " << f;

	}
		
	if(verboseMode_) cout << " : done\n";
}


void WindInstrument::roughPlot(const int nModes, const double fmin, const double fmax, const int nPoints, const bool accumulate, const int term) {
	//Produces an impedance curve of this horn, within the frequency limits given.
		

	const double step = (fmax-fmin)/(double)nPoints; 
	setnModes(nModes); //Sets given nModes as static variable in HornElement, accessible by all elements.

	if(verboseMode_) cout << "Plotting rough impedance curve: nModes=" << nModes << ", f=" << fmin << ".." << fmax << ", nPoints=" << nPoints << flush;

	for (double f=fmin + step*(fmin==0.0); f<fmax; f+=step) {
		impedanceCurve.append(f, inputImpedanceMagnitude(f,term,accumulate) );
		//if(verboseMode_) cout << " frq= " << f;

	}
		
	if(verboseMode_) cout << " : done\n";
}

int WindInstrument::TlaAccumulate (const double w, int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {
	//Returns impedance matrix for whole instrument (before radiation is considered)
	//based on the same principle as MultimodeAccumulate

	(*a).setValues(dcomp(1.0,0.0), dcomp(0.0,0.0), dcomp(0.0,0.0), dcomp(1.0,0.0)); //initialises a to the identity matrix
	SimpleMatrix temp; //cannot be static as this function can recurse
	const int size = getSize();
	if(mem[3]==0 || mem[3]==1) {setIterToBegin();}
	else {setIterToEnd();}
	if((Ind-1)>size) { //List + BranchSection Case
		mem[0] = -2;
		(*iter)->TlaAccumulate(w, IndFreq, a,Ind-1,mem,zt,ImpedanceList,IndHole); 
		return 0;
	}

	if(mem[0]==-2){//Go to the current HornElement in the List
		for(int i=0;i<Ind-1;i++){if(mem[3]==0 || mem[3]==1) iter++; else iter--;}
		while((*iter)->type()==4) {if(mem[3]==0 || mem[3]==1) iter++; else iter--;}
	} 

	if(mem.size()==5 && ImpedanceList->size()==4){
		int compt = 0;
		while((*iter)->type()!=BRANCH_TYPE) {iter--; compt++;}
		(*iter)->BranchImpedanceList(ImpedanceList);
			if((*iter)->RadiationType()==CUSTOM_RADIATION){(*zt) = (*iter)->GetImpedance(w, IndFreq)(0,0);}
			else {(*zt) = (*iter)->GetImpedance(w, -1)(0,0);}//w for the complex model;
		ImpedanceList->clear();
		for(int i=0;i<compt;i++) {iter++;}
	}


	for (int i=Ind; i<size; i++) { //Parse the Elements
		temp.setValues(dcomp(1.0,0.0), dcomp(0.0,0.0), dcomp(0.0,0.0), dcomp(1.0,0.0));
		if(mem[3]==1 && (((*iter)->type()==LISTSECTION_TYPE && i!=0) || (*iter)->type()==BRANCH_TYPE)){
			while((*iter)->type()==LISTSECTION_TYPE || (*iter)->type()==BRANCH_TYPE){iter++;} 
		}
		if(i!=0 && (*iter)->type()==LISTSECTION_TYPE){
			mem[0] = i + HoleVector[HoleVector.size()-IndHole[3]]->getSize(); 
			IndHole[3]--;
			break;
		}

		else if(((*iter)->type()==TERMINATIONELEMENT_TYPE) || (*iter)->type()==BRANCH_TYPE){
			//Radiation Impedance Calculation	
			if(mem[3]==1) break;
			if(GetTerminationImpedance(w,IndFreq,zt,Ind!=i,i-1,mem,(*iter),ImpedanceList)==dcomp(0,0)) break;
			if(mem[3]==0)iter++; else iter--;
		}
		else{ //"Real" HornElement: Transmission Matrix Calculation
			(*iter)->TlaAccumulate(w,IndFreq,&temp,Ind,mem,zt,ImpedanceList,IndHole);
			if (mem[3]==-1) (*a) = temp*(*a);
			else (*a) = (*a)*temp;
			if((*iter)->FirstElement()==1 && i!=(size-1) && mem[3]==-1){break;}
			if(mem[3]==0 || mem[3]==1)iter++; else iter--;
		} 
	}
	return 0;
}



int WindInstrument::MultimodeAccumulate (const double w,int IndFreq,const bool bends, Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {
	//Returns impedance matrix for whole instrument (before radiation is considered)

	Matrix temp = Matrix(2*nModes_,2*nModes_);
	int size = getSize();
	//If we are looking for the input impedance or some radiated parameters (we don't parse the instrument in the same direction!)
	if(mem[3]==0 || mem[3]==1) {setIterToBegin();}
	else {setIterToEnd();}
	if((Ind-1)>size) { //List + BranchSection Case
		mem[0] = -2;
		(*iter)->MultimodeAccumulate(w,IndFreq,bends, a,Ind-1,mem,zt,ImpedanceList,IndHole); // Looking for the transmission matrix of the hole
		return 0;
	}
	if(mem[0]==-2){//Go to the current HornElement in the List
		for(int i=0;i<Ind-1;i++){if(mem[3]==0 || mem[3]==1) iter++; else iter--;}
		while((*iter)->type()==4) {if(mem[3]==0 || mem[3]==1) iter++; else iter--;}
	} 

	if(mem.size()==5 && ImpedanceList->size()==4){ //Branch section case when we are looking for radiated parameters
		int compt = 0;
		while((*iter)->type()!=BRANCH_TYPE) {iter--; compt++;}
		(*iter)->BranchImpedanceList(ImpedanceList);
		if((*iter)->RadiationType()==CUSTOM_RADIATION)(*zt) = (*iter)->GetImpedance(w, IndFreq);
		else (*zt) = (*iter)->GetImpedance(w, -1); 
		ImpedanceList->clear();
		for(int i=0;i<compt;i++) {iter++;}
	}

	for (int i=Ind; i<size; i++) { //Parse the Elements
		temp.Unit();
		if(mem[3]==1 && (((*iter)->type()==LISTSECTION_TYPE && i!=0) || (*iter)->type()==BRANCH_TYPE)){//if - first element = the main list OR - Branch section, we have to go to the next geometrical element
			while((*iter)->type()==LISTSECTION_TYPE || (*iter)->type()==BRANCH_TYPE){iter++;}
		}
		if(i!=0 && (*iter)->type()==LISTSECTION_TYPE){ //new hole
			mem[0] = i + HoleVector[HoleVector.size()-IndHole[3]]->getSize(); 
			IndHole[3]--;
			break;
		}
		else if(((*iter)->type()==TERMINATIONELEMENT_TYPE) || (*iter)->type()==BRANCH_TYPE){
			//Radiation Impedance Calculation	
			if(mem[3]==1) break;
			if(GetTerminationImpedance(w,IndFreq,zt,Ind!=i,i-1,mem,(*iter),ImpedanceList)(0,0)==dcomp(0,0)) break;
			if(mem[3]==0)iter++; else iter--;
		}
		else{ //"Real" HornElement: Transmission Matrix Calculation
			(*iter)->MultimodeAccumulate(w,IndFreq,bends, &temp,Ind,mem,zt,ImpedanceList,IndHole);
			if (mem[3]==-1) (*a) = temp*(*a); 
			else (*a) = (*a)*temp;
			if((*iter)->FirstElement()==1 && i!=(size-1) && mem[3]==-1){break;}
			if(mem[3]==0 || mem[3]==1){iter++;} else iter--;

		} 
	}

	return 0;
}

dcomp WindInstrument::GetTerminationImpedance(const double w,int IndFreq, dcomp *zt, int test,int NextPos, vector<int>&mem,
HornElement * iter,list<Matrix> *ImpedanceList){
//based on "Matrix WindInstrument::GetTerminationImpedance"'s model

	if(test && mem[3]==-1){//We are at the end of a Branch Part
		mem[0] = NextPos; //Keep the position in Memory
		mem[1] = (*iter).type(); //Keep the Element_Type in Memory
		return dcomp(0,0);
	}
	else if(test){return dcomp(1,1);}

	else{ //We are at the beginning of a Branch Part: Radiation Impedance Calculation
			if((*iter).type()==BRANCH_TYPE){ //End of all the Branch Connexions
				(*iter).BranchImpedanceList(ImpedanceList);
				if((*iter).RadiationType()==CUSTOM_RADIATION) { (*zt) = ((*iter).GetParallelImpedance(w, IndFreq))(0,0); }
				else (*zt) = ((*iter).GetParallelImpedance(w, -1))(0,0); 
				ImpedanceList->clear();
			}
			if((*iter).type()==TERMINATIONELEMENT_TYPE){
				//if((*iter).RadiationType()==CUSTOM_RADIATION){(*zt) = dcomp(10)*((*iter).Mzt(IndFreq))(0,0);}
				//else{*zt = (*iter).zt(w);}
				*zt = (*iter).radiationImpedance(w);

			} 
		return *zt;
	}	
}

Matrix WindInstrument::GetTerminationImpedance(const double w,int IndFreq, Matrix *zt, int test,int NextPos, vector<int>&mem,
HornElement * iter,list<Matrix> *ImpedanceList){

	if(test && mem[3]==-1){//We are at the end of a Branch Part
		mem[0] = NextPos; //Keep the position in Memory
		mem[1] = (*iter).type(); //Keep the Element_Type in Memory
		static Matrix a = Matrix(2,2);
		(a)(0,0) = dcomp(0,0);
		return a;
	}
	else if(test){
		static Matrix a = Matrix(2,2);
		(a)(0,0) = dcomp(1.1);
		return a;
	}
	else{ //We are at the beginning of a Branch Part: Radiation Impedance Calculation
			if((*iter).type()==BRANCH_TYPE){ //End of all the Branch Connexions
				(*iter).BranchImpedanceList(ImpedanceList);
				if((*iter).RadiationType()==CUSTOM_RADIATION)(*zt) = (*iter).GetParallelImpedance(w, IndFreq);
				else (*zt) = (*iter).GetParallelImpedance(w, -1);
				ImpedanceList->clear();
			}
			if((*iter).type()==TERMINATIONELEMENT_TYPE){
				//if((*iter).RadiationType()==CUSTOM_RADIATION){(*zt) = dcomp(10)*(*iter).Mzt(IndFreq);}
				//else{ *zt = (*iter).Mzt(IndFreq);}
				(*iter).multimodeRadiationImpedance(w,hasBends_,IndFreq, zt);
			} 
			return *zt;
	}	
}


void WindInstrument::multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) {
	//Returns impedance matrix for whole instrument (before radiation is considered)
	const int imax = getSize()-1;
	setIterToEnd(); 
	for (int i=0; i<imax; i++) {
		(*iter)->multimodeInputImpedance(w, bends, Zout);
		/*
		//Sadjad
		std::cout << *Zout << "\n";
		std::cout << "\n";
		std::cout << " ************************************************** \n";
		std::cout << "\n";
		*/
		iter--;
	}

	(*iter)->multimodeInputImpedance(w, bends, Zout);
	/*
	std::cout << *Zout << "\n";
	std::cout << "\n";
	std::cout << " ************************************************** \n";
	std::cout << "\n";
	*/
}


double WindInstrument::inputImpedanceMagnitude (const double f, const int term, const bool accumulate)  {
	const double mag=1; //0.1 = MOhm, 100 = kOhm (used for thesis experiments)
	//double* tmag=0;
	//double* targ=0;
	list<Matrix> ImpedanceList;
	vector<int> IndHole;
	//===================================================
	for(int p=0;p<4;p++){IndHole.push_back(0);}
	//===================================================
	return mag*(std::abs(getInputImpedance(f,term,0,1,1E-5,0,&ImpedanceList,0,IndHole,accumulate)[0]));
}

//WK got complex output
dcomp WindInstrument::getInputImpedance (const double f, const int t, const bool accum)  {
	list<Matrix> ImpedanceList;
	vector<int> IndHole;
	//===================================================
	for(int p=0;p<4;p++){IndHole.push_back(0);}
	//===================================================
	return getInputImpedance (f, t, 0,1, 1E-5,0,&ImpedanceList,0,IndHole,accum) [0];
}


//WK got complex output
vector <dcomp> WindInstrument::getInputImpedance (const double f, const int t, int IndFreq,int Contrib, const double gain, const int Ind,list<Matrix> *ImpedanceList, const int var, vector<int> &IndHole, const int accumulate)  {
	//Sadjad: 
	//hasBranch_ = true; //force accumulation
	//Evaluates input impedance at given frequency using either transmission-line or multi-modal method.
	//Each element contains a member function for calculating its own impedance
	//Parameter t determines radiation model: 
	//  case Tla: 0=perfect reflection, 1=specified using tmag,targ, 2=Levine model into open air, 3=Zorumski model into open air
	//  case Mm:  0=perfect reflection, 1=specified using tmag,targ, 2=Zorumski model into open air
	//  case Mm: 10=perfect reflection, 11=specified using tmag,targ, 12=Zorumski model into open air (accumulation mode)
	if(t>=10) {cout<<"ERROR! term must be >=0 && < 3"<<endl; return vector <dcomp>(0);}
	vector <dcomp> zero;
	zero.push_back(dcomp(0,0));
	if (f==0.0) return zero; //Avoids division-by-zero at the origin
	const double w = 2.0*M_PI*f;
	Matrix z_in;
	Matrix z_hole;
	Matrix ListInd;

	vector <dcomp> zHole;
	vector <dcomp> Zin;

	if(nModes_==0) {z_in = Matrix(1,1); ListInd = Matrix(1,1); z_hole = Matrix(1,1);}
	//else {z_in = Matrix(nModes_,nModes_); ListInd = Matrix(nModes_,nModes_); z_hole = Matrix(nModes_,nModes_);}  
	else {z_in = Matrix(nModes_,nModes_); ListInd = Matrix(nModes_,nModes_); z_hole = Matrix(1,1);}  // only one mode for current branch implementation

	Matrix z_in1 = Matrix(1,1);

	vector<int> mem; //BranchSection case: keep in memory: position and element_type in the file (for the matrix calculation) 
	mem.push_back(-1);
	mem.push_back(-1);
	mem.push_back(-1);
	mem.push_back(-1);

	if(IndHole[0]==0 && NbHole_.size()!=0){
		IndHole[3] = HoleVector.size();
		IndHole[2] = HoleVector.size();
		IndHole[1] = IndHole[2] - NbHole_[NbHole_.size()-1];
	}
	// ************* METHODE 1 : With TERMINATION ELEMENT ************* //

	if (nModes_==0) { 	// TLA plane-wave calculation
		static SimpleMatrix temp;
		dcomp zt = 0;
		TlaAccumulate(w,IndFreq, &temp,Ind,mem,&zt,ImpedanceList,IndHole); //Accumulation AND Radiation Impedance Calculation
		if (zt == dcomp(-1) && var==0) {mem[2]=1; z_in(0,0) = temp.getArray(0,0) / temp.getArray(1,0); } 
		else {z_in(0,0) = ( temp.getArray(0,0)*zt + temp.getArray(0,1) ) / ( temp.getArray(1,0)*zt + temp.getArray(1,1) );}

		if (mem[0]>0)	{ //BranchSection Connexion case: 
			if(HoleVector.size()!=0 && IndHole[1]>=0){
				IndHole[0]++;
				ListInd(0,0) = IndHole[2]-IndHole[1];
				(*ImpedanceList).push_back(ListInd); //Number of hole for this branch
				for(int k=IndHole[1];k<IndHole[2];k++){
					list<Matrix> ImpedanceListHole;
					zHole = HoleVector[k]->getInputImpedance(f,t,IndFreq,Contrib,gain,0,&ImpedanceListHole,0,IndHole,1);
					z_hole(0,0) = zHole[0]*dcomp(gain,0);
					(*ImpedanceList).push_back(z_hole);//Impedance value
					ListInd(0,0) = HoleVector[k]->RadiationType();
					(*ImpedanceList).push_back(ListInd); //Radiation type for this hole

				}
				IndHole[2] = IndHole[1];					
				IndHole[1] = IndHole[1] - NbHole_[NbHole_.size()-IndHole[0]];
			}
			(*ImpedanceList).push_back(z_in);
			//The input impedance of this part of the windinstrument is stocked in a list
			//Impedance Calculation for the other parts of the windinstrument which are connected to the BranchSection:
			Zin = getInputImpedance (f, t, IndFreq, Contrib,gain, mem[0],ImpedanceList,mem[2],IndHole,1); 
			z_in(0,0) = Zin[0]*dcomp(gain,0);
			
		} 

	} else {	//Multi-modal calculation
		// in this case radiation impedance is determined first and propagated through all matrices
			
			/*
			// Needs Termination Element to work (MZ and Mzt not defined)
			setIterToEnd(); 
			static Matrix z_t = Matrix(nModes_,nModes_);
			//if((*iter)->type()==LISTSECTION_TYPE){cout<<"ERROR! Porpagated way: not with a Bore List!"<<endl; return 0;}	// why???
			if(t==CUSTOM_RADIATION){z_t = MZ[IndFreq];}
			else{z_t = (*iter)->Mzt(IndFreq);} //Radiation Impedance (last element)
			if((*iter)->type()==LISTSECTION_TYPE){(*iter)->multimodeInputImpedance(w, hasBends_, &z_t);}
			else{multimodeInputImpedance(w, hasBends_, &z_t);}; //Impedance propagation
			z_in = z_t;
			~z_t;
			*/

		//WASILAKIS
		
if (!accumulate && !hasBranch_) //Branches require accumulation mode
{

			//Radiation Impedance (last element)
			setIterToEnd(); 
			static Matrix z_t = Matrix(nModes_,nModes_);

			if (fixedTerm_) { // if termination is specified it neglects TERMINATION_ELEMENT
				if(t==REFLECTING_RADIATION){(*iter)->multimodeRadiationImpedanceClosedEnd(w, hasBends_, &z_t);}
				else{(*iter)->multimodeRadiationImpedanceOpenEnd(w, hasBends_, &z_t);}
			}
			else { (*iter)->multimodeRadiationImpedance(w, hasBends_,IndFreq, &z_t);} // use radiation of last element
			
			//Impedance propagation
			if((*iter)->type()==LISTSECTION_TYPE){(*iter)->multimodeInputImpedance(w, hasBends_, &z_t);}
			else{multimodeInputImpedance(w, hasBends_, &z_t);}; //Impedance propagation			// is that really necessary???

			//multimodeInputImpedance(w, hasBends_, &z_t);
			z_in = z_t;
   			//cout << "z_t=" << z_t << "\n";

			~z_t;
}		
		
else
{
		// in this case matrices are accumulated first, radiation impedance is applied later
		
		static Matrix temp = Matrix(2*nModes_,2*nModes_);
		static Matrix z_t = Matrix(nModes_,nModes_);
		temp.Unit();

		MultimodeAccumulate(w,IndFreq,hasBends_, &temp,Ind,mem,&z_t,ImpedanceList,IndHole); //Accumulation AND Radiation Impedance Calculation
		//Split of all the 4th matrices (n*n) constituting of the transmission matrix (2n*2n)
		Matrix A = Matrix(nModes_,nModes_);
		Matrix B = Matrix(nModes_,nModes_);
		Matrix C = Matrix(nModes_,nModes_);
		Matrix D = Matrix(nModes_,nModes_);

		for (int p=0;p<2*nModes_;p++){
			for (int q=0;q<2*nModes_;q++){
				if(p<nModes_ && q<nModes_){A(p,q) = temp(p,q);}
				else if(p<nModes_ && q>=nModes_){B(p,q-nModes_) = temp(p,q);}
				else if(p>=nModes_ && q<nModes_){C(p-nModes_,q) = temp(p,q);}
				else if(p>=nModes_ && q>=nModes_){D(p-nModes_,q-nModes_) = temp(p,q);}
			}
		}


		// if there are no Branches: go to last element and (if this is not a list element) calculate its termination
		// (if it is a list element a termination element is appended already and termination is calculated within MultimodeAccumulate) 
		if (hasBranch_==false) { 
			setIterToEnd(); 
			if((*iter)->type()!=LISTSECTION_TYPE || fixedTerm_==true) {
				if(t==REFLECTING_RADIATION){(*iter)->multimodeRadiationImpedanceClosedEnd(w, hasBends_, &z_t);}
				else{(*iter)->multimodeRadiationImpedanceOpenEnd(w, hasBends_, &z_t);}
			}
		}


		//input impedance calculation
		z_in = ((A*z_t+B)/(C*z_t+D));
		
}
			
		if (mem[0]>0){ //BranchSection Connexion case:
			if(HoleVector.size()!=0 && IndHole[1]>=0){
				IndHole[0]++;
				ListInd(0,0) = IndHole[2]-IndHole[1];
				(*ImpedanceList).push_back(ListInd); //Number of hole for this branch
				for(int k=IndHole[1];k<IndHole[2];k++){
					list<Matrix> ImpedanceListHole;
					zHole = HoleVector[k]->getInputImpedance(f,t,IndFreq,Contrib,gain,0,&ImpedanceListHole,0,IndHole,accumulate);
					if(Contrib==1){z_hole(0,0) = zHole[0]*dcomp(gain,0);}
					else if(Contrib<=nModes_){for(int i=0;i<Contrib;i++){z_hole(i,i) = zHole[i]*dcomp(gain,0);}}
					else {int p=0;
						for(int j=0;j<nModes_;j++){
							for(int i=0;i<nModes_;i++){z_hole(j,i) = zHole[p]*dcomp(gain,0);}p++;
						}
					}
					(*ImpedanceList).push_back(z_hole);//Impedance value
					ListInd(0,0) = HoleVector[k]->RadiationType();
					(*ImpedanceList).push_back(ListInd); //Radiation type for this hole
				}
				IndHole[2] = IndHole[1];
				IndHole[1] = IndHole[1] - NbHole_[NbHole_.size()-IndHole[0]-1];
			}
			if(hasBranch_) {
				z_in1(0,0) = z_in(0,0);
				(*ImpedanceList).push_back(z_in1);  // only one mode for current branch implementation
			}
			else{
				(*ImpedanceList).push_back(z_in);
			}
			//The input impedance of this part of the windinstrument is stocked in a list
			//Impedance Calculation for the other parts of the windinstrument which are connected to the BranchSection
			Zin = getInputImpedance (f, t, IndFreq, Contrib,gain, mem[0],ImpedanceList,mem[2],IndHole,accumulate); 
			if(Contrib==1){z_in(0,0) = Zin[0]*dcomp(gain,0);}
			else if(Contrib<=nModes_){for(int i=0;i<Contrib;i++){z_in(i,i) = Zin[i]*dcomp(gain,0);}}
			else {int p=0;
				for(int j=0;j<nModes_;j++){
					for(int i=0;i<nModes_;i++){z_in(j,i) = Zin[p]*dcomp(gain,0);}p++;
				}
			}
	
		}
		
	}

	Zin.clear();
	//Filling Zin vector with all the contributions that the user want
	if(Contrib==1){Zin.push_back(z_in(0,0)/dcomp(gain,0));}
	else if(Contrib<=nModes_){for(int i=0;i<Contrib;i++){Zin.push_back(z_in(i,i)/dcomp(gain,0));}}
	else {
		for(int i=0;i<nModes_;i++){Zin.push_back(z_in(i,i)/dcomp(gain,0));}
		for(int j=0;j<nModes_;j++){ for(int i=0;i<nModes_;i++){ if(i!=j){Zin.push_back(z_in(j,i)/dcomp(gain,0));}}}
	}
	return Zin; //complex impedance
}

vector <dcomp> WindInstrument::getRadiationImpedance(const double Freq, const double IndFreq, const int Contrib, const int Ind,list<Matrix> *ImpedanceList, const int var, vector<int> &IndHole){
//based on the same model than "getInputImpedance" 
	vector <dcomp> zero;
	zero.push_back(dcomp(0,0));
	if (Freq==0.0) return zero; //Avoids division-by-zero at the origin
	const double w = 2.0*M_PI*Freq;
	vector<int> mem; //BranchSection case: keep in memory: position and element_type in the file (for the matrix calculation) 
	mem.push_back(-1);
	mem.push_back(-1);
	mem.push_back(-1);
	mem.push_back(0);
	if(NbHole_.size()>=2){mem.push_back(NbHole_[1]-NbHole_[0]);}
	vector <dcomp> Zrad;
	vector <dcomp> zHole;
	Matrix z_hole;

	Matrix ListInd;

	if(nModes_==0) {ListInd = Matrix(1,1); z_hole = Matrix(1,1);}
	else {ListInd = Matrix(nModes_,nModes_); z_hole = Matrix(nModes_,nModes_);}

	if(IndHole[0]==0 && NbHole_.size()!=0){
		IndHole[3] = HoleVector.size();
		IndHole[2] = 1;
		IndHole[1] = 0;
	}
	if (nModes_==0) { 
		static SimpleMatrix temp;
		dcomp zin = MZ[IndFreq](0,0)*dcomp(10);
		Matrix zt = Matrix(1,1);
		TlaAccumulate(w, IndFreq, &temp,Ind,mem,&zin,ImpedanceList,IndHole);
		dcomp A = temp.getArray(0,0);
		dcomp B = temp.getArray(0,1);
		dcomp C = temp.getArray(1,0);
		dcomp D = temp.getArray(1,1);
		zt(0,0) = (((B - zin*D)/(zin*C-A))); // Radiation Impedance
		if (mem[0]>0){ //BranchSection Connexion case: 
			if(HoleVector.size()!=0 && IndHole[1]>=0){
				IndHole[0]++;
				ListInd(0,0) = 1;//IndHole[2]-IndHole[1]; //=1 yet...isn't working with more holes...
				(*ImpedanceList).push_back(ListInd); //Number of hole for this branch
				for(int k=IndHole[1];k<IndHole[2];k++){
					list<Matrix> ImpedanceListHole;
					zHole = HoleVector[k]->getInputImpedance(Freq,0,IndFreq,Contrib,1E-5,0,&ImpedanceListHole,0,IndHole,1);
					z_hole(0,0) = zHole[0]*dcomp(10);

					(*ImpedanceList).push_back(z_hole);//Impedance value
					ListInd(0,0) = HoleVector[k]->RadiationType();
					(*ImpedanceList).push_back(ListInd); //Radiation type for this hole
				}
				IndHole[1] = IndHole[2];
				IndHole[2] = IndHole[1] + 1;// NbHole_[NbHole_.size()-IndHole[0]-1];
			}

			(*ImpedanceList).push_back(zt);
			//The input impedance of this part of the windinstrument is stocked in a list
			//Impedance Calculation for the other parts of the windinstrument which are connected to the BranchSection
			Zrad = getRadiationImpedance(Freq,IndFreq,Contrib, mem[0],ImpedanceList,mem[2],IndHole); 
			zt(0,0) = Zrad[1]*dcomp(10); 

		} 

		Zrad.clear();
		Zrad.push_back(NbHole_.size());
		Zrad.push_back(zt(0,0));

		return Zrad;

	}
	else{
		static Matrix Transm = Matrix(2*nModes_,2*nModes_);
		static Matrix zin = Matrix(nModes_,nModes_);
		zin = MZ[IndFreq]*dcomp(10);
		Matrix Zt = Matrix(nModes_,nModes_);
		Matrix Pe = Matrix(nModes_,1);
		Pe(0,0) = 1;
		Matrix A = Matrix(nModes_,nModes_);
		Matrix B = Matrix(nModes_,nModes_);
		Matrix C = Matrix(nModes_,nModes_);
		Matrix D = Matrix(nModes_,nModes_);
		Matrix Inv = Matrix(nModes_,nModes_);
		Matrix Zero = Matrix(nModes_,nModes_);
		Matrix Ones = Matrix(nModes_,nModes_);
		Ones.Unit();
		Transm.Unit();

		MultimodeAccumulate(w,IndFreq,hasBends_, &Transm,Ind,mem,&zin,ImpedanceList,IndHole); 

		for (int p=0;p<2*nModes_;p++){
			for (int q=0;q<2*nModes_;q++){
				if(p<nModes_ && q<nModes_){A(p,q) = Transm(p,q);}
				else if(p<nModes_ && q>=nModes_){B(p,q-nModes_) = Transm(p,q);}
				else if(p>=nModes_ && q<nModes_){C(p-nModes_,q) = Transm(p,q);}
				else if(p>=nModes_ && q>=nModes_){D(p-nModes_,q-nModes_) = Transm(p,q);}
			}
		}
		Zt = (B - zin*D)/(zin*C-A);

		if (mem[0]>0){ //BranchSection Connexion case: 
			if(HoleVector.size()!=0 && IndHole[1]>=0){
				IndHole[0]++;
				ListInd(0,0) = 1;//IndHole[2]-IndHole[1]; //=1
				(*ImpedanceList).push_back(ListInd); //Number of hole for this branch
				for(int k=IndHole[1];k<IndHole[2];k++){
					list<Matrix> ImpedanceListHole;
					zHole = HoleVector[k]->getInputImpedance(Freq,0,IndFreq,Contrib,1E-5,0,&ImpedanceListHole,0,IndHole,1);
					if(Contrib==1){z_hole(0,0) = zHole[0]*dcomp(10);}
					else if(Contrib<=nModes_){for(int i=0;i<Contrib;i++){z_hole(i,i) = zHole[i]*dcomp(10);}}
					else {int p=0;
						for(int j=0;j<nModes_;j++){
							for(int i=0;i<nModes_;i++){z_hole(j,i) = zHole[p]*dcomp(10);}p++;
						}
					}
					(*ImpedanceList).push_back(z_hole);//Impedance value
					ListInd(0,0) = HoleVector[k]->RadiationType();
					(*ImpedanceList).push_back(ListInd); //Radiation type for this hole
				}
				IndHole[1] = IndHole[2];
				IndHole[2] = IndHole[1] + 1;// NbHole_[NbHole_.size()-IndHole[0]-1];
			}
			
			(*ImpedanceList).push_back(Zt);
			//The input impedance of this part of the windinstrument is stocked in a list
			//Impedance Calculation for the other parts of the windinstrument which are connected to the BranchSection
			Zrad = getRadiationImpedance(Freq,IndFreq,Contrib, mem[0],ImpedanceList,mem[2],IndHole); 
			if(Contrib==1){Zt(0,0) = Zrad[1]*dcomp(10);}
			else if(Contrib<=nModes_){for(int i=0;i<Contrib;i++){Zt(i,i) = Zrad[i+1]*dcomp(10);}}
			else {int p=0;
				for(int j=0;j<nModes_;j++){
					for(int i=0;i<nModes_;i++){Zt(j,i) = Zrad[p+1]*dcomp(10);}p++;
				}
			}
		}
		Zrad.clear();
		Zrad.push_back(NbHole_.size());
		if(Contrib==1) {Zrad.push_back(Zt(0,0));}
		else if(Contrib<=nModes_) {for(int i=0;i<Contrib;i++){Zrad.push_back(Zt(i,i));}}
		else {
			for(int i=0;i<nModes_;i++){Zrad.push_back(Zt(i,i));}
			for(int j=0;j<nModes_;j++){ for(int i=0;i<nModes_;i++){ if(i!=j){Zrad.push_back(Zt(j,i));}}}
		}
		return Zrad;
	}
}


vector <dcomp> WindInstrument::getParam(const double Freq, const double IndFreq, const int Contrib, const int Ind,list<Matrix> *ImpedanceList, const int var, vector<int> &IndHole){
//based on the same model than "getInputImpedance" too
	vector <dcomp> zero;
	zero.push_back(dcomp(0,0));
	if (Freq==0.0) return zero; //Avoids division-by-zero at the origin
	const double w = 2.0*M_PI*Freq;
	vector <dcomp> Zrad;

	vector<int> mem; //BranchSection case: keep in memory: position and element_type in the file (for the matrix calculation) 
	mem.push_back(-1);
	mem.push_back(-1);
	mem.push_back(-1);
	mem.push_back(1);
	vector <dcomp> Result;
	vector <dcomp> Zout;

	if (nModes_==0) { 
		static SimpleMatrix temp;
		dcomp z_in = MZ[IndFreq](0,0)*dcomp(10); //MOhm in the file
		TlaAccumulate(w, IndFreq, &temp,Ind,mem,&z_in,ImpedanceList,IndHole);

		dcomp A = temp.getArray(0,0);
		dcomp B = temp.getArray(0,1);
		dcomp C = temp.getArray(1,0);
		dcomp D = temp.getArray(1,1);

		dcomp Pout = (dcomp(10)/(A - B*(dcomp(1)/D)*C))*(dcomp(1) - (dcomp(1)/B)*(dcomp(1)/D)*MZ[IndFreq](0,0));
		Zout = getRadiationImpedance(Freq,IndFreq,Contrib,0,ImpedanceList,0,IndHole);


		Result.push_back(Pout);
		Result.push_back(Zout[1]/pow(10,abs(Zout[0])));
		Result.push_back(Pout/(Zout[1]/pow(10,abs(Zout[0]))));

		return Result;


	}
	else{
		static Matrix Transm = Matrix(2*nModes_,2*nModes_);
		Transm.Unit();
		static Matrix zin = Matrix(nModes_,nModes_);
		zin = MZ[IndFreq]*dcomp(10);
		Matrix Pe = Matrix(nModes_,1);
		Matrix Ps = Matrix(nModes_,1);
		Matrix Us = Matrix(nModes_,1);
		Matrix Zs = Matrix(nModes_,nModes_);
		Pe(0,0) = 1;
		Matrix A = Matrix(nModes_,nModes_);
		Matrix B = Matrix(nModes_,nModes_);
		Matrix C = Matrix(nModes_,nModes_);
		Matrix D = Matrix(nModes_,nModes_);
		MultimodeAccumulate(w,IndFreq,hasBends_, &Transm,Ind,mem,&zin,ImpedanceList,IndHole); 

		//declare for-loop-var as local var to resolve VC compatibility issues...
		int p;

		for ( p=0;p<2*nModes_;p++){
			for (int q=0;q<2*nModes_;q++){
				if(p<nModes_ && q<nModes_){A(p,q) = Transm(p,q);}
				else if(p<nModes_ && q>=nModes_){B(p,q-nModes_) = Transm(p,q);}
				else if(p>=nModes_ && q<nModes_){C(p-nModes_,q) = Transm(p,q);}
				else if(p>=nModes_ && q>=nModes_){D(p-nModes_,q-nModes_) = Transm(p,q);}
			}
		}

		Ps = (!(A - B*(!D)*C))*(Pe - (!B)*(!D)*MZ[IndFreq]*Pe);
		Zout = getRadiationImpedance(Freq,IndFreq,Contrib,0,ImpedanceList,0,IndHole);
		for ( p=0;p<nModes_;p++) Zs(p,p) = Zout[p+1];
		for (int r=0;r<nModes_;r++){
			for (int q=0;q<nModes_;q++){if(r!=q){Zs(r,q) = Zout[nModes_*r + q];}}
		}
		Us = !Zs*Ps;
		if(Contrib==1) {Result.push_back(Ps(0,0)*dcomp(10));
						Result.push_back(Zout[1]/pow(10,abs(Zout[0])));
						Result.push_back(Ps(0,0)*dcomp(10)/(Zout[1]/pow(10,abs(Zout[0]))));}
		else if(Contrib<=nModes_) for(int i=0;i<Contrib;i++){Result.push_back(Ps(i,0));
															 Result.push_back(Zs(i,i)/pow(10,abs(Zout[0])));
															 Result.push_back(Us(i,0)*pow(10,abs(Zout[0])));}
		else {
			for(int j=0;j<nModes_;j++){ Result.push_back(Ps(j,0));
										Result.push_back(Zs(j,j)/pow(10,abs(Zout[0])));
										Result.push_back(Us(j,0)*pow(10,abs(Zout[0])));
			}
			for(int i=0;i<nModes_;i++){
				for(int j=0;j<nModes_;j++){if(i!=j) Result.push_back(Zs(i,j)/pow(10,abs(Zout[0])));}
			}
		}
		return Result;
	}

}




void WindInstrument::refinePeaks(const double epsilon) {
	refinePeaks(epsilon, 2, 0);
}



void WindInstrument::refinePeaks(const double epsilon, const int t, const bool accumulate) {
	//Takes impedance curve and adds points near the peaks to improve accuracy (ignores troughs)

	//Uses internal iterators.
	
	bool gradient, oldGradient; //true means +ve gradient, false means -ve gradient)
	double rCurrent, xCurrent, rPrev, xPrev, rPrevPrev, xPrevPrev;
	list<double>::iterator rRight;
	list<double>::iterator fRight;
	list<double>::iterator xRight;
	
	if(verboseMode_) cout << "Finding and refining peaks: nModes=" << nModes_ << ", epsilon=" << epsilon << flush;

	impedanceCurve.setItersToBegin();
	rPrev = *(impedanceCurve.rIter_);
	xPrev = *(impedanceCurve.xIter_);
	impedanceCurve.rIter_++; impedanceCurve.fIter_++; impedanceCurve.xIter_++; // WK
	rCurrent = *(impedanceCurve.rIter_);
	xCurrent = *(impedanceCurve.xIter_);
	gradient = rPrev < rCurrent; 

	do {
		rPrevPrev = rPrev;
		xPrevPrev = xPrev;
		rPrev = rCurrent;
		xPrev = xCurrent;
		impedanceCurve.rIter_++; impedanceCurve.fIter_++; impedanceCurve.xIter_++;	// WK
		rCurrent = *(impedanceCurve.rIter_);
		xCurrent = *(impedanceCurve.xIter_);
		oldGradient = gradient;
		gradient = rPrev < rCurrent;
			
		if (oldGradient != gradient && !gradient) {
			xRight = impedanceCurve.xIter_;
			rRight = impedanceCurve.rIter_;	// WK
			fRight = impedanceCurve.fIter_; //Stores point to right of peak, from which searching will continue after this peak is refined
			
			recursiveCombinedFindPeak(xPrevPrev, rPrevPrev, xPrev, rPrev, xCurrent, rCurrent, epsilon, 1, t, accumulate); 
			xPrev=*(impedanceCurve.xIter_);
			rPrev=*(impedanceCurve.rIter_);
			impedanceCurve.xIter_ = xRight;
			impedanceCurve.rIter_ = rRight;
			impedanceCurve.fIter_ = fRight;	// WK
			rCurrent = *(impedanceCurve.rIter_);
			xCurrent = *(impedanceCurve.xIter_);
			gradient = false;
			impedanceCurve.peakListAppend(xPrev, rPrev);
		}
	} while (!impedanceCurve.itersNearEnd()); 
	if(verboseMode_) cout << ": nPoints=" << impedanceCurve.getSize() << " : done\n";
}

void WindInstrument::recursiveQuadraticFindPeak (const double xLeft, const double rLeft, const double xCentre, const double rCentre, const double xRight, const double rRight, const double epsilon, const int counter, const int t, const bool accum) {
	//Used by RefinePeaks to calculate extra points using a recursive algorithm - interpolating with polynomials and using abscissa to find next point.
	if (counter>100) {cout << "Recursion limit reached (quad)\n"; return ;}
		// prevents infinite recursion

		//**Note that iterators currently point to Right point.
	double xNew, rNew; //, fNew; //SB: unused variable
	dcomp cNew;
	bool isLeft;
	bool isHigher;

	if (fabs(xLeft - xCentre) > epsilon || fabs(xRight - xCentre) > epsilon) {
		double d = xCentre-xLeft;
		double e = xCentre-xRight;
		double f = rCentre-rRight;
		double g = rCentre-rLeft;

		xNew = xCentre - 0.5* (( d*d*f-e*e*g )/( d*f-e*g ));
		cNew = getInputImpedance(xNew, t, accum);
		rNew = abs(cNew);

		if (xNew<xCentre) {impedanceCurve.xIter_--; impedanceCurve.rIter_--; impedanceCurve.fIter_--; isLeft=true;} //pointing to centre 
			else isLeft=false;
		impedanceCurve.insert(xNew, cNew); //Now pointing to xNew (regardless of isLeft)
		//If isLeft, then new point is placed before xCentre, else after (and before xRight)
		
		//cout << "single add: " << xNew << ", " << rNew << endl;

		if (rNew>rCentre) isHigher=true; else isHigher=false;

		if (isLeft && isHigher) {impedanceCurve.xIter_++; impedanceCurve.rIter_++; impedanceCurve.fIter_++; //point to centre
		recursiveQuadraticFindPeak (xLeft, rLeft, xNew, rNew, xCentre, rCentre, epsilon, counter+1, t, accum);}
		else if(!isLeft && isHigher) {impedanceCurve.xIter_++; impedanceCurve.rIter_++; impedanceCurve.fIter_++; //point to right
		recursiveQuadraticFindPeak (xCentre, rCentre, xNew, rNew, xRight, rRight, epsilon, counter+1, t, accum);}
		else if(isLeft && !isHigher) {impedanceCurve.xIter_++; impedanceCurve.rIter_++; impedanceCurve.fIter_++; impedanceCurve.xIter_++; impedanceCurve.rIter_++; impedanceCurve.fIter_++;  //point to right
		recursiveQuadraticFindPeak (xNew, rNew, xCentre, rCentre, xRight, rRight, epsilon, counter+1, t, accum);}
		else {//point to new
		recursiveQuadraticFindPeak (xLeft, rLeft, xCentre, rCentre, xNew, rNew, epsilon, counter+1, t, accum);}

	} else {impedanceCurve.xIter_--; impedanceCurve.rIter_--; impedanceCurve.fIter_--;} //Now pointing to centre (i.e. the final peak iself ready for breaking recursion)
}

void WindInstrument::recursiveCombinedFindPeak (const double xLeft, const double rLeft, const double xCentre, const double rCentre, const double xRight, const double rRight, const double epsilon, const int counter, const int t, const bool accum) {
	//Uses (original) method of adding a point either side of current peak and recursing around the highest.
	//if applicable, calls instead the quadratic method.
	if (counter>20) {cout << "Recursion limit reached (bis)\n"; return ;}
		// prevents infinite recursion

	//**Note that iterators currently point to Right point.
	/* SB: commenting presently unused code to suppress warning
	double a, b, c;
	a=fabs(rCentre-rLeft);
	b=fabs(rCentre-rRight);
	c=fabs(rRight-rLeft);
	*/
	//cout << (c>a) << ", " << (c>b) << endl;
	if (true) { //(c>a || c>b)
		recursiveQuadraticFindPeak (xLeft, rLeft, xCentre, rCentre, xRight, rRight, epsilon, counter+1, t, accum); return;}
	
	// ### Currently set up only to use the quadratic method
	// ### There is some small numerical error somewhere occasionally giving very slightly ragged peaks after the 9th dp.
	// ### This can cause the peak-finder to register double peaks. It doesn't seem to happen with the bisection method
	// ### Have worked around with the retained right location, but requires further investigation to eliminate root cause.
	
	else {

		if (fabs(xCentre - xLeft) > epsilon || fabs(xCentre - xRight) > epsilon) {
			double xNewLeft, xNewRight, rNewLeft, rNewRight;
			//cout << "C " << xCentre << endl;
			xNewLeft = (xCentre + xLeft)/2;
			xNewRight = (xCentre + xRight)/2;
	
			dcomp cNewLeft = getInputImpedance(xNewLeft, t, accum);
			dcomp cNewRight = getInputImpedance(xNewRight, t, accum);
			rNewLeft = abs(cNewLeft);
			rNewRight = abs(cNewRight);
		
			impedanceCurve.xIter_--; impedanceCurve.rIter_--; impedanceCurve.fIter_--; //Now pointing to centre
			impedanceCurve.insert(xNewLeft, cNewLeft); //Now pointing to NewLeft
			impedanceCurve.xIter_++; impedanceCurve.rIter_++; impedanceCurve.fIter_++;
			impedanceCurve.xIter_++; impedanceCurve.rIter_++; impedanceCurve.fIter_++; //Now pointing to right
			impedanceCurve.insert(xNewRight, cNewRight); // Now pointing to NewRight

			if (rNewLeft>rCentre) {
				impedanceCurve.xIter_--; impedanceCurve.rIter_--; impedanceCurve.fIter_--; //Now pointing to centre
				recursiveCombinedFindPeak(xLeft, rLeft, xNewLeft, rNewLeft, xCentre, rCentre, epsilon, counter+1, t, accum);
			}
			else if (rNewRight>rCentre) {
				impedanceCurve.xIter_++; impedanceCurve.rIter_++; impedanceCurve.fIter_++; //Now pointing to right
				recursiveCombinedFindPeak(xCentre, rCentre, xNewRight, rNewRight, xRight, rRight, epsilon, counter+1, t, accum);
			}
			else {recursiveCombinedFindPeak(xNewLeft, rNewLeft, xCentre, rCentre, xNewRight, rNewRight, epsilon, counter+1, t, accum);}
		} else {impedanceCurve.xIter_--; impedanceCurve.rIter_--; impedanceCurve.fIter_--;} //Now pointing to centre (i.e. the final peak iself ready for breaking recursion)
	}
	
}


// ##### Parsing functions. Deprecated! Need rewriting, but no longer used anyway.

void WindInstrument::toLowerCase (char a[]) const {
	//Takes a string and returns a it in lower-case.

	int i=0;
	//char* output=new char[32];

	while (a[i]!='\0' && a[i]!='\n') {
		if (a[i]>64 && a[i]<91) a[i]=a[i]+32; 
		i++;
	}
}

bool WindInstrument::stringEquality (char a[], char b[]) const {
	//Compares two size-32 strings. If they are the same until the first \0, true is returned.

	toLowerCase(a);
	toLowerCase(b);

	for (int i=0; i<32; i++) {
		if (a[i]=='\0' && b[i]=='\0') return true;
		else if (a[i]!=b[i]) return false;
	}
	return false;
}

char* WindInstrument::parseString (const char input[], int *ptr) const {
	//Takes a string of parameters (from a script) and returns the first parameter in string form

	char* temp = new char[68];
	int old_ptr = *ptr;

	do {
		if (input[*ptr]==',' || input[*ptr]=='\n' || input[*ptr]=='\0') {
			temp[*ptr-old_ptr]='\0';
			if (input[*ptr]==',') *ptr=*ptr+1;
			return temp;
		}
		else {
			temp[*ptr-old_ptr]=input[*ptr];
			*ptr=*ptr+1;
		}
	} while (true);
	return (char *) "Error";
}

int WindInstrument::parseInt (const char input[], int *ptr) const {
	//Takes a parameter string (from a script) and returns the int parameter.

	return atoi(parseString (input, ptr));
}

double WindInstrument::parseDouble (const char input[], int *ptr) const {
	//Takes a parameter string (from a script) and returns the double parameter ###as a float###

	return (double)atof(parseString (input, ptr));
}

dcomp WindInstrument::parseDcomp (const char input[], int *ptr) const {
	//Takes a parameter string (from a script) and returns the dcomp parameter ###as a float###

	return dcomp(parseDouble (input, ptr), parseDouble (input, ptr));
}

bool WindInstrument::parseBool (const char input[], int *ptr) const {
	//Takes a parameter string (from a script) and returns the bool parameter

	int temp = parseInt(input, ptr);
	if (temp==0) return false; else return true;
}

void WindInstrument::runScript (const char filename[]) {
	//Loads a script from file and runs it; this script is a series of instructions that run
	//various commands within this class and ImpedanceCurve, designed to give a user full
	//access to needed commands.
	//Files are to be laid out with one command per line, any parameters listed after 1 space
	//(whitespace otherwise not permitted as yet), seperated by commas. Not case-sensitive. 
	//Lines beginning with ; will be ignored as comments. Dcomps are to be listed as x,y.

	//Deprecated, included for backward-compatibility
		
	cout << "Running script: " << filename << endl;

	ifstream file(filename);
	char buffer[100], command[48];
	int line=0, ptr; //Counts the number of lines read. ptr keeps track of where in each line the program is reading

	if (!file.is_open()) {
		cout << "ERROR: File failed to open\n";
		return;
	}

	while (!file.eof()) {
		line++;
		file.getline(buffer, 100);
		if (buffer[0]!=';') { //ignoring comments in the file
			command[0]='\0';
			ptr=0;
			for (int i=0; i<100; i++) { //finds command
				if (buffer[i]!=' ' && buffer[i]!='\n' && buffer[i]!='\0') {
					command[i]=buffer[i];
				} else {
					command[i]='\0';
					ptr=i+1;
					break;
				}
			}
                        char* p1=parseString(buffer, &ptr);
                        char* p2=parseString(buffer, &ptr);
                        char* p3=parseString(buffer, &ptr);
                        char* p4=parseString(buffer, &ptr);
                        char* p5=parseString(buffer, &ptr);
                        char* p6=parseString(buffer, &ptr);
			//Chooses which command to call, then reads the relevant parameters and calls it
			// Note that switch statements don't work on char arrays
			//if (stringEquality(command, "setName\0") || stringEquality(command, "sN\0")) setName(buffer, &ptr)); 
			if     (stringEquality(command, (char *) "print\0") || stringEquality(command, (char *) "pt\0")) printDescription();
			else if(stringEquality(command, (char *) "load\0") || stringEquality(command, (char *) "ld\0")) load(p1);
			else if(stringEquality(command, (char *) "loadElemental\0") || stringEquality(command, (char *) "ldE\0")) load(p1);
			else if(stringEquality(command, (char *) "loadPointwise\0") || stringEquality(command, (char *) "ldPw\0")) loadPointwise(p1);
			else if(stringEquality(command, (char *) "loadBase\0") || stringEquality(command, (char *) "ldB\0")) loadBase(p1);
			else if(stringEquality(command, (char *) "save\0") || stringEquality(command, (char *) "sv\0")) save(p1);
			else if(stringEquality(command, (char *) "saveElemental\0") || stringEquality(command, (char *) "svE\0")) save(p1);
			else if(stringEquality(command, (char *) "savePointwise\0") || stringEquality(command, (char *) "svPw\0")) savePointwise(p1);

			else if(stringEquality(command, (char *) "addJump\0") || stringEquality(command, (char *) "jump\0")) append(new BoreJumpSection((double)atof(p1), (double)atof(p2)), true);
			else if(stringEquality(command, (char *) "addCylr\0") || stringEquality(command, (char *) "cylr\0")) append(new CylinderSection(false, false,(double)atof(p1), (double)atof(p2)), true);
			else if(stringEquality(command, (char *) "addCone\0") || stringEquality(command, (char *) "cone\0")) append(new ConeSection    (false, false,(double)atof(p1), (double)atof(p2), (double)atof(p3)), true);
			else if(stringEquality(command, (char *) "addBess\0") || stringEquality(command, (char *) "bess\0")) append(new BesselSection  (false, false, (double)atof(p1), (double)atof(p2), (double)atof(p3), (double)atof(p4)), true);
			else if(stringEquality(command, (char *) "addList\0") || stringEquality(command, (char *) "list\0")) append(new WindInstrument (false, parseString(buffer, &ptr)), true);
//WK new sections
			else if(stringEquality(command, (char *) "addBCylr\0") || stringEquality(command, (char *) "bcyl\0")) append(new CylinderBendSection(false, false, (double)atof(p1), (double)atof(p2), (double)atof(p3)), true);
			else if(stringEquality(command, (char *) "addBCone\0") || stringEquality(command, (char *) "bcon\0")) append(new ConeBendSection(false, false, (double)atof(p1), (double)atof(p2), (double)atof(p3), (double)atof(p4)), true);

//WK section parameters tempC and lossF can be set after sections have been added. New values propagate when you add more sections!
			else if(stringEquality(command, (char *) "chgtempC\0") || stringEquality(command, (char *) "temp\0")) chgtempC((double)atof(p1));
			else if(stringEquality(command, (char *) "chglossF\0") || stringEquality(command, (char *) "loss\0")) chglossF((double)atof(p1));

			else if(stringEquality(command, (char *) "setoutputMode\0") || stringEquality(command, (char *) "sOM\0")) setOutputMode(!(0==atoi(p1)));	
			else if(stringEquality(command, (char *) "runScript\0") || stringEquality(command, (char *) "rS\0")) runScript(p1);
			//else if(stringEquality(command, (char *) "check\0") || stringEquality(command, (char *) "ck\0")) check(!(0==atoi(p1)));	
			else if(stringEquality(command, (char *) "inputImpedance\0") || stringEquality(command, (char *) "iI\0")) cout << "Impedance = " << inputImpedanceMagnitude((double)atof(p1),1,0);
			else if(stringEquality(command, (char *) "roughPlot\0") || stringEquality(command, (char *) "rgh\0")) roughPlot(atoi(p1), (double)atof(p2), (double)atof(p3), atoi(p4),1);
			else if(stringEquality(command, (char *) "saveAsMaple\0") || stringEquality(command, (char *) "svAM\0")) saveInstrumentPlot(p1);
			else if(stringEquality(command, (char *) "refinePeaks\0") || stringEquality(command, (char *) "ref\0")) refinePeaks((double)atof(p1));
			else if(stringEquality(command, (char *) "saveImpedanceCurve\0") || stringEquality(command, (char *) "svIC\0")) saveImpedancePlot(p1);
			else if(stringEquality(command, (char *) "saveImpedanceCurveSimple\0") || stringEquality(command, (char *) "svICs\0")) impedanceCurve.save(p1);
			else if(stringEquality(command, (char *) "saveLists\0") || stringEquality(command, (char *) "svL\0")) impedanceCurve.saveLists(p1, false);
			else if(stringEquality(command, (char *) "printImpedanceCurve\0") || stringEquality(command, (char *) "ptIC\0")) printImpedanceCurve(!(0==atoi(p1)));
			else if(stringEquality(command, (char *) "findPeaks\0") || stringEquality(command, (char *) "fp\0")) impedanceCurve.findPeaks(false);
			else if(stringEquality(command, (char *) "printPeaks\0") || stringEquality(command, (char *) "ptP\0")) impedanceCurve.printPeaks();
			else if(stringEquality(command, (char *) "importBIAS\0") || stringEquality(command, (char *) "iB\0")) impedanceCurve.importBIAS(p1);
			else if(stringEquality(command, (char *) "convertToStandard\0") || stringEquality(command, (char *) "cts\0")) impedanceCurve.convertToStandard();
			else if(stringEquality(command, (char *) "compareBore\0") || stringEquality(command, (char *) "cB\0")) compareBore(p1, p2);
			else if(stringEquality(command, (char *) "cleanElements\0") || stringEquality(command, (char *) "clr\0")) cleanElements();
			else if(stringEquality(command, (char *) "quit\0") || stringEquality(command, (char *) "qt\0")) {cout << "Quitting script\n"; return;}
			else {cout << "ERROR: command '" << command << "' not recognised on line " << line << ", aborting\n"; return;}
                        delete p1;
                        delete p2;
                        delete p3;
                        delete p4;
                        delete p5;
                        delete p6;
		}
	}
	cout << "*End of script: " << filename << endl;
}

void WindInstrument::saveImpedancePlot (const char filename[]) {
	//Saves impedance curve
	
	impedanceCurve.saveImpedancePlot(filename);
}

void WindInstrument::loadPointwise (const char filename[]) {
	//Loads instrument from given filename, converts it to elemental form and APPENDS it to this WindInstrument.
	//Uses BrassGA-style pointwise instrument files (.int extension), which are arranged thus:
	/*  name
	 *  Xcoord, Rcoord
	 *  etc.
	 */
	//Any line starting with a semi-colon ';' will be ignored as a comment.
	//#######Note that only float precision is used here######

	if(verboseMode_) cout << "Loading pointwise file '" << filename << "'" << flush;

	ifstream file(filename);
	char buffer[100];
	double xOld, xNew, rOld, rNew;
	bool firstPoint=true;
	xNew=0; rNew=0;
	char nameArray[32];

	if (!file.is_open()) {
		cout << "ERROR: File failed to open\n";
		return;
	}

	file.getline(nameArray, 32);
	name_ = nameArray;

	char xBuffer[100], rBuffer[100];
	int pointer=0;
	
	while (!file.eof()) { //Reading horn from file
		xBuffer[0]='\0';
		rBuffer[0]='\0';
		file.getline(buffer, 100);
		if (buffer[0]!=';' && buffer[0]!='\0') { //ignoring comments
			int i=0;
			for (i=0; i<100; i++) {
				if (buffer[i]!=',') {xBuffer[i]=buffer[i];}
				else {xBuffer[i]='\0'; break;}
			}
		
			pointer=i;
	
			for (i=pointer+2; i<100; i++) {
				if (buffer[i]!='\0') rBuffer[i-pointer-2]=buffer[i];
				else {rBuffer[i-pointer-2]='\0'; break;}
			}

			xOld=xNew; rOld=rNew;
			xNew=(double)atof(xBuffer);
			rNew=(double)atof(rBuffer);
			
//???? needs section parameters tempC and lossF
			if(!firstPoint) {
				if(xNew==xOld) append(new BoreJumpSection(rOld, rNew), true);
				else {	
					if(rNew==rOld) append(new CylinderSection(false, false, xNew-xOld, rNew), true);
					else append (new ConeSection(false, false, xNew-xOld, rOld, rNew), true);
				}
			} else firstPoint=false;
		}
	}
	file.close();
	cout << " : done" << endl;
}

void WindInstrument::saveInstrumentPlot (const char filename[]) {
	if(outputMode_) saveInstrumentPlotMaple(filename); else saveInstrumentPlotMatlab(filename);
}

void WindInstrument::saveInstrumentPlotMaple(const char filename[]) {
	//Saves the instrument as a Maple plot.
	
	char filenameWithExtension[17];
	strcpy(filenameWithExtension, filename);
	strcat(filenameWithExtension, ".mws");
	if(verboseMode_) cout << "Saving instrument as Maple plot '" << filenameWithExtension << "'"  << flush;
	ofstream file(filenameWithExtension);
	
	file << "hornList:=[";
	double x=0.0;
	savePointwise (&file, &x, true);
	file << "]:";
	file << "plot1:=plot(hornList, style=line, color=red):";
	file << "plot2:=plot(hornList, style=point, color=black):";
	file << "plots[display]({plot1, plot2});";
	file.close();
	if(verboseMode_) cout << " : done" << endl;
}

void WindInstrument::saveInstrumentPlotMatlab(const char filename[]) {
	//Saves the instrument as a Matlab plot.
	//Somewhat kludged - first saves in Maple-style [[x1,y1],[x2,y2]...] 2d list format
	//then has Matlab convert it to X=[x1, x2...]; Y=[y1, y2...] vectors 
	
	char filenameWithExtension[17];
	strcpy(filenameWithExtension, filename);
	strcat(filenameWithExtension, ".m");
	if(verboseMode_) cout << "Saving instrument as Matlab plot '" << filenameWithExtension << "'"  << flush;
	ofstream file(filenameWithExtension);

	file << "function [hornX, hornY] = " << filename << "(makePlot);\n";
	file << "hornList=[";
	double x=0.0;
	const int counterMax = savePointwise (&file, &x, true);
	file << "]; ";
	file << "for i=1:" << counterMax << ";";
	file << "hornX(i)=hornList((2*i)-1);";
	file << "hornY(i)=hornList(2*i);";
	file << "end; ";
	file << "if(makePlot) plot(hornX, hornY); axis([0 " << length_ << " 0 " << rOut() << "]); end;";

	file.close();
	if(verboseMode_) cout << " : done" << endl;
}

void WindInstrument::savePointwise (const char filename[]) {
	// Saves the instrument bore as a simple list of coordinates (.int format)

	cout << "Saving pointwise file '" << filename << "'" << flush;
	ofstream file(filename);
	file << name_ << endl;
	double x=0.0;
	savePointwise(&file, &x, false);
	file.close();
	cout << " : done" << endl;
}

int WindInstrument::savePointwise (ofstream *file, double *x, const bool asMaple) {
	//Saves instrument to given file as a pointwise representation (a brassGA .int file)
	
	iter++;
	double rOld, rNewIn, rNewOut, length, step;
	int counter=0; //counts the number of points to plot
	rNewOut=0.0;
	
	while (true) { 
		
		rOld=rNewOut;
		rNewIn=(*iter)->rIn();
		rNewOut=(*iter)->rOut();
		length=(*iter)->length();
	
		if (abs(rNewIn-rOld) > eps) { // Deals with jump discontinuities
			if (asMaple) *file << "[" << *x << ", " << rNewIn << "], ";
			else *file << *x << ", " << rNewIn << endl; 
			counter++;
		}
		
		if ((*iter)->type()<3 || (*iter)->type()==CYLINDERBENDSECTION_TYPE) { //General case
			*x+=length;
			if (asMaple) *file << "[" << *x << ", " << rNewOut << "]";
			else *file << *x << ", " << rNewOut << endl;
			counter++;
		} 
		
		else if((*iter)->type()==BESSELSECTION_TYPE){ //Bessel sections
			step=length/3.0/(int)length;
			for (double xBess=0+(1-(rOld==0))*step; xBess<=length+1e-4; xBess+=step) {
				if (asMaple) {
					*file << "[" << xBess+*x << ", " << (*iter)->findRadius(xBess) << "]";
					if (fabs(xBess-length)>1e-4) *file << ",    ";
				}
				else *file << xBess+*x << ", " << (*iter)->findRadius(xBess) << endl;
				counter++;
			}
			*x+=length;
		}
		
		else if((*iter)->type()==LISTSECTION_TYPE) { //list sections
			// This is definitely a WindInstrument object so cast is appropriate
			WindInstrument *tempListSection;
			tempListSection = static_cast<WindInstrument*> (*iter);
			counter += tempListSection->savePointwise(file, x, asMaple);
		}
		iter++;
		if (iter == elementList_.end()) break; else if(asMaple) *file << ", ";
	}
	return counter;
}


void WindInstrument::save (const char filename[]) {
	saveElemental(filename);
}

void WindInstrument::saveElemental (const char filename[]) {
	//Saves instrument to given file as an elemental representation (.ins file)
	
	cout << "Saving elemental file '" << filename << "'" << flush;
		
	int listCounter=0; //Counts number of list elements in file. Is concatenated with filename of containing instrument to form the filenames of contained instruments.

	ofstream file(filename);
	file << name_ << endl;

	char kludge[32];
	
	iter++;
	for (int i=0; i<getSize(); i++) {
		if ((*iter)->type()!=LISTSECTION_TYPE) file << (*iter)->getSaveString(); //general case
		else { //list element
			file << LISTSECTION_TYPE << "," << listCounter << filename; 
			itoa(listCounter, kludge, 10);
			strcat(kludge, filename);
			cout << endl << "     ";
		
			WindInstrument *current = (WindInstrument *)(*iter);
			(*current).saveElemental(kludge);
			listCounter++;
		}
		file << endl;
		iter++;
	}

	file.close();
	cout << " : done" << endl;
}

bool WindInstrument::load (const char filename[]) {
	return loadElemental (filename);
}

bool WindInstrument::loadElemental (const char filename[]) {
	//Loads instrument from given filename, and APPENDS onto this WindInstrument
	// (03/10/05) Improved version, uses strings internally and is more robust. 
	// 	Can largely deal with whitespace, except around leading names.
	// 	TO DO - when all other file-handling is moved over to C++-style strings, change prototype
	//		insert checks to ensure sensible values.
	//
	//Uses new elemental file format:
	//	name
	//	type, <params>
	//Where type is {0,1,2,3,4} denoting {jump,cylr,cone,bess,list} and params are (respectively)
	//	jump:  rIn, rOut [, tempC, lossF] [, Name]				2..5 par
	//	cylr:  length, radius [, tempC, lossF] [, Name]				2..5 par
	//	cone:  length, rIn, rOut [, tempC, lossF] [, Name]			3..6 par
	//	bess:  length, rIn, rOut, flare [, tempC, lossF] [, Name]		4..7 par
	//	list:  filename
	//	cylbend:  length, radius, bendRadius [, tempC, lossF] [, Name]		3..6 par
	//	conbend:  length, rIn, rOut, bendRadius [, tempC, lossF] [, Name]	4..7 par
	//Any line starting with a semi-colon ';' will be ignored as a comment.
	//#######Note that only float precision is used here######
	if(verboseMode_) cout << "Loading elemental file '" << filename << "'" << flush; 
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "ERROR: File '" << filename << "' failed to open" << endl;
		return false;	
	}

	int const arraySize=11;
	string line;
	string tempValues[arraySize];
	int lineCounter=1, objectType;
	bool nearEnd=false, noErrors=true;
	
	getline(file, line, '\n');
	name_=line.c_str();
	int length = (int)name_.size();
	if (length>0) if (name_[length-1]=='\r') name_[length-1]='\0';
	double radius = -1;
	double PrevInputRadius = -1;
	int type = -1;
	NbHole_.push_back(0);
	HornElement *mem;
	HornElement *branch;


	while (!file.eof()) { 
		getline(file, line, '\n');
		length=(int)line.size();
		if (length>0) if (line[length-1]=='\r') line[length-1]='\0'; //Correcting for Windows \r\n endlines.

		lineCounter++;
		if (file.eof()) nearEnd=true; //checks to see if this is the last line in the file

		if (line[0]!=';') { //checks for comments in the file
			int counter=0;
			for(int i=0, itemp=0; i<=length; i++) {
				
				if((line[i]==',' || line[i]=='\n' || line[i]=='\0' || line[i]=='\r' || (nearEnd && i==length)) && i!=itemp) {
					tempValues[counter]=line.substr(itemp, (i-itemp));
					counter++;
					itemp=i+1;
				}
			}		
			if(counter>0) objectType=atoi(tempValues[0].c_str()); 
			else objectType=-1; //Empty line case
//WK got optional section parameters tempC and lossF (before optional name)
			if(objectType==BOREJUMPSECTION_TYPE) {
				if(counter==7)      append(new BoreJumpSection((double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()),tempValues[7].c_str()), true); 
				else if(counter==7) append(new BoreJumpSection((double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str())), true); 
				else if(counter==6) append(new BoreJumpSection((double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str())), true); 
				else if(counter==5) append(new BoreJumpSection((double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str())), true);
				else if(counter==4) append(new BoreJumpSection((double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str())), true); 
				else if(counter==3) append(new BoreJumpSection((double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str())), true);
//				else if(counter==2) append(new BoreJumpSection((double)atof(tempValues[1].c_str())), true);
				else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : incorrect number of parameters (" << counter << ")"; noErrors=false;}		
				mem = getElement(elementList_.size()-1);
				radius = mem->rOut();
				type = mem->type();
				if(getElement(elementList_.size()-2)->type()==BRANCH_TYPE){getElement(elementList_.size()-2)->PutRoutParam(rIn());}
				if(getElement(elementList_.size()-2)->type()==-1){
					if(getElement(elementList_.size()-3)->type()==BRANCH_TYPE){getElement(elementList_.size()-3)->PutRoutParam(rIn());}
				}

			} else if(objectType==CYLINDERSECTION_TYPE) {
				if(counter==8)		append(new CylinderSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()),tempValues[7].c_str()), true);
				else if(counter==7) append(new CylinderSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str())), true); //ensures unnamed (in file) cyls aren't given names (in program)
 				else if(counter==6) append(new CylinderSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str())), true); 
				else if(counter==5) append(new CylinderSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str())), true);
				else if(counter==4) append(new CylinderSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str())), true); //ensures unnamed (in file) cyls aren't given names (in program)
				else if(counter==3) append(new CylinderSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str())), true);
				else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : incorrect number of parameters (" << counter << ")"; noErrors=false;}
				mem = getElement(elementList_.size()-1);
				radius = mem->rOut();
				type = mem->type();
				if(getElement(elementList_.size()-2)->type()==BRANCH_TYPE){getElement(elementList_.size()-2)->PutRoutParam(rIn());}
				if(getElement(elementList_.size()-2)->type()==-1){
					if(getElement(elementList_.size()-3)->type()==BRANCH_TYPE){getElement(elementList_.size()-3)->PutRoutParam(rIn());}
				}
			} else if(objectType==CONESECTION_TYPE) {
				if (counter==9)     append(new ConeSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str()), tempValues[8].c_str()), true); 
				else if(counter==8) append(new ConeSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str())), true); 
				else if(counter==7) append(new ConeSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str())), true); 
				else if(counter==6) append(new ConeSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str())), true);
				else if(counter==5) append(new ConeSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str())), true); 
				else if(counter==4) append(new ConeSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str())), true);
				else if(counter==3) append(new ConeSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str())), true);
				else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : incorrect number of parameters (" << counter << ")"; noErrors=false;}
				mem = getElement(elementList_.size()-1);
				PrevInputRadius = mem->rIn();
				radius = mem->rOut();
				type = mem->type();
				if(getElement(elementList_.size()-2)->type()==BRANCH_TYPE){getElement(elementList_.size()-2)->PutRoutParam(rIn());}
				if(getElement(elementList_.size()-2)->type()==-1){
					if(getElement(elementList_.size()-3)->type()==BRANCH_TYPE){getElement(elementList_.size()-3)->PutRoutParam(rIn());}
				}
			} else if(objectType==BESSELSECTION_TYPE) {
				if (counter==10)    append(new BesselSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (double)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str()), (float)atof(tempValues[8].c_str()),tempValues[8].c_str()), true);
				else if(counter==9) append(new BesselSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (double)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str()), (float)atof(tempValues[8].c_str())), true);
				else if(counter==8) append(new BesselSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (double)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str())), true); 
				else if(counter==7) append(new BesselSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (double)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str())), true);
				else if(counter==6) append(new BesselSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (double)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str())), true); 
				else if(counter==5) append(new BesselSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (double)atof(tempValues[4].c_str())), true);
				else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : incorrect number of parameters (" << counter << ")"; noErrors=false;}
				mem = getElement(elementList_.size()-1);
				PrevInputRadius = mem->rIn();
				radius = mem->rOut();
				type = mem->type();
				if(getElement(elementList_.size()-2)->type()==BRANCH_TYPE){getElement(elementList_.size()-2)->PutRoutParam(rIn());}
				if(getElement(elementList_.size()-2)->type()==-1){
					if(getElement(elementList_.size()-3)->type()==BRANCH_TYPE){getElement(elementList_.size()-3)->PutRoutParam(rIn());}
				}
			} else if(objectType==LISTSECTION_TYPE) {
				if(counter==2) {
					Hole = new WindInstrument();
					if( (*Hole).load(tempValues[1].c_str()) == false ) noErrors=false;
					append (Hole, true);
					HoleVector.push_back(Hole);
					int i = 2;
					branch = getElement(elementList_.size()-1);
					while(branch->type()!=8){branch = getElement(elementList_.size()-i);i++;}
					NbHole_.pop_back();
					branch->PutHoleParam(Hole->length(),Hole->rIn());
				}
				else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : incorrect number of parameters (" << counter << ")"; noErrors=false;}
			} /*else if(objectType==CYLINDERBENDSECTION_TYPE) {
				if(counter==9)      append(new CylinderBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str()), (float)atof(tempValues[8].c_str()), tempValues[9].c_str()), true);
				else if(counter==8) append(new CylinderBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str()), (float)atof(tempValues[8].c_str())), true);
				else if(counter==7) append(new CylinderBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str())), true); //ensures unnamed (in file) cyls aren't given names (in program)
				else if(counter==6) append(new CylinderBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str())), true);
				else if(counter==5) append(new CylinderBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str())), true); //ensures unnamed (in file) cyls aren't given names (in program)
				else if(counter==4)	append(new CylinderBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str())), true);
				else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : incorrect number of parameters (" << counter << ")"; noErrors=false;}
				mem = getElement(elementList_.size()-1);
				radius = mem->rOut();
				type = mem->type();
				if(getElement(elementList_.size()-2)->type()==BRANCH_TYPE){getElement(elementList_.size()-2)->PutRoutParam(rIn());}
				if(getElement(elementList_.size()-2)->type()==-1){
					if(getElement(elementList_.size()-3)->type()==BRANCH_TYPE){getElement(elementList_.size()-3)->PutRoutParam(rIn());}
				}*/

		//WASILAKIS (correction of the above commented out type)
			else if(objectType==CYLINDERBENDSECTION_TYPE) {
				if(counter==9)      append(new CylinderBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str()), tempValues[8].c_str()), true);
				else if(counter==8) append(new CylinderBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str())), true);
				else if(counter==7) append(new CylinderBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str())), true); //ensures unnamed (in file) cyls aren't given names (in program)
				else if(counter==6) append(new CylinderBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str())), true);
				else if(counter==5) append(new CylinderBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str())), true); //ensures unnamed (in file) cyls aren't given names (in program)
				else if(counter==4)	append(new CylinderBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str())), true);
				else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : incorrect number of parameters (" << counter << ")"; noErrors=false;}
				mem = getElement(elementList_.size()-1);
				radius = mem->rOut();
				type = mem->type();
				hasBends_ = true;
				if(getElement(elementList_.size()-2)->type()==BRANCH_TYPE){getElement(elementList_.size()-2)->PutRoutParam(rIn());}
				if(getElement(elementList_.size()-2)->type()==-1){
					if(getElement(elementList_.size()-3)->type()==BRANCH_TYPE){getElement(elementList_.size()-3)->PutRoutParam(rIn());}
				}
		///////////////////////////////////////



			} else if(objectType==CONEBENDSECTION_TYPE) {
				if (counter==10)    append(new ConeBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (double)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str()), (float)atof(tempValues[8].c_str()),tempValues[8].c_str()), true);
				else if(counter==9) append(new ConeBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (double)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str()), (float)atof(tempValues[8].c_str())), true);
				else if(counter==8) append(new ConeBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (double)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str()), (float)atof(tempValues[7].c_str())), true); 
				else if(counter==7) append(new ConeBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (double)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str())), true);
				else if(counter==6) append(new ConeBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (double)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str())), true); 
				else if(counter==5) append(new ConeBendSection(false, false, (double)atof(tempValues[1].c_str()), (double)atof(tempValues[2].c_str()), (double)atof(tempValues[3].c_str()), (double)atof(tempValues[4].c_str())), true);
				else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : incorrect number of parameters (" << counter << ")"; noErrors=false;}
				mem = getElement(elementList_.size()-1);
				PrevInputRadius = mem->rIn();
				radius = mem->rOut();
				type = mem->type();
				hasBends_ = true;
				if(getElement(elementList_.size()-2)->type()==BRANCH_TYPE){getElement(elementList_.size()-2)->PutRoutParam(rIn());}
				if(getElement(elementList_.size()-2)->type()==-1){
					if(getElement(elementList_.size()-3)->type()==BRANCH_TYPE){getElement(elementList_.size()-3)->PutRoutParam(rIn());}
				}
			}else if(objectType==TERMINATIONELEMENT_TYPE) {
				if(counter==1)      append(new TerminationElement(false, false, 0,1, radius, type, PrevInputRadius,-1), false);
				else if(tempValues[1].length()<4){
					const char * chaine = " ";
					if(counter==2)      append(new TerminationElement(false, false, 0,1, radius, type, PrevInputRadius,(int)atof(tempValues[1].c_str())), false);
					else if(counter==3) append(new TerminationElement(false, false, 0,(int)atof(tempValues[2].c_str()),radius, type, PrevInputRadius,(int)atof(tempValues[1].c_str()),chaine,(float)atof(tempValues[2].c_str())), false);
					else if(counter==4) append(new TerminationElement(false, false, 0,(int)atof(tempValues[2].c_str()),radius, type, PrevInputRadius,(int)atof(tempValues[1].c_str()),chaine,(float)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str())), false);
					else if(counter==5) append(new TerminationElement(false, false, 0,(int)atof(tempValues[2].c_str()),radius, type, PrevInputRadius,(int)atof(tempValues[1].c_str()),chaine,(float)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str())), false);
					else if(counter==6) append(new TerminationElement(false, false, 0,(int)atof(tempValues[2].c_str()),radius, type, PrevInputRadius,(int)atof(tempValues[1].c_str()),chaine,(float)atof(tempValues[2].c_str()), (float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str())), false);
					else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : incorrect number of parameters (" << counter << ")"; noErrors=false;}
				}
				else{
					if(counter==2) append(new TerminationElement(false, false,0, 1, radius, type, PrevInputRadius,3,tempValues[1].c_str()), false);
					else if(counter==3) append(new TerminationElement(false, false,0,(int)atof(tempValues[2].c_str()),radius, type, PrevInputRadius,3,tempValues[1].c_str(),(float)atof(tempValues[3].c_str())), false);
					else if(counter==4) append(new TerminationElement(false, false,0,(int)atof(tempValues[2].c_str()),radius, type, PrevInputRadius,3,tempValues[1].c_str(),(float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str())), false);
					else if(counter==5) append(new TerminationElement(false, false,0,(int)atof(tempValues[2].c_str()),radius, type, PrevInputRadius,3,tempValues[1].c_str(),(float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str())), false);
					else if(counter==6) append(new TerminationElement(false, false,0,(int)atof(tempValues[2].c_str()),radius, type, PrevInputRadius,3,tempValues[1].c_str(),(float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str()), (float)atof(tempValues[6].c_str())), false);
					else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : incorrect number of parameters (" << counter << ")"; noErrors=false;}
				}
				mem = getElement(elementList_.size()-1);
				type = mem->type();
			}else if(objectType==BRANCH_TYPE) {
				//command in the bore list: 8, number of holes (don't working for more than 1 yet...),type of calculation (0= simple and 1= complete model), temperature etc
				if(nModes_>1) {cout << "WARNING: Branch calculation only considers plane wave mode\n";}
				if(counter==2)      append(new Branch(false, false, radius, radius), true); //ensures unnamed (in file) cyls aren't given names (in program)
				else if(counter==3) append(new Branch(false, false, (int)atoi(tempValues[1].c_str()), radius, radius, (int)atoi(tempValues[2].c_str())), false); //ensures unnamed (in file) cyls aren't given names (in program)
				else if(counter==4) append(new Branch(false, false, (int)atoi(tempValues[1].c_str()), radius, radius, (int)atoi(tempValues[2].c_str()),(float)atof(tempValues[3].c_str())), false); //ensures unnamed (in file) cyls aren't given names (in program)
				else if(counter==5) append(new Branch(false, false, (int)atoi(tempValues[1].c_str()), radius, radius, (int)atoi(tempValues[2].c_str()),(float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str())), false);
				else if(counter==6) append(new Branch(false, false, (int)atoi(tempValues[1].c_str()), radius, radius, (int)atoi(tempValues[2].c_str()),(float)atof(tempValues[3].c_str()), (float)atof(tempValues[4].c_str()), (float)atof(tempValues[5].c_str())), false);
				else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : incorrect number of parameters (" << counter << ")"; noErrors=false;}
				mem = getElement(elementList_.size()-1);
				hasBranch_ = true;
				NbHole_.push_back(mem->NbHole());
			}else if(objectType!=-1) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : object type code (" << objectType << ") not recognised"; noErrors=false;}		
		}
	}


	//Add termination element at the end of the list if it doesn't exist
	if (mem->type()!=TERMINATIONELEMENT_TYPE) {
		append(new TerminationElement(false, false, 0,1, mem->rOut(), mem->rOut(), mem->type(),1), false);
	}
	
	
	file.close();
	if(verboseMode_) {
		if (noErrors) cout << " : done" << endl; else cout << " : errors found, load failed" << endl; 
	}
	return noErrors;
}


void WindInstrument::loadBase (const char filename[]) { 
	//Loads from a file the template for the genome and sets it to the static defaultList_.
	
	if(verboseMode_) cout << "Loading HornElemental file: '" << filename << "' " << flush;

	ifstream file(filename);
	char buffer[100];
	
	if (!file.is_open()) {
		cout << "ERROR: File failed to open\n";
		return;
	}
		int counter=0; 
	int const arraySize=11;
	char valueBuffer[arraySize][15];
	char temp[15];
	double value[arraySize];
		while (!file.eof()) { //Reading from file
		file.getline(buffer, 100);
		counter=0;

		for(int j=0; j<arraySize; j++) {
			valueBuffer[j][0]='\0';
			value[j]=-1;
			if (buffer[0]!=';' && buffer[0]!='\0') { //ignoring comments
				int i; 
				for (i=0; i<100; i++) {
					if (buffer[i+counter]=='\0') {valueBuffer[j][i]='\0'; break;} //Cases with <10 params
				
					if (buffer[i+counter]!=',' && buffer[i+counter]!='\0') {valueBuffer[j][i]=buffer[i+counter];}  //loop continues
					else {valueBuffer[j][i]=buffer[i+counter]; valueBuffer[j][i]='\0'; counter+=i+1; i=0; break;}
				}
				for (int a=0; a<15; a++) temp[a]=valueBuffer[j][a];			
				value[j]=(double)atof(temp);
				if (buffer[i+counter]=='\0') break; //Cases with <10 params
			} 
		}
//WK needs section parameters tempC and lossF
		if(value[0]==0) defaultList_.push_back(new BoreJumpSection (value[1],value[2]));
		else if(value[0]==1) defaultList_.push_back(new CylinderSection (value[1]==1, value[2]==1, value[3], value[4], value[5], value[6]));
		else if(value[0]==2) defaultList_.push_back(new ConeSection (value[1]==1, value[2]==1, value[3], value[4], value[5], value[6], value[7], value[8]));
		else if(value[0]==2.5) defaultList_.push_back(new ConeSection (value[1], value[2])); //Cone defined with dimensions relative to adjacent elements - ###Depreciated?
		else if(value[0]==3) defaultList_.push_back(new BesselSection (value[1]==1, value[2]==1, value[3], value[4], value[5], value[6], value[7], value[8], value[9], value[10]));
		else if(value[0]==4) {defaultList_.push_back(new WindInstrument(temp, false));}
	}
	file.close();
	cout << " : done" << endl;
}




#endif

