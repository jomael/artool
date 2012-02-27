/******************************************************************************\
 *                      _____  _    ____          _____
 *                     |_   _|/ \  |  _ \  __   _|___ /
 *                       | | / _ \ | |_) | \ \ / / |_ \
 *                       | |/ ___ \|  __/   \ V / ___) |
 *                       |_/_/   \_\_|       \_/ |____/
 *
 ******************************************************************************
 *
 *  Copyright (C) 2006       Herbert Nachtnebel
 *                           Research Unit for Integrated Sensor Systems
 *                           Austrian Academy of Science
 *
 ******************************************************************************
 *
 *  Project:  TAP - Transfer Analysis Package
 *
 *  Title:    Constant String Objects aka Tokens
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
 *  Tokens are strings stored uniquely in a binary sorted array.  Since
 *  they are unique, the pointer returned is a direct represention of the
 *  content and can be used to optimize comparisions or to avoid too
 *  complicated memory management.  Think of it as the hash of the content.
 *
 *  Three reasons to use them:
 *
 *    1) They make the code more readable since many string compares
 *       can be replaced with pointer compares;  the pointer is
 *       equivalent to the actual string contents.
 *
 *    2) They avoid strdup's and, successively, memory management when
 *       returning strings from functions.
 *
 *    3) When looking up new tokens is seldom and comparision is done
 *       often, the application will run faster since strcmp's can be
 *       replaced with more efficient pointer compares.  That's a huge
 *       advantage for interpreted languages like a lisp interpreter
 *       or even for a simple parser.
 *
 *
 *  This version should work on Windows using the MSVC compiler and on most
 *  POSIX systems using the gcc compiler.  On POSIX systems it is necessary
 *  to correctly place the .token section via this ld script:
 *
 *    SECTIONS
 *    {
 *        .rodata :
 *        {
 *            . = ALIGN(16) ;
 *            __beg_tokens = . ;
 *            *(.token)
 *            __end_tokens = . ;
 *        }
 *    }
 *
\******************************************************************************/
#ifndef TOKEN_INCLUDED
#define TOKEN_INCLUDED


/******************************************************************************/
/*                                 Definitions                                */
/******************************************************************************/
/*
 *  Some macros to place objects into specific sections.
 */
#if defined(__GNUC__)

#define TOKEN __attribute__((section(".token"))) const token
#define INIT_FUNC(f) __attribute__((section(".ctors"))) void (*f##_ptr)(void) = &f;

#elif defined(_MSC_VER)

#pragma section(".TOKEN$BEG",read)
#pragma section(".TOKEN$DAT",read)
#pragma section(".TOKEN$END",read)
#pragma comment(linker, "/merge:.TOKEN=.rdata")

#pragma section(".CRT$XCB")
#define TOKEN __declspec(allocate(".TOKEN$DAT")) const token
#define INIT_FUNC(f) __declspec(allocate(".CRT$XCB")) void (*f##_ptr)(void) = &f;

#define strdup _strdup

#else

#error "unspecified compiler and platform"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*
 *  The incredible complicated interface.
 */
typedef const char *token;
token unique (const char *s);


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * The following definitions may be included only once into your project!
 *
 * You should include it into the source file were your main procedure is declared.
 * To include it, define MAIN before the statement, which includes this source file.
 */
#ifdef MAIN

#include <stdlib.h>
#include <string.h>


/******************************************************************************/
/*                               Static Objects                               */
/******************************************************************************/
/*
 *  Labels placed around the ".token" section to iterate over it.
 */
#if defined(__GNUC__)
extern token __beg_tokens;
extern token __end_tokens;
#elif defined(_MSC_VER)
const token __declspec(allocate(".TOKEN$BEG")) __beg_tokens = NULL;
const token __declspec(allocate(".TOKEN$END")) __end_tokens = NULL;
#endif

/*
 *  Label to distinghuish between constant, initialised data and runtime 
 *  allocated objects.
 */
#if defined(__GNUC__)
extern const int __bss_start;
#elif defined(_MSC_VER)
#define __bss_start _pgmptr
#endif

/*
 *  unique
 *  ------
 *
 *    Create a unique token (string object).
 *
 *    Tokens are stored only once!  To assure this, a sorted list of tokens
 *    is kept.
 *
 *    Note!  Super simple implementation of a binary sorted array.  Insertion
 *           has bad O(N) worst case runtime and building the complete database
 *           thus O(N²), but it is seldom run and barely runtime critical.
 *           Hence, not worth for optimization.
 */
token
unique (const char *s)
{
    static token *xref = 0;
    static size_t xtop = 0;

    int c = 0;
    unsigned int n = 0, l = 0, u = xtop;

    /* binary search for an existing token */
    while (l != u)
    {
	n = (l+u)/2;
	c = strcmp (xref[n], s);
	if (c < 0) u = n; else if (c > 0) l = n+1; else
	{
	    return xref[n];
	}
    }

    /* create a new token at the position given by l (or u) */
    if (xtop % 32 == 0)
	xref = (token*) realloc ((void*) xref, (xtop + 32) * sizeof *xref);

    for (n = xtop++; n > l; n--)
	xref[n] = xref[n-1];

    /*
    * Note!  Attention, linker trick!  Don't duplicate string constants.
    *        Works, since the linker places all constant data before the
    *        __bss_start symbol (since they are initialized).
    */
    return xref[l] = s < (char*)&__bss_start ? s : strdup(s);
}
    
/*
 *  init_tokens
 *  -----------
 *
 *    Initialize all tokens found in this application and declared as such.
 *
 *    Note!  Linker trick II.  All tokens have to be put into the ".token"
 *           section.  Then, with a linker script, this section is placed
 *           into the read-only data section and is surrounded by the
 *           __beg_tokens and __end_tokens labels.  Therefore, all pointers
 *           found in between can easily be put into the uniqueness database.
 */
void
init_tokens (void)
{
    const token *pt;

    for (pt = &__beg_tokens; pt < &__end_tokens; pt++)
	if (*pt)
	    unique(*pt);
}

/*
 *  Execute the init_tokens function before main is run!
 *
 *  Note!  Linker trick III.  The init_tokens function should be run with
 *         all initializers of the application.  This is achieved by putting
 *         a pointer to the function into the C++ constructors section.
 */
INIT_FUNC (init_tokens)

#endif /* MAIN */

#ifdef __cplusplus
}
#endif

#endif /* TOKEN_INCLUDED */
