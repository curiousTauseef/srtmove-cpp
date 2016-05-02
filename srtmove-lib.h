/***** SRTmove ******
Author: Ophir LOJKINE
Licence: LGPLv3
*/

#ifndef SRTMOVE_LIB_H
#define SRTMOVE_LIB_H

#include <iostream>

/** Reads subtitles in srt format from fin, and writes it to fout, with addition
milliseconds added to each subtitles. addition can be a negative number.
Returns the number of subtitles that were processed, or a negative number if anb error occured.
*/
template<typename INTYPE, typename OUTTYPE>
int srtmove (INTYPE &fin, OUTTYPE &fout, int addition);

/** Converts a value like 12:03, 2m3s, or 8s3ms to a number of milliseconds
arg2msec("1h") == 3600000
arg2msec("1h1m1s1ms") == 60*60*1000 + 60*1000 + 1*1000 + 1 == 3661001
arg2msec("-2:03") == 2*60*1000 + 3*1000 == -123000 //Removes 2 minutes and 3 seconds to every subtitles
*/
int arg2msec(const char *arg);

#include "srtmove-lib.cpp"
#endif
