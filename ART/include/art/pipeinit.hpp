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
 *  The pipe classes allow the connection of any stream to any predefined
 *  input or output stream (stdin, stdout, stderr), any inherited, opened
 *  input, output, or bidirectional stream or to arbitrary files.  This is
 *  controlled via command line arguments and/or over the argv[0] argument
 *  (the program name/path) from the calling shell.
 *
 *  The ShellInfoParser class interprets the information which descriptors
 *  are inherited from the calling shell.  The tap shell sets them up and
 *  puts their id's into the argv[0] argument.
 *
\******************************************************************************/
#ifndef PIPE_INIT_INCLUDED
#define PIPE_INIT_INCLUDED


/******************************************************************************/
/*                            Needed Include Files                            */
/******************************************************************************/
#include <iosfwd>

#if defined(_WIN32)
   #include <io.h>
   #include <fcntl.h>
#else
   #include <unistd.h>
#endif

#include "token.h"
#include "autoinit.hpp"

#if defined(_WIN32)
   #define set_binary(fd) _setmode((fd), _O_BINARY)
   #define isatty _isatty
#else
   #define set_binary(fd)
#endif


/******************************************************************************/
/*                             Class Declarations                             */
/******************************************************************************/
class ShellInfoParser : public AutoPgmInit       // automatically init this at pgm startup
{
    class PgmHandleList                          // container for program handle table
    {
	short  len;                              // length of handle table
	short *tab;                              // pointer to begin of handle table

    protected:
	PgmHandleList() : len(0), tab(0) {}
       ~PgmHandleList() { delete[] tab; }        // remove handle list

	int parse (char*);                       // parse the handle list

    public:
	int operator[] (int i)                   // return a value of array
	{
	    return (i < 0 || i >= len) ? -1 : tab[i];
	}

	friend class ShellInfoParser;            // this class may access the functions above
    };

protected:
    int auto_init (int, char**, char**);         // parse command line parameters

public:
    int   pid;                                   // process id
    int   shpid;                                 // process id of shell
    token name;                                  // program name
    token shell;                                 // shell program name

    PgmHandleList in;                            // table of inherited input pipe handles
    PgmHandleList out;                           // table of inherited output pipe handles
    PgmHandleList io;                            // table of inherited bidirectional handles

    ShellInfoParser() : pid(0), shpid(0), name(0), shell(0) {}
};


/******************************************************************************/
/*                                Definitions                                 */
/******************************************************************************/
extern TOKEN T_STDIN;
extern TOKEN T_STDOUT;
extern TOKEN T_STDERR;

extern ShellInfoParser pgm;                      // global program information block


/******************************************************************************/
/*                       Class Declarations - Continued                       */
/******************************************************************************/
class pipe
{
protected:
    short index;                                 // index of stream in input pipe table
    token name;                                  // name of file to open

    pipe() { index = -1, name = 0; }             // init member vars

    void parse (const char *s)                   // parse name to resolve where to attach a stream
    {
	name  = unique(s);
	index = name[0] == ':' ? atoi(name+1) : -1;
    }

    short std_handle_or (short h)
    {
        return name == T_STDIN  ? 0 :
               name == T_STDOUT ? 1 :
               name == T_STDERR ? 2 : h;
    }

public:
    friend std::istream& operator>> (std::istream& i, pipe& p)
    {
	char s[256];

	i >> s;
	p.parse(s);
	return i;
    }

    friend std::ostream& operator<< (std::ostream& o, const pipe& p)
    {
        return o << p.name;
    }
    
    operator const char *() { return name; }
};

class ipipe : public pipe                        // info about an input stream into program
{
public:
    ipipe() {}                                   // construct empty
    ipipe(const char *s) { parse(s); }           // construct with name parsing

    ipipe& operator= (const char *s) { parse(s); return *this; }

    operator short() { return std_handle_or (pgm.in[index]); }
};

class opipe : public pipe                        // info about an output stream of program
{
public:
    opipe() {}                                   // construct empty
    opipe(const char *s) { parse(s); }           // construct with name parsing

