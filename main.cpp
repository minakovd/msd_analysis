#include <iostream>
#include <stdexcept>
#include <string>
#include "Reader.h"
#include "Constants.h"

using namespace std;

//const string LAMMPS = "lammps";
//const string VASP = "vasp";

void print_usage() {
    cout << "USAGE: msd_analysis [path_to_trajectory_file] [path_to_aiuxiliary_file]" << endl;
    return;
}

void read_argumets() {
    return;
}

int main(int argc, char* argv[]) {
    string filename, auxfilename, inputfilename, skip, stepsize;
    if (argc > 1) {
        filename = argv[1];
        auxfilename = argv[2];
//        stepsize = argv[3];
//        filetype = argv[1];
//        if (filetype == Constants::lammps) {
//            cout << "lammps" << endl;
//        } else if (filetype == Constants::vasp) {
//            cout << "vasp" << endl;
        } else {
            print_usage();
            return -1;
        }
    Reader reader(filename, auxfilename);
    reader.printVariables();
    try {
        reader.readFile();
    }
    catch (invalid_argument& e) {
        cerr << "ERROR: "<< e.what() << endl;
        return -1;
    }

    return 0;
}