#include "drifires/medium.hpp"
#include "drifires/factory.hpp"
#include "drifires/util.hpp"

#include "drifires/object.hpp"
using LarCfg = drifires::MediumLarCfg;


drifires::Medium& drifires::medium(drifires::object cfg)
{
    return drifires::factory_getcfg<drifires::Medium>(cfg);
}


#include "drifires/MediumLar.hpp"
struct mediumlar : public drifires::Medium {
    Garfield::MediumLar med;
    Garfield::Medium& medium() { return med; }
    void configure(drifires::object obj) {
        auto cfg = obj.get<LarCfg>();
        med.SetTemperature(cfg.temperature/gfunits::temperature);
        med.SetMassDensity(cfg.density/gfunits::density);
    }
};


#include "drifires/factory.hpp"
static std::string
mytypename = drifires::factory<drifires::Medium>("MediumLar").set_maker([] {
    return new mediumlar;
});
