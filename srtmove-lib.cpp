/***** SRTmove ******
Author: Ophir LOJKINE
Licence: LGPLv3

Adds or removes time from the subtitles inside an .srt file.

This version is easier to work with on Windows. It takes a list of subtitles as command-line
arguments, and prompts the user for a time, and outputs the resynced subtitles to files in the same
directory as the input files.

You can use Windows' "Open with" functionality with this program.

*/
#include <iostream>     // std::cin, std::cout
#include <stdio.h>

#define LINE_LEN 256

using namespace std;

int str2msec (char *line) {
	int mul[12] = {36000000, 3600000, 0, 600000, 60000, 0, 10000, 1000, 0, 100, 10, 1},
		msec = 0;
	for (int i=11; i>=0; i--) {
		msec += mul[i] * (int) (line[i]-'0');
	}
	return msec;
}

void msec2str (int msec, char *out) {
	int ms = msec%1000; msec /= 1000;
	int s  = msec%60;   msec /= 60;
	int m  = msec%60;   msec /= 60;
	int h  = msec;
	sprintf(out, "%02d:%02d:%02d,%02d", h,m,s,ms/10);
	out[11] = (char) ('0' + ms%10);
}

/**Returns true if s begins with pat, where pat is a string with a simple format 'N' matches a number, any other character matches only itself.*/
bool matchpattern(char *s, char *pat) {
	int i;
	for (i=0; pat[i] != '\0' && s[i] != '\0'; i++) {
		if (pat[i] == 'N' && s[i] <= '9' && s[i] >= '0') continue;
		if (pat[i] != s[i]) return false;
	}
	return (pat[i] == '\0');
}

int arg2msec(const char *arg) { // arg can ba "6" for 6 seconds or "3:10" for 3 minutes
	size_t len = char_traits<char>::length(arg);
	int mul = 1000;
	int res = 0;
	int sgn = (arg[0] == '-') ? -1 : 1;
	for (int i = len-1; i>=0 && arg[i] != '-' && arg[i] != '+'; i--) {
		switch (arg[i]) {
			case 's': mul=1000; break;
			case ':': mul = mul/10 * 6; break;
			case 'm':
				if(i<len-1 && arg[i+1] == 's') mul=1;//ms
				else mul = 60000;
				break;
			case 'h': mul = 3600000; break;
			default:
				if ('0' <= arg[i] && arg[i] <= '9'){
					res += mul * int(arg[i] - '0');
					mul *= 10;
				} else {
					cerr << "Malformed argument." << endl;
					cerr << "You can type -10m3s for making your subtitles appear 10 minutes and 3 seconds earlier, for instance." <<endl;
					return 0;
				}
		}
	}
	return sgn*res;
}

template<typename INTYPE, typename OUTTYPE>
int srtmove (INTYPE &fin, OUTTYPE &fout, int addition) {
	char pattern[] = "NN:NN:NN,NNN --> NN:NN:NN,NNN";
	char line[LINE_LEN];

	if (!fin.good() || !fout.good()) return -1;

    int subcount = 0;

    while (fin.good()) {
        fin.getline (line, LINE_LEN);
        size_t size = char_traits<char>::length(line);
        if (matchpattern(line, pattern)) {
            int t1 = str2msec(line),
                t2 = str2msec(line + 17);
            t1 = (t1 > -addition) ? t1 + addition : 0;
            t2 = (t2 > -addition) ? t2 + addition : 0;
            msec2str(t1, line);
            msec2str(t2, line+17);
            subcount++;
        }
        fout.write(line, size);
        if (!fin.fail()) { //A full line was read
            fout << endl;
        }
    }
    return subcount;
}

