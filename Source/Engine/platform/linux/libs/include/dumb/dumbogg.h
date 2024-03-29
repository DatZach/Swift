/*  _______         ____    __         ___    ___
 * \    _  \       \    /  \  /       \   \  /   /     '   '  '
 *  |  | \  \       |  |    ||         |   \/   |       .      .
 *  |  |  |  |      |  |    ||         ||\  /|  |
 *  |  |  |  |      |  |    ||         || \/ |  |       '  '  '
 *  |  |  |  |      |  |    ||         ||    |  |       .      .
 *  |  |_/  /        \  \__//          ||    |  |
 * /_______/edicated  \____/niversal  /__\  /____\usic /|  .  . astardisation
 *                                                    /  \
 *                                                   / .  \
 * dumbogg.h                                        / / \  \
 *                                                 | <  /   \_
 * Written by Sven.                                |  \/ /\   /
 *                                                  \_  /  > /
 *                                                    | \ / /
 *                                                    |  ' /
 *                                                     \__/
 */



#ifndef DUMBOGG_H
#define DUMBOGG_H


#include <dumb/dumb.h>

#if DUMB_MAJOR_VERSION == 0 && DUMB_MINOR_VERSION == 7
#error DUMB v0.7 has a bug that would seriously affect this plug-in.
#error Please download the latest version of DUMB!
#endif

#define DUMBOGG_MAJOR_VERSION 0
#define DUMBOGG_MINOR_VERSION 5
#define DUMBOGG_REVISION_VERSION 0

#define DUMBOGG_VERSION (DUMBOGG_MAJOR_VERSION*10000 + DUMBOGG_MINOR_VERSION*100 + DUMBOGG_REVISION_VERSION)

#if DUMBOGG_REVISION_VERSION > 0
#define DUMBOGG_VERSION_STR #DUMBOGG_MAJOR_VERSION"."#DUMBOGG_MINOR_VERSION"."#DUMBOGG_REVISION_VERSION
#else
#define DUMBOGG_VERSION_STR #DUMBOGG_MAJOR_VERSION"."#DUMBOGG_MINOR_VERSION
#endif

#define DUMBOGG_NAME "DUMBOGG v"DUMBOGG_VERSION_STR

#define DUMBOGG_YEAR  2002
#define DUMBOGG_MONTH 8
#define DUMBOGG_DAY   16

#define DUMBOGG_YEAR_STR2 "02"

#define DUMBOGG_YEAR_STR4  #DUMB_YEAR
#define DUMBOGG_MONTH_STR1 #DUMB_MONTH
#define DUMBOGG_DAY_STR1   #DUMB_DAY

#if DUMBOGG_MONTH < 10
#define DUMBOGG_MONTH_STR2 "0"DUMBOGG_MONTH_STR1
#else
#define DUMBOGG_MONTH_STR2 DUMBOGG_MONTH_STR1
#endif

#if DUMBOGG_DAY < 10
#define DUMBOGG_DAY_STR2 "0"DUMBOGG_DAY_STR1
#else
#define DUMBOGG_DAY_STR2 DUMBOGG_DAY_STR1
#endif


#define DUMBOGG_DATE (DUMBOGG_YEAR*10000 + DUMBOGG_MONTH*100 + DUMBOGG_DAY)

#define DUMBOGG_DATE_STR DUMBOGG_DAY_STR1"."DUMBOGG_MONTH_STR1"."DUMBOGG_YEAR_STR4


DUH *dumb_load_ogg(const char *filename, int loop);


#endif /* DUMBOGG_H */

