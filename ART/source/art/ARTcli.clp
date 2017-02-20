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
/*     Date     Rev                                 Description                                   */
/*  ----------- --- ----------------------------------------------------------------------------  */
/*  07-Jan-1997 BAR Original implementation.                                                      */
/*  13-Mar-1997 BAR Fixed misinterpretation of negative numbers as options.                       */
/*  17-Apr-1997 BAR Error class outputs program name now.                                         */
/*  06-Jul-1997 BAR Split error class into own include file.                                      */
/*  23-Sep-1998 BAR Minor bug fixes, alias characters for options inserted.                       */
/*  03-May-1999 BAR Added support for handling more than one input and output pipes.              */
/*  04-May-1999 BAR Changed destination of help output: cout if -help, cerr if +help is given.    */
/*  09-May-1999 BAR Split support for handling more than one io pipes into own include file.      */
/*  09-May-1999 BAR Added support for automatically initialization of cmdln.                      */
/*  26-Jun-2000 BAR New error handling support merged.                                            */
/*  15-Feb-2011 GEN Generated from application description file: Art.dsc                          */
/***************************************************************************************** //A// **/
/*  Description:                                                                                  */
/*                                                                                                */
/*    TAP - Transfer Analysis Package - Acoustic Research Tool - Version 1.0                      */
/*    Copyright (c) W. Kausel, IWK MDW-Vienna 2009. All rights reserved.                          */
/*                                                                                                */
/*    Sample main program demonstrating some of the capabilities of ART                           */
/*     using a command line interface. All acoustic elements can be tested                        */
/*     individually or in sophisticated systems described by script.\n                            */
/*     Receives frequencies as single column input stream with double values in binary format     */
/*     and creates triple column output stream consisting of (frequency, magnitude, argument)     */
/*     triples. Alternatively start frequency, stop frequency and frequency increment can be      */
/*     given as command line option.\n                                                            */
/*                                                                                                */
/*    Usage: art [options] < infile > outfile                                                     */
/*                                                                                                */
/*    Options:                                                                                    */
/*       -isignal ipipe                 read input signal from                  ("stdin")         */
/*       -osignal opipe                 write output signal to                  ("stdout")        */
/*       -borejump rin rout loss temperature humidity CO2 bore jump section (id=0)                */
/*          double rin        [cm]      input radius                            (1.0 cm)          */
/*          double rout       [cm]      output radius                           (2.0 cm)          */
/*          double loss                 local loss factor                       (1.0)             */
/*          double temperature[degC]    local temperature                       (21.0 degC)       */
/*          double humidity   [%]       local relative humidity                 (80.0 %)          */
/*          double CO2        [ppm]     local Mol fraction of CO2               (382.0 ppm)       */
/*       -cylinder len rin loss temperature humidity CO2 cylinder section (id=1)                  */
/*          double len        [cm]      length                                  (100.0 cm)        */
/*          double rin        [cm]      radius                                  (1 cm)            */
/*          double loss                 local loss factor                       (1.0)             */
/*          double temperature[degC]    local temperature                       (21.0 degC)       */
/*          double humidity   [%]       local relative humidity                 (80.0 %)          */
/*          double CO2        [ppm]     local Mol fraction of CO2               (382.0 ppm)       */
/*       -cone len rin rout loss temperature humidity CO2 cone section (id=2)                     */
/*          double len        [cm]      length                                  (100.0 cm)        */
/*          double rin        [cm]      input radius                            (1.0 cm)          */
/*          double rout       [cm]      output radius                           (2.0 cm)          */
/*          double loss                 local loss factor                       (1.0)             */
/*          double temperature[degC]    local temperature                       (21.0 degC)       */
/*          double humidity   [%]       local relative humidity                 (80.0 %)          */
/*          double CO2        [ppm]     local Mol fraction of CO2               (382.0 ppm)       */
/*       -bessel len rin rout flare loss temperature humidity CO2 bessel section (id=3)           */
/*          double len        [cm]      length                                  (100.0 cm)        */
/*          double rin        [cm]      input radius                            (1.0 cm)          */
/*          double rout       [cm]      output radius                           (2.0 cm)          */
/*          double flare      [cm]      flare                                   (2.0 cm)          */
/*          double loss                 local loss factor                       (1.0)             */
/*          double temperature[degC]    local temperature                       (21.0 degC)       */
/*          double humidity   [%]       local relative humidity                 (80.0 %)          */
/*          double CO2        [ppm]     local Mol fraction of CO2               (382.0 ppm)       */
/*       -list filename                 list section element (id=4)                               */
/*          char*  filename             name of script file containing instrument description     */
/*       -bcylinder len rin rbend loss temperature humidity CO2 cylinder bend section (id=5)      */
/*          double len        [cm]      length                                  (100.0 cm)        */
/*          double rin        [cm]      radius                                  (1 cm)            */
/*          double rbend      [cm]      bend radius                             (50.0 cm)         */
/*          double loss                 local loss factor                       (1.0)             */
/*          double temperature[degC]    local temperature                       (21.0 degC)       */
/*          double humidity   [%]       local relative humidity                 (80.0 %)          */
/*          double CO2        [ppm]     local Mol fraction of CO2               (382.0 ppm)       */
/*       -bcone len rin rout rbend loss temperature humidity CO2 cone bend section (id=6)         */
/*          double len        [cm]      length                                  (100.0 cm)        */
/*          double rin        [cm]      input radius                            (1.0 cm)          */
/*          double rout       [cm]      output radius                           (2.0 cm)          */
/*          double rbend      [cm]      bend radius                             (50.0 cm)         */
/*          double loss                 local loss factor                       (1.0)             */
/*          double temperature[degC]    local temperature                       (21.0 degC)       */
/*          double humidity   [%]       local relative humidity                 (80.0 %)          */
/*          double CO2        [ppm]     local Mol fraction of CO2               (382.0 ppm)       */
/*       -pressure double     [kPa]     athmospheric pressure                   (101.325 kPa)     */
/*       -modes int                     number of modes to calculate                              */
/*                                       0   = Mapes Riordan (fastest)                            */
/*                                       1   = plane wave (fast, accurate)                        */
/*                                       2-3 = efficient multimode                                */
/*                                       4-n = very slow multimode              (1)               */
/*       -bends int                     enforce calculation of radial modes                       */
/*                                       0   = ignore radial modes              (1)               */
/*       -accumulate                    enforce matrix accumulation                               */
/*       -lossless                      use lossless models                                       */
/*       -simplified                    use simplified method                                     */
/*       -accuracy double     [%]       slicing accuracy for cone and bessel sections(0.5 %)      */
/*       -columns int                   number of returned complex data points per frequency      */
/*                                       0 = composite wave                                       */
/*                                       1 = plane wave only                                      */
/*                                       n = diagonal impedance modes only (if n<=modes)          */
/*                                       N = complete square matrix (if N>modes)(1)               */
/*       -radiationfile filename columns specify radiation impedance, calculate input impedance   */
/*          char*  filename             name of multi column blank separated text file            */
/*                                       freq mag_0 arg_0 mag_1 arg_1...\n                        */
/*          int    columns              number of data points per frequency                       */
/*                                       1 = plane wave only                                      */
/*                                       n = diagonal impedance modes only (if n<=modes)          */
/*                                       N = complete square matrix (if N>modes)(1)               */
/*       -impedancefile filename columns specify input impedance, calculate radiation impedance   */
/*          char*  filename             name of multi column blank separated text file            */
/*                                       freq mag_0 arg_0 mag_1 arg_1...\n                        */
/*          int    columns              number of data points per frequency                       */
/*                                       1 = plane wave only                                      */
/*                                       n = diagonal impedance modes only (if n<=modes)          */
/*                                       N = complete square matrix (if N>modes)(1)               */
/*       -interiorwave impedancefile columns elementname radius arg calculate interior wave quantities p, u, z right of specified element*/
/*          char*  impedancefile        name of multi column blank separated text file            */
/*                                       freq mag_0 arg_0 mag_1 arg_1...\n                        */
/*          int    columns              number of data points per frequency                       */
/*                                       1 = plane wave only                                      */
/*                                       n = diagonal impedance modes only (if n<=modes)          */
/*                                       N = complete square matrix (if N>modes)(1)               */
/*          char*  elementname          name of horn element to calculate wave quantities at      */
/*          double radius     [m]       distance from element axis in radial dimension (or first coordinate)(0 m)*/
/*          double arg        [rad|m]   angular distance from horizontal plane (or second coordinate)(0 rad|m)*/
/*       -termination int               termination mode                                          */
/*                                       0..reflecting                                            */
/*                                       1..Levine/Zorumsky                     (1)               */
/*       -frequencies f0 f1 df refine target specify frequency grid (do not use input stream)     */
/*          double f0         [Hz]      starting frequency                      (10.0 Hz)         */
/*          double f1         [Hz]      stop frequency                          (1800.0 Hz)       */
/*          double df         [Hz]      frequency increment                     (10.0 Hz)         */
/*          int    refine     [bool]    refine peaks                            (0 bool)          */
/*          double target     [%]       accuracy target                         (0.00001 %)       */
/*       -run filename                  run complete script file                                  */
/*          char*  filename             name of script file to execute                            */
/*       -capabilities                  list library capabilities                                 */
/*       -debug                         debug output to statistics                                */
/*       -stat char*                    write program statistics to file                          */
/*       -swap                          exchange adjacent bytes of values                         */
/*       -? | -help                     generates this help screen                                */
/***************************************************************************************** //B// **/
#ifndef CMDLINE_PARSER_INCLUDED
#define CMDLINE_PARSER_INCLUDED


