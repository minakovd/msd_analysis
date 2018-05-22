//
// Created by Дмитрий Минаков on 08.05.2018.
//

#ifndef MSD_ANALYSIS_CELL_H
#define MSD_ANALYSIS_CELL_H

#include <vector>
#include <iostream>
#include "AtomCoordinate.h"
#include "MsdPoint.h"
#include <cmath>
#include <stdexcept>
#include <limits>

using namespace std;

class Cell {
private:
    int numberOfAtoms_;
    vector<double> x_vector_;
    vector<double> y_vector_;
    vector<double> z_vector_;
    double xlength_;
    double ylength_;
    double zlength_;
    vector<AtomCoordinate> initialConfiguration_;
    vector<AtomCoordinate> currentConfiguration_;
    vector<AtomCoordinate> previousConfiguration_;

public:
    Cell() : x_vector_(3), y_vector_(3), z_vector_(3) {}
//    Setters
    void setX_vector(double xx, double xy, double xz);
    void setY_vector(double yx, double yy, double yz);
    void setZ_vector(double zx, double zy, double zz);
    void setXlength(vector<double>);
    void setYlength(vector<double>);
    void setZlength(vector<double>);
    void setNumberOfAtoms(int);

//    Getters
    const vector<double> &getX_vector() const;
    const vector<double> &getY_vector() const;
    const vector<double> &getZ_vector() const;
    double getXlength() const;
    double getYlength() const;
    double getZlength() const;
    int getNumberOfAtoms() const;
    const vector<AtomCoordinate> &getInitialConfiguration() const;
    const vector<AtomCoordinate> &getCurrentConfiguration() const;
    const vector<AtomCoordinate> &getPreviousConfiguration() const;

//    Configuration methods
    void setInitialConfigurationAtom(int, double, double, double, string);
    void setCurrentConfigurationAtom(int, double, double, double, string);
    void setPreviousConfigurationAtom(int, double, double, double, string);
    void copyInitialToPreviousConfiguration();
    void copyCurrentToPreviousConfiguration();
    void printInitialConfiguration();
    void printCurrentConfiguration();

//    Other
    void cartesianToDirect(vector<double>);
};


#endif //MSD_ANALYSIS_CELL_H
