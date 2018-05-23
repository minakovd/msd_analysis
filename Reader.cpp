//
// Created by Дмитрий Минаков on 07.05.2018.
//

#include "Reader.h"

Reader::Reader(string filename, string auxfilename, int skipLines) {
    setFilename(filename);
//    TODO: input filename
    setAuxiliaryFilename(auxfilename);
    setInputFilename("");
    setSkipLines(skipLines);
}

//<editor-fold desc="Setters">

void Reader::setFilename(string filename) {
    filename_ = filename;
}

void Reader::setFiletype(string filetype) {
    filetype_ = filetype;
}

void Reader::setAuxiliaryFilename(string filename) {
    auxiliaryFilename_ = filename;
}

void Reader::setInputFilename(string filename) {
    inputFilename_ = filename;
}

void Reader::setSkipLines(int skipLines) {
    skipLines_ = skipLines;
}

void Reader::setMsdCalculator(const MsdCalculator &msdCalculator_) {
    Reader::msdCalculator_ = msdCalculator_;
}

//</editor-fold>

//<editor-fold desc="Getters">

const string &Reader::getAuxiliaryFilename() const {
    return auxiliaryFilename_;
}

const string &Reader::getInputFilename() const {
    return inputFilename_;
}

const string &Reader::getFilename() const {
    return filename_;
}

const string &Reader::getFiletype() const {
    return filetype_;
}

int Reader::getSkipLines() const {
    return skipLines_;
}

const MsdCalculator &Reader::getMsdCalculator() const {
    return msdCalculator_;
}

//non-const getter for msdCalculator
MsdCalculator &Reader::getMsdCalculator() {
    return const_cast<MsdCalculator&>(static_cast<const Reader*>(this)->getMsdCalculator());
}

//</editor-fold>

void Reader::printVariables() {
    cout << getFiletype() << " " << getFilename() << " " << getSkipLines()<< endl;
}

void Reader::readFile() {
    ifstream infile (getFilename());
    string line;

    if (infile.is_open()) {
        //Check for file type: read first line and check for identity substring
        getline(infile, line);
        if (line.find(Constants::lammpsIdentityString) != string::npos) {
            setFiletype(Constants::lammps);
            readAuxiliaryFile(getAuxiliaryFilename(), getFiletype());
            readLammpsFile(infile);
        }
//        Check for OUTCAR file
        else if (line.find(Constants::vaspIdentityString) != string::npos) {
            setFiletype(Constants::vasp);
            readAuxiliaryFile(getAuxiliaryFilename(), getFiletype());
            readVaspFile(infile);
        }
//        Check for XDATCAR file
        else {
            for (int i = 0; i < 6; i++) {
                infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //head
            }
            getline(infile, line);
            if (line.find(Constants::vaspStepString) != string::npos) {
                setFiletype(Constants::vasp);
                readAuxiliaryFile(getAuxiliaryFilename(), getFiletype());
                readVaspFile(infile);
            }
            else {
                    infile.close();
                    throw invalid_argument("Unknown file type");
                }
        }
    }
    else {
        throw invalid_argument("Unable to open file");
    }
    writeResultsIntoFile(getMsdCalculator());
    printDiffusionCoefficient();
    writeDiffusionCoefficient(getMsdCalculator());
}

