#include "drifires/medium.hpp"
#include "drifires/factory.hpp"

drifires::Medium& drifires::medium(drifires::object cfg)
{
    return drifires::factory_getcfg<drifires::Medium>(cfg);
}

// Hook in Garfield media

#include "drifires/MediumLar.hpp"
struct mediumlar : public drifires::Medium {
    Garfield::MediumLar med;
    Garfield::Medium& medium() { return med; }
    void configure(drifires::object cfg) {
    }
};


#include "drifires/factory.hpp"
static std::string
mytypename = drifires::factory<drifires::Medium>("MediumLar").set_maker([] {
    return new mediumlar;
});
