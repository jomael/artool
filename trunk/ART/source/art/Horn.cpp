#include "Horn.h"
#include <iomanip>

void Horn::copyArrays (double x[], double r[], int size) {
	for (int i=0; i<size; i++) {
		xArray_.push_back(x[i]);
		rArray_.push_back(r[i]);
		fArray_.push_back(0); 		// WK
	}

	//cout << "copying array to std::vector: \n";
	//for (int i=0; i<size; i++) cout << i << ": " << xArray__.at(i) << " from " << x[i] << endl;
}

// WK
void Horn::copyArrays (double x[], double r[], double f[], int size) {
	for (int i=0; i<size; i++) {
		xArray_.push_back(x[i]);
		rArray_.push_back(r[i]);
		fArray_.push_back(f[i]);
	}
}

void Horn::copyLists (list<double> x, list<double> r) {
	xArray_ = x;
	rArray_ = r;
}

// WK
void Horn::copyLists (list<double> x, list<double> r, list<double> f) {
	xArray_ = x;
	rArray_ = r;
	fArray_ = f;
}

Horn::Horn() {
	xArray_ = list<double>();
	rArray_ = list<double>();
	fArray_ = list<double>();		// WK
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();		// WK
}


Horn::Horn (double x[], double r[], int size) {
	xArray_ = list<double>();
	rArray_ = list<double>();
	fArray_ = list<double>();		// WK
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();		// WK
	copyArrays (x, r, size);
}

// WK
Horn::Horn (double x[], double r[], double f[], int size) {
	xArray_ = list<double>();
	rArray_ = list<double>();
	fArray_ = list<double>();
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();
	copyArrays (x, r, f, size);
}

Horn::Horn (double x, double r) {
	xArray_ = list<double>();
	rArray_ = list<double>();
	fArray_ = list<double>(); 		// WK
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();		// WK
	xArray_.push_back(x);
	rArray_.push_back(r);
	fArray_.push_back(0);			// WK
}

// WK
Horn::Horn (double x, double r, double f) {
	xArray_ = list<double>();
	rArray_ = list<double>();
	fArray_ = list<double>();
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();
	xArray_.push_back(x);
	rArray_.push_back(r);
	fArray_.push_back(f);
}

Horn::Horn (Horn& a) { 
	xArray_ = a.getX();
	rArray_ = a.getR();
	fArray_ = a.getF();
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();
}

Horn::~Horn() {
	xArray_.clear();
	rArray_.clear();
	fArray_.clear();
}

void Horn::operator = (Horn& a) { //Copies a horn but DOES NOT copy iterators.
	xArray_=a.getX();
	rArray_=a.getR();
	fArray_=a.getF();

	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();
}

void Horn::clear() {
	//clears the lists.
	
	xArray_.clear();
	rArray_.clear();
	fArray_.clear();
}

list<double> Horn::getX()  const { return xArray_; }

double Horn::getX (int i) { //random access - deprecated

	list<double>::iterator tempIter = xArray_.begin();
	for (int j=0; j<i; j++) tempIter++; 

	return (*tempIter);
}

list<double> Horn::getR() const { return rArray_; }

double Horn::getR (int i)  { //random access - deprecated

	list<double>::iterator tempIter = rArray_.begin();
	for (int j=0; j<i; j++) tempIter++;
	
	return *tempIter;
}

// WK
list<double> Horn::getF() const { return fArray_; }

double Horn::getF (int i)  { //random access - deprecated

	list<double>::iterator tempIter = fArray_.begin();
	for (int j=0; j<i; j++) tempIter++;
	
	return *tempIter;
}

double Horn::getXcurrent() const {return *(xIter_);}

double Horn::getRcurrent() const {return *(rIter_);} // WK: corrected iterator

double Horn::getFcurrent() const {return *(fIter_);} // WK

int Horn::getSize() const { return xArray_.size(); }

void Horn::insert (const double x, const double r) {
	xIter_ = xArray_.insert(xIter_, x);
	rIter_ = rArray_.insert(rIter_, r);
	fIter_ = fArray_.insert(fIter_, 0);			// WK
}

// WK
void Horn::insert (const double x, const double r, const double f) {
	xIter_ = xArray_.insert(xIter_, x);
	rIter_ = rArray_.insert(rIter_, r);
	fIter_ = fArray_.insert(fIter_, f);
}

// WK
void Horn::insert (const double x, const dcomp r) {
        insert (x, abs(r), arg(r));
}

void Horn::append (const double x, const double r) {
	//Appends given point onto the end of the list.
		
	xArray_.push_back( x );
	rArray_.push_back( r );
	fArray_.push_back( 0 );					// WK
	if (xArray_.size()==1) setItersToBegin(); // If arrays were empty, sets pointer to begin (would be NULL else)
	//cout << "append :" << xArray_.size() << endl;
}

