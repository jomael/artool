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
/*  04-Feb-1997 BAR 2.0   New implementation in C++.                                              */
/*  06-May-1999 BAR 2.1   Reviewed: new handling of input and output stream initialization,       */
/*                                  checked Unix compatibility.                                   */
/*  07-May-1999 BAR 2.1a  Pipe classes added. These classes allows one, to attach any program     */
/*                        input or output to stdin, stdout, stderr or any other file from the     */
/*                        commandline. There's also support for the TAP shell to redirect any     */
/*                        of this streams to another program.                                     */
/*  29-Sep-2000 BAR 2.1b  Timestemp in pidmsk added                                               */
/**************************************************************************************************/
/*  Description:                                                                                  */
/*                                                                                                */
/*                                                                                                */
/*                                                                                                */
/*                                                                                                */
/*                                                                                                */
/**************************************************************************************************/
#ifndef COMMON_INCLUDED
#define COMMON_INCLUDED


/**************************************************************************************************/
/*                                      Needed Include Files                                      */
/**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <strstrea.h>


#include <math.h>
#include <time.h>
#include <limits.h>
#include <fcntl.h>
#include <ctype.h>
#include <new.h>
#include <malloc.h>
#include <time.h>

namespace tap {
#include "error_vs6.hpp"
#include "autoinit.hpp"
}

using namespace tap;

#if defined(_WIN32)
   #include <io.h>
   #include <process.h>
#else  /* POSIX */
   #include <unistd.h>
#endif /* _WIN32 */


/**************************************************************************************************/
/*                                          Definitions                                           */
/**************************************************************************************************/
#define SHELLMARK    "tapsh:"
#define TMPSTRBUFLEN 256
#define FLUSHSIZE    8192

#ifndef FALSE
   #define FALSE 0
#endif
#ifndef TRUE
   #define TRUE  1
#endif

#ifndef LN10 
   #define LN10      2.3025850929940456840179915
#endif /* LN10 */
#ifndef PI
   #define PI        3.1415926535897932384626433
#endif /* PI */

#define EPS       1.0E-10
#define MAXRAND2  16383.5
#define MAXALAW   4096
#define MAXULAW   8192

#ifndef _INTEGRAL_MAX_BITS
   #define _INTEGRAL_MAX_BITS 64
#endif /* _INTEGRAL_MAX_BITS */

typedef unsigned int uint;


/**************************************************************************************************/
/*                                             Macros                                             */
/**************************************************************************************************/
#define odd(x)     ((x)&1)
#define even(x)    (!((x)&1))

#ifdef _WIN32
  #define ALLOCA _alloca
#else
  #define ALLOCA alloca
#endif

class __ln_t;
inline void* operator new (size_t s, __ln_t* p) { return p ? (void*)p : (void*)s; }
#define localnew(expr) new((__ln_t*)ALLOCA((size_t)new((__ln_t*)0) expr)) expr

template<class T> T xchg(T& a, T& b) { T c = a; a = b; return c; }

#ifdef _WIN32
   #define O_BINARY _O_BINARY
   #define O_TEXT   _O_TEXT
   #define NEW_HANDLER_2
   inline void   sleep (int sec)                   { _sleep (sec); }
// inline int    read (int f, void* p, unsigned l) { return _read (f, p, l); }
   inline int    isatty (int handle)               { return _isatty (handle); }
   inline int    setmode (int fileno, int mode)    { return _setmode (fileno, mode); }
   inline int    fileno (FILE *stream)             { return _fileno (stream); }
#else
   #define O_BINARY 0
   #define O_TEXT   0
   inline int    setmode (int fileno, int mode)    { return 0; }
#endif /* _WIN32 */

   inline int    fileno (const fstream& f)         { return f.rdbuf()->fd(); }
   inline int    fileno (const ifstream& f)        { return f.rdbuf()->fd(); }
   inline int    fileno (const ofstream& f)        { return f.rdbuf()->fd(); }
   

/**************************************************************************************************/
/*                                       Class Definitions                                        */
/**************************************************************************************************/
class ShellInfoParser
   : public AutoPgmInit                               // automatically init this at pgm startup
{
   class PgmHandleList                                // container for program handle table
   {
      int  sio;                                       // standard value given back
      int  len;                                       // length of handle table
      int *tab;                                       // pointer to begin of handle table
   protected:
     ~PgmHandleList() { delete[] tab; }               // remove handle list
      PgmHandleList (int s=-1) { sio=s,len=0,tab=0; } // initialize with this standard value
      int parse (char*);                              // parse the handle list
      friend ShellInfoParser;                         // this class may access the functions above
   public:   
      int operator[] (int i)                          // return a value of array
         { return (len==0) ? sio : ((i<0)||(i>=len)) ? -1 : tab[i]; }
      int count() { return len ? len : sio+1; }       // size of Array
   };

   int auto_init (int, char**, char**);               // parse command line parameters

public:
   ShellInfoParser();

   int   pid;                                         // process id 
   char* name;                                        // program name
   int   shpid;                                       // process id of shell 
   char* shell;                                       // shell program name
   PgmHandleList in;                                  // table of inherited input pipe handles
   PgmHandleList out;                                 // table of inherited output pipe handles
   PgmHandleList io;                                  // table of inherited io file handles
};

