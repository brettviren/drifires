#include "drifires/action.hpp"
#include "drifires/util.hpp"

#include <iostream>
#include <fstream>

using namespace drifires;

struct HalfRegionResponse : public Action {

    Binning trange, irange;
    double ystart;
    int neighbors;
    double nudge;
    double pitch;
    object FR;
    std::string filename;

    virtual void configure(object cfg) {
        std::cerr << "response: configure" << std::endl;
        cfg["neighbors"].get_to(neighbors);
        cfg["nudge"].get_to(nudge);

        cfg["ystart"].get_to(ystart);

        cfg["trange"].get_to(trange);
        cfg["irange"].get_to(irange);
        cfg["filename"].get_to(filename);

        FR = cfg["FieldResponse"];
    }

    virtual object act(Component& cmp, Drifter& dft) {
        auto& sens = cmp.sensor();
        sens.SetArea();
        sens.SetTimeWindow(trange.lo, trange.binsize(), trange.nbins);
        
        auto labels = cmp.readout_labels();

        object ret;

        std::vector<int> iwires(1,0);
        for (int iwire=0; iwire<neighbors; ++iwire) {
            iwires.push_back(  iwire+1);
            iwires.push_back(-(iwire+1));
        }

        auto impacts = irange.edges();
        for (size_t ind=0; ind < impacts.size(); ++ind) {
            double imp = impacts[ind];
            if (ind == 0) {
                imp += nudge;
            }
            if (ind == impacts.size() -1) {
                imp -= nudge;
            }

            std::cerr << "drift: #"<<ind<<" at x="<<imp << std::endl;
            dft.DriftElectron(imp, ystart, 0, 0);

            for (auto& plane : FR["planes"]) {
                const double pitch = plane["pitch"];
                
                for (int iwire : iwires) {
                    auto name = wire_name(plane["name"], iwire);
                    object path;
                    path["pitchpos"] =  pitch*iwire + impacts[0];
                    path["wirepos"] = 0.0;
                    object signal;
                    for (int icur=0; icur<trange.nbins; ++icur) {
                        double sig = sens.GetElectronSignal(name, icur);
                        signal.push_back(sig);
                    }
                    path["current"] = signal;
                    plane["paths"].push_back(path);
                }
            }
        }
        std::ofstream out(filename);
        if (!out) {
            throw std::runtime_error("fail to open " + filename);
        }
        out << FR.dump(4);
        return object({});
    }


};

#include "drifires/factory.hpp"
static std::string
_HalfRegionResponse = factory<Action>("HalfRegionResponse").set_maker([] {
    return new HalfRegionResponse;
});





