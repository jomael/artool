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
/*  07-Jan-1997 BAR  0.5  Started implementation.                                                 */
/*  08-Jan-1997 BAR  1.0  It seems to work.                                                       */
/*  01-Jul-1997 BAR  2.0  New implementation, now it is capable to handle Warnings, Errors,       */
/*                        Fatal Errors and Internal Errors.                                       */
/*  27-Apr-1999 BAR  2.1  Made it compatible with gcc.                                            */
/*  06-May-1999 BAR       Reviewed: checked Unix compatibility.                                   */
/*  11-May-2000 BAR  2.2  Complain prefix operator simulation included.                           */
/*  21-Jun-2000 BAR  2.9  Rewritten to simplify error message generation                          */
/**************************************************************************************************/
/*  Description:                                                                                  */
/*                                                                                                */
/*     The classes defined in this include file simplifies the generation of an error handling    */
/*     system in an application. One is able to comfortable add warnings, normal errors, fatal    */
/*     errors or internal errors. Each message can be supplied with additional error information  */
/*     and may display a line number information. The additional error information may be         */
/*     formated with the normal stream put operator (operator<<).                                 */
/*                                                                                                */
/*     Hierarchical relationship:                                                                 */
/*     --------------------------                                                                 */
/*                                      class Failure                                             */
/*                                     /       |     \                                            */
/*                                    /        |      \                                           */
/*                             class Warning   |  class InternalError                             */
/*                                             |                                                  */
/*                                             |                                                  */
/*                                       class Error                                              */
/*                                             |                                                  */
/*                                             |                                                  */
/*                                     class FatalError                                           */
/*                                                                                                */
/*     class Failure:                                                                             */
/*     --------------                                                                             */
/*        This is the virtual base class for the other error classes. Cannot be used directly.    */
/*                                                                                                */
/*     class Error:                                                                               */
/*     ------------                                                                               */
/*        Normal error class. Execution will not be stopped, until there wasn't more than         */
/*        MAXERRS (100) errors.                                                                   */
/*                                                                                                */
/*     class FatalError;                                                                          */
/*     -----------------                                                                          */
/*        If you throw this error the program will be terminated during error handling.           */
/*                                                                                                */
/*     class InternalError:                                                                       */
/*     --------------------                                                                       */
/*        Use it for unexpected program behaviour. The program will be terminated.                */
/*                                                                                                */
/*     class Warning:                                                                             */
/*     --------------                                                                             */
/*        If you don't want to break program execution use the complain prefix operator           */
/*        instead of a throw statement.                                                           */
/*                                                                                                */
/*     Usage:                                                                                     */
/*     ------                                                                                     */
/*        To use the error handling system, one must declare a static variable of the requested   */
/*        error type for each possible error message. To generate an error, use the standard C++  */
/*        exception handling to throw the error variable. To add additional error information,    */
/*        format the message in the same statement the error is thrown. To display the error,     */
/*        call the handle function for the error in the execution handler were you catch the      */
/*        error. If the catched error is of type fatal error or internal error, than the pro-     */
/*        gram will be terminated.                                                                */
/*                                                                                                */
/*                                                                                                */
/**************************************************************************************************/
#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

/**************************************************************************************************/
/*                                          Definitions                                           */
/**************************************************************************************************/
#ifndef NULLPTR
#define NULLPTR(type) ((type*)0)
#endif

#ifndef NULLREF
#define NULLREF(type) (*(type*)0)
#endif

#ifndef MAXERRS
#define MAXERRS 100
#endif

#ifndef LITERALAPPEND
#define LITERALAPPEND(str,lit) str #lit
#endif