extern ShellInfoParser pgm;                           // forwarded static object

class ibinary : public ifstream {                     // binary input stream
   int swapping;
   
   void init() { swapping = 0; }
   
public:
   ibinary () { init(); }
   ibinary (int);                                     // attach to an existing file handle
   
   // open new file
   ibinary (char* s)         : ifstream (s, ios::binary | ios::in)     { init(); }
   ibinary (char* s, long l) : ifstream (s, ios::binary | ios::in | l) { init(); }
   
   void open (char* s)         { ifstream::open (s, ios::binary | ios::in);     init(); }
   void open (char* s, long l) { ifstream::open (s, ios::binary | ios::in | l); init(); }
   
   void attach (int);                                 // attach to an existing file handle
   
   int swapmode () { return swapping; }               // get file swapping mode
   int swapmode (int m) { return xchg(swapping,m); }  // switch byte swapping mode on or off
};

class obinary : public ofstream {                     // binary output stream
   int swapping;
   int written;
   int flushpos;
   
   void init() { swapping = 0, flushpos = 0, written = 0; }
   
public:
   obinary () { init(); }
   obinary (int);                                     // attach to an existing file handle

   // open new file
   obinary (char* s)         : ofstream (s, ios::binary | ios::out)     { init(); }
   obinary (char* s, long l) : ofstream (s, ios::binary | ios::out | l) { init(); }

   void open (char* s)         { ofstream::open (s, ios::binary | ios::out);     init(); }
   void open (char* s, long l) { ofstream::open (s, ios::binary | ios::out | l); init(); }
   
   void attach (int);                                 // attach to an existing file handle

   int swapmode () { return swapping; }               // get file swapping mode
   int swapmode (int m) { return xchg(swapping,m); }  // switch byte swapping mode on or off
   int needflush (int);                               // flush the output file?
};

class iobinary : public fstream {                     // binary input and output stream
   int swapping;
   int written;
   int flushpos;
   
   void init() { swapping = 0, flushpos = 0, written = 0; }
   
public:
   iobinary () { init(); }
   iobinary (int);                                    // attach to an existing file handle

   // open new file
   iobinary (char* s)         : fstream (s, ios::binary | ios::in | ios::out)      { init(); }
   iobinary (char* s, long l) : fstream (s, ios::binary | ios::in | ios::out | l)  { init(); }

   void open (char* s)         { fstream::open (s, ios::binary | ios::in | ios::out);     init(); }
   void open (char* s, long l) { fstream::open (s, ios::binary | ios::in | ios::out | l); init(); }
   
   void attach (int);                                 // attach to an existing file handle

   int swapmode () { return swapping; }               // get file swapping mode
   int swapmode (int m) { return xchg(swapping,m); }  // switch byte swapping mode on or off
   int needflush (int);                               // flush the output file?
};

// -------------------------------------------------------------------------------------------------

class pipe 
{
protected:
   int   index;                                       // index of stream in input pipe table
   char* name;                                        // name of file to open
   long  flags;                                       // opening flags of file

  ~pipe() { delete[] name; }
   pipe() { index = -1; name = 0; flags = 0; }        // init member vars
   void parse (char*);                                // parse name to resolve where to attach a stream

public:   
   long open_mode (long f) { return xchg(flags,f); }  // set open mode flags
   friend istream& operator>> (istream& i, pipe& p)
      { char s[256]; i >> s; p.parse(s); return i; }
   friend ostream& operator<< (ostream& o, const pipe& p) { return o << p.name; }
};

class ipipe : public pipe                             // info about an input stream into program
{
public:
   ipipe() {}                                         // construct empty
   ipipe(char* s) { parse(s); }                       // construct with name parsing 

   ipipe& operator= (char* s)                         // parse name
      { parse(s); return *this; }

   ipipe& operator() (long f) { open_mode(f); return *this; }
   
   template<class S> void attach (S& s)               // attach this stream to file or pipe specified
   {
//    if (0) ifstream i = s;                          // compile-time check of type
      if (index >= 0) {                               // index of file handle parsed
         int h = pgm.in[index];                       // get file handle
         if (h >= 0) s.attach (h);                    // if valid file handle attach the stream
      } else if (name) {                              // a name was parsed
         s.open (name, flags | ios::in | ios::nocreate);
         if (!s) throw OpErr << name;
      } /* endif */
   }
};

class opipe : public pipe                             // info about an output stream of program
{
public:
   opipe() {}                                         // construct empty
   opipe(char* s) { parse(s); }                       // construct with name parsing
   
   opipe& operator= (char* s)                         // parse name
      { parse(s); return *this; }
   
   opipe& operator() (long f) { open_mode(f); return *this; }

   template<class S> void attach (S& s)               // attach this stream to file or pipe specified
   {
//    if (0) ofstream o = s;                          // compile-time check of type
      if (index >= 0) {                               // index of file handle parsed
         int h = pgm.out[index];                      // get file handle
         if (h >= 0) s.attach (h);                    // if valid file handle attach the stream
      } else if (name) {                              // a name was parsed
         s.open (name, flags | ios::out);
         if (!s) throw OpErr << name;
      } /* endif */
   }
};

