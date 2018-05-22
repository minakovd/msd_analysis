//
// Created by Дмитрий Минаков on 08.05.2018.
//

#ifndef MSD_ANALYSIS_ATOMCOORDINATE_H
#define MSD_ANALYSIS_ATOMCOORDINATE_H


class AtomCoordinate {

private:
    double x_;
    double y_;
    double z_;
    int cellXtransit_;
    int cellYtransit_;
    int cellZtransit_;

public:

    void setX(double x_);
    void setY(double y_);
    void setZ(double z_);
    void setCellXtransit(int cellXtransit_);
    void setCellYtransit(int cellYtransit_);
    void setCellZtransit(int cellZtransit_);

    void setXYZ(double, double, double);
    void setInitXYZ(double, double, double);
    void setInit(double, double, double, int, int, int);

    double getX() const;
    double getY() const;
    double getZ() const;
    int getCellXtransit() const;
    int getCellYtransit() const;
    int getCellZtransit() const;

};


#endif //MSD_ANALYSIS_ATOMCOORDINATE_H