/**************************************************************************************************/
/*                                      Needed Include Files                                      */
/**************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <strstrea.h>
#include <iomanip.h>

/**************************************************************************************************/
/*                                       Class Definitions                                        */
/**************************************************************************************************/
class Failure
{
    typedef struct tag_ErrDesc {
// gcc don't want anonymous structures :-(
//        struct {
            unsigned int refcnt :  4;                 // reference count, if zero => element is free
            unsigned int type   :  4;                 // internal failure type flag
            unsigned int number : 24;                 // error number, distinct ranges for various types
//        };
        union {
            const char         *msg;                  // the error message
            struct tag_ErrDesc *free;                 // linked list of free error descriptions
        };
    } ErrDesc;

    class ErrJunk {
        enum { JUNK_SIZE=32 };

        ErrJunk *next;                                // next error description container
        ErrDesc  storage [JUNK_SIZE];                 // a junk of 32 error descriptions

        static ErrDesc *free;                         // linked list of free elements

    public:
        ErrJunk();                                    // link new entries to free list
       ~ErrJunk() { delete next; }                    // destroy's also all linked in containers

        ErrDesc* get ();                              // get an empty error description entry
        void     put (ErrDesc*);                      // free an error description entry
        ErrDesc* find (unsigned int, unsigned int);
        ErrDesc* find (const char*);
    };

    friend class ErrJunk;

    static ostream      *out;                         // output stream
    static const char   *pgm;                         // program name

    static ErrJunk       desc;                        // first error description container
    static ErrDesc      *proc;                        // next field belongs to this failure
    static strstream    *info;                        // generated error message
    static unsigned int  lineno;                      // line number where error occured
    static const char   *source;                      // source file where error occured

    void Process (ErrDesc *);                         // set failure currently processed

protected:
    template<class T> static T xchg (T& a, T b) { T old = a; a = b; return old; }

    // For efficiency, the one and only member variable of the various failure classes!
    ErrDesc *data;                                    // pointer to underlying description entry

    Failure (unsigned int number, unsigned int type, const char* msg = 0);

    virtual void Print (ostream&);                    // print message   
    virtual const char* Type () = 0;                  // printable failure type

public:
    virtual ~Failure ();                              // base classes should have virtual destructors

    // Classes which are thrown must allow temporary objects to be created, 
    // therefore we need a copy constructor and an assignment operator.
    // Since there will be at least one copy of a thrown object created,
    // the copy constructor and assignment operator should be as efficient
    // as it could be.
    Failure (const Failure&);                         // copy constructor
    Failure& operator= (const Failure&);              // assignment

    strstream&   Info ();                             // error information is appended to this stream

    static ostream&     Connect (ostream&);           // change output stream
    static const char*  ContactString (const char*);  // change contact string
    static const char*  ProgramName (const char*);    // change program name

    const char*  SourceFile (const char*);            // set current source file name
    unsigned int SourceLine (unsigned int);           // set current source line

    struct Complain {                                 // simulate a prefix operator
        Complain() {}
        void operator, (Failure& Fault) { Fault.Handle(); }
    };
    #define complain Failure::Complain(), 

protected:
    Failure& operator() (unsigned int);               // set source line
    Failure& operator() (const char*);                // set source file
    Failure& operator() (unsigned int, const char*);  // set source line and source file
    Failure& operator() (const char*, unsigned int);  // set soufce file and source line

    static const char* Lit1;                          // ": "
    static const char* Lit2;                          // " #"
    static const char* Lit3;                          // "> "
    static const char* Lit4;                          // ", "
    static const char* Line;                          // "Line "
    static const char* File;                          // "File "
    static const char* Aborting;                      // "Aborting..."
    static const char* Contact;

    enum { FAILURE, WARNING, ERROR, INTERNAL };       // failure type's

    virtual void Handle ();                           // common error handling
    friend struct Complain;                           // grant accass to protected Handle() function
};

class Warning : public Failure
{
    static unsigned int count;                        // warnings counter
    static unsigned int ilast;                        // last warning number

protected:
    virtual void Print (ostream&);                    // print warning message

public:
    Warning (unsigned int);                           // look for an existing warning
    Warning (const char*);                            // construct with automatic warning number
    Warning (unsigned int, const char*);              // construct with explicit warning number
    Warning (const char*, unsigned int);              // same like above

