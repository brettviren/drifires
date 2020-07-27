#include "drifires/action.hpp"

#include "Garfield/ViewDrift.hh"
#include "Garfield/ViewCell.hh"
#include "TCanvas.h"

#include <iostream>

using namespace drifires;

struct PlotDrifts : public Action {

    Binning trange, drange;
    double ystart;
    object cfg;

    virtual void configure(object obj) {
        cfg = obj;
        cfg["ystart"].get_to(ystart);
        trange = cfg["trange"].get<Binning>();
        drange = cfg["drange"].get<Binning>();
    }

    virtual object act(Component& cmp, Drifter& dft) {
        auto& sens = cmp.sensor();
        sens.SetArea();
        sens.SetTimeWindow(trange.lo, trange.binsize(), trange.nbins);

        
        Garfield::ViewDrift driftView;
        dft.EnablePlotting(driftView);
        auto bb = cmp.bounds();
        driftView.SetArea(bb[0].first, bb[1].first,
                          bb[0].second,bb[1].second);

        auto impacts = drange.edges();
        for (const auto& imp : impacts) {
            std::cout << "drift: x="<<imp << std::endl;
            dft.DriftElectron(imp, ystart, 0, 0);
        }

        auto pdf = cfg["pdf"].get<std::string>();

        std::cerr << "Making canvas\n";
        TCanvas canvas("c", "", 600, 600); // leak

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
            driftView.SetArea(xmin, ymin, xmax, ymax);
            cellView.SetArea(xmin, ymin, xmax, ymax);
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