/**************************************************************************************************/
/*                                          Definitions                                           */
/**************************************************************************************************/
#ifndef HLPOUT
#define HLPOUT cout
#endif

#define USES_STDIN
#define USES_STDOUT
                                                                                           //N//
/**************************************************************************************************/
/*                                      Needed Include Files                                      */
/**************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#if defined(_MSC_VER) && _MSC_VER < 1600
	#include <iostream.h>
	#include <strstrea.h>
	#include "error_vs6.hpp"
#else
	#include <iostream>
	#include <strstream>
	#include "error.hpp"
#endif 
#include <ctype.h>



/**************************************************************************************************/
/*               Inline Functions, Forwarded Function Definitions, Template Functions             */
/**************************************************************************************************/
inline bool IsOption (char *s) { return ('-' == *s || '+' == *s) && !isdigit(s[1]); }

template<class T> int ParseArg (T&, char**);          // Converts a string to needed type
int ParseArg (char*&, char**);                        // Overrides above function template for char*

#ifdef LIST_INCLUDED
template<class T> int ParseArgList (List<T>&, char**);// Converts a string list to needed type list
int ParseArgList (List<char*>&, char**);              // Overrides above function template for char*
#endif /* LIST_INCLUDED */

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
class CommandLineParser                               // Command line parser class
#ifdef AUTOINIT_INCLUDED
   : public AutoPgmInit                               // Automatically init this at pgm startup
