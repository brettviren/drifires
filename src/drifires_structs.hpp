#ifndef DRIFIRES_DRIFIRES_STRUCTS_HPP
#define DRIFIRES_DRIFIRES_STRUCTS_HPP

#include <string>
#include <vector>

namespace drifires {


    struct TypeName{
        std::string type;
        std::string name;
    };


    struct Binning{
        float lo;
        float hi;
        int nbins;
    };


    struct Point{
        float x;
        float y;
        float z;
        float t;
    };


    struct Layer{
        std::string name;
        float loc;
        float pot;
        bool readout;
        int nwires;
        int nextra;
        float pitch;
        float dia;
    };


    struct Layered{
        TypeName tn;
        float periodicity;
        std::vector<Layer> layers;
    };


    struct Stepper{
        TypeName tn;
        float accuracy;
        float maxstep;
        bool throw_ok;
    };


} // namespace drifires

#endif