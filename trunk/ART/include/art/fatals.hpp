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
 *  Title:    Fatal Errors
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
\******************************************************************************/
#ifndef FATAL_ERRORS_INCLUDED
#define FATAL_ERRORS_INCLUDED

#include "error.hpp"

extern FatalError RdErr;
extern FatalError WrErr;
extern FatalError OpErr;
extern FatalError EofErr;
extern FatalError SysErr;
extern FatalError MemErr;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * The following definitions may be included only once into your project!
 *
 * You should include it into the source file were your main procedure is declared.
 * To include it, define MAIN before the statement, which includes this source file.
 */
#ifdef MAIN

FatalError OpErr  ("error opening file %1");
FatalError WrErr  ("error writing output stream %1");
FatalError RdErr  ("error reading input stream %1");
FatalError EofErr ("stream size mismatch");
FatalError SysErr ("command exec failure: %1");
FatalError MemErr ("error allocating memory");

#endif /* MAIN */
#endif /* FATAL_ERRORS_INCLUDED */
