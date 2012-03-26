/******************************************************************************\
 *                      _____  _    ____          _____
 *                     |_   _|/ \  |  _ \  __   _|___ /
 *                       | | / _ \ | |_) | \ \ / / |_ \
 *                       | |/ ___ \|  __/   \ V / ___) |
 *                       |_/_/   \_\_|       \_/ |____/
 *
 ******************************************************************************
 *
 *  Copyright (C) 1996       Wilfried Kausel, Nikolaus Kerö
 *                           Inst. of Electronics and Quantum Electronics
 *                           Vienna University of Technology
 *
 *  Copyright (C) 1997-2001  Herbert Nachtnebel, Nikolaus Kerö
 *                           Inst. of Ind. Electronics and Material Science
 *                           Vienna University of Technology
 *
 *  Copyright (C) 1997-2005  Wilfried Kausel
 *                           Department of Musical Acoustics
 *                           University of Music, Vienna
 *
 *  Copyright (C) 2001-2005  Herbert Nachtnebel
 *                           Inst. of Computer Science
 *                           Vienna University of Technology
 *
 *  Copyright (C) 2006       Herbert Nachtnebel
 *                           Research Unit for Integrated Sensor Systems
 *                           Austrian Academy of Science
 *
 ******************************************************************************
 *
 *  Project:  TAP - Transfer Analysis Package
 *
 *  Title:    Error Handling Classes
 *
 *  Author:   Herbert Nachtnebel
 *
 *  Version:  $Revision$
 *
 *  Date:     $Date$
 *
 *  Revisor:  $Author$
 *
 ******************************************************************************
 *
 *  Changelog:
 *
 *  $Log$
 *
 *
 *  Original Changelog:
 *
 *     Date     Rev  Ver                        Description
 *  ----------- --- ----- -----------------------------------------------------
 *  07-Jan-1997 BAR  0.5  Started implementation.
 *  08-Jan-1997 BAR  1.0  It seems to work.
 *  01-Jul-1997 BAR  2.0  New implementation, now it is capable to handle
 *                        Warnings, Errors, Fatal Errors and Internal Errors.
 *  27-Apr-1999 BAR  2.1  Made it compatible with gcc.
 *  06-May-1999 BAR       Reviewed: checked Unix compatibility.
 *  11-May-2000 BAR  2.2  Complain prefix operator simulation included.
 *  21-Jun-2000 BAR  2.9  Rewritten to simplify error message generation.
 *  20-Jul-2001 BAR  2.99 Adopted to use the STL.
 *  26-Jul-2001 BAR  3.0  Internal data structure changed to binary sorted array.
 *
 ******************************************************************************
 *
 *  Description
 *  -----------
 *
 *  The classes defined in this include file simplifies the generation of an
 *  error handling system in an application. One is able to comfortable add
 *  warnings, normal errors, fatal errors or internal errors. Each message
 *  can be supplied with additional error information and may display a line
 *  number information. The additional error information may be formated
 *  with the normal stream put operator (operator<<).
 *
 *  Hierarchical relationship
 *  -------------------------
 *                                     class Failure
 *                                    /       |     \
 *                                   /        |      \
 *                            class Warning   |  class InternalError
 *                                            |
 *                                            |
 *                                      class Error
 *                                            |
 *                                            |
 *                                    class FatalError
 *
 *  Class Failure
 *  -------------
 *     This is the virtual base class for the other error classes.
 *     Cannot be used directly.
 *
 *  Class Error
 *  -----------
 *     Normal error class. Execution will not be stopped, until there were
 *     no more than MAXERRS (100) errors.
 *
 *  Class FatalError
 *  ----------------
 *     If you throw this error the program will be terminated during error
 *     handling.
 *
 *  class InternalError
 *  -------------------
 *     Use it for unexpected program behaviour.
 *     The program will be terminated.
 *
 *  class Warning
 *  -------------
 *     If you don't want to break program execution use the complain prefix
 *     operator instead of a throw statement.
 *
 *  Usage
 *  -----
 *     To use the error handling system, one must declare a static variable
 *     of the requested error type for each possible error message. To
 *     generate an error, use the standard C++ exception handling to throw
 *     the error variable. To add additional error information, format the
 *     message in the same statement the error is thrown. To display the
 *     error, call the handle function for the error in the execution handler
 *     were you catch the error. If the catched error is of type fatal error
 *     or internal error, than the program will be terminated.
 *
 *
\******************************************************************************/
#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED


