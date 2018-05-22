//
// Created by Дмитрий Минаков on 08.05.2018.
//

#include "AtomCoordinate.h"

//<editor-fold desc="Setters">

void AtomCoordinate::setX(double x_) {
    AtomCoordinate::x_ = x_;
}

void AtomCoordinate::setY(double y_) {
    AtomCoordinate::y_ = y_;
}

void AtomCoordinate::setZ(double z_) {
    AtomCoordinate::z_ = z_;
}

void AtomCoordinate::setCellXtransit(int cellXtransit_) {
    AtomCoordinate::cellXtransit_ = cellXtransit_;
}

void AtomCoordinate::setCellYtransit(int cellYtransit_) {
    AtomCoordinate::cellYtransit_ = cellYtransit_;
}

void AtomCoordinate::setCellZtransit(int cellZtransit_) {
    AtomCoordinate::cellZtransit_ = cellZtransit_;
}

//</editor-fold>

//<editor-fold desc="Unusual setters">

void AtomCoordinate::setXYZ(double xx, double yy, double zz) {
    x_ = xx;
    y_ = yy;
    z_ = zz;
}

void AtomCoordinate::setInitXYZ(double xx, double yy, double zz) {
    x_ = xx;
    y_ = yy;
    z_ = zz;
}

void AtomCoordinate::setInit(double xx, double yy, double zz, int xtr, int ytr, int ztr) {
    x_ = xx;
    y_ = yy;
    z_ = zz;
    cellXtransit_ = xtr;
    cellYtransit_ = ytr;
    cellZtransit_ = ztr;
}

//</editor-fold>

//<editor-fold desc="Getters">

double AtomCoordinate::getX() const {
    return x_;
}

double AtomCoordinate::getY() const {
    return y_;
}

double AtomCoordinate::getZ() const {
    return z_;
}

int AtomCoordinate::getCellXtransit() const {
    return cellXtransit_;
}

int AtomCoordinate::getCellYtransit() const {
    return cellYtransit_;
}

int AtomCoordinate::getCellZtransit() const {
    return cellZtransit_;
}

//</editor-fold>