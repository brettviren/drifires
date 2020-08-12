#include "drifires/action.hpp"
#include "drifires/util.hpp"

#include "Garfield/ViewDrift.hh"
#include "Garfield/ViewCell.hh"
#include "TCanvas.h"

#include <iostream>

using namespace drifires;

struct PlotDrifts : public Action {

    Binning trange;
    std::vector<double> impacts;
    double ystart;
    object cfg;

    virtual void configure(object obj) {
        cfg = obj;
        cfg["ystart"].get_to(ystart);
        trange = cfg["trange"].get<Binning>();
        impacts = cfg["impacts"].get<std::vector<double>>();
    }

    virtual object act(Component& cmp, Drifter& dft) {
        auto& sens = cmp.sensor();
        sens.SetArea();
        sens.SetTimeWindow(trange.lo/gfunits::time,
                           binsize(trange)/gfunits::time, trange.nbins);

        
        Garfield::ViewDrift driftView;
        dft.enable_plotting(driftView);
        // auto bb = cmp.bounds();
        // driftView.SetArea(bb[0].first/gfunits::length,
        //                   bb[1].first/gfunits::length,
        //                   bb[0].second/gfunits::length,
        //                   bb[1].second/gfunits::length);
        driftView.SetArea();

        for (const auto& imp : impacts) {
            std::cerr << "drift: x="<<imp/units::mm << " mm"
                      << " y="<<ystart/units::mm
                      << std::endl;
            dft.drift_electron(imp, ystart, 0, 0);
        }

        auto pdf = cfg["pdf"].get<std::string>();

        std::cerr << "Making canvas\n";
        TCanvas canvas("c", "", 600, 600); // pixels

        canvas.Print(Form("%s[", pdf.c_str()),"pdf");
        driftView.SetCanvas(&canvas);

        Garfield::ViewCell cellView;
        cellView.SetCanvas(&canvas);
        cmp.enable_view(cellView);
        cellView.DisableWireMarkers();

        for (auto area : cfg["areas"]) {
            double xmin=area["xmin"];
            double ymin=area["ymin"];
            double xmax=area["xmax"];
            double ymax=area["ymax"];
            
            canvas.Clear();
            driftView.SetArea(xmin/gfunits::length,
                              ymin/gfunits::length,
                              xmax/gfunits::length,
                              ymax/gfunits::length);
            cellView.SetArea(xmin/gfunits::length,
                             ymin/gfunits::length,
                             xmax/gfunits::length,
                             ymax/gfunits::length);
            driftView.SetCanvas(&canvas);
            driftView.Plot(true);
            cellView.Plot2d();
            canvas.Print(pdf.c_str(),"pdf");
        }
    
        canvas.Print(Form("%s]", pdf.c_str()),"pdf");
        canvas.Clear();

        return cfg;             // fixme: return something more usefl
    } // act
};



#include "drifires/factory.hpp"
static std::string
_PlotDrifts = factory<Action>("PlotDrifts").set_maker([] {
    return new PlotDrifts;
});