/******************************************************************************/
/*                                Definitions                                 */
/******************************************************************************/
#ifndef MAXERRS
#define MAXERRS 100
#endif

#ifndef ERROR_TABLE_GRANULARITY
#define ERROR_TABLE_GRANULARITY 32
#endif


/******************************************************************************/
/*                            Needed Include Files                            */
/******************************************************************************/
#include <cstddef>

#include <ios>
#include <iosfwd>
#include <iostream>
#include <iomanip>
#include <sstream>

#if defined(_WIN32)
   #include <malloc.h>
#endif

// cbg: g++ does not know strlen and exit per default
#if defined(__GNUG__)
	#include <cstring>
	#include <cstdlib>
	#include <alloca.h>
	#define _alloca(a) alloca(a)
#endif

// forward declaration
class CommandLineParser;

/******************************************************************************/
/*                               Class Definitions                            */
/******************************************************************************/
class Failure
{
    struct ErrObj
    {
        unsigned int number : 24;                 // error number, distinct ranges for the types
        unsigned int type   :  4;                 // internal failure type flag
        unsigned int ref    :  4;                 // reference count

        const char *msg;                          // the error message

        ErrObj() throw() { if (ref++ == 0) insert(); }    // increment reference count
       ~ErrObj() throw() { if (--ref == 0) remove(); }    // decrement reference count

    private:
        ErrObj (const ErrObj&);                   // unimplemented
        ErrObj& operator= (const ErrObj&);        // unimplemented

        // variables to control the sorted cross reference table
        static size_t   rlen;                     // current length of cross reference table
        static size_t   rtop;                     // number of used items in xref table
        static ErrObj **rtab;                     // sorted cross reference table
        static unsigned int ipos;                 // position where element should be inserted

        int order (unsigned int number, unsigned int type);
        static ErrObj* search (unsigned int number, unsigned int type);

        void insert();                            // insert into binary sorted array
        void remove();                            // remove from binary sorted array

        // variables to control the memory chunks
        static size_t cused;                      // number of used entries in carr
        static size_t cnext;                      // next node entry to use in carr[cused-1]
        static void*  cfree;                      // linked list of free memory chunks
        static char** carr;                       // array of memory chunks

    public:
        // memory management
        void* operator new (size_t);              // get memory from memory chunk
        void  operator delete (void*, size_t);    // give memory back to memory chunk

        // placement control
        void* operator new (size_t, unsigned int number, unsigned int type);
        void* operator new (size_t, ErrObj* p) { return p; }
    };

    static std::ostream      *out;                // output stream
    static ErrObj            *proc;               // currently processed failure
    static std::stringstream *info;               // additional failure information
    static unsigned int       argc;               // number of additional failure informations

    static const char        *pgm;                // program name
    static unsigned int       lineno;             // line number where failure occured
    static const char        *source;             // source file where failure occured

    void Process (ErrObj *);                      // set failure currently processed
    std::stringstream& Info ();                   // get internal info stream

protected:
    template<class T> static T xchg (T& a, T b) { T old = a; a = b; return old; }

    // For efficiency, the one and only member variable of the failure class!
    ErrObj *data;                                 // pointer to underlying description entry

    // Construct with full failure information
    Failure (unsigned int number, unsigned int type, const char* msg = 0);

    virtual void Handle ();                       // common error handling
    virtual void Print (std::ostream&);           // print message
    virtual const char* Type () = 0;              // printable failure type

public:
    virtual ~Failure () { delete data; }          // base classes should have virtual dtors

    // Classes which are thrown must allow temporary objects to be created,
    // therefore we need a copy constructor and an assignment operator.
    // Since there will be at least one copy of a thrown object created,
    // the copy constructor and assignment operator should be as efficient
    // as it could be.
    Failure (const Failure&);                     // copy constructor
    Failure& operator= (const Failure&);          // assignment operator

