#include "drifires/action.hpp"
#include "drifires/configurable.hpp"
#include "drifires/util.hpp"
#include "drifires/object.hpp"
#include "drifires/medium.hpp"
#include "drifires/factory.hpp"

#include "Garfield/ViewCell.hh"
#include "Garfield/ViewField.hh"
#include "Garfield/ViewMedium.hh"

#include "TCanvas.h"

#include <iostream>

using namespace drifires;

struct PlotDiags : public Action, public Configurable<PlotDiagCfg> {

    virtual object act(Component& cmp, Drifter& dft) {
        auto& sensor = cmp.sensor();
        sensor.SetArea();

        auto& med = drifires::medium(cfg.medium);

        TCanvas canvas("c", "", 600, 600); // pixels
        canvas.Print(Form("%s[", cfg.pdf.c_str()),"pdf");

        Garfield::ViewCell cellView;
        cellView.SetCanvas(&canvas);
        cellView.SetArea();
        cmp.enable_view(cellView);
        cellView.DisableWireMarkers();

        Garfield::ViewField fieldView;
        fieldView.SetCanvas(&canvas);
        fieldView.SetSensor(&sensor);

        Garfield::ViewMedium mediumView;
        mediumView.SetMedium(&med.medium());
        mediumView.SetCanvas(&canvas);
        mediumView.EnableAutoRangeX(true);
        mediumView.EnableAutoRangeY(true);

        mediumView.PlotElectronVelocity('e');
        canvas.Print(cfg.pdf.c_str(),"pdf");
        canvas.SetLogx(false);

        for (auto area : cfg.areas) {

            fieldView.SetArea(area.xmin/gfunits::length,
                              area.ymin/gfunits::length,
                              area.xmax/gfunits::length,
                              area.ymax/gfunits::length);
            cellView.SetArea(area.xmin/gfunits::length,
                             area.ymin/gfunits::length,
                             area.xmax/gfunits::length,
                             area.ymax/gfunits::length);

            canvas.Clear();
            fieldView.Plot("v", "COLZ");
            canvas.Print(cfg.pdf.c_str(),"pdf");

            canvas.Clear();
            fieldView.Plot("e", "COLZ");
            canvas.Print(cfg.pdf.c_str(),"pdf");

            canvas.Clear();
            fieldView.PlotContourWeightingField("u00","v");
            cellView.Plot2d();
            canvas.Print(cfg.pdf.c_str(),"pdf");

            canvas.Clear();
            fieldView.PlotContourWeightingField("v00","v");
            cellView.Plot2d();
            canvas.Print(cfg.pdf.c_str(),"pdf");

            canvas.Clear();
            fieldView.PlotContourWeightingField("w00","v");
            cellView.Plot2d();
            canvas.Print(cfg.pdf.c_str(),"pdf");

        }
        canvas.Print(Form("%s]", cfg.pdf.c_str()),"pdf");
        canvas.Clear();
        return {};
    }
};

static std::string
_PlotDiags = factory<Action>("PlotDiags").set_maker([] {
    return new PlotDiags;
});



