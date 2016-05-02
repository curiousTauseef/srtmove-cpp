/* SRTmove
by Ophir LOJKINE
Adds or removes time from the subtitles inside an .srt file.

This version is easier to work with on Windows. It takes a list of subtitles as command-line
arguments, and prompts the user for a time, and outputs the resynced subtitles to files in the same
directory as the input files.

You can use Windows' "Open with" functionality with this program.

*/
#include "srtmove-lib.h"

#include <fstream>     // std::ifstream, std::ofstream
#include <vector>
#include <string>

using namespace std;

struct t_outfile {
	string name;
	int subcount;
};

int main (int argc, char **argv) {
    if (argc < 2) {
        cerr << "No file to handle..." << endl;
        return 1;
    }

    string addstr;
    int addition=0;
    while (addition == 0) {
        cout << "Time to add (MM:SS): ";
        cin >> addstr;
        if (addstr.find("exit") == 0 || addstr.find("quit") == 0) return 1;
        addition = arg2msec(addstr.c_str());
    };

    vector <t_outfile*> outfiles;

    for (int numfile=1; numfile < argc; numfile++) {
        char *finname = argv[numfile];
        string foutname (finname);
        size_t pos = foutname.rfind(".srt");
        if (pos == string::npos) pos = foutname.size()-1;
        foutname.replace(pos, 4, "-resync.srt");

        ifstream fin (finname);
        if (!fin.good()) {
            cerr << "Unable to open input file: " << finname << endl;
            continue;
        }
        ofstream fout (foutname.c_str());
        if (!fout.good()) {
            cerr << "Unable to open output file: " << foutname << endl;
            continue;
        }

		int subcount = srtmove (fin, fout, addition);
		if (subcount < 0) {
			cerr << "Sorry, an error occured in srtmove." << endl;
			continue;
		} else if (subcount == 0) {
			cerr << "No subtitles found in file: " << finname << endl;
			continue;
		} else { //Everything went well
		    t_outfile* outfile = new t_outfile;
		    outfiles.push_back(outfile);

		    outfile->name = foutname;
		    outfile->subcount = subcount;
		}
    }

	cerr << "End of program. Files created:" << endl;
	for (int i=0; i<outfiles.size(); i++) {
		cerr << " - " << outfiles[i]->name;
		cerr << " (" << outfiles[i]->subcount << " subtitles)" <<endl;
		delete outfiles[i];
	}

  return 0;
}