    static std::ostream& Connect (std::ostream&); // change output stream
    static const char* ContactString (const char*); // change contact string
    static const char* ProgramName (const char*); // change program name

    const char*  SourceFile (const char*);        // set current source file name
    unsigned int SourceLine (unsigned int);       // set current source line

    struct Complain                               // class to simulate a prefix operator
    {
        Complain() {}                             // construct empty object
        void operator, (Failure& Fault) { Fault.Handle(); }
    };
    friend struct Complain;                       // grant accass to protected Handle() function
#define complain Failure::Complain(),             // DIRTY TRICK: prefix operator simulation

    template<class T>
        Failure& operator<< (const T& t)          // supply additional failure information
    {
	      Info() << t << std::ends;                 // append information to internal info stream
        return *this;
    }

protected:
    Failure& operator() (unsigned int);           // set source line
    Failure& operator() (const char*);            // set source file
    Failure& operator() (unsigned int, const char*);  // set source line and source file
    Failure& operator() (const char*, unsigned int);  // set source file and source line

    static const char* Lit1;                      // ": "
    static const char* Lit2;                      // " #"
    static const char* Lit3;                      // "> "
    static const char* Lit4;                      // ", "
    static const char* Line;                      // "Line "
    static const char* File;                      // "File "
    static const char* Aborting;                  // "Aborting..."
    static const char* Contact;

    enum { FAILURE, WARNING, ERROR, INTERNAL };   // failure type's
};

/******************************************************************************/

class Warning : public Failure
{
    static unsigned int count;                    // warnings counter
    static unsigned int ilast;                    // last warning number

protected:
    virtual void Handle ();                       // handle warning
    virtual void Print (std::ostream&);           // print warning message
    virtual const char* Type ();                  // failure type: warning

public:
    Warning (unsigned int);                       // look for an existing warning
    Warning (const char*);                        // construct with automatic warning number
    Warning (unsigned int, const char*);          // construct with explicit warning number
    Warning (const char*, unsigned int);          // same like above

    Warning (const Warning& copy) : Failure(copy) {}
    Warning& operator= (const Warning& copy) { Failure::operator= (copy); return *this; }

    Warning& operator() (unsigned int n)                { Failure::operator()(n);   return *this; }
    Warning& operator() (const char* s)                 { Failure::operator()(s);   return *this; }
    Warning& operator() (unsigned int n, const char* s) { Failure::operator()(n,s); return *this; }
    Warning& operator() (const char* s, unsigned int n) { Failure::operator()(s,n); return *this; }

    static unsigned int SetNext (unsigned int);   // set next warning number
    static unsigned int Count ();                 // get warnings counter
    static void Reset ();                         // reset warnings counter

    template<class T>
        Warning& operator<< (const T& t)
    {
        Failure::operator<< (t);
        return *this;
    }
};

/******************************************************************************/

class Error : public Failure
{
	friend class CommandLineParser;
    static unsigned int ilast;                    // last error number

protected:
    static unsigned int count;                    // error counter

    virtual void Handle ();                       // handle error
    virtual void Print (std::ostream&);           // print error
    virtual const char* Type ();                  // failure type: error

public:
    Error (unsigned int);                         // look for an existing error
    Error (const char*);                          // construct with automatic error number
    Error (unsigned int, const char*);            // construct with explicit error number
    Error (const char*, unsigned int);            // same like above

    Error (const Error& copy) : Failure(copy) {}
    Error& operator= (const Error& copy) { Failure::operator= (copy); return *this; }

    Error& operator() (unsigned int n)                { Failure::operator()(n);   return *this; }
    Error& operator() (const char* s)                 { Failure::operator()(s);   return *this; }
    Error& operator() (unsigned int n, const char* s) { Failure::operator()(n,s); return *this; }
    Error& operator() (const char* s, unsigned int n) { Failure::operator()(s,n); return *this; }

    static unsigned int SetNext (unsigned int);   // set next error number
    static unsigned int Count ();                 // get error counter
    static void Reset ();                         // reset error counter

    template<class T>
        Error& operator<< (const T& t)
    {
        Failure::operator<< (t);
        return *this;
    }
};