class iopipe : public pipe                            // info about an iostream of program
{
   int   index;                                       // index of stream in iofile table
   char* name;                                        // name of file to open
   long  flags;                                       // opening flags of file

public:
   iopipe() {}                                        // construct empty
   iopipe(char* s) { parse(s); }                      // construct with name parsing
   
   iopipe& operator= (char* s)                        // parse name
      { parse(s); return *this; }
   
   iopipe& operator() (long f) { open_mode(f); return *this; }

   template<class S> void attach (S& s)               // attach this stream to file specified
   {
//    if (0) fstream f = s;                           // compile-time check of type
      if (index >= 0) {                               // index of file handle parsed
         int h = pgm.io[index];                       // get file handle
         if (h >= 0) s.attach (h);                    // if valid file handle attach the stream
      } else if (name) {                              // a name was parsed
         s.open (name, flags | ios::in | ios::out);
         if (!s) throw OpErr << name;
      } /* endif */
   }
};

// -------------------------------------------------------------------------------------------------

class s2stream : public strstream {
   char* str;
   
public:
   s2stream (char* s) : strstream (s, INT_MAX, ios::in | ios::out) { str = s; }
   s2stream (char* s, size_t l) : strstream (s, l, ios::in | ios::out) { str = s; }
   operator char* () { return str; }
};

// -------------------------------------------------------------------------------------------------

class Statistic : public strstream {                  // generate statistics
   time_t    start;                                   // initialization time of this class
   clock_t   cputime;                                 // cpu time counter
   char*     oname;                                   // write statistics to this file
   ostream*  pout;                                    // write statistics to this stream pointer
   
public:
  ~Statistic ();                                      // generate statistical information
   Statistic ()             { oname = 0; pout = 0; time (&start); cputime = clock (); }
   
   void attach (char* n)    { oname = n; }            // attach statistics to this file
   void attach (ostream& o) { pout = &o; }            // attach statistics to this stream
}; /* Statistic */

// -------------------------------------------------------------------------------------------------

class ascii {
   size_t   outw;
   unsigned base;
   char* p;
   char  buffer [_INTEGRAL_MAX_BITS+2];

   char* utoa (unsigned long);
   char* ltoa (signed long);

public:
   ascii () { buffer[0] = 0;  p = 0;  outw = 0, base = 10; }
   ascii (unsigned long  z, size_t w=0, unsigned b=10) { outw = w, base = b; utoa (z); }
   ascii (unsigned int   z, size_t w=0, unsigned b=10) { outw = w, base = b; utoa (z); }
   ascii (unsigned short z, size_t w=0, unsigned b=10) { outw = w, base = b; utoa (z); }
   ascii (unsigned char  z, size_t w=0, unsigned b=10) { outw = w, base = b; utoa (z); }
   ascii (char           z, size_t w=0, unsigned b=10) { outw = w, base = b; utoa (z); }
   
   ascii (signed long  z, size_t w=0, unsigned b=10) { outw = w, base = b; ltoa (z); }
   ascii (signed int   z, size_t w=0, unsigned b=10) { outw = w, base = b; ltoa (z); }
   ascii (signed short z, size_t w=0, unsigned b=10) { outw = w, base = b; ltoa (z); }
   ascii (signed char  z, size_t w=0, unsigned b=10) { outw = w, base = b; ltoa (z); }

   ascii& operator= (unsigned long  z) { utoa (z);  return *this; }
   ascii& operator= (unsigned int   z) { utoa (z);  return *this; }
   ascii& operator= (unsigned short z) { utoa (z);  return *this; }
   ascii& operator= (unsigned char  z) { utoa (z);  return *this; }
   ascii& operator= (char           z) { utoa (z);  return *this; }

   ascii& operator= (signed long  z)   { ltoa (z);  return *this; }
   ascii& operator= (signed int   z)   { ltoa (z);  return *this; }
   ascii& operator= (signed short z)   { ltoa (z);  return *this; }
   ascii& operator= (signed char  z)   { ltoa (z);  return *this; }

   size_t wide (size_t w) {
      size_t o = outw;  outw = (w>_INTEGRAL_MAX_BITS) ? _INTEGRAL_MAX_BITS : w;  return o;
   } /* wide */

   unsigned radix (unsigned b) { 
      unsigned o = base;  base = b;  return o;
   } /* radix */

   operator const char* () const { return p; }
}; /* ascii */

class value {
   enum TYPE  { UL, UI, US, UC, CH, SL, SI, SS, SC };
   enum FLAGS { POS, NEG, ERR };

   void*         pval;
   unsigned int  base;
   unsigned long ovn, ovr;
   TYPE          type;
   unsigned int  read;
   unsigned int  flags;
   unsigned long val;

   void init (void*, unsigned, TYPE);

public:
   value (unsigned long  &z, unsigned b=10) { init ((void*) &z, b, UL); }
   value (unsigned int   &z, unsigned b=10) { init ((void*) &z, b, UI); }
   value (unsigned short &z, unsigned b=10) { init ((void*) &z, b, US); }
   value (unsigned char  &z, unsigned b=10) { init ((void*) &z, b, UC); }
   value (char           &z, unsigned b=10) { init ((void*) &z, b, CH); }
   
