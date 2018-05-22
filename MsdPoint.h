//
// Created by Дмитрий Минаков on 14.05.2018.
//

#ifndef MSD_ANALYSIS_MSDPOINT_H
#define MSD_ANALYSIS_MSDPOINT_H


class MsdPoint {
private:
    double time_;
    double msd_;
public:
//    Setters
    void setTime(double);
    void setMsd(double);
//    Getters
    double getTime() const;
    double getMsd() const;
};

#endif //MSD_ANALYSIS_MSDPOINT_H
