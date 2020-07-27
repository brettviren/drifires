#include "drifires/drifter.hpp"
#include "Garfield/DriftLineRKF.hh"

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

    virtual void SetSensor(Garfield::Sensor &sensor) {
        rkf.SetSensor(&sensor);
        rkf.EnableSignalCalculation();
    }
    virtual void EnablePlotting(Garfield::ViewDrift& driftView) {
        rkf.EnablePlotting(&driftView);
    }
    virtual void DriftElectron(double x, double y, double z, double t) {
        rkf.DriftElectron(x,y,z,t);
    }

    virtual void configure(drifires::object obj) {
        RkfCfg cfg = obj;
        rkf.SetIntegrationAccuracy(cfg.accuracy);
        rkf.SetMaximumStepSize(cfg.maxstep);
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
