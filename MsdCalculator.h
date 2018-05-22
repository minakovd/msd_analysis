//
// Created by Дмитрий Минаков on 16.05.2018.
//

#ifndef MSD_ANALYSIS_MSDCALCULATOR_H
#define MSD_ANALYSIS_MSDCALCULATOR_H

#include <string>
#include "Cell.h"
#include "MsdPoint.h"

using namespace std;

class MsdCalculator {

private:
    string units_;
    double timeStepSize_;
    int currentTimeStepNumber_;
    double currentTimeStep_;
    int numberOfSteps_;
    Cell cell_;
    vector<MsdPoint> msdPoints_;

//  Private setters
    void setCurrentTimeStep(int, double);

public:
    MsdCalculator();

//    Setters
    void setUnits(const string &units_);
    void setTimeStepSize(double timestep_);
    void setCurrentTimeStepNumber(int timestep);
    void setNumberOfSteps(int numberOfSteps_);
    void setCell(const Cell &cell_);

//    Getters
    const string &getUnits() const;
    double getTimeStepSize() const;
    int getCurrentTimeStepNumber() const;
    int getNumberOfSteps() const;
    const Cell &getCell() const;
    Cell &getCell(); // non-const getter
    const vector<MsdPoint> &getMsdPoints() const;
    double getCurrentTimeStep() const;

//    Msd methods
    void calculateMsdPoint();
    void printMsd();
};


#endif //MSD_ANALYSIS_MSDCALCULATOR_H