// WK
void Horn::append (const double x, const double r, const double f) {
	//Appends given point onto the end of the list.
		
	xArray_.push_back( x );
	rArray_.push_back( r );
	fArray_.push_back( f );
	if (xArray_.size()==1) setItersToBegin(); // If arrays were empty, sets pointer to begin (would be NULL else)
	//cout << "append :" << xArray_.size() << endl;
}

// WK
void Horn::append (const double x, const dcomp r) {
        append (x, abs(r), arg(r));
}
		
void Horn::print (const bool carriageReturn) const {
	// Returns a text representation to the screen.
	// If argument is true, a new line is used for each entry.
	// Output is formatted to be cut'n'pasted into Maple as a list

	list<double>::const_iterator tempxIter_ = xArray_.begin();
	list<double>::const_iterator temprIter_ = rArray_.begin();
	list<double>::const_iterator tempfIter_ = fArray_.begin();	// WK
	int size = this->getSize();
	cout << "size: " << size << endl;
	cout << "[";
	
//	cout << setiosflags (ios::fixed | ios::showpoint) << setprecision(6);

	while (true) { 
		cout << "[" << *tempxIter_ << ", " << *temprIter_ << ", " << *tempfIter_ << "]";
		tempxIter_++;
		temprIter_++;
		tempfIter_++;		// WK
		if (tempxIter_ == xArray_.end()) break; else cout << ", ";
		if (carriageReturn) cout << "\n";
	}
	cout << "]\n";
}

void Horn::setItersToBegin() {
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin(); 	// WK
}

void Horn::setItersToEnd() { 
	//Sets iterators to point to the last value (NOT to the actual end of the list)
	
	xIter_ = xArray_.end();
	rIter_ = rArray_.end();
	fIter_ = fArray_.end();		// WK
	xIter_--; rIter_--; fIter_--;	// WK
}

bool Horn::load (const char filename[]) {
	//Loads instrument from given filename, and sets this horn to equal it.
	//Instrument files use .int extension, and are arranged thus:
	/*  name
	 *  horn.getX(0), horn.getR(0)
	 *  etc.
	 */
	//Any line starting with a semi-colon ';' will be ignored as a comment.
	//The last horn r-value must match cyl[1] (the radius) unless cyl[1]=0.
	//#######Note that only float precision is used here######

	cout << "Loading ic file: " << filename << " ";

	ifstream file(filename);
	char buffer[100];

	if (!file.is_open()) {
		cout << "ERROR: File failed to open\n";
		return false;
	}

	//file.getline(name, 32);

	char xBuffer[100], rBuffer[100], fBuffer[100];
	int pointer=0;
	
	// WK
	while (!file.eof()) { //Reading horn from file
		xBuffer[0]='\0';
		rBuffer[0]='\0';
		fBuffer[0]='\0';		// WK
		file.getline(buffer, 100);
		if (buffer[0]!=';') { //ignoring comments
			pointer=100; //default case, will be reassigned in for loop
			//declare for-loop-var as local var to resolve VC compatibility issues...
			int i;
			for ( i=0; i<100; i++) {
				if (buffer[i]!=',') {xBuffer[i]=buffer[i];}
				else {xBuffer[i]='\0'; pointer=i; break;}
			}
						
			for ( i=pointer+2; i<100; i++) {							
				if (buffer[i]!=',') {rBuffer[i-pointer-2]=buffer[i];}
				else {rBuffer[i]='\0'; pointer=i; break;}
			}
						
			for ( i=pointer+2; i<100; i++) {
				if (buffer[i]!='\0') fBuffer[i-pointer-2]=buffer[i];
				else {fBuffer[i-pointer-2]='\0'; break;}
			}
			if (fBuffer[0]!='\0') 	append ((double)atof(xBuffer), (double)atof(rBuffer), (double)atof(fBuffer));
			else  			append ((double)atof(xBuffer), (double)atof(rBuffer));
		}
	}
	file.close();
	cout << " : done\n";
	return true;
}

void Horn::save (const char filename[]) {
	//Saves instrument to given file
	
	cout << "Saving impedance curve as file '" << filename << "'";
	setItersToBegin();
	
	ofstream file(filename);
	file << setiosflags (ios::fixed | ios::showpoint) << setprecision(6);

	//file << name << endl;
	for (int i=0; i<getSize()-1; i++) {
		file << *(xIter_) << ", " << *(rIter_) << ", " << *(fIter_) << endl; 	// WK
		xIter_++; rIter_++; fIter_++;								// WK
	}
	file << *(xIter_) << ", " << *(rIter_)<< ", " << *(fIter_);

	file.close();
	cout << " : done\n";
}
