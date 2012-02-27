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
 *  Title:    Collect Statistical Informations
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
 ******************************************************************************
 *
 *  Description
 *  -----------
 *
 *  A subclass of the stringstream class where statistical informations of the
 *  program are collected during the run of the application.  When the class
 *  is desctructed, it writes the remembered strings to an output stream.
 *
\******************************************************************************/
#ifndef STATISTICS_INCLUDED
#define STATISTICS_INCLUDED


/******************************************************************************/
/*                            Needed Include Files                            */
/******************************************************************************/
#include <fstream>
#include <sstream>

#if defined(_WIN32)
   #include <time.h>
#endif /* _WIN32 */


/******************************************************************************/
/*                            Class Definitions                               */
/******************************************************************************/
class Statistic : public std::stringstream        // generate statistics
{
   time_t   start;                                // initialization time of this class
   clock_t  cputime;                              // cpu time counter
   char    *oname;                                // write statistics to this file
   std::ostream *pout;                            // write statistics to this stream pointer

public:
  ~Statistic ();                                  // generate statistical information
   Statistic () { oname = 0; pout = 0; time (&start); cputime = clock (); }

   void attach (char* n)         { oname = n; }   // attach statistics to this file
   void attach (std::ostream& o) { pout = &o; }   // attach statistics to this stream
};


/*
 * The following definitions may be included only once into your project!
 *
 * You should include it into the source file were your main procedure is declared.
 * To include it, define MAIN before the statement, which includes this source file.
 */
#ifdef MAIN


/******************************************************************************/
/*                                 Functions                                  */
/******************************************************************************/
Statistic::~Statistic ()                          // generate statistics
{
   if (oname)                                     // Append to file?
       pout = new std::ofstream (oname, std::ios::app);

   if (pout) {                                    // Statistic attached to stream?
      char buffer [256];                          // line buffer
      time_t stop;                                // stop time
      time (&stop);                               // get current time

      // write program statistics
      if (Error::Count())
          *this << Error::Count() << " Error(s) occured\n";
      strftime (buffer, 256, "%d-%b-%Y %H:%M:%S", localtime (&start));
      *this << "Process started: " << buffer << '\n';
      strftime (buffer, 256, "%d-%b-%Y %H:%M:%S", localtime (&stop));
      *this << "Process stopped: " << buffer << '\n';
      *this << "CPU time: " << double(clock()-cputime) / CLOCKS_PER_SEC << " seconds";

      // copy statistics buffer to output
      while (getline (buffer, 256))
          *pout << buffer << '\n';
   } /* endif */

   if (oname)
      delete pout;                                // close constructed file
}

#endif /* MAIN */
#endif /* STATISTICS_INCLUDED */
