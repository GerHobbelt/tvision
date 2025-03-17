/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   CONFIG.H                                                              */
/*                                                                         */
/*   miscellaneous system-wide configuration parameters                    */
/*   FOR INTERNAL USE ONLY                                                 */
/*                                                                         */
/* ------------------------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#if !defined( __CONFIG_H )
#define __CONFIG_H

#if defined(BUILD_MONOLITHIC)

#ifndef abort
#error "Expected abort to be redefined to point at our override function(s) below..."
#endif
#ifndef exit
#error "Expected exit to be redefined to point at our override function(s) below..."
#endif

#endif

#if !defined( __LIMITS_H )
#include <limits.h>
#endif  // __LIMITS_H

const int eventQSize = 16;
const int maxCollectionSize = (int)(( (unsigned long) UINT_MAX - 16)/sizeof( void * ));

const int maxViewWidth = 132;

const int maxFindStrLen    = 80;
const int maxReplaceStrLen = 80;

const int minPasteEventCount = 3;

#if !defined( __BORLANDC__ )
const int maxCharSize = 4; // A UTF-8-encoded character is up to 4 bytes long.
#else
const int maxCharSize = 1; // All characters are single-byte-encoded.
#endif

#endif  // __CONFIG_H