/// Reads LAMMPS trajectory file
/// \param - lammps trajectory file stream
void Reader::readLammpsFile(ifstream &infile) {
    string line;
    double tmp, tmpi, tmpl, tmpx, tmpy, tmpz;
    string t1, t2, t3, t4, t5;
    int tmpid, tmpint;
    int currentStepNumber = 0;

    istringstream parser;

    if (infile.bad()) {
        throw invalid_argument("Problems while reading LAMMPS file");
    }
//    Examine trajectory file for calulating number of time steps
    calculateNumberOfSteps(infile, getFiletype());
    printNumberOfSteps();
//    Read auxiliary file and look for units and timestep size
    readAuxiliaryFile(getAuxiliaryFilename(), getFiletype());
    printUnits();

    infile.clear();
    infile.seekg(0, ios::beg);
//    Lammps file parsing
    printProgressHeader(++currentStepNumber);
//    Current time step
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ITEM: TIMESTEP
    getline(infile, line); // timestep
    parser.clear();
    parser.str(line);
    parser >> tmpint;
    getMsdCalculator().setCurrentTimeStepNumber(tmpint);
//    Number of atoms
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ITEM: NUMBER OF ATOMS
    getline(infile, line); //number of atoms
    parser.clear();
    parser.str(line);
    parser >> tmpint;
    getMsdCalculator().getCell().setNumberOfAtoms(tmpint);
//    X-vector
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ITEM: BOX BOUNDS
    getline(infile, line); //x length
    parser.clear();
    parser.str(line);
    parser >> tmpi >> tmpl;
    getMsdCalculator().getCell().setX_vector(tmpl, 0.0, 0.0);
//    Y-vector
    getline(infile, line); //y length
    parser.clear();
    parser.str(line);
    parser >> tmpi >> tmpl;
    getMsdCalculator().getCell().setY_vector(0.0, tmpl, 0.0);
//    Z-vector
    getline(infile, line); //z length
    parser.clear();
    parser.str(line);
    parser >> tmpi >> tmpl;
    getMsdCalculator().getCell().setZ_vector(0.0, 0.0, tmpl);
//   Initial configuration
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ITEM: ATOMS id type ...
//    TODO: determine position of coordinates in line
    for (int i = 0; i < getMsdCalculator().getCell().getNumberOfAtoms(); i++) {
        getline(infile, line);
        parser.clear();
        parser.str(line);
        parser >> tmpid >> tmp >> tmpx >> tmpy >> tmpz;
        getMsdCalculator().getCell().setInitialConfigurationAtom(
                tmpid-1, tmpx, tmpy, tmpz, Constants::cartesian);
//        TODO: check if it may be not cartesian in LAMMPS file
    }
    getMsdCalculator().getCell().copyInitialToPreviousConfiguration();
//    Process all next configurations
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ITEM: TIMESTEP
    while(!infile.eof()) {
        printProgress(++currentStepNumber);
        getline(infile, line); // timestep
        parser.clear();
        parser.str(line);
        parser >> tmpint;
        getMsdCalculator().setCurrentTimeStepNumber(tmpint);
//        Skip lines with vectors
        for (int i = 0; i < 7; i++) {
            infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
//        Read atoms coordinates
        for (int i = 0; i < getMsdCalculator().getCell().getNumberOfAtoms(); i++) {
            getline(infile, line);
            parser.clear();
            parser.str(line);
            parser >> tmpid >> tmp >> tmpx >> tmpy >> tmpz;
            getMsdCalculator().getCell().setCurrentConfigurationAtom(
                    tmpid-1, tmpx, tmpy, tmpz, Constants::cartesian);
        }
        getMsdCalculator().calculateMsdPoint();
        getMsdCalculator().getCell().copyCurrentToPreviousConfiguration();
        infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ITEM: TIMESTEP
    }
    printProgressFooter();
//    cell.printMsd();
}

void Reader::writeResultsIntoFile(MsdCalculator &msdCalculator) {
    ofstream outFile(Constants::outputDataFileName, ofstream::out);
    const int width = 12;
    const int precision = 6;
    if (outFile.bad()){
        throw invalid_argument("Problems while writing output file");
    }
//    Write head
    outFile << setw(width) << left << "N"
            << setw(width) << left << "Time" // TODO: units
            << setw(width) << left << "MSD" // TODO: units
            << endl;
//    Write data
    outFile.precision(precision);
    for (int i = 0; i < msdCalculator.getMsdPoints().size(); i++) {
        outFile << setw(width) << left << i
                << setw(width) << left << msdCalculator.getMsdPoints()[i].getTime()
                << setw(width) << left << msdCalculator.getMsdPoints()[i].getMsd()
                << endl;
    }
    outFile.close();
}

/// Goes through the trajectory file and calculates number of ion configurations
/// \param infile - Input file stream
/// \param fileType - Type of trajectory file (vasp or lammps), options are declared in Constants
void Reader::calculateNumberOfSteps(ifstream &infile, string fileType) {

    string line;
    int number = 0;
    if (infile.bad()) {
        throw invalid_argument("Problems while calculating number of steps");
    }
    infile.seekg(ios::beg);
    if (fileType == Constants::lammps) {
        while (!infile.eof()){
            getline(infile, line);
            if (line.find(Constants::lammpsStepString) != string::npos) {
                number++;
            }
        }
    }
    else if (fileType == Constants::vasp) {
        while (!infile.eof()){
            getline(infile, line);
            if (line.find(Constants::vaspStepString) != string::npos) {
                number++;
            }
        }
    }
    else
        throw invalid_argument("Problems while calculating number of steps");
    getMsdCalculator().setNumberOfSteps(number);
}

/// Reads VASP trajectory file (XDATCAR)
/// \param VASP trajectory file stream
void Reader::readVaspFile(ifstream &infile) {
    string line;
    istringstream parser;
    double tmpx = 0., tmpy = 0., tmpz = 0.;
    int tmpint = 0;
    int currentStepNumber = 0;
    string t1, t2, t3;

    if (infile.bad()) {
        throw invalid_argument("Problems while reading XDATCAR file");
    }
//    Examine trajectory file for calulating number of time steps
    calculateNumberOfSteps(infile, getFiletype());
    printNumberOfSteps();
//    Read auxiliary file and look for units and timestep size
    readAuxiliaryFile(getAuxiliaryFilename(), getFiletype());
    printUnits();

    infile.clear();
    infile.seekg(0, ios::beg);
//    VASP file parsing
    printProgressHeader(++currentStepNumber);
//    Parsing head
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ELEMENT NAME
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Lattice multiplier
//    X-vector
    getline(infile, line); //x vector
    parser.clear();
    parser.str(line);
    parser >> tmpx >> tmpy >> tmpz;
    getMsdCalculator().getCell().setX_vector(tmpx, tmpy, tmpz);
//    Y-vector
    getline(infile, line); //y length
    parser.clear();
    parser.str(line);
    parser >> tmpx >> tmpy >> tmpz;
    getMsdCalculator().getCell().setY_vector(tmpx, tmpy, tmpz);
//    Z-vector
    getline(infile, line); //z length
    parser.clear();
    parser.str(line);
    parser >> tmpx >> tmpy >> tmpz;
    getMsdCalculator().getCell().setZ_vector(tmpx, tmpy, tmpz);
//    Number of atoms
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ELEMENT NAME
    getline(infile, line); //number of atoms
    parser.clear();
    parser.str(line);
    parser >> tmpint;
    getMsdCalculator().getCell().setNumberOfAtoms(tmpint);
//    Current timestep
    getline(infile, line); // timestep
    parser.clear();
    parser.str(line);
    parser >> t1 >> t2 >> tmpint;
//    FIXME: potential error if step number is big
    getMsdCalculator().setCurrentTimeStepNumber(tmpint-1);
//    Read initial configuration
    for (int i = 0; i < getMsdCalculator().getCell().getNumberOfAtoms(); i++) {
        getline(infile, line);
        parser.clear();
        parser.str(line);
        parser >> tmpx >> tmpy >> tmpz;
        getMsdCalculator().getCell().setInitialConfigurationAtom(
                i, tmpx, tmpy, tmpz, Constants::direct);
    }
    getMsdCalculator().getCell().copyInitialToPreviousConfiguration();
//    Process all next configurations
    getline(infile, line); // timestep
    while(!infile.eof()) {
        printProgress(++currentStepNumber);
//    Current timestep
        parser.clear();
        parser.str(line);
        parser >> t1 >> t2 >> tmpint;
        getMsdCalculator().setCurrentTimeStepNumber(tmpint-1);
//        Read atoms coordinates
        for (int i = 0; i < getMsdCalculator().getCell().getNumberOfAtoms(); i++) {
            getline(infile, line);
            parser.clear();
            parser.str(line);
            parser >> tmpx >> tmpy >> tmpz;
            getMsdCalculator().getCell().setCurrentConfigurationAtom(
                    i, tmpx, tmpy, tmpz, Constants::direct);
        }
        getMsdCalculator().calculateMsdPoint();
        getMsdCalculator().getCell().copyCurrentToPreviousConfiguration();
        getline(infile, line); // timestep
    }
    printProgressFooter();
}

void Reader::readAuxiliaryFile(string filename, string filetype) {
    string line;
    ifstream auxfile;
    istringstream parser;
    bool unitsAreIdentified, timestepIsIdentified;
    unitsAreIdentified = false;
    timestepIsIdentified = false;

    if (!filename.empty()) {
        auxfile.open(filename);
        if (auxfile.bad())
            throw invalid_argument("Problems while open auxiliary file");
    }
    else
        throw invalid_argument("Problems while open auxiliary file");
//    VASP
    if (filetype == Constants::vasp) {
        getMsdCalculator().setUnits(Constants::unitsTypeReal);
        unitsAreIdentified = true;
        while(!auxfile.eof()) {
//            We do not need check units, as defaults time is in fs, distance is in A
//            Check time step size
            getline(auxfile, line);
            if (line.find(Constants::vaspAuxFileTimestepString) != string::npos) {
                parser.clear();
                parser.str(line);
                vector<string> parsedstring = vector<string>(istream_iterator<string>(parser),
                        istream_iterator<string>());
                double parsedTimestep = 0.0;
                parsedTimestep = stod(parsedstring[2]);
                if (parsedTimestep != 0.0) {
                    getMsdCalculator().setTimeStepSize(parsedTimestep);
                    timestepIsIdentified = true;
                    break;
                } else
                    throw invalid_argument("Error while searching for timestep in auxiliary file");
            }
        }
    }
//    LAMMPS
    else if (filetype == Constants::lammps) {
        while(!auxfile.eof()) {
            getline(auxfile, line);
//            Check units
            if (line.find(Constants::lammpsAuxFileUnitsString) != string::npos) {
                parser.clear();
                parser.str(line);
                vector<string> parsedstring = vector<string>(istream_iterator<string>(parser),
                        istream_iterator<string>());
                if (parsedstring[1] == Constants::lammpsAuxFileUnitsTypeLJString) {
                    getMsdCalculator().setUnits(Constants::unitsTypeLJ);
                    unitsAreIdentified = true;
                }
                else if (parsedstring[1] == Constants::lammpsAuxFileUnitsTypeMetalString) {
                    getMsdCalculator().setUnits(Constants::unitsTypeMetal);
                    unitsAreIdentified = true;
                } else
                    throw invalid_argument("Units have not been identified");
            }
//            Check time step size
            else if (line.find(Constants::lammpsAuxFileTimestepString) != string::npos) {
                parser.clear();
                parser.str(line);
                vector<string> parsedstring = vector<string>(istream_iterator<string>(parser),
                        istream_iterator<string>());
                double parsedTimestep = 0.0;
                parsedTimestep = stod(parsedstring[1]);
                if (parsedTimestep != 0.0) {
                    getMsdCalculator().setTimeStepSize(parsedTimestep);
                    timestepIsIdentified = true;
                }
                else
                    throw invalid_argument("Error while searching for timestep in auxiliary file");
            }
        }
    }
    else
        throw invalid_argument("Unknown file type for reading an auxiliary file");

    auxfile.close();

    if (!unitsAreIdentified || !timestepIsIdentified)
        throw invalid_argument("Information about units or time step was not found");
}

void Reader::printNumberOfSteps() {
    cout << "We have found " << getMsdCalculator().getNumberOfSteps()
         << " steps in this " << getFiletype()
         << " trajectory file" << endl;
}

void Reader::printUnits() {
    cout << "Units are " << getMsdCalculator().getUnits()
         << "; timestep is " << getMsdCalculator().getTimeStepSize() << endl;
}

//<editor-fold desc="Progress bar">

void Reader::printProgressHeader(int step) {
    cout << "Progress: " << endl;
    cout << step << " step of " << getMsdCalculator().getNumberOfSteps() << " steps" << '\r';
    cout.flush();
}

void Reader::printProgress(int step) {
    cout << step << " step of " << getMsdCalculator().getNumberOfSteps() << " steps" << '\r';
    cout.flush();
}

void Reader::printProgressFooter() {
    cout << endl;
}

//</editor-fold>

void Reader::readInputFile(string filename) {
//    TODO: realize several atom types
}

void Reader::printDiffusionCoefficient() {
    double coef, sigma;
    getMsdCalculator().calculateDiffusionCoefficient(getMsdCalculator().getMsdPoints(), coef, sigma);
    cout << "Diffusion coefficient: " << coef
         << " +/- " << sigma;
    if (getMsdCalculator().getUnits() != Constants::unitsTypeLJ) {
        cout << " 10^-9 m2/s";
    }
    cout << endl;
}

void Reader::writeDiffusionCoefficient(MsdCalculator &msdCalculator) {
    ofstream outFile(Constants::coefDataFileName, ofstream::out);
    const int width = 30;
    const int precision = 8;
    double coef, sigma;
    string header = "Diffusion";
    string units = ", 1E-9 m2/s";

    if (outFile.bad()){
        throw invalid_argument("Problems while writing output file");
    }
    if (msdCalculator.getUnits() != Constants::unitsTypeLJ) {
        header.append(units);
    }
    getMsdCalculator().calculateDiffusionCoefficient(msdCalculator.getMsdPoints(), coef, sigma);
//    Header
    outFile << fixed << left << setfill(' ')
            << setw(width) << header
            << setw(width) << "Sigma"
            << endl;
//    Data
    outFile << fixed << left << setprecision(precision)
            << setw(width) << coef
            << setw(width) << sigma
            << endl;
    outFile.close();
}