//
// Created by Дмитрий Минаков on 08.05.2018.
//

#include "Cell.h"
#include "Constants.h"

//<editor-fold desc="Setters">

void Cell::setX_vector(double xx, double xy, double xz) {
    x_vector_[0] = xx;
    x_vector_[1] = xy;
    x_vector_[2] = xz;
    Cell::setXlength(getX_vector());
}

void Cell::setY_vector(double yx, double yy, double yz) {
    y_vector_[0] = yx;
    y_vector_[1] = yy;
    y_vector_[2] = yz;
    Cell::setYlength(getY_vector());
}

void Cell::setZ_vector(double zx, double zy, double zz) {
    z_vector_[0] = zx;
    z_vector_[1] = zy;
    z_vector_[2] = zz;
    Cell::setZlength(getZ_vector());
}

void Cell::setXlength(vector<double> x_vector) {
    xlength_ = sqrt(x_vector[0]*x_vector[0] + x_vector[1]*x_vector[1] + x_vector[2]*x_vector[2]);
}

void Cell::setYlength(vector<double> y_vector) {
    ylength_ = sqrt(y_vector[0] * y_vector[0] + y_vector[1] * y_vector[1] + y_vector[2] * y_vector[2]);
}

void Cell::setZlength(vector<double> z_vector) {
    zlength_ = sqrt(z_vector[0] * z_vector[0] + z_vector[1] * z_vector[1] + z_vector[2] * z_vector[2]);
}

void Cell::setNumberOfAtoms(int numberOfAtoms) {
    numberOfAtoms_ = numberOfAtoms;
    initialConfiguration_.resize(numberOfAtoms_);
    currentConfiguration_.resize(numberOfAtoms_);
    previousConfiguration_.resize(numberOfAtoms_);
}

//</editor-fold>

//<editor-fold desc="Getters">

int Cell::getNumberOfAtoms() const{
    return numberOfAtoms_;
}

const vector<AtomCoordinate> &Cell::getInitialConfiguration() const {
    return initialConfiguration_;
}

const vector<AtomCoordinate> &Cell::getCurrentConfiguration() const {
    return currentConfiguration_;
}

const vector<AtomCoordinate> &Cell::getPreviousConfiguration() const {
    return previousConfiguration_;
}

double Cell::getXlength() const {
    return xlength_;
}

double Cell::getYlength() const {
    return ylength_;
}

double Cell::getZlength() const {
    return zlength_;
}

const vector<double> &Cell::getX_vector() const {
    return x_vector_;
}

const vector<double> &Cell::getY_vector() const {
    return y_vector_;
}

const vector<double> &Cell::getZ_vector() const {
    return z_vector_;
}

//</editor-fold>

//<editor-fold desc="Configuration methods">

void Cell::setInitialConfigurationAtom(int id, double xx, double yy, double zz, string configType) {
    AtomCoordinate atom;
    double x, y, z;
//    TODO: check for errors
    if (configType == Constants::cartesian) {
        x = xx / xlength_;
        y = yy / ylength_;
        z = zz / zlength_;
    }
    else if (configType == Constants::direct) {
        x = xx;
        y = yy;
        z = zz;
    }
    else {
        throw(invalid_argument("Unknown type of coordinate records"));
    }
//    atom.setInitXYZ(x, y, z);
    atom.setInit(x, y, z, 0, 0, 0);
    initialConfiguration_[id] = atom;
    currentConfiguration_[id] = atom;
    previousConfiguration_[id] = atom;
}

void Cell::copyInitialToPreviousConfiguration() {
    MsdPoint msdPoint;
    for (int i = 0; i < initialConfiguration_.size(); i++) {
        previousConfiguration_[i] = initialConfiguration_[i];
    }
}

void Cell::copyCurrentToPreviousConfiguration() {
    for (int i = 0; i < currentConfiguration_.size(); i++) {
        previousConfiguration_[i] = currentConfiguration_[i];
    }
}

void Cell::setCurrentConfigurationAtom(int id, double xx, double yy, double zz, string configType) {
    AtomCoordinate currentPosition, previousPosition;
    double x, y, z;
    int xt, yt, zt;
    if (configType == Constants::cartesian) {
        x = xx / xlength_;
        y = yy / ylength_;
        z = zz / zlength_;
    }
    else {
        x = xx;
        y = yy;
        z = zz;
    }
    currentPosition.setXYZ(x, y, z);
    previousPosition = previousConfiguration_[id];
    xt = previousPosition.getCellXtransit();
    yt = previousPosition.getCellYtransit();
    zt = previousPosition.getCellZtransit();
    if (abs(previousPosition.getX() - currentPosition.getX()) > 0.5) {
        if (previousPosition.getX() > currentPosition.getX())
            currentPosition.setCellXtransit(++xt);
        else
            currentPosition.setCellXtransit(--xt);
    } else {
        currentPosition.setCellXtransit(xt);
    }

    if (abs(previousPosition.getY() - currentPosition.getY()) > 0.5) {
        if (previousPosition.getY() > currentPosition.getY())
            currentPosition.setCellYtransit(++yt);
        else
            currentPosition.setCellYtransit(--yt);
    } else {
        currentPosition.setCellYtransit(yt);
    }

    if (abs(previousPosition.getZ() - currentPosition.getZ()) > 0.5) {
        if (previousPosition.getZ() > currentPosition.getZ())
            currentPosition.setCellZtransit(++zt);
        else
            currentPosition.setCellZtransit(--zt);
    } else {
        currentPosition.setCellZtransit(zt);
    }

    currentConfiguration_[id] = currentPosition;
}

void Cell::printInitialConfiguration() {
    for (int i = 0; i < initialConfiguration_.size(); i++) {
        cout << i << " "
             << initialConfiguration_[i].getX() << " "
             << initialConfiguration_[i].getY() << " "
             << initialConfiguration_[i].getZ() << endl;
    }
}

void Cell::printCurrentConfiguration() {
    for (int i = 0; i < currentConfiguration_.size(); i++) {
        cout << i << " "
                  << currentConfiguration_[i].getX() << " " << currentConfiguration_[i].getCellXtransit()
                  << " "
                  << currentConfiguration_[i].getY() << " " << currentConfiguration_[i].getCellYtransit()
                  << " "
                  << currentConfiguration_[i].getZ() << " " << currentConfiguration_[i].getCellZtransit()
                  << endl;
    }
}
//</editor-fold>

void Cell::cartesianToDirect(vector<double>) {
//    TODO: conversion to direct from cartesian
}