#endif
{
   class Option                                       // Options are derived from this
   {
      Option *next;                                   // Options are joined through this

   protected:
      int   pos;                                      // Argument position
      int   argc;                                     // Number of args parsed
      int   flags;                                    // Several option flags, see enum below for values
      char *name;                                     // Name of this option

      enum { EXIST=1, DEFAULT=2, SWITCH=4 };          // Option flags values

   public:
      Option ();                                      // Base constructor
      virtual ~Option() {}                            // Virtual desctructor

      int Match (char*);                              // Is given string a substring of name?
      int Exact (char*);                              // Is given string equal to name?
      int Init  (int, char**);                        // Initialization if option is found
      Option *Next () { return next; }
      
      virtual int  Parse (char**) { return 0; }       // Parses additional option arguments
      virtual void Help  (ostream&) {}                // Help message for this option
      virtual void Valid () {}                        // Validity check
      virtual void Invalid () {}                      // Invalidity check
      virtual char Alias () { return 0; }             // Shortform of option (-? via -help)?

      friend int IsDef     (const Option& o) { return o.flags & EXIST;   } // Option defined?
      friend int IsntDef   (const Option& o) { return ~o.flags & EXIST;  } // Option undefined?
      friend int IsDefault (const Option& o) { return o.flags & DEFAULT; } // Default value of Option?
      friend int IsOn      (const Option& o) { return o.flags & SWITCH;  } // Option preceded with '+'
      friend int IsOff     (const Option& o) { return ~o.flags & SWITCH; } // Option preceded with '-'
   } *first;

   struct IsignalOption : public Option {
      ipipe  value;
      IsignalOption ();
      ipipe& operator= (ipipe const &x) { return value = x; }
      ipipe* operator& () { return &value; }
      template<class T> ipipe& operator()(T t) { return value(t); }
      template<class T> ipipe& operator[](T t) { return value[t]; }
      operator ipipe& () { return value; }
      int  Parse (char**);
      void Help (ostream&);
   };

   struct OsignalOption : public Option {
      opipe  value;
      OsignalOption ();
      opipe& operator= (opipe const &x) { return value = x; }
      opipe* operator& () { return &value; }
      template<class T> opipe& operator()(T t) { return value(t); }
      template<class T> opipe& operator[](T t) { return value[t]; }
      operator opipe& () { return value; }
      int  Parse (char**);
      void Help (ostream&);
   };

   struct BorejumpOption : public Option {
      double rin;
      double rout;
      double loss;
      double temperature;
      double humidity;
      double CO2;
      BorejumpOption ();
      int  Parse (char**);
      void Help (ostream&);
   };

   struct CylinderOption : public Option {
      double len;
      double rin;
      double loss;
      double temperature;
      double humidity;
      double CO2;
      CylinderOption ();
      int  Parse (char**);
      void Help (ostream&);
   };

   struct ConeOption : public Option {
      double len;
      double rin;
      double rout;
      double loss;
      double temperature;
      double humidity;
      double CO2;
      ConeOption ();
      int  Parse (char**);
      void Help (ostream&);
   };

   struct BesselOption : public Option {
      double len;
      double rin;
      double rout;
      double flare;
      double loss;
      double temperature;
      double humidity;
      double CO2;
      BesselOption ();
      int  Parse (char**);
      void Help (ostream&);
   };

   struct ListOption : public Option {
      char*  filename;
      ListOption ();
      int  Parse (char**);
      void Help (ostream&);
   };

   struct BcylinderOption : public Option {
      double len;
      double rin;
      double rbend;
      double loss;
      double temperature;
      double humidity;
      double CO2;
      BcylinderOption ();
      int  Parse (char**);
      void Help (ostream&);
   };

   struct BconeOption : public Option {
      double len;
      double rin;
      double rout;
      double rbend;
      double loss;
      double temperature;
      double humidity;
      double CO2;
      BconeOption ();
      int  Parse (char**);
      void Help (ostream&);
   };

   struct PressureOption : public Option {
      double value;
      PressureOption ();
      double& operator= (double const &x) { return value = x; }
      double* operator& () { return &value; }
      operator double() { return value; }
      int  Parse (char**);
      void Help (ostream&);
   };

   struct ModesOption : public Option {
      int    value;
      ModesOption ();
      int& operator= (int const &x) { return value = x; }
      int* operator& () { return &value; }
      operator int() { return value; }
      int  Parse (char**);
      void Help (ostream&);
   };

   struct BendsOption : public Option {
      int    value;
      BendsOption ();
      int& operator= (int const &x) { return value = x; }
      int* operator& () { return &value; }
      operator int() { return value; }
      int  Parse (char**);
      void Help (ostream&);
   };

   struct AccumulateOption : public Option {
      AccumulateOption ();
      void Help (ostream&);
   };

   struct LosslessOption : public Option {
      LosslessOption ();
      void Help (ostream&);
   };

   struct SimplifiedOption : public Option {
      SimplifiedOption ();
      void Help (ostream&);
   };

   struct AccuracyOption : public Option {
      double value;
      AccuracyOption ();
      double& operator= (double const &x) { return value = x; }
      double* operator& () { return &value; }
      operator double() { return value; }
      int  Parse (char**);
      void Help (ostream&);
   };

   struct ColumnsOption : public Option {
      int    value;
      ColumnsOption ();
      int& operator= (int const &x) { return value = x; }
      int* operator& () { return &value; }
      operator int() { return value; }
      int  Parse (char**);
      void Help (ostream&);
   };

   struct RadiationfileOption : public Option {
      char*  filename;
      int    columns;
      RadiationfileOption ();
      int  Parse (char**);
      void Help (ostream&);
   };

   struct ImpedancefileOption : public Option {
      char*  filename;
      int    columns;
      ImpedancefileOption ();
      int  Parse (char**);
      void Help (ostream&);
   };

   struct InteriorwaveOption : public Option {
      char*  impedancefile;
      int    columns;
      char*  elementname;
      double radius;
      double arg;
      InteriorwaveOption ();
      int  Parse (char**);
      void Help (ostream&);
   };

   struct TerminationOption : public Option {
      int    value;
      TerminationOption ();
      int& operator= (int const &x) { return value = x; }
      int* operator& () { return &value; }
      operator int() { return value; }
      int  Parse (char**);
      void Help (ostream&);
   };

   struct FrequenciesOption : public Option {
      double f0;
      double f1;
      double df;
      int    refine;
      double target;
      FrequenciesOption ();
      int  Parse (char**);
      void Help (ostream&);
   };

   struct RunOption : public Option {
      char*  filename;
      RunOption ();
      int  Parse (char**);
      void Help (ostream&);
   };

   struct CapabilitiesOption : public Option {
      CapabilitiesOption ();
      void Help (ostream&);
   };

   struct DebugOption : public Option {
      DebugOption ();
      void Help (ostream&);
   };

   struct StatOption : public Option {
      char*  value;
      StatOption ();
      char*& operator= (char* const &x) { return value = x; }
      char** operator& () { return &value; }
      operator char*() { return value; }
      int  Parse (char**);
      void Help (ostream&);
   };

   struct SwapOption : public Option {
      SwapOption ();
      void Help (ostream&);
   };

   struct HelpOption : public Option {
      HelpOption ();
      void Help (ostream&);
      char Alias() { return '?'; }
   };

public:                                                                                    //C//
   char *pgmname;                                     // Program name

   HelpOption help;
   SwapOption swap;
   StatOption stat;
   DebugOption debug;
   CapabilitiesOption capabilities;
   RunOption run;
   FrequenciesOption frequencies;
   TerminationOption termination;
   InteriorwaveOption interiorwave;
   ImpedancefileOption impedancefile;
   RadiationfileOption radiationfile;
   ColumnsOption columns;
   AccuracyOption accuracy;
   SimplifiedOption simplified;
   LosslessOption lossless;
   AccumulateOption accumulate;
   BendsOption bends;
   ModesOption modes;
   PressureOption pressure;
   BconeOption bcone;
   BcylinderOption bcylinder;
   ListOption list;
   BesselOption bessel;
   ConeOption cone;
   CylinderOption cylinder;
   BorejumpOption borejump;
   OsignalOption osignal;
   IsignalOption isignal;
                                                                                           //D//
   CommandLineParser();                               // Constructor
   int init (char**);                                 // Initialize command line, call it from main!

private:
   int  argc;                                         // Number of args parsed
   
   int  Parse (char**);                               // Parses command line for arguments
   void Help (ostream&);                              // Display help message
   void Valid ();                                     // Final validity check
   void Invalid ();                                   // Final invalidity check
   void Adjust ();                                    // Adjust values after parsing

#ifdef AUTOINIT_INCLUDED
   int auto_init (int, char** argv, char**) { init (argv); return 0; }
#endif
};


#ifndef MAIN
/**************************************************************************************************/
/*                                    Forwarded Static Objects                                    */
/**************************************************************************************************/
extern FatalError Error1;                             // Ambiguos option                     
extern FatalError Error2;                             // Invalid option
extern FatalError Error3;                             // Additional command line argument
extern FatalError Error4;                             // Options must precede arguments
extern FatalError Error5;                             // Argument conversion failure
extern FatalError Error6;                             // Validity check fails
extern FatalError Error7;                             // Arguments invalid
extern FatalError Error8;                             // section types and script file are exclusive options
extern FatalError Error9;                             // one section type or a script file must be specified
extern FatalError Error10;                            // radiationfile, impedancefile and frequencies are exclusive
                                                                                           //E//
extern CommandLineParser cmdln;                       // Command line parser class


/**************************************************************************************************/
/* The following definitions may be included only once into your project!                         */
/*                                                                                                */
/* You should include it into the source file were your main procedure is declared.               */
/* To include it, define MAIN before the statement, which includes this source file.              */
/**************************************************************************************************/
#else /* defined(MAIN) */

/**************************************************************************************************/
/*                                        Static Objects                                          */
/**************************************************************************************************/
FatalError Error1 ("ambiguous option %1");
FatalError Error2 ("invalid option %1");
FatalError Error3 ("additional command line argument %1");
FatalError Error4 ("options must precede arguments");
FatalError Error5 ("argument conversion failure %1");
FatalError Error6 ("validity check fails");
FatalError Error7 ("arguments invalid");
Warning  Warning1 ("ignoring additional characters in argument %1");
FatalError Error8 ("section types and script file are exclusive options");
FatalError Error9 ("one section type or a script file must be specified");
FatalError Error10 ("radiationfile, impedancefile and frequencies are exclusive");
                                                                                           //L//
CommandLineParser cmdln;