/******************************************************************************/

class FatalError : public Error
{
protected:
    virtual void Handle ();                       // handle error, never returns
    virtual void Print (std::ostream&);           // print fatal error

public:
    FatalError (unsigned int);                    // look for an existing error
    FatalError (const char*);
    FatalError (unsigned int, const char*);
    FatalError (const char*, unsigned int);

    FatalError& operator() (unsigned int n)                { Error::operator()(n);   return *this; }
    FatalError& operator() (const char* s)                 { Error::operator()(s);   return *this; }
    FatalError& operator() (unsigned int n, const char* s) { Error::operator()(n,s); return *this; }
    FatalError& operator() (const char* s, unsigned int n) { Error::operator()(s,n); return *this; }

    FatalError (const FatalError& copy) : Error(copy) {}
    FatalError& operator= (const FatalError& copy) { Error::operator= (copy); return *this; }

    template<class T>
        FatalError& operator<< (const T& t)
    {
        Error::operator<< (t);
        return *this;
    }
};

/******************************************************************************/

class InternalError : public Failure
{
    static unsigned int ilast;                    // last error number

protected:
    virtual void Handle ();                       // handle error, never returns
    virtual void Print (std::ostream&);           // print internal error
    virtual const char* Type ();                  // failure type: internal error

public:
    InternalError (unsigned int);
    InternalError (const char* str);
    InternalError (unsigned int, const char*);
    InternalError (const char*, unsigned int);

    InternalError (const InternalError& copy) : Failure(copy) {}
    InternalError& operator= (const InternalError& copy) { Failure::operator= (copy); return *this; }

    InternalError& operator() (unsigned int n)                { Failure::operator()(n);   return *this; }
    InternalError& operator() (const char* s)                 { Failure::operator()(s);   return *this; }
    InternalError& operator() (unsigned int n, const char* s) { Failure::operator()(n,s); return *this; }
    InternalError& operator() (const char* s, unsigned int n) { Failure::operator()(s,n); return *this; }

    static unsigned int SetNext (unsigned int);   // set next error number

    template<class T>
        InternalError& operator<< (const T& t)
    {
        Failure::operator<< (t);
        return *this;
    }
};


/******************************************************************************/
/*    Inline Functions, Forwarded Function Definitions, Template Functions    */
/******************************************************************************/
inline Failure::Failure (unsigned int n, unsigned int t, const char* m)
{
    data = new(n, t) ErrObj;            // create error description
    data->msg = m;                      // set error message
}

inline Failure::Failure (const Failure &copy)
{
    data = new(copy.data) ErrObj;       // copy existing error description
}

inline Failure& Failure::operator= (const Failure &copy)
{
    delete data;                        // delete old error description
    data = new(copy.data) ErrObj;       // copy existing error description
    return *this;
}

inline std::ostream& Failure::Connect (std::ostream& os)
{
    std::ostream* old = out; out = &os;
    return *old;                        // exchange and convert reference to pointer
}

inline const char* Failure::ContactString (const char* string)
{
    return xchg (Contact, string);
}

inline const char* Failure::ProgramName (const char* name)
{
    return xchg (pgm, name);
}

inline const char* Failure::SourceFile (const char* src)
{
    Process (data);
    return xchg (source, src);
}

inline unsigned int Failure::SourceLine (unsigned int line)
{
    Process (data);
    return xchg (lineno, line);
}

inline Failure& Failure::operator() (unsigned int line)
{
    Process (data);
    lineno = line;
    return *this;
}

inline Failure& Failure::operator() (const char* src)
{
    Process (data);
    source = src;
    return *this;
}

inline Failure& Failure::operator() (unsigned int line, const char* src)
{
    Process (data);
    lineno = line;
    source = src;
    return *this;
}

inline Failure& Failure::operator() (const char* src, unsigned int line)
{
    Process (data);
    source = src;
    lineno = line;
    return *this;
}

/******************************************************************************/

inline int Failure::ErrObj::order (unsigned int n, unsigned int t)
{
    register int c = t - type;
    return c ? c : n - number;
}

/******************************************************************************/

inline Warning::Warning (unsigned int num)
    : Failure (num, WARNING)
{}

