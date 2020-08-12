#ifndef DRIFIRES_DRIFIRES_STRUCTS_HPP
#define DRIFIRES_DRIFIRES_STRUCTS_HPP

#include <string>
#include <vector>

namespace drifires {


    struct TypeName{
        std::string type;
        std::string name;
    };


    struct Area2d{
        double xmin;
        double ymin;
        double xmax;
        double ymax;
    };


    struct Binning{
        double lo;
        double hi;
        int nbins;
    };


    struct Point{
        double x;
        double y;
        double z;
        double t;
    };


    struct Layer{
        std::string name;
        double loc;
        double pot;
        bool readout;
        int nwires;
        int nextra;
        double pitch;
        double dia;
    };


    struct Layered{
        TypeName tn;
        double periodicity;
        std::vector<Layer> layers;
    };


    struct Stepper{
        TypeName tn;
        double accuracy;
        double maxstep;
        bool throw_ok;
    };


    struct MediumLarCfg{
        double temperature;
        double density;
    };


    struct PlotDriftsCfg{
        double ystart;
        Binning trange;
        std::vector<double> impacts;
        std::vector<Area2d> areas;
        std::string pdf;
    };


} // namespace drifires

#endif