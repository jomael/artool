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
 *  Title:    Binary File Streams
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
 *  Communication between simulation applications is made over files or pipes.
 *  To avoid to convert the signals twice from binary to ascii and vice versa
 *  when communicating over the pipes, the signals are put untranslated into
 *  the streams with the help of the classes i-, o-, and iobinary.
 *
\******************************************************************************/
#ifndef BINARY_INCLUDED
#define BINARY_INCLUDED


/******************************************************************************/
/*                                Definitions                                 */
/******************************************************************************/
#ifndef FLUSHSIZE
#define FLUSHSIZE 16384
#endif


/******************************************************************************/
/*                            Needed Include Files                            */
/******************************************************************************/
#include <ios>
#include <iosfwd>
#include <fstream>


/******************************************************************************/
/*                            Class Definitions                               */
/******************************************************************************/
class ibinary : public std::ifstream              // binary input stream
{
    typedef std::ios_base::openmode mode;

    bool swapme;

    void init() { swapme = false; }

public:
    ibinary () { init(); }

#define defmode  std::ios::binary | std::ios::in

    ibinary (const char* s)         : std::ifstream (s, defmode)     { init(); }
    ibinary (const char* s, mode m) : std::ifstream (s, defmode | m) { init(); }

    void open (const char* s)         { std::ifstream::open (s, defmode);     init(); }
    void open (const char* s, mode m) { std::ifstream::open (s, defmode | m); init(); }

#undef defmode

    bool swap ()       { return swapme; }         // get file swapping mode
    bool swap (bool m) { return xchg(swapme,m); } // switch byte swapping mode on or off
};

class obinary : public std::ofstream              // binary output stream
{
    typedef std::ios_base::openmode mode;

    bool swapme;
    unsigned int flushme;

    void init() { swapme = false, flushme = 0; }

public:
    obinary () { init(); }

#define defmode  std::ios::binary | std::ios::out

    obinary (const char* s)         : std::ofstream (s, defmode)     { init(); }
    obinary (const char* s, mode m) : std::ofstream (s, defmode | m) { init(); }

    void open (const char* s)         { std::ofstream::open (s, defmode);     init(); }
    void open (const char* s, mode m) { std::ofstream::open (s, defmode | m); init(); }

#undef defmode

    bool swap ()       { return swapme; }         // get file swapping mode
    bool swap (bool m) { return xchg(swapme,m); } // switch byte swapping mode on or off

    bool flush (unsigned int written)             // flush the output file?
    {
        if ((flushme += written) >= FLUSHSIZE)
        {
            flushme %= FLUSHSIZE;
	    std::ofstream::flush();
            return true;
        }
        return false;
    }
};

class iobinary : public std::fstream              // binary input and output stream
{
    typedef std::ios_base::openmode mode;

    bool swapme;
    unsigned int flushme;

    void init() { swapme = false, flushme = 0; }

public:
    iobinary () { init(); }

#define defmode  std::ios::binary | std::ios::in | std::ios::out

    iobinary (const char* s)         : std::fstream (s, defmode)      { init(); }
    iobinary (const char* s, mode m) : std::fstream (s, defmode | m)  { init(); }

    void open (const char* s)         { std::fstream::open (s, defmode);     init(); }
    void open (const char* s, mode m) { std::fstream::open (s, defmode | m); init(); }

#undef defmode

    bool swap ()       { return swapme; }         // get file swapping mode
    bool swap (bool m) { return xchg(swapme,m); } // switch byte swapping mode on or off

    bool flush (unsigned int written)             // flush the output file?
    {
        if ((flushme += written) >= FLUSHSIZE)
        {
            flushme %= FLUSHSIZE;
	    std::fstream::flush();
            return true;
        }
        return false;
    }
};


/******************************************************************************/
/*    Inline Functions, Forwarded Function Definitions, Template Functions    */
/******************************************************************************/
template<class T>
ibinary& operator>> (ibinary& i, T& t)
{
    if (i.swap())
    {
        char buf [sizeof (T)];

        i.read (buf, sizeof (T));
        bswap ((char*)&t, buf, sizeof (T));
    }
    else
    {
        i.read ((char*)&t, sizeof (T));
    }
    return i;
}

template<class T>
obinary& operator<< (obinary& o, T t)
{
    if (o.swap())
    {
        char buf [sizeof (T)];

        bswap (buf, (char*)&t, sizeof (T));
        o.write (buf, sizeof (T));
    }
    else
    {
        o.write ((char*)&t, sizeof (T));
    }
    o.flush (sizeof (T));
    return o;
}

template<class T>
iobinary& operator>> (iobinary& io, T& t)
{
    if (io.swap())
    {
        char buf [sizeof (T)];

        io.read (buf, sizeof (T));
        bswap ((char*)&t, buf, sizeof (T));
    }
    else
    {
        io.read ((char*)&t, sizeof (T));
    }
    return io;
}

template<class T>
iobinary& operator<< (iobinary& io, T t)
{
    if (io.swap())
    {
        char buf [sizeof (T)];

        bswap (buf, (char*)&t, sizeof (T));
        io.write (buf, sizeof (T));
    }
    else
    {
        io.write ((char*)&t, sizeof (T));
    }
    io.flush (sizeof (T));
    return io;
}

#endif /* BINARY_INCLUDED */
