#include "drifires/drifter.hpp"
#include "drifires/util.hpp"
#include "Garfield/DriftLineRKF.hh"

#include <iostream>

struct RkfCfg {
    drifires::TypeName tn;
    double accuracy;
    double maxstep;
};

void from_json(const drifires::object& j, RkfCfg& c)
{
    from_json(j, c.tn);
    j.at("accuracy").get_to(c.accuracy);
    j.at("maxstep").get_to(c.maxstep);
}

struct driftlinerkf : public drifires::Drifter {
    Garfield::DriftLineRKF rkf;

    virtual void set_sensor(Garfield::Sensor &sensor) {
        rkf.SetSensor(&sensor);
        rkf.EnableSignalCalculation();
    }
    virtual void enable_plotting(Garfield::ViewDrift& driftView) {
        rkf.EnablePlotting(&driftView);
    }
    virtual void drift_electron(double x, double y, double z, double t) {
        bool ok = rkf.DriftElectron(x/gfunits::length,
                                    y/gfunits::length,
                                    z/gfunits::length,
                                    t/gfunits::time);
        if (!ok) {
            throw std::runtime_error("failed to drift");
        }
    }

    virtual void configure(drifires::object obj) {
        RkfCfg cfg = obj;
        std::cerr << "RKF: acc=" << cfg.accuracy/units::mm
                  << " mm, max step=" << cfg.maxstep/units::mm << " mm" << std::endl;
        rkf.SetIntegrationAccuracy(cfg.accuracy/gfunits::length);
        rkf.SetMaximumStepSize(cfg.maxstep/gfunits::length);
        rkf.RejectKinks(true);
    }
};

#include "drifires/factory.hpp"

drifires::Drifter& drifires::drifter(object cfg)
{
    return drifires::factory_getcfg<drifires::Drifter>(cfg);
}

static std::string
mytypename = drifires::factory<drifires::Drifter>("DriftLineRKF").set_maker([] {
    return new driftlinerkf;
});
