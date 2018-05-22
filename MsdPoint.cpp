//
// Created by Дмитрий Минаков on 14.05.2018.
//

#include "MsdPoint.h"

//<editor-fold desc="Setters">

void MsdPoint::setMsd(double msd) {
    msd_ = msd;
}

void MsdPoint::setTime(double time) {
    time_ = time;
}

//</editor-fold>

//<editor-fold desc="Getters">

double MsdPoint::getMsd() const {
    return msd_;
}

double MsdPoint::getTime() const {
    return time_;
}

//</editor-fold>
