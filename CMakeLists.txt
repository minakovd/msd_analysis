cmake_minimum_required(VERSION 3.7)
project(msd_analysis)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Reader.cpp Reader.h Cell.cpp Cell.h AtomCoordinate.cpp AtomCoordinate.h Constants.cpp Constants.h MsdPoint.cpp MsdPoint.h MsdCalculator.cpp MsdCalculator.h)
add_executable(msd_analysis ${SOURCE_FILES})

#link_directories(/opt/local/lib)
find_package(GSL REQUIRED)
target_link_libraries(msd_analysis GSL::gsl GSL::gslcblas)