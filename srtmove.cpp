/*    SRTmove
by Ophir LOJKINE
Adds or removes time from the subtitles inside an .srt file.

This version is UNIX friendly it reads a subtitle file from it's standard input,
takes it's parameter as a command-line argument, and outputs the new subtitles to it's standard output.

*/
#include <iostream>
#include "srtmove-lib.h"

int main (int argc, char **argv) {
	if (argc != 2) {
		cerr << "Wrong argument count.\n\tUsage: srtmove time\n\t(where time can be in format -3m2s for substracting 3 minutes and 2 seconds to all subtitles)" << endl;
		return 1;
	}

	int addition = arg2msec(argv[1]);
	int num = srtmove(std::cin, std::cout, addition);

	return (num > 0) ? 0 : 2;
}