   value (signed long  &z, unsigned b=10) { init ((void*) &z, b, SL); }
   value (signed int   &z, unsigned b=10) { init ((void*) &z, b, SI); }
   value (signed short &z, unsigned b=10) { init ((void*) &z, b, SS); }
   value (signed char  &z, unsigned b=10) { init ((void*) &z, b, SC); }

   value& operator= (const char*);
   operator value* () { return this; }

private:
   void convinit ();
   int  accept (int);
   int  convdone (); 

   friend istream& operator>> (istream&, value*);
}; /* value */

// -------------------------------------------------------------------------------------------------

class pidmsk {
   char* s;

public:
  ~pidmsk ()                     { if (s) delete[] s; }
   pidmsk (const char*);

   operator char* ()             { return s; }
   operator const char* () const { return s; }

   pidmsk& operator= (const pidmsk&);
}; /* pidmsk */

// -------------------------------------------------------------------------------------------------
// -- The following is for internal use only

class NewFailed : public FatalError {
#ifdef NEW_HANDLER_2
   int (*old)(size_t);
#else
   void (*old)();
#endif

public:
  ~NewFailed ();
   NewFailed ();
}; /* NewFailed */

// -------------------------------------------------------------------------------------------------

struct InitRandom {
   InitRandom () 
   {
      time_t start;
      time (&start);
      srand (start);
   }
}; /* InitRandom */


/**************************************************************************************************/
/*                                    Forwarded Static Objects                                    */
/**************************************************************************************************/
extern FatalError RdErr;
extern FatalError WrErr;
extern FatalError OpErr;
extern FatalError EofErr;
extern FatalError SysErr;


/**************************************************************************************************/
/*                                Forwarded (Function) Definitions                                */
/**************************************************************************************************/
#ifdef NEW_HANDLER_2
extern int    MemErrorHandler (size_t);
#else
extern void   MemErrorHandler ();
#endif /* NEW_HANDLER_2 */

extern char*  ppstring (char*);                       // preprocess strings
extern double interpolate (const double*, const double*, int, double);
extern double db (double, double);
extern double read_result (const char*);
extern double adjust_frequency (double& freq, double rate, int len, int& adjust);


/**************************************************************************************************/
/*                              Inline Functions, Template Functions                              */
/**************************************************************************************************/
inline int OpenCheck (ios& io) 
{
   if (!io) throw OpErr;
   return 1;
}

inline int OpenCheck (ios& io, const char* n)
{
   if (!io) throw OpErr << n;
   return 1;
}

inline int EofCheck (istream& i)
{
   if (i.eof ()) return 1;
   if (!i) throw RdErr;
   return 0;
}

inline int EofCheck (istream& i, const char* n)
{
   if (i.eof ()) return 1;
   if (!i) throw RdErr << n;
   return 0;
}

inline int Checked (istream& i)
{
   if (i.eof ()) throw EofErr;
   if (!i) throw RdErr;
   return 1;
}

inline int Checked (istream& i, const char* n)
{
   if (i.eof ()) throw EofErr << n;
   if (!i) throw RdErr << n;
   return 1;
}

inline int Checked (ostream& o)
{
   if (o.eof ()) throw EofErr;
   if (!o) throw WrErr;
   return 1;
}

inline int Checked (ostream& o, const char* n)
{
   if (o.eof ()) throw EofErr << n;
   if (!o) throw WrErr << n;
   return 1;
}

inline int SYSTEM (const char* cmd)
{
   if (system (cmd)) {
      throw SysErr << cmd;
   } /* endif */
   return 0;
}

// -------------------------------------------------------------------------------------------------

inline void bswap (char* dst, char* src, int n)
{
   memcpy (dst, src, n);
   for (src = dst, dst += n-1, n/=2; n; n--, src++, dst--) {
      char c = *src; *src = *dst; *dst = c;
   } /* endfor */
}

inline double rint (double x)
{
   return floor (x+0.5);
}

inline double rint (double x, double r)
{
   return floor ((x+0.5)/r) * r;
}

inline unsigned _bittst (unsigned val, unsigned pos) {
   return 0 != (val & (1 << pos));
} /* _bittst */

inline unsigned _bitset (unsigned val, unsigned pos) {
   return val | (1 << pos);
} /* _bitset */

inline unsigned _bitclr (unsigned val, unsigned pos) {
   return val & ~(1 << pos);
} /* _bitclr */

inline unsigned _bittgl (unsigned val, unsigned pos) {
   return val ^ (1 << pos);
} /* _bittgl */

inline unsigned _lsb (unsigned val) 
{
   int i, m;
   for (i=0, m=1; i<32; i++, m<<=1) if (val & m) break;
   return i;
} /* _lsb */

inline unsigned _msb (unsigned val) {
   int i, m;
   for (i=31, m=0x80000000; i>=0; i--, m>>=1) if (val & m) break;
   return i;
} /* _msb */

inline unsigned _bit (unsigned val) 
{
   return 1 << val;
} /* _bit */