inline Warning::Warning (const char* str)
    : Failure (++ilast, WARNING, str)
{}

inline Warning::Warning (unsigned int num, const char* str)
    : Failure (num, WARNING, str)
{
    if (num > ilast) ilast = num;       // memorize highest warning initialized
}

inline Warning::Warning (const char* str, unsigned int num)
    : Failure (num, WARNING, str)
{
    if (num > ilast) ilast = num;       // memorize highest warning initialized
}

inline const char* Warning::Type ()
{
    return "Warning";                   // name of this failure type
}

inline unsigned int Warning::SetNext (unsigned int i)
{
    return xchg (ilast, i);
}

inline unsigned int Warning::Count ()
{
    return count;                       // return number of warnings encountered
}

inline void Warning::Reset ()
{
    count = 0;                          // reset warning counter
}

/******************************************************************************/

inline Error::Error (unsigned int num)
    : Failure (num, ERROR)
{}

inline Error::Error (const char* str)
    : Failure (++ilast, ERROR, str)
{}

inline Error::Error (unsigned int num, const char* str)
    : Failure (num, ERROR, str)
{
    if (num > ilast) ilast = num;       // memorize highest error initialized
}

inline Error::Error (const char* str, unsigned int num)
    : Failure (num, ERROR, str)
{
    if (num > ilast) ilast = num;       // memorize highest error initialized
}

inline const char* Error::Type ()
{
    return "Error";                     // name of this failure type
}

inline unsigned int Error::SetNext (unsigned int i)
{
    return xchg (ilast, i);
}

inline unsigned int Error::Count ()
{
    return count;                       // return number of errors encountered
}

inline void Error::Reset ()
{
    count = 0;                          // reset error counter
}

/******************************************************************************/

inline FatalError::FatalError (unsigned int num)
    : Error (num)
{}

inline FatalError::FatalError (const char* str)
    : Error (str)
{}

inline FatalError::FatalError (unsigned int num, const char* str)
    : Error (num, str)
{}

inline FatalError::FatalError (const char* str, unsigned int num)
    : Error (str, num)
{}

/******************************************************************************/

inline InternalError::InternalError (unsigned int num)
    : Failure (num, INTERNAL)
{}

inline InternalError::InternalError (const char* str)
    : Failure (++ilast, INTERNAL, str)
{}

inline InternalError::InternalError (unsigned int num, const char* str)
    : Failure (num, INTERNAL, str)
{
    if (num > ilast) ilast = num;       // memorize highest error initialized
}

inline InternalError::InternalError (const char* str, unsigned int num)
    : Failure (num, INTERNAL, str)
{
    if (num > ilast) ilast = num;       // memorize highest error initialized
}

inline const char* InternalError::Type ()
{
    return "Internal Error";            // name of this failure type
}