    opipe& operator= (const char *s) { parse(s); return *this; }

    operator short() { return std_handle_or (pgm.out[index]); }
};

class iopipe : public pipe                       // info about an iostream of program
{
public:
    iopipe() {}                                  // construct empty
    iopipe(const char *s) { parse(s); }          // construct with name parsing

    iopipe& operator= (const char *s) { parse(s); return *this; }

    operator short() { return std_handle_or (pgm.io[index]); }
};


/******************************************************************************/
/*    Inline Functions, Forwarded Function Definitions, Template Functions    */
/******************************************************************************/
inline int attach_stream (std::ios& t, short h)
{
    switch (h)
    {
    case 0:
	t.rdbuf (std::cin.rdbuf());
        break;

    case 1:
	t.rdbuf (std::cout.rdbuf());
        break;

    case 2:
	t.rdbuf (std::cerr.rdbuf());
        break;

    default:
        if (h < 0)
            return -1;
    }
    set_binary(h);
    return h;
}

inline int& map_handle (std::ios& t)
{
    void *rdbuf = (void*) t.rdbuf();

    static size_t xtop = 0;
    static struct cross_reference
    {
        void *key;
        int h;
    } *xref = 0;

    unsigned int n = 0, l = 0, u = xtop;

    /* binary search for an existing token */
    while (l != u)
    {
	n = (l+u)/2;
	if (xref[n].key < rdbuf) u = n; else if (xref[n].key > rdbuf) l = n+1; else
            goto found;
    }

    /* reallocate if necessary */
    if (xtop % 32 == 0)
	xref = (struct cross_reference*) realloc (xref, (xtop + 32) * sizeof *xref);

    /* create a new token at the position given by l (or u) */
    for (n = xtop++; n > l; n--)
	xref[n] = xref[n-1];
    xref[n].key = rdbuf, xref[n].h = -1;

  found:
    return xref[n].h;
}

template<class T>
inline bool isatty (T& t)
{
    return isatty (map_handle(t)) > 0;
}

template<class T>
inline T& operator<= (T& i, ipipe& p)
{
    int h = attach_stream (i, p);
    
    if (h < 0)
    {
	const char *name = p;
	i.open (name);
    }
    map_handle(i) = h;
    return i;
}

template<class T>
inline T& operator<= (T& o, opipe& p)
{
    int h = attach_stream (o, p);
    
    if (h < 0)
    {
	const char *name = p;
	o.open (name);
    }
    map_handle(o) = h;
    return o;
}

template<class T>
inline T& operator<= (T& io, iopipe& p)
{
    int h = attach_stream (io, p);
    
    if (h < 0)
    {
	const char *name = p;
	io.open (name);
    }
    map_handle(io) = h;
    return io;
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * The following definitions may be included only once into your project!
 *
 * You should include it into the source file were your main procedure is declared.
 * To include it, define MAIN before the statement, which includes this source file.
 */
#ifdef MAIN


/******************************************************************************/
/*                              Static Objects                                */
/******************************************************************************/
TOKEN T_STDIN  = "stdin";
TOKEN T_STDOUT = "stdout";
TOKEN T_STDERR = "stderr";

ShellInfoParser pgm;


/******************************************************************************/
/*                                 Functions                                  */
/******************************************************************************/
int ShellInfoParser::PgmHandleList::parse (char *list)
{
    int i;

    for (len=1, i=0; *list; i++)                 // Count number of integers in list
	if (list[i] == ',')                      // List elements are seperated with ','
	    list[i]=' ', len++;                  // Increment count and remove ','

    tab = new short[len];                        // Create table
    std::istringstream s (list);                 // Initialize a string stream with parameter

    for (i=0; i<len; i++)                        // Handle all list elements
	s >> tab[i];                             // Convert the handle
    return len;
}

int ShellInfoParser::auto_init (int argc, char** argv, char** envp)
{
#ifdef __NEVER__
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
#endif
    return 0;                                          // No errors
}

#endif /* MAIN */
#endif /* PIPE_INIT_INCLUDED */