/**************************************************************************************************/
/*                                           Functions                                            */
/**************************************************************************************************/
template<class T>
   int ParseArg (T& t, char **argv)                   // Reads an argument and converts it into type T
   {
      if (argv[0] && !IsOption (argv[0])) {           // Exists argument and it isn't an option?
         istrstream s (argv[0]);                      // Initialize string stream
         s >> t;                                      // Stream class does conversion for us
         if (!s) throw Error5 << argv[0];             // Argument conversion failure
         char c; s >> c;                              // Try to read additional character
         if (!s.eof())                                // Print warning if there are additional chars
            complain Warning1 << argv[0];             // Don't throw the warning, we want to continue
         return 1;                                    // One argument read    
      } /* endif */
      return 0;                                       // Sorry, no argument or argument is an option
   } /* ParseArg */

int ParseArg (char *&t, char **argv)                  // Same as above, but suitable for char*
{
   if (argv[0] && !IsOption (argv[0])) {              // Exists argument and it is not an option?
      t = argv[0];                                    // Copy pointer, no conversion needed
      return 1;                                       // One argument read
   } /* endif */
   return 0;                                          // Sorry, no argument or argument is an option
} /* ParseArg */

#ifdef LIST_INCLUDED
template<class T>
   int ParseArgList (List<T> &t, char **argv)         // Read an argument list, type safe
   {
      int i = 0;                                      // Argument counter
      while (argv[i]) {                               // Exists argument?
         if (IsOption (argv[i])) break;               // If it is an option, bye
         if (strcmp (argv[i], ".") == 0) break;       // If explicit end of list given, bye
         istrstream s (argv[i]);                      // Initialize string stream
         s >> t [ListPtr::TAIL];                      // Read argument into list
         if (!s) throw Error5 << argv[i];             // Argument conversion failure
         char c; s >> c;                              // Try to read additional character
         if (!s.eof())                                // Print warning if there are additional chars
            complain Warning1 << argv[0];             // Don't throw the warning, we want to continue
         i++;                                         // Next arg
      } /* endwhile */
      return i;                                       // Return number of args read
   } /* ParseArgList */

int ParseArgList (List<char*> &t, char **argv)        // Same as above, for char*
{
   int i = 0;                                         // Argument counter
   while (argv[i]) {                                  // Exists argument?
      if (IsOption (argv[i])) break;                  // If it is an option, bye
      if (strcmp (argv[i], ".") == 0) break;          // If explicit end of list given, bye
      t [ListPtr::TAIL] = argv[i];                    // Copy string pointer into list
      i++;                                            // Next arg
   } /* endwhile */
   return i;                                          // Return number of args read
} /* ParseArgList */
#endif /* LIST_INCLUDED */


CommandLineParser::Option::Option ()                  // Option class base constructor
{
   next  = DelayPointer (this);                       // Link this object into list
   flags = DEFAULT, name = 0, argc = 0;               // Init member vars
} /* constructor */

int CommandLineParser::Option::Match (char *sw) {     // Option name comparator
   return 0 == strncmp (name, sw+1, strlen (sw+1));
} /* Match */

int CommandLineParser::Option::Exact (char *sw) {     // Option name comparator
   return ( (0 == sw[2] && Alias() == sw[1]) ||       // Short form of option given?
            (0 == strcmp (name, sw+1)) );             // Long name given?
} /* Match */

int CommandLineParser::Option::Init (int argc, char **argv) // Common option initializer
{
   pos = argc;                                        // Save option position
   flags |= EXIST;                                    // Option seen on command line
   if (**argv == '+') flags |= SWITCH;                // Option switched on?
   return 1;                                          // One command line argument interpreted
} /* Init */

CommandLineParser::IsignalOption::IsignalOption ()
{
   Option::name  = "isignal";
   value = "stdin";
} /* constructor */