inline unsigned int InternalError::SetNext (unsigned int i)
{
    return xchg (ilast, i);
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * The following definitions may be included only once into your project!
 *
 * You should include it into the source file were your main procedure is declared.
 * To include it, define MAIN before the statement, which includes this source file.
 */
#ifdef MAIN
#include <cassert>


/******************************************************************************/
/*                              Static Objects                                */
/******************************************************************************/
std::ostream*      Failure::out   = &std::cerr;   // output connected to standard error stream
std::stringstream* Failure::info  = 0;            // no string stream during program startup
unsigned int       Failure::argc   = 0;           // number of additional information strings
const char*        Failure::pgm    = 0;           // no standard program name
unsigned int       Failure::lineno = 0;           // source line of processed error
const char*        Failure::source = 0;           // source file of processed error
Failure::ErrObj*   Failure::proc   = 0;           // currently processed error message

size_t            Failure::ErrObj::rlen = 0;      // current length of cross reference table
size_t            Failure::ErrObj::rtop = 0;      // number of items in cross reference table
Failure::ErrObj** Failure::ErrObj::rtab = 0;      // sorted cross reference table
unsigned int      Failure::ErrObj::ipos = ~0UL;   // position where element should be inserted

size_t            Failure::ErrObj::cused = 0;     // number of used entries in carr
size_t            Failure::ErrObj::cnext = ~0UL;  // next node entry to use in carr[cused-1]
void*             Failure::ErrObj::cfree = 0;     // linked list of free memory chunks
char**            Failure::ErrObj::carr  = 0;     // array of memory chunks

unsigned int      Warning::count = 0;             // number of warnings encountered
unsigned int      Warning::ilast = 0;             // last initialized warning

unsigned int      Error::count = 0;               // number of errors encountered
unsigned int      Error::ilast = 0;               // last initialized error

unsigned int      InternalError::ilast = 0;       // last initialized internal error

const char*       Failure::Lit1 = ": ";           // common string literal
const char*       Failure::Lit2 = " #";           // - " -
const char*       Failure::Lit3 = "> ";           // - " -
const char*       Failure::Lit4 = ", ";           // - " -
const char*       Failure::Line = "Line ";        // - " -
const char*       Failure::File = "File ";        // - " -
const char*       Failure::Aborting = "Aborting...";  // - " -
const char*       Failure::Contact  = "    Please contact your vendor of this software\n"
                                       "    and supply him with the above information.";

FatalError ErrorCountExceeded (0, "Error count exceeds %1");


/******************************************************************************/
/*                                 Functions                                  */
/******************************************************************************/
Failure::ErrObj* Failure::ErrObj::search (unsigned int n, unsigned int t)
{
    int c = 0;
    unsigned int x = 0, l = 0, u = rtop;

    // binary search for existing error descriptor
    while (l != u)
    {
        x = (l+u)/2;
        c = rtab[x]->order(n,t);
        if (c < 0)
            u = x;
        else if (c > 0)
            l = x+1;
        else
            return rtab[x];
    }
    ipos = l;                           // insert position of element
    return 0;
}

void Failure::ErrObj::insert()
{
    assert (ipos <= rlen);		// avoids creation on stack

    unsigned int n;

    // resize table if necessary
    if (rlen == rtop)
    {
	ErrObj **parr = new ErrObj* [rlen+ERROR_TABLE_GRANULARITY];
	assert (parr != 0);

	if (rtab)
	{
	    for (n = 0; n < rlen; n++) parr[n] = rtab[n];
	    delete[] rtab;
	}
	rtab  = parr;
	rlen += ERROR_TABLE_GRANULARITY;
    }

    // insert at position ipos
    for (n = rtop++; n > ipos; n--) rtab[n] = rtab[n-1];
    rtab[ipos] = this;
    ipos = ~0UL;
}

void Failure::ErrObj::remove()
{
    int c = 0;
    unsigned int x = 0, l = 0, u = rtop;

    // search for this element in cross reference table
    while (l != u)
    {
	x = (l+u)/2;
	c = rtab[x] -> order (number, type);
	if (c < 0) u = x; else if (c > 0) l = x+1; else
	{
	    // delete it
	    for (rtop--; x < rtop; x++) rtab[x] = rtab[x+1];
	    break;
	}
    }
}

void* Failure::ErrObj::operator new (size_t len)
{
    void* p;

    if (cnext < ERROR_TABLE_GRANULARITY)
    {
        p = carr[cused-1] + len * cnext++;
    }
    else if (cfree)
    {
        p = cfree; cfree = *(void**)cfree;
    }
    else
    {
        if (0 == cused % ERROR_TABLE_GRANULARITY)
        {
            char** parr = new char* [cused+ERROR_TABLE_GRANULARITY];
            assert (parr != 0);

            if (cused)
            {
                for (unsigned int n = 0; n < cused; n++) parr[n] = carr[n];
                delete[] carr;
            }
            carr = parr;
        }

        char *parr = new char[ERROR_TABLE_GRANULARITY*len];
        assert (parr != 0);
        carr[cused++] = parr;   cnext = 0;
        p = carr[cused-1] + len * cnext++;
    }

    reinterpret_cast<ErrObj*>(p)->ref = 0;
    return p;
}

void Failure::ErrObj::operator delete (void* p, size_t)
{
    if (p && 0 >= reinterpret_cast<ErrObj*>(p)->ref)
    {
        *(void**)p = cfree; cfree = p;
    }
}

void* Failure::ErrObj::operator new (size_t len, unsigned int n, unsigned int t)
{
    // search for existing error descriptor
    ErrObj* obj = search(n,t);
    if (obj) return obj;

    // create new error descriptor
    obj = (ErrObj*) operator new (len);
    obj->number = n, obj->type = t;
    return obj;
}

/******************************************************************************/

std::stringstream& Failure::Info ()
{
    Process (data);                     // set failure currently processed
    if (!info)
    {
	info = new std::stringstream;   // be shure, string stream is initialised
        argc = 0;
    }
    argc++;                             // increment additional info string counter
    return *info;
}

void Failure::Process (ErrObj* curr)
{
    if (proc != curr)
    {
        delete info;                    // must delete wrong failure information
        info   = 0;                     // no current info
        argc   = 0;                     // no additional information strings
        proc   = curr;                  // this failure is processed
        lineno = 0;                     // no line number information
        source = 0;                     // no source line
    }
}

void Failure::Handle ()
{
    Print (*out);                       // print failure message
    Process (0);                        // delete internal string stream
}

void Failure::Print (std::ostream& out)
{
#if defined(_WIN32) || defined(__GNUG__)
    const char **argv = (const char **)_alloca (argc*sizeof(const char*));
#else    
    const char* argv [argc];            // pointer to additional error informations
#endif    
    unsigned int i;

    // get pointers to additional args
    if (argc)
    {
        const char* s;
        const char* args = info->str().c_str();   // get message string

        for (i=0, s=args; i<argc; i++)
            argv[i] = s, s = s + strlen(s) + 1;
    }

    // common message
    if (pgm)
    {
        out << pgm << Lit3;
    }
    if (source && lineno)
    {
        out << File << source << Lit4 << Line << lineno << Lit1;
    }
    else if (source)
    {
        out << File << source << Lit1;
    }
    else if (lineno)
    {
        out << Line << lineno << Lit1;
    }

    if (data->number)
    {
        out << Type() << Lit2 << data->number << Lit1;
    }

    // write message
    for (const char* s = data->msg; s && s[0]; s++)
    {
        if (s[0] == '%')
        {
            s++;
            if (s[0] == '%')
            {
                out << '%';
            }
            else
            {
                i = 0;
                while ('0' <= s[0] && s[0] <= '9')
                    i = 10*i + *(s++) - '0';
                if (i)
                    s--;
                if (i > 0 && i <= argc)
                    out << argv[i-1];
            }
        }
        else
        {
            out << *s;
        }
    }
    out << std::flush;
}

/******************************************************************************/

void Warning::Handle ()
{
    count++;                            // increment warnings encountered
    Failure::Handle ();                 // standard handle procedure
}

void Warning::Print (std::ostream& out)
{
    Failure::Print (out);               // standard message print
    out << '.' << std::endl;            // terminate the message
}

/******************************************************************************/

void Error::Handle ()
{
    count++;                            // increment errors encountered
    Failure::Handle ();                 // standard handle procedure
    if (count >= MAXERRS)               // To many errors?
        complain ErrorCountExceeded << count; // turn this error into a fatal error
}

void Error::Print (std::ostream& out)
{
    Failure::Print (out);               // standard message print
    out << '.' << std::endl;            // terminate the message
}

/******************************************************************************/

void FatalError::Handle ()
{
    count++;                            // increment errors encountered
    Failure::Handle ();                 // standard handle procedure
    exit (data->number);                // Bye!
}

void FatalError::Print (std::ostream& out)
{
    Failure::Print (out);               // standard message print
    out << '!' << std::endl;            // terminate the message
    out << Aborting << std::endl;       // tell user that we are terminating
}

/******************************************************************************/

void InternalError::Handle ()
{
    Failure::Handle ();                 // standard handle procedure
    exit (data->number);                // Bye!
}

void InternalError::Print (std::ostream& out)
{
    Failure::Print (out);               // standard message print
    out << '!' << std::endl;            // terminate the message
    out << Contact << std::endl;        // tell user what to do
    out << Aborting << std::endl;       // tell user that we are terminating
}

#endif /* MAIN */
#endif /* ERROR_INCLUDED */
