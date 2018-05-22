//
// Created by Дмитрий Минаков on 08.05.2018.
//

#include "Constants.h"

namespace Constants {
    extern const std::string vasp = "VASP";
    extern const std::string lammps = "LAMMPS";
    extern const std::string vaspIdentityString = "vasp";
    extern const std::string lammpsIdentityString = "ITEM";
    extern const std::string direct = "direct";
    extern const std::string cartesian = "cartesian";
    extern const std::string vaspStepString = "Direct";
    extern const std::string lammpsStepString = "ITEM: ATOMS";

    extern const std::string lammpsAuxFileUnitsString = "units";
    extern const std::string lammpsAuxFileUnitsTypeLJString = "lj"; //dimensionless
    extern const std::string lammpsAuxFileUnitsTypeMetalString = "metal"; //t = ps, d = A
    extern const std::string lammpsAuxFileUnitsTypeRealString = "real"; //t = fs, d = A
    extern const std::string unitsTypeLJ = "dimensionless (Lennard-Jones)";
    extern const std::string unitsTypeMetal = "metallic (ps, Angstroms)";
    extern const std::string unitsTypeReal = "real (fs, Angstroms)";

    extern const std::string lammpsAuxFileTimestepString = "timestep";
    extern const std::string vaspAuxFileTimestepString = "POTIM";

    extern const std::string outputDataFileName = "msd_data.dat";
}