int CommandLineParser::IsignalOption::Parse (char** argv)
{
   argc += ParseArg (value, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::IsignalOption::Help (ostream& hout)
{
   hout << "   -isignal ipipe                       read input signal from                  ";
   if (argc) argc--; 
   //Sadjad hout << '(' << value << ')' << '\n';
} /* Help */

CommandLineParser::OsignalOption::OsignalOption ()
{
   Option::name  = "osignal";
   value = "stdout";
} /* constructor */

int CommandLineParser::OsignalOption::Parse (char** argv)
{
   argc += ParseArg (value, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::OsignalOption::Help (ostream& hout)
{
   hout << "   -osignal opipe                       write output signal to                  ";
   if (argc) argc--; 
   //Sadjad hout << '(' << value << ')' << '\n';
} /* Help */

CommandLineParser::BorejumpOption::BorejumpOption ()
{
   Option::name  = "borejump";
   rin = 1.0;
   rout = 2.0;
   loss = 1.0;
   temperature = 21.0;
   humidity = 80.0;
   CO2 = 382.0;
} /* constructor */

int CommandLineParser::BorejumpOption::Parse (char** argv)
{
   argc += ParseArg (rin, argv+argc);
   argc += ParseArg (rout, argv+argc);
   argc += ParseArg (loss, argv+argc);
   argc += ParseArg (temperature, argv+argc);
   argc += ParseArg (humidity, argv+argc);
   argc += ParseArg (CO2, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::BorejumpOption::Help (ostream& hout)
{
   hout << "   -borejump rin rout loss temperature humidity CO2 bore jump section (id=0)    ";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
   hout << "      double rin              [cm]      input radius                            ";
   if (argc) hout << '(' << rin  << " cm" << ')', argc--;
   else      hout << "(1.0 cm)";
   hout << '\n';
   hout << "      double rout             [cm]      output radius                           ";
   if (argc) hout << '(' << rout  << " cm" << ')', argc--;
   else      hout << "(2.0 cm)";
   hout << '\n';
   hout << "      double loss                       local loss factor                       ";
   if (argc) hout << '(' << loss << ')', argc--;
   else      hout << "(1.0)";
   hout << '\n';
   hout << "      double temperature      [degC]    local temperature                       ";
   if (argc) hout << '(' << temperature  << " degC" << ')', argc--;
   else      hout << "(21.0 degC)";
   hout << '\n';
   hout << "      double humidity         [%]       local relative humidity                 ";
   if (argc) hout << '(' << humidity  << " %" << ')', argc--;
   else      hout << "(80.0 %)";
   hout << '\n';
   hout << "      double CO2              [ppm]     local Mol fraction of CO2               ";
   if (argc) hout << '(' << CO2  << " ppm" << ')', argc--;
   else      hout << "(382.0 ppm)";
   hout << '\n';
} /* Help */

CommandLineParser::CylinderOption::CylinderOption ()
{
   Option::name  = "cylinder";
   len = 100.0;
   rin = 1;
   loss = 1.0;
   temperature = 21.0;
   humidity = 80.0;
   CO2 = 382.0;
} /* constructor */

int CommandLineParser::CylinderOption::Parse (char** argv)
{
   argc += ParseArg (len, argv+argc);
   argc += ParseArg (rin, argv+argc);
   argc += ParseArg (loss, argv+argc);
   argc += ParseArg (temperature, argv+argc);
   argc += ParseArg (humidity, argv+argc);
   argc += ParseArg (CO2, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::CylinderOption::Help (ostream& hout)
{
   hout << "   -cylinder len rin loss temperature humidity CO2 cylinder section (id=1)      ";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
   hout << "      double len              [cm]      length                                  ";
   if (argc) hout << '(' << len  << " cm" << ')', argc--;
   else      hout << "(100.0 cm)";
   hout << '\n';
   hout << "      double rin              [cm]      radius                                  ";
   if (argc) hout << '(' << rin  << " cm" << ')', argc--;
   else      hout << "(1 cm)";
   hout << '\n';
   hout << "      double loss                       local loss factor                       ";
   if (argc) hout << '(' << loss << ')', argc--;
   else      hout << "(1.0)";
   hout << '\n';
   hout << "      double temperature      [degC]    local temperature                       ";
   if (argc) hout << '(' << temperature  << " degC" << ')', argc--;
   else      hout << "(21.0 degC)";
   hout << '\n';
   hout << "      double humidity         [%]       local relative humidity                 ";
   if (argc) hout << '(' << humidity  << " %" << ')', argc--;
   else      hout << "(80.0 %)";
   hout << '\n';
   hout << "      double CO2              [ppm]     local Mol fraction of CO2               ";
   if (argc) hout << '(' << CO2  << " ppm" << ')', argc--;
   else      hout << "(382.0 ppm)";
   hout << '\n';
} /* Help */

CommandLineParser::ConeOption::ConeOption ()
{
   Option::name  = "cone";
   len = 100.0;
   rin = 1.0;
   rout = 2.0;
   loss = 1.0;
   temperature = 21.0;
   humidity = 80.0;
   CO2 = 382.0;
} /* constructor */

int CommandLineParser::ConeOption::Parse (char** argv)
{
   argc += ParseArg (len, argv+argc);
   argc += ParseArg (rin, argv+argc);
   argc += ParseArg (rout, argv+argc);
   argc += ParseArg (loss, argv+argc);
   argc += ParseArg (temperature, argv+argc);
   argc += ParseArg (humidity, argv+argc);
   argc += ParseArg (CO2, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::ConeOption::Help (ostream& hout)
{
   hout << "   -cone len rin rout loss temperature humidity CO2 cone section (id=2)         ";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
   hout << "      double len              [cm]      length                                  ";
   if (argc) hout << '(' << len  << " cm" << ')', argc--;
   else      hout << "(100.0 cm)";
   hout << '\n';
   hout << "      double rin              [cm]      input radius                            ";
   if (argc) hout << '(' << rin  << " cm" << ')', argc--;
   else      hout << "(1.0 cm)";
   hout << '\n';
   hout << "      double rout             [cm]      output radius                           ";
   if (argc) hout << '(' << rout  << " cm" << ')', argc--;
   else      hout << "(2.0 cm)";
   hout << '\n';
   hout << "      double loss                       local loss factor                       ";
   if (argc) hout << '(' << loss << ')', argc--;
   else      hout << "(1.0)";
   hout << '\n';
   hout << "      double temperature      [degC]    local temperature                       ";
   if (argc) hout << '(' << temperature  << " degC" << ')', argc--;
   else      hout << "(21.0 degC)";
   hout << '\n';
   hout << "      double humidity         [%]       local relative humidity                 ";
   if (argc) hout << '(' << humidity  << " %" << ')', argc--;
   else      hout << "(80.0 %)";
   hout << '\n';
   hout << "      double CO2              [ppm]     local Mol fraction of CO2               ";
   if (argc) hout << '(' << CO2  << " ppm" << ')', argc--;
   else      hout << "(382.0 ppm)";
   hout << '\n';
} /* Help */

CommandLineParser::BesselOption::BesselOption ()
{
   Option::name  = "bessel";
   len = 100.0;
   rin = 1.0;
   rout = 2.0;
   flare = 2.0;
   loss = 1.0;
   temperature = 21.0;
   humidity = 80.0;
   CO2 = 382.0;
} /* constructor */

int CommandLineParser::BesselOption::Parse (char** argv)
{
   argc += ParseArg (len, argv+argc);
   argc += ParseArg (rin, argv+argc);
   argc += ParseArg (rout, argv+argc);
   argc += ParseArg (flare, argv+argc);
   argc += ParseArg (loss, argv+argc);
   argc += ParseArg (temperature, argv+argc);
   argc += ParseArg (humidity, argv+argc);
   argc += ParseArg (CO2, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::BesselOption::Help (ostream& hout)
{
   hout << "   -bessel len rin rout flare loss temperature humidity CO2 bessel section (id=3)";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
   hout << "      double len              [cm]      length                                  ";
   if (argc) hout << '(' << len  << " cm" << ')', argc--;
   else      hout << "(100.0 cm)";
   hout << '\n';
   hout << "      double rin              [cm]      input radius                            ";
   if (argc) hout << '(' << rin  << " cm" << ')', argc--;
   else      hout << "(1.0 cm)";
   hout << '\n';
   hout << "      double rout             [cm]      output radius                           ";
   if (argc) hout << '(' << rout  << " cm" << ')', argc--;
   else      hout << "(2.0 cm)";
   hout << '\n';
   hout << "      double flare            [cm]      flare                                   ";
   if (argc) hout << '(' << flare  << " cm" << ')', argc--;
   else      hout << "(2.0 cm)";
   hout << '\n';
   hout << "      double loss                       local loss factor                       ";
   if (argc) hout << '(' << loss << ')', argc--;
   else      hout << "(1.0)";
   hout << '\n';
   hout << "      double temperature      [degC]    local temperature                       ";
   if (argc) hout << '(' << temperature  << " degC" << ')', argc--;
   else      hout << "(21.0 degC)";
   hout << '\n';
   hout << "      double humidity         [%]       local relative humidity                 ";
   if (argc) hout << '(' << humidity  << " %" << ')', argc--;
   else      hout << "(80.0 %)";
   hout << '\n';
   hout << "      double CO2              [ppm]     local Mol fraction of CO2               ";
   if (argc) hout << '(' << CO2  << " ppm" << ')', argc--;
   else      hout << "(382.0 ppm)";
   hout << '\n';
} /* Help */

CommandLineParser::ListOption::ListOption ()
{
   Option::name  = "list";
} /* constructor */

int CommandLineParser::ListOption::Parse (char** argv)
{
   argc += ParseArg (filename, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::ListOption::Help (ostream& hout)
{
   hout << "   -list filename                       list section element (id=4)             ";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
   hout << "      char* filename                    name of script file containing instrument description";
   if (argc) hout << '(' << filename << ')', argc--;
   hout << '\n';
} /* Help */

CommandLineParser::BcylinderOption::BcylinderOption ()
{
   Option::name  = "bcylinder";
   len = 100.0;
   rin = 1;
   rbend = 50.0;
   loss = 1.0;
   temperature = 21.0;
   humidity = 80.0;
   CO2 = 382.0;
} /* constructor */

int CommandLineParser::BcylinderOption::Parse (char** argv)
{
   argc += ParseArg (len, argv+argc);
   argc += ParseArg (rin, argv+argc);
   argc += ParseArg (rbend, argv+argc);
   argc += ParseArg (loss, argv+argc);
   argc += ParseArg (temperature, argv+argc);
   argc += ParseArg (humidity, argv+argc);
   argc += ParseArg (CO2, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::BcylinderOption::Help (ostream& hout)
{
   hout << "   -bcylinder len rin rbend loss temperature humidity CO2 cylinder bend section (id=5)";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
   hout << "      double len              [cm]      length                                  ";
   if (argc) hout << '(' << len  << " cm" << ')', argc--;
   else      hout << "(100.0 cm)";
   hout << '\n';
   hout << "      double rin              [cm]      radius                                  ";
   if (argc) hout << '(' << rin  << " cm" << ')', argc--;
   else      hout << "(1 cm)";
   hout << '\n';
   hout << "      double rbend            [cm]      bend radius                             ";
   if (argc) hout << '(' << rbend  << " cm" << ')', argc--;
   else      hout << "(50.0 cm)";
   hout << '\n';
   hout << "      double loss                       local loss factor                       ";
   if (argc) hout << '(' << loss << ')', argc--;
   else      hout << "(1.0)";
   hout << '\n';
   hout << "      double temperature      [degC]    local temperature                       ";
   if (argc) hout << '(' << temperature  << " degC" << ')', argc--;
   else      hout << "(21.0 degC)";
   hout << '\n';
   hout << "      double humidity         [%]       local relative humidity                 ";
   if (argc) hout << '(' << humidity  << " %" << ')', argc--;
   else      hout << "(80.0 %)";
   hout << '\n';
   hout << "      double CO2              [ppm]     local Mol fraction of CO2               ";
   if (argc) hout << '(' << CO2  << " ppm" << ')', argc--;
   else      hout << "(382.0 ppm)";
   hout << '\n';
} /* Help */

CommandLineParser::BconeOption::BconeOption ()
{
   Option::name  = "bcone";
   len = 100.0;
   rin = 1.0;
   rout = 2.0;
   rbend = 50.0;
   loss = 1.0;
   temperature = 21.0;
   humidity = 80.0;
   CO2 = 382.0;
} /* constructor */

int CommandLineParser::BconeOption::Parse (char** argv)
{
   argc += ParseArg (len, argv+argc);
   argc += ParseArg (rin, argv+argc);
   argc += ParseArg (rout, argv+argc);
   argc += ParseArg (rbend, argv+argc);
   argc += ParseArg (loss, argv+argc);
   argc += ParseArg (temperature, argv+argc);
   argc += ParseArg (humidity, argv+argc);
   argc += ParseArg (CO2, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::BconeOption::Help (ostream& hout)
{
   hout << "   -bcone len rin rout rbend loss temperature humidity CO2 cone bend section (id=6)";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
   hout << "      double len              [cm]      length                                  ";
   if (argc) hout << '(' << len  << " cm" << ')', argc--;
   else      hout << "(100.0 cm)";
   hout << '\n';
   hout << "      double rin              [cm]      input radius                            ";
   if (argc) hout << '(' << rin  << " cm" << ')', argc--;
   else      hout << "(1.0 cm)";
   hout << '\n';
   hout << "      double rout             [cm]      output radius                           ";
   if (argc) hout << '(' << rout  << " cm" << ')', argc--;
   else      hout << "(2.0 cm)";
   hout << '\n';
   hout << "      double rbend            [cm]      bend radius                             ";
   if (argc) hout << '(' << rbend  << " cm" << ')', argc--;
   else      hout << "(50.0 cm)";
   hout << '\n';
   hout << "      double loss                       local loss factor                       ";
   if (argc) hout << '(' << loss << ')', argc--;
   else      hout << "(1.0)";
   hout << '\n';
   hout << "      double temperature      [degC]    local temperature                       ";
   if (argc) hout << '(' << temperature  << " degC" << ')', argc--;
   else      hout << "(21.0 degC)";
   hout << '\n';
   hout << "      double humidity         [%]       local relative humidity                 ";
   if (argc) hout << '(' << humidity  << " %" << ')', argc--;
   else      hout << "(80.0 %)";
   hout << '\n';
   hout << "      double CO2              [ppm]     local Mol fraction of CO2               ";
   if (argc) hout << '(' << CO2  << " ppm" << ')', argc--;
   else      hout << "(382.0 ppm)";
   hout << '\n';
} /* Help */

CommandLineParser::PressureOption::PressureOption ()
{
   Option::name  = "pressure";
   value = 101.325;
} /* constructor */

int CommandLineParser::PressureOption::Parse (char** argv)
{
   argc += ParseArg (value, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::PressureOption::Help (ostream& hout)
{
   hout << "   -pressure double           [kPa]     athmospheric pressure                   ";
   if (argc) argc--; hout << '(' << value  << " kPa" << ')' << '\n';
} /* Help */

CommandLineParser::ModesOption::ModesOption ()
{
   Option::name  = "modes";
   value = 1;
} /* constructor */

int CommandLineParser::ModesOption::Parse (char** argv)
{
   argc += ParseArg (value, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::ModesOption::Help (ostream& hout)
{
   hout << "   -modes int                           number of modes to calculate\n";
   hout << "                                         0   = Mapes Riordan (fastest)\n";
   hout << "                                         1   = plane wave (fast, accurate)\n";
   hout << "                                         2-3 = efficient multimode\n";
   hout << "                                         4-n = very slow multimode              ";
   if (argc) argc--; hout << '(' << value << ')' << '\n';
} /* Help */

CommandLineParser::BendsOption::BendsOption ()
{
   Option::name  = "bends";
   value = 1;
} /* constructor */

int CommandLineParser::BendsOption::Parse (char** argv)
{
   argc += ParseArg (value, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::BendsOption::Help (ostream& hout)
{
   hout << "   -bends int                           enforce calculation of radial modes\n";
   hout << "                                         0   = ignore radial modes              ";
   if (argc) argc--; hout << '(' << value << ')' << '\n';
} /* Help */

CommandLineParser::AccumulateOption::AccumulateOption ()
{
   Option::name  = "accumulate";
} /* constructor */

void CommandLineParser::AccumulateOption::Help (ostream& hout)
{
   hout << "   -accumulate                          enforce matrix accumulation             ";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
} /* Help */

CommandLineParser::LosslessOption::LosslessOption ()
{
   Option::name  = "lossless";
} /* constructor */

void CommandLineParser::LosslessOption::Help (ostream& hout)
{
   hout << "   -lossless                            use lossless models                     ";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
} /* Help */

CommandLineParser::SimplifiedOption::SimplifiedOption ()
{
   Option::name  = "simplified";
} /* constructor */

void CommandLineParser::SimplifiedOption::Help (ostream& hout)
{
   hout << "   -simplified                          use simplified method                   ";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
} /* Help */

CommandLineParser::AccuracyOption::AccuracyOption ()
{
   Option::name  = "accuracy";
   value = 0.5;
} /* constructor */

int CommandLineParser::AccuracyOption::Parse (char** argv)
{
   argc += ParseArg (value, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::AccuracyOption::Help (ostream& hout)
{
   hout << "   -accuracy double           [%]       slicing accuracy for cone and bessel sections";
   if (argc) argc--; hout << '(' << value  << " %" << ')' << '\n';
} /* Help */

CommandLineParser::ColumnsOption::ColumnsOption ()
{
   Option::name  = "columns";
   value = 1;
} /* constructor */

int CommandLineParser::ColumnsOption::Parse (char** argv)
{
   argc += ParseArg (value, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::ColumnsOption::Help (ostream& hout)
{
   hout << "   -columns int                         number of returned complex data points per frequency\n";
   hout << "                                         0 = composite wave\n";
   hout << "                                         1 = plane wave only\n";
   hout << "                                         n = diagonal impedance modes only (if n<=modes)\n";
   hout << "                                         N = complete square matrix (if N>modes)";
   if (argc) argc--; hout << '(' << value << ')' << '\n';
} /* Help */

CommandLineParser::RadiationfileOption::RadiationfileOption ()
{
   Option::name  = "radiationfile";
   columns = 1;
} /* constructor */

int CommandLineParser::RadiationfileOption::Parse (char** argv)
{
   argc += ParseArg (filename, argv+argc);
   argc += ParseArg (columns, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::RadiationfileOption::Help (ostream& hout)
{
   hout << "   -radiationfile filename columns      specify radiation impedance, calculate input impedance";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
   hout << "      char* filename                    name of multi column blank separated text file \n";
   hout << "                                         freq mag_0 arg_0 mag_1 arg_1...\n      ";
   if (argc) hout << '(' << filename << ')', argc--;
   hout << '\n';
   hout << "      int columns                       number of data points per frequency\n";
   hout << "                                         1 = plane wave only\n";
   hout << "                                         n = diagonal impedance modes only (if n<=modes)\n";
   hout << "                                         N = complete square matrix (if N>modes)";
   if (argc) hout << '(' << columns << ')', argc--;
   else      hout << "(1)";
   hout << '\n';
} /* Help */

CommandLineParser::ImpedancefileOption::ImpedancefileOption ()
{
   Option::name  = "impedancefile";
   columns = 1;
} /* constructor */

int CommandLineParser::ImpedancefileOption::Parse (char** argv)
{
   argc += ParseArg (filename, argv+argc);
   argc += ParseArg (columns, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::ImpedancefileOption::Help (ostream& hout)
{
   hout << "   -impedancefile filename columns      specify input impedance, calculate radiation impedance";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
   hout << "      char* filename                    name of multi column blank separated text file \n";
   hout << "                                         freq mag_0 arg_0 mag_1 arg_1...\n      ";
   if (argc) hout << '(' << filename << ')', argc--;
   hout << '\n';
   hout << "      int columns                       number of data points per frequency\n";
   hout << "                                         1 = plane wave only\n";
   hout << "                                         n = diagonal impedance modes only (if n<=modes)\n";
   hout << "                                         N = complete square matrix (if N>modes)";
   if (argc) hout << '(' << columns << ')', argc--;
   else      hout << "(1)";
   hout << '\n';
} /* Help */

CommandLineParser::InteriorwaveOption::InteriorwaveOption ()
{
   Option::name  = "interiorwave";
   columns = 1;
   radius = 0;
   arg = 0;
} /* constructor */

int CommandLineParser::InteriorwaveOption::Parse (char** argv)
{
   argc += ParseArg (impedancefile, argv+argc);
   argc += ParseArg (columns, argv+argc);
   argc += ParseArg (elementname, argv+argc);
   argc += ParseArg (radius, argv+argc);
   argc += ParseArg (arg, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::InteriorwaveOption::Help (ostream& hout)
{
   hout << "   -interiorwave impedancefile columns elementname radius arg calculate interior wave quantities p, u, z right of specified element";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
   hout << "      char* impedancefile               name of multi column blank separated text file \n";
   hout << "                                         freq mag_0 arg_0 mag_1 arg_1...\n      ";
   if (argc) hout << '(' << impedancefile << ')', argc--;
   hout << '\n';
   hout << "      int columns                       number of data points per frequency\n";
   hout << "                                         1 = plane wave only\n";
   hout << "                                         n = diagonal impedance modes only (if n<=modes)\n";
   hout << "                                         N = complete square matrix (if N>modes)";
   if (argc) hout << '(' << columns << ')', argc--;
   else      hout << "(1)";
   hout << '\n';
   hout << "      char* elementname                 name of horn element to calculate wave quantities at ";
   if (argc) hout << '(' << elementname << ')', argc--;
   hout << '\n';
   hout << "      double radius           [m]       distance from element axis in radial dimension (or first coordinate)";
   if (argc) hout << '(' << radius  << " m" << ')', argc--;
   else      hout << "(0 m)";
   hout << '\n';
   hout << "      double arg              [rad|m]   angular distance from horizontal plane (or second coordinate)";
   if (argc) hout << '(' << arg  << " rad|m" << ')', argc--;
   else      hout << "(0 rad|m)";
   hout << '\n';
} /* Help */

CommandLineParser::TerminationOption::TerminationOption ()
{
   Option::name  = "termination";
   value = 1;
} /* constructor */

int CommandLineParser::TerminationOption::Parse (char** argv)
{
   argc += ParseArg (value, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::TerminationOption::Help (ostream& hout)
{
   hout << "   -termination int                     termination mode\n";
   hout << "                                         0..reflecting\n";
   hout << "                                         1..Levine/Zorumsky                     ";
   if (argc) argc--; hout << '(' << value << ')' << '\n';
} /* Help */

CommandLineParser::FrequenciesOption::FrequenciesOption ()
{
   Option::name  = "frequencies";
   f0 = 10.0;
   f1 = 1800.0;
   df = 10.0;
   refine = 0;
   target = 0.00001;
} /* constructor */

int CommandLineParser::FrequenciesOption::Parse (char** argv)
{
   argc += ParseArg (f0, argv+argc);
   argc += ParseArg (f1, argv+argc);
   argc += ParseArg (df, argv+argc);
   argc += ParseArg (refine, argv+argc);
   argc += ParseArg (target, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::FrequenciesOption::Help (ostream& hout)
{
   hout << "   -frequencies f0 f1 df refine target  specify frequency grid (do not use input stream)";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
   hout << "      double f0               [Hz]      starting frequency                      ";
   if (argc) hout << '(' << f0  << " Hz" << ')', argc--;
   else      hout << "(10.0 Hz)";
   hout << '\n';
   hout << "      double f1               [Hz]      stop frequency                          ";
   if (argc) hout << '(' << f1  << " Hz" << ')', argc--;
   else      hout << "(1800.0 Hz)";
   hout << '\n';
   hout << "      double df               [Hz]      frequency increment                     ";
   if (argc) hout << '(' << df  << " Hz" << ')', argc--;
   else      hout << "(10.0 Hz)";
   hout << '\n';
   hout << "      int refine              [bool]    refine peaks                            ";
   if (argc) hout << '(' << refine  << " bool" << ')', argc--;
   else      hout << "(0 bool)";
   hout << '\n';
   hout << "      double target           [%]       accuracy target                         ";
   if (argc) hout << '(' << target  << " %" << ')', argc--;
   else      hout << "(0.00001 %)";
   hout << '\n';
} /* Help */

CommandLineParser::RunOption::RunOption ()
{
   Option::name  = "run";
} /* constructor */

int CommandLineParser::RunOption::Parse (char** argv)
{
   argc += ParseArg (filename, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::RunOption::Help (ostream& hout)
{
   hout << "   -run filename                        run complete script file                ";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
   hout << "      char* filename                    name of script file to execute          ";
   if (argc) hout << '(' << filename << ')', argc--;
   hout << '\n';
} /* Help */

CommandLineParser::CapabilitiesOption::CapabilitiesOption ()
{
   Option::name  = "capabilities";
} /* constructor */

void CommandLineParser::CapabilitiesOption::Help (ostream& hout)
{
   hout << "   -capabilities                        list library capabilities               ";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
} /* Help */

CommandLineParser::DebugOption::DebugOption ()
{
   Option::name  = "debug";
} /* constructor */

void CommandLineParser::DebugOption::Help (ostream& hout)
{
   hout << "   -debug                               debug output to statistics              ";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
} /* Help */

CommandLineParser::StatOption::StatOption ()
{
   Option::name  = "stat";
} /* constructor */

int CommandLineParser::StatOption::Parse (char** argv)
{
   argc += ParseArg (value, argv+argc);
   if (argc) flags &= ~DEFAULT;
   return argc;
} /* Parse */

void CommandLineParser::StatOption::Help (ostream& hout)
{
   hout << "   -stat char*                          write program statistics to file        ";
   if (argc) hout << '(' << value << ')', argc--;
   else if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
} /* Help */

CommandLineParser::SwapOption::SwapOption ()
{
   Option::name  = "swap";
} /* constructor */

void CommandLineParser::SwapOption::Help (ostream& hout)
{
   hout << "   -swap                                exchange adjacent bytes of values       ";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
} /* Help */

CommandLineParser::HelpOption::HelpOption ()
{
   Option::name  = "help";
} /* constructor */

void CommandLineParser::HelpOption::Help (ostream& hout)
{
   hout << "   -? | -help                           generates this help screen              ";
   if (IsOn (*this)) hout << "<on>";
   else if (IsDef (*this)) hout << "<act>";
   else hout << "<off>";
   hout << '\n';
} /* Help */

                                                                                           //F//
CommandLineParser::CommandLineParser ()               // Init CommandLineParser class
{
   argc  = 0;                                         // Number of args parsed
   first = DelayPointer ((Option*) 0);                // Get head of linked option list
} /* constructor */                                                                        //G//

int CommandLineParser::init (char **argv)             // Parse command line arguments
{
   int i = 0;                                         // Argument counter

   try {                                              // Parsing errors are catched
      pgmname = argv[i++];                            // Get the program name

#if defined (USES_STDIN) || defined (USES_STDOUT)     // May this program be used in a pipe?
      char *name = pgmname;                           // Get program name
      char *pos1 = strrchr (name, '\\');              // Get last occurance of '\'
      char *pos2 = strrchr (name, '/');               // Get last occurance of '/'
      if (pos1 || pos2)                               // Path in program name?
         name = pos1 > pos2 ? pos1+1 : pos2+1;        // Remove path specifier from program name
      Error::ProgramName (name);                      // Set program name used in error outputs
#endif

      // Option parser
      Option *p;
      while (argv[i] && IsOption (argv[i])) {         // Are there options given?
         Option *o = 0;
         for (p = first; p; p = p->Next())            // Search for given option
            if (p->Exact(argv[i])) {                  // Option found?
               o = p;  break;                         // Save Pointer
            } /* endif */
         if (!o)                                      // No exact option match?
            for (p = first; p; p = p->Next())         // Search for given option
               if (p->Match(argv[i])) {               // Option found?
                  if (o) throw Error1 << argv[i];     // Ambiguous option
                  o = p;                              // Save pointer
               } /* endif */
         if (!o) throw Error2 << argv[i];             // Invalid option
         i += o->Init(i, argv+i);                     // Common option initializer
         i += o->Parse(argv+i);                       // Parse additional option arguments
      } /* endwhile */

      // Argument parser
      i += Parse (argv+i);                            // Parse command line arguments

      if (argv[i])                                    // Any further argument?
         throw IsOption (argv[i]) ?                   // Select error
            Error4 :                                  // Options must be grouped first
            Error3 << argv[i];                        // Additional command line parameter

      // Adjust parsed values
      Adjust ();                                      // Adjust parsed values

      // Output help message
      if (IsDef (help)) {
         ostream& hout = IsOn(help) ? cerr : HLPOUT;  // Redirect hout if necessary
         Help (hout);                                 // Print global help message
         for (p = first; p; p = p->Next()) {          // Generate help message for each option
            p->Help (hout);                           // Print help message
         } /* endfor */
         hout . flush ();                             // Be shure help screen is displayed
         hout << '\n';                                // Seperate help from further output
         if (!IsOn (help)) exit (0);                  // Exit after help?
      } /* endif */

      // Validity check
      for (p = first; p; p = p->Next()) {             // Check each option for errors
         p->Valid(), p->Invalid();                    // Generate exception if failure
      } /* endfor */
      Valid (), Invalid ();                           // Global validity checks
   } catch (Error& Error) {                           // Catch errors occured during parsing
      Error.Handle ();
   } /* endtry */

   return i;
} /* init */

int CommandLineParser::Parse (char **argv)            // Additional command line arguments parser
{
   argv = argv;                                       // Supress unused argument warning
   return argc;                                       // Return number of args parsed      //H//
} /* Parse */

void CommandLineParser::Help (ostream& hout)          // Global help message
{
   hout << "TAP - Transfer Analysis Package - Acoustic Research Tool - Version 1.0" << '\n';
   hout << "Copyright (c) W. Kausel, IWK MDW-Vienna 2009. All rights reserved." << '\n';
   hout << '\n';
   hout << "Sample main program demonstrating some of the capabilities of ART" << '\n';
   hout << " using a command line interface. All acoustic elements can be tested" << '\n';
   hout << " individually or in sophisticated systems described by script.\n" << '\n';
   hout << " Receives frequencies as single column input stream with double values in binary format" << '\n';
   hout << " and creates triple column output stream consisting of (frequency, magnitude, argument)" << '\n';
   hout << " triples. Alternatively start frequency, stop frequency and frequency increment can be" << '\n';
   hout << " given as command line option.\n" << '\n';
   hout << '\n';
   hout << "Usage: art [options] < infile > outfile " << '\n';
   hout << '\n';
   hout << "Options:" << '\n';
} /* Help */                                                                               //I//

void CommandLineParser::Valid ()                      // Gobal validity check
{
} /* Valid */                                                                              //J//

void CommandLineParser::Invalid ()                    // Global invalidity check
{
   if (IsDef (borejump) +
IsDef (cylinder) +
IsDef (cone) +
IsDef (bessel) +
IsDef (list) +
IsDef (bcylinder) +
IsDef (bcone) +
IsDef (capabilities) +
IsDef (run) > 1) throw Error8;
   if (IsDef (borejump) +
IsDef (cylinder) +
IsDef (cone) +
IsDef (bessel) +
IsDef (list) +
IsDef (bcylinder) +
IsDef (bcone) +
IsDef (capabilities) +
IsDef (run) < 1) throw Error9;
   if (IsDef (radiationfile) +
IsDef (impedancefile) +
IsDef (frequencies) > 1) throw Error10;
} /* Invalid */                                                                            //K//

void CommandLineParser::Adjust ()                     // Adjust parsed values
{
} /* Adjust */                                                                             //M//


#endif /* MAIN */
#endif /* CMDLINE_PARSER_INCLUDED */