    Warning (const Warning& copy) : Failure(copy) {}
    Warning& operator= (const Warning& copy) { Failure::operator= (copy); return *this; }

    Warning& operator() (unsigned int n)                { Failure::operator()(n);   return *this; }
    Warning& operator() (const char* s)                 { Failure::operator()(s);   return *this; }
    Warning& operator() (unsigned int n, const char* s) { Failure::operator()(n,s); return *this; }
    Warning& operator() (const char* s, unsigned int n) { Failure::operator()(s,n); return *this; }

    virtual void Handle ();                           // handle warning
    virtual const char* Type ();                      // failure type: warning

    static unsigned int SetNext (unsigned int);       // set next warning number
    static unsigned int Count ();                     // get warnings counter
    static void Reset ();                             // reset warnings counter
};

class Error : public Failure
{
    static unsigned int ilast;                        // last error number

protected:
    static unsigned int count;                        // error counter

    virtual void Print (ostream&);                    // print error

public:
    Error (unsigned int);                             // look for an existing error
    Error (const char*);                              // construct with automatic error number
    Error (unsigned int, const char*);                // construct with explicit error number
    Error (const char*, unsigned int);                // same like above

    Error (const Error& copy) : Failure(copy) {}
    Error& operator= (const Error& copy) { Failure::operator= (copy); return *this; }

    Error& operator() (unsigned int n)                { Failure::operator()(n);   return *this; }
    Error& operator() (const char* s)                 { Failure::operator()(s);   return *this; }
    Error& operator() (unsigned int n, const char* s) { Failure::operator()(n,s); return *this; }
    Error& operator() (const char* s, unsigned int n) { Failure::operator()(s,n); return *this; }

    virtual void Handle ();                           // handle error
    virtual const char* Type ();                      // failure type: error

    static unsigned int SetNext (unsigned int);       // set next error number
    static unsigned int Count ();                     // get error counter
    static void Reset ();                             // reset error counter
};

class FatalError : public Error
{
protected:
    virtual void Print (ostream&);                    // print fatal error

public:
    FatalError (unsigned int);                        // look for an existing error
    FatalError (const char*);
    FatalError (unsigned int, const char*);
    FatalError (const char*, unsigned int);

    FatalError& operator() (unsigned int n)                { Failure::operator()(n);   return *this; }
    FatalError& operator() (const char* s)                 { Failure::operator()(s);   return *this; }
    FatalError& operator() (unsigned int n, const char* s) { Failure::operator()(n,s); return *this; }
    FatalError& operator() (const char* s, unsigned int n) { Failure::operator()(s,n); return *this; }

    FatalError (const FatalError& copy) : Error(copy) {}
    FatalError& operator= (const FatalError& copy) { Error::operator= (copy); return *this; }

    virtual void Handle ();                           // handle error, never returns
};

class InternalError : public Failure
{
    static unsigned int ilast;                        // last error number

protected:
    virtual void Print (ostream&);                    // print internal error

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

    virtual void Handle ();                           // handle error, never returns
    virtual const char* Type ();                      // failure type: internal error

    static unsigned int SetNext (unsigned int);       // set next error number
};


/**************************************************************************************************/
/*               Inline Functions, Forwarded Function Definitions, Template Functions             */
/**************************************************************************************************/
extern ostream& terminate (ostream&);                 // forward definition of manipulator

template<class T>
inline Failure& operator<< (Failure& Obj, T& t)       // capture all stream put operators
{
    Obj.Info() << t << terminate;                     // Info returns a string stream
    return Obj;
}

template<class T>
inline Warning& operator<< (Warning& Obj, T& t)       // capture all stream put operators
{
    Obj.Info() << t << terminate;                     // Info returns a string stream
    return Obj;
}

template<class T>
inline Error& operator<< (Error& Obj, T& t)           // capture all stream put operators
{
    Obj.Info() << t << terminate;
    return Obj;
}

