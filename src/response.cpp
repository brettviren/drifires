#include "drifires/action.hpp"
#include "drifires/util.hpp"

#include <iostream>
#include <fstream>
#include <vector>

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

        // FR schema requires each type to be held with a key of the type name.
        FR["FieldResponse"] = cfg["FieldResponse"];
    }

    virtual object act(Component& cmp, Drifter& dft) {
        auto& sens = cmp.sensor();
        sens.SetArea();
        sens.SetTimeWindow(trange.lo/gfunits::time, trange.binsize()/gfunits::time, trange.nbins);
        
        auto labels = cmp.readout_labels();

        std::vector<int> iwires;
        for (int iwire=-neighbors; iwire<0; ++iwire) {
            iwires.push_back(iwire);
        }
        for (int iwire=0; iwire<=neighbors; ++iwire) {
            iwires.push_back(iwire);
        }


        auto impacts = irange.edges();
        for (size_t ind=0; ind < impacts.size(); ++ind) {
            const double impact = impacts[ind];
            double impact_nudged = impact;
            if (ind == 0) {
                impact_nudged += nudge;
            }
            if (ind == impacts.size() -1) {
                impact_nudged -= nudge;
            }

            std::cerr << "drift: #"<<ind
                      <<" at x="<<impact_nudged/units::mm << " mm"
                      <<" y=" << ystart/units::mm << " mm"<< std::endl;
            dft.drift_electron(impact_nudged, ystart, 0, 0);

            // .FieldResponse.planes[0].PlaneResponse.paths[0].PathResponse.current.array|keys'
            // [ "elements", "shape" ]

            for (auto& plane : FR["FieldResponse"]["planes"]) {
                auto pname = plane["name"].get<std::string>();
                auto& plane_response = plane["PlaneResponse"];
                const double pitch = plane_response["pitch"];
                
                std::cerr << "\tplane: " << pname
                          << " nwires=" << iwires.size()
                          << " pitch=" << pitch/units::mm << "mm" << std::endl;
                for (int iwire : iwires) {
                    // note, we invert the iwire.  a response on wire0
                    // due to drift near wire+3 is same response on
                    // wire-3 from drift near wire0 and all drifts are
                    // near wire0.
                    auto wname = wire_name(pname, -iwire);
                    object elements;
                    for (int icur=0; icur<trange.nbins; ++icur) {
                        double sig = sens.GetElectronSignal(wname, icur)*gfunits::current;
                        elements.push_back(sig);
                    }
                    object shape{elements.size()};
                    object array{{"elements",elements}, {"shape",shape}};
                    object current{{"array",array}};
                    const double pitchpos = pitch*iwire + impact;

                    std::cerr << "\t\t" << ind << ": pp=" << pitchpos
                              << " iwire="<<iwire
                              << " '" << wname << "'"
                              << " imp="<<impact << std::endl;

                    object path_response{{"current",current},
                                         {"pitchpos", pitchpos},
                                         {"wirepos",0}};
                    object PR{{"PathResponse",path_response}};
                    plane_response["paths"].push_back(PR);
                }
            }

            sens.ClearSignal();
        }
        //std::ofstream out(filename);
        // if (!out) {
        //     throw std::runtime_error("fail to open " + filename);
        // }
        // out << FR.dump(4);
        // return object({});
        return FR;
    }


};

#include "drifires/factory.hpp"
static std::string
_HalfRegionResponse = factory<Action>("HalfRegionResponse").set_maker([] {
    return new HalfRegionResponse;
});





