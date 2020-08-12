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


    struct Layered : public TypeName {
        double periodicity;
        std::vector<Layer> layers;
        TypeName medium;
    };


    struct Stepper : public TypeName {
        double accuracy;
        double maxstep;
        bool throw_ok;
    };


    struct MediumLarCfg : public TypeName {
        double temperature;
        double density;
    };


    struct PlotDriftsCfg{
        std::string type;
        std::string name;
        double ystart;
        Binning trange;
        std::vector<double> impacts;
        std::vector<Area2d> areas;
        std::string pdf;
    };


    struct PlotDiagCfg{
        std::string type;
        std::string name;
        std::vector<Area2d> areas;
        std::string pdf;
        TypeName medium;
    };


    struct PlaneResponseStub{
        int planeid;
        double location;
        double pitch;
    };


    struct PlaneResponseWrap{
        std::string name;
        PlaneResponseStub PlaneResponse;
    };


    struct FieldResponseStub{
        std::vector<PlaneResponseWrap> planes;
        std::vector<double> axis;
        double tstart;
        double origin;
        double period;
        double speed;
    };


    struct HalfRegionResponseCfg{
        Binning trange;
        Binning irange;
        double ystart;
        int neighbors;
        double nudge;
        FieldResponseStub FieldResponse;
    };


} // namespace drifires

#endif