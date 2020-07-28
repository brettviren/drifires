#include "drifires/medium.hpp"
#include "drifires/factory.hpp"
#include "drifires/util.hpp"

using drifires::maybe_to;

drifires::Medium& drifires::medium(drifires::object cfg)
{
    return drifires::factory_getcfg<drifires::Medium>(cfg);
}

// Hook in Garfield media

struct LarCfg {
    double temperature{89.0*units::K};
    double density{1.3954*units::gram/units::cm3};
};
void from_json(const drifires::object& j, LarCfg c)
{
    maybe_to(j, "temperature", c.temperature);
    maybe_to(j, "density", c.density);
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