template<class T>
   inline unsigned _bitcnt (T& val)
   {
      static unsigned char _bctbl [256] =
      {
         0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
         1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
         1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
         2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
         1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
         2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
         2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
         3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
         1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
         2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
         2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
         3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
         2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
         3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
         3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
         4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
      };
      unsigned n, i;

      for (n = 0, i = 0; i < sizeof (T); i++)
         n += _bctbl [((unsigned char*)&val)[i]];
      return n;
   } /* _bitcnt */

// -------------------------------------------------------------------------------------------------

inline NewFailed::NewFailed ()
   : FatalError ("memory allocation failed")
{
#ifdef NEW_HANDLER_2   
   old = _set_new_handler (MemErrorHandler);
   _set_new_mode (1);
#else
   old = set_new_handler (MemErrorHandler);
#endif /* NEW_HANDLER_2 */
}

inline NewFailed::~NewFailed ()
{
#ifdef NEW_HANDLER_2   
   _set_new_handler (old);
#else
   set_new_handler (old);
#endif /* NEW_HANDLER_2 */
}

// -------------------------------------------------------------------------------------------------

inline ibinary::ibinary (int h)
   : ifstream (h)
{
   init();
   ::setmode (h, O_BINARY);                           // turn translation off
}

inline void ibinary::attach (int h)
{
   ::setmode (h, O_BINARY);                           // turn translation off
   ifstream::attach (h);
}

inline obinary::obinary (int h)
   : ofstream (h)
{
   init();
   ::setmode (h, O_BINARY);                           // turn translation off
}

inline void obinary::attach (int h)
{
   ::setmode (h, O_BINARY);                           // turn translation off
   ofstream::attach (h);
}

inline iobinary::iobinary (int h)
   : fstream (h)
{
   init();
   ::setmode (h, O_BINARY);                           // turn translation off
}

inline void iobinary::attach (int h)
{
   ::setmode (h, O_BINARY);                           // turn translation off
   fstream::attach (h);
}

inline int obinary::needflush (int l)
{
   written += l;
   if (written >= flushpos + FLUSHSIZE) {
      flushpos = written;
      return 1;      
   } /* endif */
   return 0;
}

inline int iobinary::needflush (int l)
{
   written += l;
   if (written >= flushpos + FLUSHSIZE) {
      flushpos = written;
      return 1;      
   } /* endif */
   return 0;
}

template<class T> 
   ibinary& operator>> (ibinary& i, T& t)             // binary input stream, read values of any type
   {
      if (i.swapmode()) {
         char buf [sizeof (T)];
         i.read (buf, sizeof (T));
         bswap ((char*)&t, buf, sizeof (T));
      } else {
         i.read ((char*)&t, sizeof (T));
      } /* endif */
      return i;
   }

template<class T>
   obinary& operator<< (obinary& o, T t)              // binary output stream, write values of any type
   {
      if (o.swapmode()) {
         char buf [sizeof (T)];
         bswap (buf, (char*)&t, sizeof (T));
         o.write (buf, sizeof (T));
      } else {
         o.write ((char*)&t, sizeof (T));
      } /* endif */
      if (o.needflush (sizeof (T))) o.flush ();
      return o;
   }

template<class T> 
   iobinary& operator>> (iobinary& io, T& t)          // binary input output stream, read values
   {
      if (io.swapmode()) {
         char buf [sizeof (T)];
         io.read (buf, sizeof (T));
         bswap ((char*)&t, buf, sizeof (T));
      } else {
         io.read ((char*)&t, sizeof (T));
      } /* endif */
      if (io.needflush (sizeof (T))) io.flush ();
      return io;
   }

template<class T> 
   iobinary& operator<< (iobinary& io, T t)           // binary input output stream, write values
   {
      if (io.swapping) {
         char buf [sizeof (T)];
         bswap (buf, (char*)&t, sizeof (T));
         io.write (buf, sizeof (T));
      } else {
         io.write ((char*)&t, sizeof (T));
      } /* endif */
      return io;
   }

// -------------------------------------------------------------------------------------------------

inline pidmsk::pidmsk (const char* msk)
{
   char tmp [TMPSTRBUFLEN];
   time_t now;

   time (&now);
   sprintf (tmp, msk, (now + 256*getpid()) & 0xFFFFF);
   size_t l = strlen (tmp)+1;
   s = new char [l];
   memcpy (s, tmp, l);
}

inline pidmsk& pidmsk::operator= (const pidmsk& r)
{
    s = new char [strlen (r.s) + 1];
    strcpy (s, r.s);
    return *this;
}


// -------------------------------------------------------------------------------------------------

template<class T>
   inline T& operator<= (T& i, ipipe& p)
   {
      p.attach (i);
      return i;
   }
   
template<class T>
   inline T& operator<= (T& o, opipe& p)
   {
      p.attach (o);
      return o;
   }
   
template<class T>
   inline T& operator<= (T& io, iopipe& p)
   {
      p.attach (io);
      return io;
   }

// -------------------------------------------------------------------------------------------------

template<class T>
   inline s2stream& operator<< (s2stream& s, T& t)
   {
      strstream& x = s;  x << t;
      ((char*)s) [s.tellp ()] = 0;
      return s;
   }

