/*****************************************************************************/
/* SISCO SOFTWARE MODULE HEADER **********************************************/
/*****************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,            */
/*            2005 All Rights Reserved                                       */
/*                                                                           */
/* MODULE NAME : stdtime_tzrules.h                                           */
/* PRODUCT(S)  : Standard Time Management Library                            */
/*                                                                           */
/* MODULE DESCRIPTION:                                                       */
/*   Data file to fix historical rules for daylight saving time.             */
/*                                                                           */
/* MODIFICATION LOG:                                                         */
/*   Date    Who  Rev  Comments                                              */
/* --------  ---  ---  ----------------------------------------------------- */
/* 06/07/06  RLH   01  Created                                               */
/*****************************************************************************/

/* this data is part of a table of type STDTIME_FIX_TZ_RULES */
/* the struct defining type STDTIME_FIX_TZ_RULES is in stdtime.h */
/* entries with region codes must appear before entries without them */


    {"US", "IN", 2007, 9999, {2,0,3}, {1, 2,0,0}, {1,0,11}, {1, 2,0,0}},
    {"US", "IN", 1945, 2006, {0,0,0}, {0, 0,0,0}, {0,0,00}, {0, 0,0,0}},
    {"US", "HI", 1945, 9999, {0,0,0}, {0, 0,0,0}, {0,0,00}, {0, 0,0,0}},
    {"US", "AZ", 1945, 9999, {0,0,0}, {0, 0,0,0}, {0,0,00}, {0, 0,0,0}},

    {"US", "",   2007, 9999, {2,0,3}, {1, 2,0,0}, {1,0,11}, {1, 2,0,0}},
    {"US", "",   1987, 2006, {1,0,4}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"US", "",   1976, 1986, {5,0,4}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"US", "",   1975, 1975, {5,0,2}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"US", "",   1974, 1974, {1,0,1}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"US", "",   1967, 1973, {5,0,4}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"US", "",   1945, 1966, {5,0,4}, {1, 2,0,0}, {5,0, 9}, {1, 2,0,0}},


    {"CA", "SK", 1945, 9999, {0,0,0}, {0, 0,0,0}, {0,0,00}, {0, 0,0,0}},

    /* for now, CA and MX just duplicate US rules */

    {"CA", "",   2007, 9999, {2,0,3}, {1, 2,0,0}, {1,0,11}, {1, 2,0,0}},
    {"CA", "",   1987, 2006, {1,0,4}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"CA", "",   1976, 1986, {5,0,4}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"CA", "",   1975, 1975, {5,0,2}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"CA", "",   1974, 1974, {1,0,1}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"CA", "",   1967, 1973, {5,0,4}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"CA", "",   1945, 1966, {5,0,4}, {1, 2,0,0}, {5,0, 9}, {1, 2,0,0}},


    {"MX", "",   2007, 9999, {2,0,3}, {1, 2,0,0}, {1,0,11}, {1, 2,0,0}},
    {"MX", "",   1987, 2006, {1,0,4}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"MX", "",   1976, 1986, {5,0,4}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"MX", "",   1975, 1975, {5,0,2}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"MX", "",   1974, 1974, {1,0,1}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"MX", "",   1967, 1973, {5,0,4}, {1, 2,0,0}, {5,0,10}, {1, 2,0,0}},
    {"MX", "",   1945, 1966, {5,0,4}, {1, 2,0,0}, {5,0, 9}, {1, 2,0,0}}  /**/
