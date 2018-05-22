//
// Created by Дмитрий Минаков on 08.05.2018.
//

#ifndef MSD_ANALYSIS_CONSTANTS_H
#define MSD_ANALYSIS_CONSTANTS_H

#include <string>

namespace Constants {
    extern const std::string vasp;
    extern const std::string lammps;
    extern const std::string vaspIdentityString;
    extern const std::string lammpsIdentityString;
    extern const std::string lammpsStepString;
    extern const std::string vaspStepString;
    extern const std::string direct;
    extern const std::string cartesian;

    extern const std::string lammpsAuxFileUnitsString;
    extern const std::string lammpsAuxFileUnitsTypeLJString;
    extern const std::string lammpsAuxFileUnitsTypeMetalString;
    extern const std::string lammpsAuxFileUnitsTypeRealString;
    extern const std::string unitsTypeLJ;
    extern const std::string unitsTypeMetal;
    extern const std::string unitsTypeReal;
    extern const std::string lammpsAuxFileTimestepString;
    extern const std::string vaspAuxFileTimestepString;

    extern const std::string outputDataFileName;
};


#endif //MSD_ANALYSIS_CONSTANTS_H