template<class T>
   inline T max (T x, T y) {
      return x > y ? x : y;
   }
   
template<class T>
   inline T min (T x, T y) {
      return x < y ? x : y;
   }


/**************************************************************************************************/
/* The following definitions may be included only once into your project!                         */
/*                                                                                                */
/* You should include it into the source file were your main procedure is declared.               */
/* To include it, define MAIN before the statement that includes this source file.                */
/**************************************************************************************************/
#ifdef MAIN

/**************************************************************************************************/
/*                                         Static Objects                                         */
/**************************************************************************************************/
NewFailed   MemErr;             // install new failure exception handler
InitRandom  SeedNow;            // init the random number generator now

ShellInfoParser pgm;            // global program information block

FatalError OpErr  ("error opening file %1");
FatalError WrErr  ("error writing output stream %1");
FatalError RdErr  ("error reading input stream %1");
FatalError EofErr ("stream size mismatch");
FatalError SysErr ("command exec failure: %1");


/**************************************************************************************************/
/*                                        Static Functions                                        */
/**************************************************************************************************/
#ifdef NEW_HANDLER_2
int MemErrorHandler (size_t l) { throw MemErr; return 0; }
#else
void MemErrorHandler () { throw MemErr; }
#endif /* NEW_HANDLER_2 */

// -------------------------------------------------------------------------------------------------

ShellInfoParser::ShellInfoParser ()
   : in(0), out(1)
{
   pid = 0;
   name = 0;
   shpid = 0;
   shell = 0;
}

int ShellInfoParser::PgmHandleList::parse (char *list)
{
   int i;
   for (len=1, i=0; *list; i++)                       // Count number of integers in list
      if (list[i] == ',')                             // List elements are seperated with ','
         list[i]=' ', len++;                          // Increment count and remove ','
   tab = new int[len];                                // Create table
   istrstream s (list);                               // Initialize a string stream with parameter
   for (i=0; i<len; i++)                              // Handle all list elements
      s >> tab[i];                                    // Convert the handle
   return len;
}

int ShellInfoParser::auto_init (int argc, char** argv, char** envp)
{
   // Parse arguments passed to us from shell through argv[0]
   char *arg0 = strdup (argv[0]);                     // Make a copy of argv[0]
   if (!arg0) arg0 = argv[0];                         // Failsafe mechanism
   if (strncmp (arg0, SHELLMARK, sizeof(SHELLMARK))){ // Normal program startup? 
      name = arg0;                                    // Get program name
   } else {                                           // Shell mark found in arg0!
      arg0 += sizeof(SHELLMARK);                      // Skip shell mark
      char *chr = strchr (arg0, '>');                 // Search for end of additional data
      if (chr) {                                      // If end is found in string, parse string
         *chr = 0;                                    // Terminate rest of string
         name = chr+1;                                // Original program name
         argv[0] = name;                              // Change argv[0]
         chr = strchr (arg0, '@');                    // Search for pid mark
         if (chr) {                                   // If found, parse pid
            *chr = 0;                                 // Split string
            istrstream s (arg0);                      // Initialize string stream
            s >> shpid;                               // Convert pid from ascii to integer
            arg0 = chr+1;                             // Skip pid
         } /* endif */
         shell = arg0;                                // Path to program shell
         chr = strchr (arg0, '(');                    // Search for begin of handle list
         if (chr) {
            *chr = 0;                                 // Split String
            arg0 = chr+1;                             // Skip program shell
            chr = strchr (arg0, ')');                 // Search for end of handle lists
            if (chr) *chr = 0;                        // Terminate list
            chr = strchr (arg0, ';');                 // Search for end of input handle list
            if (chr) *chr = 0;                        // Terminate the list
            in.parse (arg0);                          // Parse the input handle list
            if (chr) {                                // Terminator found during last search
               arg0 = chr+1;                          // Skip input handle list
               chr = strchr (arg0, ';');              // Search for end of output handle list
               if (chr) *chr = 0;                     // Terminate the list
               out.parse (arg0);                      // Parse the output handle list
               if (chr) {
                  arg0 = chr+1;                       // Skip output handle list
                  chr = strchr (arg0, ';');           // Search for end of io handle list
                  if (chr) *chr = 0;                  // Terminate the list
                  io.parse (arg0);                    // Parse the io handle list
               } /* endif */
            } /* endif */
         } /* endif */
      } else {                                        // No additional data in arg0
         name = arg0;                                 // Get program name
         argv[0] = name;                              // Change argv[0]
      } /* endif */
   } /* endif */
   return 0;                                          // No errors
}

// -------------------------------------------------------------------------------------------------

void pipe::parse (char* s)
{
   delete[] name;
   name = new char [strlen(s)+1];
   strcpy(name, s);
   
   if (strcmp (name, "stdin")  == 0) 
      index = 0;
   else if (strcmp (name, "stdout") == 0) 
      index = 0;
   else if (name[1] == ':')
   {
      istrstream s (name+1);
      s >> index;
   }
   else
      index = -1;
}

// -------------------------------------------------------------------------------------------------

