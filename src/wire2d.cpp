#include "drifires/component.hpp"
#include "drifires/medium.hpp"
#include "drifires/configurable.hpp"
#include "drifires/util.hpp"
#include "drifires/object.hpp"

#include "Garfield/ComponentAnalyticField.hh"

#include <iostream>

struct AnalyticWire2D : public drifires::Component, public drifires::Configurable<drifires::Layered> {
    Garfield::ComponentAnalyticField cmp;
    Garfield::Sensor sens;

    std::vector<std::string> readouts;

    virtual std::vector<std::string> readout_labels() {
        return readouts;
    }

    virtual Garfield::ComponentBase& component() { return cmp; }

    virtual void enable_view(Garfield::ViewCell& vc) {
        vc.SetComponent(&cmp);
    }

    virtual void initialize() {

        // medium is intrisic to the component
        auto& med = drifires::medium(cfg.medium);
        cmp.SetMedium(&med.medium());

        sens.AddComponent(&cmp);

        double ymin=1, ymax=-1;

        for (auto& layer : cfg.layers) {

            if (ymin > ymax) {
                ymin = ymax = layer.loc;
            }
            else {
                ymin = std::min(ymin,layer.loc);
                ymax = std::max(ymax,layer.loc);
            }

            const int nwires = layer.nwires;
            const int nextra = layer.nextra;
            if (nwires > 0) {
                const int ntotwires = 2*nextra + nwires;
                const double w0 = -ntotwires/2 * layer.pitch;
                for (int iwire=0; iwire < ntotwires; ++iwire) {
                    const double wx = w0 + iwire * layer.pitch;
                    const int nrel = iwire - ntotwires/2;
                    auto nam = drifires::wire_name(layer.name, nrel);
                    std::cerr << "add wire: " << nam
                              << " nrel:"<<nrel<<" iwire:"<<iwire
                              << " wx:"<<wx << " w0:"<<w0 <<std::endl;
                    cmp.AddWire(wx/gfunits::length,        // x
                                layer.loc/gfunits::length, // y
                                layer.dia/gfunits::length, // diameter
                                layer.pot/gfunits::pot, // potential
                                nam);

                    if (layer.readout and std::abs(nrel) <= nwires/2) {
                        cmp.AddReadout(nam);
                        sens.AddElectrode(&cmp, nam);
                        readouts.push_back(nam);
                    }
                }
            }
            else {              // plane
                cmp.AddPlaneY(layer.loc/gfunits::length, layer.pot/gfunits::pot, layer.name);
                if (layer.readout) {
                    cmp.AddReadout(layer.name);
                    sens.AddElectrode(&cmp, layer.name);
                    readouts.push_back(layer.name);
                }
            }
        }

        if (cfg.periodicity > 0.0) {
            cmp.SetPeriodicityX(cfg.periodicity/gfunits::length);
        }
        // bb.resize(3);
        // bb[0].first  = -cfg.periodicity/2.0;
        // bb[0].second =  cfg.periodicity/2.0;
        // bb[1].first  = ymin;
        // bb[1].second = ymax;
        // bb[2].first  = -50;
        // bb[2].second = +50;

        sens.EnableComponent(0, true);

    } // initialize

    Garfield::Sensor& sensor() { return sens; };

};


#include "drifires/factory.hpp"
static std::string
mytypename = drifires::factory<drifires::Component>("AnalyticWire2D").set_maker([] {
    return new AnalyticWire2D;
});
