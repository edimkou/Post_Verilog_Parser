
/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** keywords.h
** Verilog keywords
**
** Author: David Kebo Houngninou
** -------------------------------------------------------------------------*/
#ifndef _keywords_h
#define _keywords_h

#ifdef IDENT_H
static const char *const keywords_h_Id =
    "$Id$";
#endif

#define GATESNUM 20
extern const char *GATE_NAME[GATESNUM];

#define RESERVEDNUM 34
extern const char *RESERVED_WORD[RESERVEDNUM];

// #define NUM_PUNCTUATIONS 4
// char *punctuations[] = {
//     ";",
//     ".",
//     " ",
//     "\n",
// };

#endif