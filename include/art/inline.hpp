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
 *  Title:    Inline Functions
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
 *  Some commonly used inline functions.
 *
\******************************************************************************/
#ifndef INLINE_FUNCTIONS_INCLUDED
#define INLINE_FUNCTIONS_INCLUDED


/******************************************************************************/
/*                            Needed Include Files                            */
/******************************************************************************/
#include <cstdlib>
#include <cstring>
#include <cmath>

#include <ios>
#include <iosfwd>


#define arrsize(arr) (sizeof(arr)/sizeof(arr[0]))


/******************************************************************************/
/*    Inline Functions, Forwarded Function Definitions, Template Functions    */
/******************************************************************************/
template<class T>
inline T xchg(T& a, T& b) { T c = a; a = b; return c; }

template<class T>
bool odd (T x) { return (x & 1) != 0; }

template<class T>
bool even (T x) { return (x & 1) == 0; }

template<class T>
inline unsigned bitcnt (T& val)
{
    static unsigned char bits[] =
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
	n += bits [((unsigned char*)&val)[i]];
    return n;
}


inline double rint (double x) throw ()
{
    return floor (x+0.5);
}

inline double rint (double x, double r) throw ()
{
    return floor ((x+0.5)/r) * r;
}


inline int OpenCheck (std::ios& io)
{
    if (!io) throw OpErr;
    return 1;
}

inline int OpenCheck (std::ios& io, const char* n)
{
    if (!io) throw OpErr << n;
    return 1;
}

inline int EofCheck (std::istream& i)
{
    if (i.eof ()) return 1;
    if (!i) throw RdErr;
    return 0;
}

inline int EofCheck (std::istream& i, const char* n)
{
    if (i.eof ()) return 1;
    if (!i) throw RdErr << n;
    return 0;
}

inline int Checked (std::istream& i)
{
    if (i.eof ()) throw EofErr;
    if (!i) throw RdErr;
    return 1;
}

inline int Checked (std::istream& i, const char* n)
{
    if (i.eof ()) throw EofErr << n;
    if (!i) throw RdErr << n;
    return 1;
}

inline int Checked (std::ostream& o)
{
    if (o.eof ()) throw EofErr;
    if (!o) throw WrErr;
    return 1;
}

inline int Checked (std::ostream& o, const char* n)
{
    if (o.eof ()) throw EofErr << n;
    if (!o) throw WrErr << n;
    return 1;
}

inline int SYSTEM (const char* cmd)
{
    if (system (cmd))
    {
	throw SysErr << cmd;
    } /* endif */
    return 0;
}

inline void bswap (char* dst, char* src, int n)
{
    memcpy (dst, src, n);
    for (src = dst, dst += n-1, n/=2; n; n--, src++, dst--)
    {
	char c = *src; *src = *dst; *dst = c;
    } /* endfor */
}


#endif /* INLINE_FUNCTIONS_INCLUDED */
