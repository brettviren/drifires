#ifndef DRIFIRES_DRIFTER_HPP
#define DRIFIRES_DRIFTER_HPP

#include "drifires/object.hpp"

#include "Garfield/Sensor.hh"
#include "Garfield/ViewDrift.hh"

namespace drifires {

    // Garfield doesn't have a base for drifting.  Try to soak up that
    // here.
    struct Drifter {
        virtual void SetSensor(Garfield::Sensor &sensor) = 0;
        virtual void EnablePlotting(Garfield::ViewDrift& driftView) = 0;
        virtual void DriftElectron(double x, double y, double z, double t) = 0;

        virtual void configure(object cfg) = 0;
    };

    Drifter& drifter(object cfg);
}

#endif
