//
// Created by Дмитрий Минаков on 16.05.2018.
//

#include "MsdCalculator.h"

MsdCalculator::MsdCalculator() {
    MsdPoint msdPoint;
    msdPoint.setTime(0.0);
    msdPoint.setMsd(0.0);
    msdPoints_.push_back(msdPoint);
}

//<editor-fold desc="Setters">

void MsdCalculator::setUnits(const string &units_) {
    MsdCalculator::units_ = units_;
}

void MsdCalculator::setNumberOfSteps(int numberOfSteps_) {
    MsdCalculator::numberOfSteps_ = numberOfSteps_;
}

void MsdCalculator::setCell(const Cell &cell_) {
    MsdCalculator::cell_ = cell_;
}

void MsdCalculator::setTimeStepSize(double timestep_) {
    MsdCalculator::timeStepSize_ = timestep_;
}

void MsdCalculator::setCurrentTimeStepNumber(int timestep) {
    MsdCalculator::currentTimeStepNumber_ = timestep;
    MsdCalculator::setCurrentTimeStep(timestep, getTimeStepSize());
}

void MsdCalculator::setCurrentTimeStep(int stepnumber, double timestepsize) {
    MsdCalculator::currentTimeStep_ = stepnumber * timestepsize;
}

//</editor-fold>

//<editor-fold desc="Getters">

const string &MsdCalculator::getUnits() const {
    return units_;
}

int MsdCalculator::getNumberOfSteps() const {
    return numberOfSteps_;
}

const Cell &MsdCalculator::getCell() const {
    return cell_;
}

//non-const getter to access cell
Cell &MsdCalculator::getCell() {
    return const_cast<Cell&>(static_cast<const MsdCalculator*>(this)->getCell());
}

double MsdCalculator::getTimeStepSize() const {
    return timeStepSize_;
}

int MsdCalculator::getCurrentTimeStepNumber() const {
    return currentTimeStepNumber_;
}

double MsdCalculator::getCurrentTimeStep() const {
    return currentTimeStep_;
}

const vector<MsdPoint> &MsdCalculator::getMsdPoints() const {
    return msdPoints_;
}

//</editor-fold>

//<editor-fold desc="Msd methods">

void MsdCalculator::calculateMsdPoint() {
    double sum = 0.0, msd = 0.0;
    MsdPoint msdPoint;
    for (int id = 0; id < getCell().getCurrentConfiguration().size(); id++) {
        sum += pow( ((double)getCell().getCurrentConfiguration()[id].getCellXtransit()
                     + getCell().getCurrentConfiguration()[id].getX()
                     - getCell().getInitialConfiguration()[id].getX()) * getCell().getXlength() , 2 )
               + pow( ((double)getCell().getCurrentConfiguration()[id].getCellYtransit()
                       + getCell().getCurrentConfiguration()[id].getY()
                       - getCell().getInitialConfiguration()[id].getY()) * getCell().getYlength() , 2 )
               + pow( ((double)getCell().getCurrentConfiguration()[id].getCellZtransit()
                       + getCell().getCurrentConfiguration()[id].getZ()
                       - getCell().getInitialConfiguration()[id].getZ()) * getCell().getZlength() , 2);
    }
    msd = sum / getCell().getNumberOfAtoms();
    msdPoint.setMsd(msd);
    msdPoint.setTime(getCurrentTimeStep());
    msdPoints_.push_back(msdPoint);
}

void MsdCalculator::printMsd() {
    for (int i = 0; i < getMsdPoints().size(); i++) {
        cout << getMsdPoints()[i].getTime() << " " << getMsdPoints()[i].getMsd() << endl;
    }
}

void MsdCalculator::calculateDiffusionCoefficient(const vector<MsdPoint> &msdPoints, double &coef, double &sigma) {
    vector<double> tvec(msdPoints.size()), msdvec(msdPoints.size());
    double factor = 1.;
//    TODO: shift?

    if (getUnits() == Constants::unitsTypeReal) {
        factor = 1E4; // 10^-9 m2/s for diffusion
    }
    else if (getUnits() == Constants::unitsTypeMetal) {
        factor = 10; // 10^-9 m2/s for diffusion
    }

    for (int i = 0; i < msdPoints.size(); i++) {
        tvec[i] = msdPoints[i].getTime();
        msdvec[i] = msdPoints[i].getMsd() * factor;
    }
    double c1, cov11, sumsq;
    double *t = &tvec[0];
    double *msd = &msdvec[0];
    const size_t tStride = 1;
    const size_t msdStride = 1;

    gsl_fit_mul(t, tStride, msd, msdStride, msdPoints.size(), &c1, &cov11, &sumsq);
    coef = c1 / 6.0;
    sigma = (sqrt(cov11));
}

//</editor-fold>