char* ascii::ltoa (long l) {
   unsigned n = 0, i = _INTEGRAL_MAX_BITS+1;
   buffer[i] = 0;
   if (l < 0) l = -l, n = 1;
   do {
      --i;
      buffer[i]  = char (l % base);  l /= char (base);
      buffer[i] += (buffer[i] < 10) ? '0' : 'A'-10;
   } while (l);
   while (_INTEGRAL_MAX_BITS+1 - i + n < outw) buffer[--i] = '0';
   if (n) buffer[--i] = '-';
   return p = buffer + i;
} /* ascii::ltoa */

char* ascii::utoa (unsigned long u) {
   unsigned i = _INTEGRAL_MAX_BITS+1;
   buffer[i] = 0;
   do {
      --i;
      buffer[i]  = char (u % base);  u /= char (base);
      buffer[i] += (buffer[i] < 10) ? '0' : 'A'-10;
   } while (u);
   while (_INTEGRAL_MAX_BITS+1 - i < outw) buffer[--i] = '0';
   return p = buffer + i;
} /* ascii::utoa */

// -------------------------------------------------------------------------------------------------

void value::init (void* p, unsigned b, TYPE t)
{
   pval = p, base = b, type = t;
   ovr  = ULONG_MAX % b;
   ovn  = ULONG_MAX / b;
}

void value::convinit ()
{
   read  = 0, val  = 0;
   flags = POS;
}

int value::accept (int c)
{
   c = toupper (c);
   if (!read && isspace (c)) return 1;
   if ('-' == c || '+' == c) {
      if (UL == type || UI == type || US == type ||
          UC == type || CH == type || read) return 0;
      if ('-' == c) flags = NEG;
   } /* endif */

   unsigned u = UINT_MAX;
   if (isdigit (c)) u = c - '0';
   if (isalpha (c)) u = c - 'A' + 10;
   if (u >= base) return 0;
   read++;
   if (val > ovn || (val == ovn && u > ovr)) flags = ERR;
   val = val*base + u;
   return 1;
}

int value::convdone ()
{
   unsigned long max = 0;
   signed long   min = 0;
   signed long  sval = (NEG == flags) ? - (signed long) val : (signed long) val;
   
   if (!read) flags = ERR;
   if (ERR == flags) return 0;
   switch (type) {
   case UL:  max = ULONG_MAX;  break;
   case UI:  max = UINT_MAX;   break;
   case US:  max = USHRT_MAX;  break;
   case UC: 
   case CH:  max = UCHAR_MAX;  break;
   case SL:  max = LONG_MAX, min = LONG_MIN;  break;
   case SI:  max = INT_MAX,  min = INT_MIN;   break;
   case SS:  max = SHRT_MAX, min = SHRT_MIN;  break;
   case SC:  max = CHAR_MAX, min = CHAR_MIN;  break;
   } /* endswitch */
   if (((POS == flags) && (val > max)) || ((NEG == flags) && (sval < min))) return 0;
   switch (type) {
   case UL:  *(unsigned long*)  pval = (unsigned long)  val;  break;
   case UI:  *(unsigned int*)   pval = (unsigned int)   val;  break;
   case US:  *(unsigned short*) pval = (unsigned short) val;  break;
   case UC: 
   case CH:  *(unsigned char*)  pval = (unsigned char)  val;  break;
   case SL:  *(signed long*)    pval = (signed long)   sval;  break;
   case SI:  *(signed int*)     pval = (signed int)    sval;  break;
   case SS:  *(signed short*)   pval = (signed short)  sval;  break;
   case SC:  *(signed char*)    pval = (signed char)   sval;  break;
   } /* endswitch */
   return 1;
}

value& value::operator= (const char* s)
{
   convinit ();
   while (*s) if (!accept (*(s++))) break;
   convdone ();
   return *this;
}

inline istream& operator>> (istream& is, value* pv)
{
   if (is.ipfx ()) {
      char c = 0;

      pv->convinit ();
      do {
         is.get (c);
         if (is.bad ()) return is;
         if (!pv->accept (c)) break;
      } while (1);
      is.putback (c);
      if (!pv->convdone ()) is.clear (ios::failbit);  // Set fail bit!
      is.isfx ();
   } /* endif */
   return is;
}

// -------------------------------------------------------------------------------------------------

Statistic::~Statistic ()                              // generate statistics
{
   if (oname) pout = new ofstream (oname, ios::app);  // Append to file?
   if (pout) {                                        // Statistic attached to stream?
      char buffer [256];                              // line buffer
      time_t stop;                                    // stop time
      time (&stop);                                   // get current time

      // write program statistics
      if (Error::Count()) *this << Error::Count() << " Error(s) occured\n";
      strftime (buffer, 256, "%d-%b-%Y %H:%M:%S", localtime (&start));
      *this << "Process started: " << buffer << '\n';
      strftime (buffer, 256, "%d-%b-%Y %H:%M:%S", localtime (&stop));
      *this << "Process stopped: " << buffer << '\n';
      *this << "CPU time: " << double(clock()-cputime) / CLOCKS_PER_SEC << " seconds";

      // copy statistics buffer to output
      while (getline (buffer, 256)) *pout << buffer << '\n';   
   } /* endif */
   if (oname) delete pout;                            // close constructed file
}

