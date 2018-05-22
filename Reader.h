//
// Created by Дмитрий Минаков on 07.05.2018.
//

#ifndef MSD_ANALYSIS_READER_H
#define MSD_ANALYSIS_READER_H

#include <string>
#include <iostream>
#include <fstream>
#include "Constants.h"
#include "Cell.h"
#include "sstream"
#include <stdexcept>
#include <limits>
#include "MsdCalculator.h"
#include <iomanip>
#include <iterator>

using namespace std;

class Reader {
private:
    string filetype_;
    string filename_;
    string auxiliaryFilename_;
    string inputFilename_;
    int skipLines_;
    MsdCalculator msdCalculator_;


private:

//    Setters
    void setFiletype(string);
    void setFilename(string);
    void setAuxiliaryFilename(string);
    void setInputFilename(string);
    void setSkipLines(int);
    void setMsdCalculator(const MsdCalculator &msdCalculator_);

//    Read file methods
    void readLammpsFile(ifstream &);
    void readVaspFile(ifstream &);
    void calculateNumberOfSteps(ifstream &, string);



public:
//    Constructor
    Reader (string, string, int skipLines = 0);
//    Getters
    const string &getFiletype() const ;
    const string &getFilename() const;
    const string &getAuxiliaryFilename() const;
    const string &getInputFilename() const;
    int getSkipLines() const;
    const MsdCalculator &getMsdCalculator() const;
    MsdCalculator &getMsdCalculator();

    void printVariables();
    void readFile();
    void writeResultsIntoFile(MsdCalculator &);

    void readAuxiliaryFile(string, string);
    void readInputFile(string);

    void printNumberOfSteps();
    void printUnits();
    void printProgressHeader(int);
    void printProgress(int);
    void printProgressFooter();
};


#endif //MSD_ANALYSIS_READER_H
