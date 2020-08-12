#include "drifires/action.hpp"
#include "drifires/configurable.hpp"
#include "drifires/util.hpp"
#include "drifires/object.hpp"

#include "Garfield/ViewDrift.hh"
#include "Garfield/ViewCell.hh"
#include "TCanvas.h"



#include <iostream>

using namespace drifires;

struct PlotDrifts : public Action, public Configurable<PlotDriftsCfg> {

    virtual object act(Component& cmp, Drifter& dft) {
        auto& sens = cmp.sensor();
        sens.SetArea();
        sens.SetTimeWindow(cfg.trange.lo/gfunits::time,
                           binsize(cfg.trange)/gfunits::time, cfg.trange.nbins);

        
        Garfield::ViewDrift driftView;
        dft.enable_plotting(driftView);
        driftView.SetArea();

        for (const auto& imp : cfg.impacts) {
            std::cerr << "drift: x="<<imp/units::mm << " mm"
                      << " y="<<cfg.ystart/units::mm
                      << std::endl;
            dft.drift_electron(imp, cfg.ystart, 0, 0);
        }

        std::cerr << "Making canvas\n";
        TCanvas canvas("c", "", 600, 600); // pixels

        canvas.Print(Form("%s[", cfg.pdf.c_str()),"pdf");
        driftView.SetCanvas(&canvas);

        Garfield::ViewCell cellView;
        cellView.SetCanvas(&canvas);
        cmp.enable_view(cellView);
        cellView.DisableWireMarkers();

        for (auto area : cfg.areas) {
            canvas.Clear();
            driftView.SetArea(area.xmin/gfunits::length,
                              area.ymin/gfunits::length,
                              area.xmax/gfunits::length,
                              area.ymax/gfunits::length);
            cellView.SetArea(area.xmin/gfunits::length,
                             area.ymin/gfunits::length,
                             area.xmax/gfunits::length,
                             area.ymax/gfunits::length);
            driftView.SetCanvas(&canvas);
            driftView.Plot(true);
            cellView.Plot2d();
            canvas.Print(cfg.pdf.c_str(),"pdf");
        }
    
        canvas.Print(Form("%s]", cfg.pdf.c_str()),"pdf");
        canvas.Clear();

        return cfg;             // fixme: return something more usefl
    } // act
};



#include "drifires/factory.hpp"
static std::string
_PlotDrifts = factory<Action>("PlotDrifts").set_maker([] {
    return new PlotDrifts;
});