// -------------------------------------------------------------------------------------------------

// Forward declaration of user main
#ifdef _CONSOLE
extern int _user_main_ (int, char**);

int main (int argc, char** argv, char** envp)
{
   try {
      int rci = 0, rcm = 0, rct = 0;
      rci = AutoPgmInit::init (argc, argv, envp);     // init all registered objects
      if (!rci) rcm = _user_main_ (argc, argv);       // call user program
      rct = AutoPgmInit::term ();                     // deregister objects
      return rci ? rci : rcm ? rcm : rct ? rct : 0;
   } catch (Failure& failure) {                       // catch errors generated
      AutoPgmInit::term ();                           // be shure to deregister correctly
      complain failure;                               // and handle the error
      return -1;
   } /* endtry */
}

// A trick to subclass main
#define main _user_main_
#endif

// -------------------------------------------------------------------------------------------------

/**************************************************************************************************/
/*  Function:    char* ppstring (char* s)                                                         */
/*                                                                                                */
/*  Description: Preprocess string. All escape sequences in the string are converted and          */
/*               eleminated. Uses the same escape sequences like in C/C++.                        */
/*  Arguments:   s = in this string the escape sequences are processed                            */
/**************************************************************************************************/
char* ppstring (char* s)
{
  char  c;
  char* d = new char [strlen (s) + 1];
  while (0 != (c = *d = *s++)) {
     if ('\\' == c)
        switch (c = *s++) {
           case 'n': *d = '\n'; break; 
           case 'r': *d = '\r'; break;
           case 't': *d = '\t'; break;
           case 'v': *d = '\v'; break;
           case 'b': *d = '\b'; break;
           case 'a': *d = '\a'; break;
           case 'f': *d = '\f'; break;
           default:  *++d = c;
        } /* endswitch */
     d++;
  } /* endwhile */
  return d;
}

/**************************************************************************************************/
/*  Function:    double interpolate (double* x, double* y, int l, double x0)                      */
/*                                                                                                */
/*  Description: Linear interpolation function.                                                   */
/*  Arguments:                                                                                    */
/*  Result:                                                                                       */
/**************************************************************************************************/
double interpolate (const double* x, const double* y, int l, double x0)
{
  const double *x1;
  const double *x2;
  const double *y1;
  const double *y2;

//  if (l < 2) fatal ("List for Interpolation too short!");

  x1 = x, y1 = y;
  x++, y++;
  x2 = x, y2 = y;
  
  if (x0 > *x1)
    for (int i = 1; i < l-1; i++)
      if (x0 > *x2) x1++, x2++, y1++, y2++;

  double k = (*y2 - *y1) / (*x2 - *x1);
  return (*y1 + k * (x0 - *x1));
}

/**************************************************************************************************/
/*  Function:    double db (double num, double den)                                               */
/*                                                                                                */
/*  Description: Calculate 20 log10 (num/den).                                                    */
/*  Arguments:   num = numerator                                                                  */
/*               den = denominator                                                                */
/*  Result:      -200 if num == 0, 200 if den == 0, 20 log10 (num/den) else                       */
/**************************************************************************************************/
double db (double num, double den)
{
   return (num == 0.0) ? -200.0 : (den == 0.0) ? 200.0 : 20.0 * log10 (num/den);
}

/**************************************************************************************************/
/*  Function:    double read_result (const char* tmp)                                             */
/*                                                                                                */
/*  Description: Reads the last double value from the file with name tmp.                         */
/*  Arguments:   tmp = filename of file to read                                                   */
/*  Result:      the last valid value read from file                                              */
/**************************************************************************************************/
double read_result (const char* tmp)
{
   ifstream in (tmp, ios::nocreate);
   if (!in) throw OpErr << tmp;
   double val, next;
   Checked (in >> val, tmp);
   while (in >> next) val = next;
   return val;
}

/**************************************************************************************************/
/*  Function:    void adjust_frequency (double& freq, double& periods,                            */
/*                                      double rate, int len, int mode)                           */
/*                                                                                                */
/*  Description: Adjusts frequency according to mode.                                             */
/*                                                                                                */
/*  Arguments:   freq    = frequency (adjusted)                                                   */
/*               rate    = sampling rate                                                          */
/*               len     = number of samples                                                      */
/*               mode    = 0: calculate number of periods in buffer                               */
/*                         1: adjust frequency for integer number of periods in buffer            */
/*                         2: adjust frequency for odd integer number of periods in buffer        */
/**************************************************************************************************/
double adjust_frequency (double& freq, double rate, int len, int& adjust)
{
   static FatalError AdjError ("frequency adjustment method must be 0, 1 or 2");
   double periods;

   if (freq < EPS) adjust = 0;
   switch (adjust) {
      case 0:  periods = freq / rate * len; break;
      case 1:  periods = max (rint (freq / rate * len), 1.0); break;
      case 2:  periods = max (rint (freq / rate * len - 1.0, 2.0) + 1.0, 1.0); break;
      default: throw AdjError;
   } /* endswitch */
   freq = periods * rate / len;
   return periods;
}

#endif /* MAIN */

#endif /* COMMON_INCLUDED */
