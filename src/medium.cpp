#include "drifires/medium.hpp"
#include "drifires/factory.hpp"
#include "drifires/util.hpp"

#include "drifires/object.hpp"

drifires::Medium& drifires::medium(drifires::object cfg)
{
    return drifires::factory_getcfg<drifires::Medium>(cfg);
}


#include "drifires/MediumLar.hpp"
struct mediumlar : public drifires::Medium, public drifires::Configurable<drifires::MediumLarCfg> {
    Garfield::MediumLar med;
    Garfield::Medium& medium() { return med; }
    void initialize() {
        med.SetTemperature(cfg.temperature/gfunits::temperature);
        med.SetMassDensity(cfg.density/gfunits::density);
    }
};


#include "drifires/factory.hpp"
static std::string
mytypename = drifires::factory<drifires::Medium>("MediumLar").set_maker([] {
    return new mediumlar;
});
