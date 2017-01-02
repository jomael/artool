#ifndef HORN_H
#define HORN_H

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <complex>
#include <math.h>
#include <string.h>
#include <vector>
#include <list>

using std::list;
typedef std::complex<double> dcomp;
using namespace std;


class Horn { 
//This is a base class implementing a two linked-lists of coordinates. When first written, there were good reasons to have it separate from ImpedanceCurve
// - these reasons no longer exist, but the class remains. Could do with an update. 

protected:
	list<double> xArray_;
	list<double> rArray_;
	list<double> fArray_;						// WK

public:
	//Constructors
	Horn ();
	Horn (double x[], double r[], int size);
	Horn (double x[], double r[], double f[], int size); 	// WK
	Horn (double x, double r);
	Horn (double x, double r, double f);			// WK
	Horn (Horn& a);
	~Horn ();
	list<double>::iterator xIter_;
	list<double>::iterator rIter_;
	list<double>::iterator fIter_;				// WK
	
	//Member functions
	void operator = (Horn& a);
	void copyArrays (double x[], double r[], int size);
	void copyArrays (double x[], double r[], double f[], int size); // WK
	void copyLists (list<double> x, list<double> r);
	void copyLists (list<double> x, list<double> r, list<double> f); // WK
	void clear();
	list<double> getX () const;
	double getX (int i);
	list<double> getR () const;
	double getR (int i);
	list<double> getF () const;					// WK
	double getF (int i);
	double getXcurrent () const;
	double getRcurrent () const;
	double getFcurrent () const;					// WK
	int getSize() const;
	void append (const double x, const double r); 		// Appends a point to the end of the list.
	void append (const double x, const double r, const double f); 	// WK
	void append (const double x, const dcomp r); 			// WK
	void insert (const double x, const double r);
	void insert (const double x, const double r, const double f);	// WK
	void insert (const double x, const dcomp r);			// WK
	void orderedAdd (const double x, const double r);  	// Adds a point just before the first point that has a higher x-value (deprecated)
	void orderedAdd (const double x, const dcomp r);  		// WK
	void print (const bool carriageReturn) const;
	void setItersToBegin();
	void setItersToEnd();
	bool itersNearEnd() {xIter_++; bool result = xIter_==xArray_.end(); xIter_--; return result;}
	bool itersAtEnd() {return xIter_==xArray_.end();}
	bool load (const char filename[]);
	void save (const char filename[]);
};
#endif //HORN_H
