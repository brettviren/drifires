#include "drifires/action.hpp"
#include "drifires/util.hpp"

#include "Garfield/ViewField.hh"

#include "TCanvas.h"

using namespace drifires;

struct PlotField3D : public Action {
    
    virtual void configure(object obj) {}
    virtual object act(Component& cmp, Drifter& dft) {


        Garfield::Medium* medium = nullptr; 
        double ex = 0., ey = 0., ez = 0., v = 0.;
        int status = 0;
        cmp.component().ElectricField(0, 0, 0, ex, ey, ez, v, medium, status);
        std::printf("E = (%15.8f, %15.8f %15.8f), V = %15.8f, status = %d\n", ex, ey, ez, v, status);

        TCanvas canvas("c", "", 600, 600); // pixels
        
        Garfield::ViewField fieldView;
        fieldView.SetCanvas(&canvas);
        fieldView.SetComponent(&cmp.component());
        //fieldView.SetArea(-10, -10, -1, 10, 10, 2);
        fieldView.SetArea();
        fieldView.SetPlane(0, 0, 1, 0, 0, 0.2);
        fieldView.PlotContour();
        
        canvas.Print("plot-field-3d.pdf","pdf");
        canvas.Clear();

        return object({});
    }
};


#include "drifires/factory.hpp"
static std::string
_PlotField3d = factory<Action>("PlotField3D").set_maker([] {
    return new PlotField3D;
});