template<class T>
inline FatalError& operator<< (FatalError& Obj, T& t) // capture all stream put operators
{
    Obj.Info() << t << terminate;
    return Obj;
}

template<class T>
inline InternalError& operator<< (InternalError& Obj, T& t) // capture all stream put operators
{
    Obj.Info() << t << terminate;
    return Obj;
}


inline Failure::Failure (const Failure &copy)
{
    data = copy.data;
    data->refcnt++;
}

inline Failure& Failure::operator= (const Failure &copy)
{
    this->~Failure();
    data = copy.data;
    data->refcnt++;
    return *this;
}

inline ostream& Failure::Connect (ostream& os)
{
    ostream* old = out; out = &os; return *old;
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


inline void Failure::ErrJunk::put (Failure::ErrDesc* d)
{
    d->free = free, free = d;
}


inline Warning::Warning (unsigned int num)
    : Failure (num, WARNING)
{}

inline Warning::Warning (const char* str)
    : Failure (++ilast, WARNING, str)
{}

inline Warning::Warning (unsigned int num, const char* str)
    : Failure (num, WARNING, str)
{
    if (num > ilast) ilast = num;                     // memorize highest warning initialized
}

inline Warning::Warning (const char* str, unsigned int num)
    : Failure (num, WARNING, str)
{
    if (num > ilast) ilast = num;                     // memorize highest warning initialized
}

inline const char* Warning::Type ()
{
    return "Warning";                                 // name of this failure type
}

inline unsigned int Warning::SetNext (unsigned int i)
{
    return xchg (ilast, i);
}

inline unsigned int Warning::Count ()
{
    return count;                                     // return number of warnings encountered
}

inline void Warning::Reset ()
{
    count = 0;                                        // reset warning counter
}


inline Error::Error (unsigned int num)
    : Failure (num, ERROR)
{}

inline Error::Error (const char* str)
    : Failure (++ilast, ERROR, str)
{}

inline Error::Error (unsigned int num, const char* str)
    : Failure (num, ERROR, str)
{
    if (num > ilast) ilast = num;                     // memorize highest error initialized
}

inline Error::Error (const char* str, unsigned int num)
    : Failure (num, ERROR, str)
{
    if (num > ilast) ilast = num;                     // memorize highest error initialized
}

inline const char* Error::Type ()
{
    return "Error";                                   // name of this failure type
}

inline unsigned int Error::SetNext (unsigned int i)
{
    return xchg (ilast, i);
}

inline unsigned int Error::Count ()
{
    return count;                                     // return number of errors encountered
}

inline void Error::Reset ()
{
    count = 0;                                        // reset error counter
}


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


inline InternalError::InternalError (unsigned int num)
    : Failure (num, INTERNAL)
{}

inline InternalError::InternalError (const char* str)
    : Failure (++ilast, INTERNAL, str)
{}

inline InternalError::InternalError (unsigned int num, const char* str)
    : Failure (num, INTERNAL, str)
{
    if (num > ilast) ilast = num;                     // memorize highest error initialized
}

inline InternalError::InternalError (const char* str, unsigned int num)
    : Failure (num, INTERNAL, str)
{
    if (num > ilast) ilast = num;                     // memorize highest error initialized
}

inline const char* InternalError::Type ()
{
    return "Internal Error";                          // name of this failure type
}

inline unsigned int InternalError::SetNext (unsigned int i)
{
    return xchg (ilast, i);
}


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
ostream*     Failure::out    = &cerr;                 // output connected to standard error stream
strstream*   Failure::info   = 0;                     // no string stream during program startup
const char*  Failure::pgm    = 0;                     // no standard program name
unsigned int Failure::lineno = 0;                     // source line of processed error
const char*  Failure::source = 0;                     // source file of processed error
Failure::ErrJunk  Failure::desc;                      // first error description container
Failure::ErrDesc* Failure::proc = 0;                  // currently process error message
Failure::ErrDesc* Failure::ErrJunk::free = 0;

unsigned int Warning::count = 0;                      // number of warnings encountered
unsigned int Warning::ilast = 0;                      // last initialized warning

unsigned int Error::count = 0;                        // number of errors encountered
unsigned int Error::ilast = 0;                        // last initialized error

unsigned int InternalError::ilast = 0;                // last initialized internal error

const char* Failure::Lit1 = ": ";                     // common string literal
const char* Failure::Lit2 = " #";                     // - " -
const char* Failure::Lit3 = "> ";                     // - " -
const char* Failure::Lit4 = ", ";                     // - " -
const char* Failure::Line = "Line ";                  // - " -
const char* Failure::File = "File ";                  // - " -
const char* Failure::Aborting  = "Aborting...";       // - " -
const char* Failure::Contact   = "    Please contact your vendor of this software\n"
                                 "    and supply him with the above information.";

FatalError ErrorCountExceeded (0, "Error count exceeds %1");
                         


/**************************************************************************************************/
/*                                           Functions                                            */
/**************************************************************************************************/
ostream& terminate (ostream& os)                      // new stream manipulator
{
    os << ends << ends;                               // write end of string to stream
    os.seekp (-1, ios::cur);                          // and reposition stream write pointer
    return os;   
}

Failure::Failure (unsigned int n, unsigned int t, const char* m)
{
    data = desc.find (n, t);                          // search for an existing error description
    if (data)                                         // if it exists
    { 
        data->refcnt++;                               // increment the reference count
        if (m)
            data->msg = m;                            // overwrite old error message with new one
    }
    else
    {
        data = desc.get ();                           // create a new error description entry
        if (data)                                     // check if it went ok
        {
            data->refcnt = 1;                         // set reference count
            data->number = n;                         // set error number
            data->type   = t;                         // set error type
            data->msg    = m;                         // set error message
        }
    }
}

Failure::~Failure ()
{
    if (data && --data->refcnt == 0)
    {
        desc.put (data);
        data = 0;
    }
}

void Failure::Process (ErrDesc* curr)
{
    if (proc != curr)
    {
        delete info;                                  // must delete wrong failure information
        info   = 0;                                   // no current info
        proc   = curr;                                // this failure is processed
        lineno = 0;                                   // no line number information
        source = 0;                                   // no source line        
    }
}

strstream& Failure::Info ()
{
    Process (data);                                   // set failure currently processed
    if (!info) info = new strstream;                  // be shure, string stream is initialised
    return *info;
}

void Failure::Handle ()
{
    Print (*out);                                     // print failure message
    Process (0);                                      // delete internal string stream
}

void Failure::Print (ostream& out)
{
    int    argc = 0;                                  // number of strings in argv
    char** argv = 0;                                  // pointer to additional error informations
    int    argh = 0;                                  // highest argument displayed
    int    i;

    if (info)
    {
        char* s;
        char* args = info->str ();                    // fix message string
        
        for (s=args; s && s[0]; argc++)
            s = s + strlen(s) + 1;
        if (argc)
        {
            argv = new char*[argc];
            if (argv)
            {
                for (i=0, s=args; i<argc; i++)
                    argv[i] = s, s = s + strlen(s) + 1;
            }
            else
            {
                argc = 0;
            }
        }
    }

    static const char* string  = "%s";
    static const char* integer = "%d";
    char   buf [2048];
    char*  msg = buf;

    if (pgm)
    {
        msg += sprintf (msg, string, pgm);
        msg += sprintf (msg, string, Lit3);
    }
    if (source && lineno)
    {
        msg += sprintf (msg, string, File);
        msg += sprintf (msg, string, source);
        msg += sprintf (msg, string, Lit4);
        msg += sprintf (msg, string, Line);
        msg += sprintf (msg, integer, lineno);
        msg += sprintf (msg, string, Lit1);
    }
    else if (source)
    {
        msg += sprintf (msg, string, File);
        msg += sprintf (msg, string, source);
        msg += sprintf (msg, string, Lit1);
    }
    else if (lineno)
    {
        msg += sprintf (msg, string, Line);
        msg += sprintf (msg, integer, lineno);
        msg += sprintf (msg, string, Lit1);
    }

    if (data->number)
    {
        msg += sprintf (msg, string, Type());
        msg += sprintf (msg, string, Lit2);
        msg += sprintf (msg, integer, data->number);
        msg += sprintf (msg, string, Lit1);
    }

    for (const char* s = data->msg; s && s[0]; s++)
    {
        if (s[0] == '%')
        {
            s++;
            if (s[0] == '%')
            {
                *msg++ = '%';
            }
            else
            {
                i = 0;
                while ('0' <= s[0] && s[0] <= '9')
                    i = 10*i + *(s++) - '0';
                if (i)
                    s--;
                if (i > 0 && i <= argc)
                    msg += sprintf(msg, string, argv[i-1]);
                if (argh < i)
                    argh = i;
            }
        }
        else
        {
            *msg++ = *s;
        }
    }

    *msg++ = 0;

    if (info)
    {
        delete[] argv;
        info -> rdbuf() -> freeze (0);                // release message string
    }

    out << buf << flush;
}


Failure::ErrJunk::ErrJunk ()
{
    for (int i=0; i<JUNK_SIZE; i++)
    {
        storage[i].refcnt = 0;
        storage[i].free = free;
        free = &storage[i];
    }
}

Failure::ErrDesc* Failure::ErrJunk::get ()
{
    Failure::ErrDesc *d = 0;
    if (!free)
    {
        ErrJunk *n = new ErrJunk;
        if (n) n->next = next, next = n;
    }
    if (free) d = free, free = d->free;
    return d;
}

Failure::ErrDesc* Failure::ErrJunk::find (unsigned int num, unsigned int type)
{
    for (Failure::ErrJunk* j = this; j; j = j->next)
        for (int i=0; i<JUNK_SIZE; i++)
            if (j->storage[i].refcnt && 
                j->storage[i].number == num &&
                j->storage[i].type == type)
                return &j->storage[i];
    return 0;
}

Failure::ErrDesc* Failure::ErrJunk::find (const char* msg)
{
    for (Failure::ErrJunk* j = this; j; j = j->next)
        for (int i=0; i<JUNK_SIZE; i++)
            if (j->storage[i].refcnt &&
                strcmp (j->storage[i].msg, msg) == 0)
                return &j->storage[i];
    return 0;
}


void Warning::Handle ()
{
    count++;                                          // increment warnings encountered
    Failure::Handle ();                               // standard handle procedure
}

void Warning::Print (ostream& out)
{
    Failure::Print (out);                             // standard message print
    out << '.' << endl;                               // terminate the message
}


void Error::Handle ()
{
    count++;                                          // increment errors encountered
    Failure::Handle ();                               // standard handle procedure
    if (count >= MAXERRS)                             // To many errors?
        complain ErrorCountExceeded << count;         // turn this error into a fatal error
}

void Error::Print (ostream& out)
{
    Failure::Print (out);                             // standard message print
    out << '.' << endl;                               // terminate the message
}
    

void FatalError::Handle ()
{
    count++;                                          // increment errors encountered
    Failure::Handle ();                               // standard handle procedure
    exit (data->number);                              // Bye!
}

void FatalError::Print (ostream& out)
{
    Failure::Print (out);                             // standard message print
    out << '!' << endl;                               // terminate the message
    out << Aborting << endl;                          // tell user that we are terminating
}


void InternalError::Handle ()
{
    Failure::Handle ();                               // standard handle procedure
    exit (data->number);                              // Bye!
}

void InternalError::Print (ostream& out)
{
    Failure::Print (out);                             // standard message print
    out << '!' << endl;                               // terminate the message
    out << Contact << endl;                           // tell user what to do
    out << Aborting << endl;                          // tell user that we are terminating
}

#endif /* MAIN */
#endif /* ERROR_INCLUDED */
