#ifndef DRIFIRES_DRIFTER_HPP
#define DRIFIRES_DRIFTER_HPP

#include "drifires/object.hpp"

#include "Garfield/Sensor.hh"
#include "Garfield/ViewDrift.hh"

namespace drifires {

    // Garfield doesn't have a base for drifting.  Try to soak up that
    // here.
    struct Drifter {
        virtual void set_sensor(Garfield::Sensor &sensor) = 0;
        virtual void enable_plotting(Garfield::ViewDrift& driftView) = 0;
        virtual void drift_electron(double x, double y, double z, double t) = 0;
    };

    Drifter& drifter(object cfg);
}

#endif
