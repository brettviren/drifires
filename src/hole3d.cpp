/**
   holes3d provides a 3D neBEM model of the PCB anode detector.

   This is two planes of strips in mutually orthogonal directions with
   a hexagonal pattern of holes.  

   Here, induction strips run parallel to the Y axis (anti-drift is Z
   axis) and numbered in X direction (large, positive X has large,
   positive number).  Collection strips assumed to run parallel to the
   X axis and numbered in Y direction.  Note, this is a cyclic
   permutation from convention used by WCT where X-axis is anti-drift.

 */

#include "drifires/util.hpp"
#include "drifires/object.hpp"
#include "drifires/medium.hpp"
#include "drifires/component.hpp"

#include "Garfield/SolidHole.hh"
#include "Garfield/GeometrySimple.hh"
#include "Garfield/MediumConductor.hh"
#include "Garfield/MediumPlastic.hh"
#include "Garfield/ComponentNeBem3d.hh"

#include <iostream>

using drifires::maybe_to;

struct PlaneConfig {
    // Distance between hole centers along this plane's strip
    // direction.
    double hole_pitch;
    // Distance between centerlines of neighboring strips.
    double strip_pitch;
    // Width of strip electrode 
    double strip_width;
    // Location in drift direction
    double depth;
    // Thickness of electrode
    double thickness;
    // Electrical potential (in drifires system of units)
    double potential;
   
};

void from_json(const drifires::object& j, PlaneConfig& c) {
    maybe_to(j, "hole_pitch", c.hole_pitch);
    maybe_to(j, "strip_pitch", c.strip_pitch);
    maybe_to(j, "strip_width", c.strip_width);
    maybe_to(j, "depth", c.depth);
    maybe_to(j, "thickness", c.thickness);
    maybe_to(j, "potential", c.potential);
}

// Configuration for hole3d.  Defaults taken from a CAD diagram for
// the 50L PCB anode.  
struct Config {
    // larger hole is 2.5mm diameter
    double diameter{2.0*units::mm}; 

    PlaneConfig ind{
        10.0*units::mm/3.0,
        5.0*units::mm,
        4.5*units::mm,
        0.32*units::cm,
        0.150*units::mm,
        0.0*units::volt};
    PlaneConfig col{
        10.0*units::mm/3.0,
        5.0*units::mm,
        4.7*units::mm,
        0.00*units::cm,
        0.150*units::mm,
        2000*units::volt};

    // Number of strips beyond central strip to consider.
    int neighbors{5};
};

void from_json(const drifires::object& j, Config& c) {
    maybe_to(j, "diameter", c.diameter);
    if (j.contains("ind")) {
        from_json(j["ind"], c.ind);
    }
    if (j.contains("col")) {
        from_json(j["col"], c.col);
    }
    maybe_to(j, "neighbors", c.neighbors);
}

struct Holes3d : public drifires::Component {
    Garfield::ComponentNeBem3d cmp;
    Garfield::Sensor sens;
    Garfield::GeometrySimple geo;
    Garfield::MediumConductor copper;
    Garfield::MediumPlastic g10;
    std::vector<Garfield::Solid*> sgarbage;

    virtual void configure(drifires::object cfgobj) {
        auto& med = drifires::medium(cfgobj["medium"]);
        geo.SetMedium(&med.medium());
        sens.AddComponent(&cmp);

        const auto cfg = cfgobj.get<Config>();

        const double radius = 0.5 * cfg.diameter;

        // induction strips run in Y direction, march across in X direction
        const auto& ic = cfg.ind;
        const double ind_strip_length = (1+2*cfg.neighbors) * cfg.col.strip_pitch;
        for (int istrip = -cfg.neighbors; istrip <= cfg.neighbors; ++istrip) {
            double center_x = istrip * ic.strip_pitch;
            const std::string label = drifires::wire_name("v",istrip);

            // along strip, do center full holes

            int ijump = -1;
            for (double c0_y = -0.5*ind_strip_length + 0.5*ic.hole_pitch + radius;
                 c0_y <= 0.5*ind_strip_length + 0.5*ic.hole_pitch + radius;
                 c0_y += ic.hole_pitch)
            {
                ++ijump;

                std::cerr << ijump << ": " << c0_y << std::endl;
                auto hole = new Garfield::SolidHole(
                    center_x, c0_y, ic.depth, radius, radius,
                    0.5*ic.strip_width,0.499*ic.hole_pitch,0.5*ic.thickness);
                hole->SetBoundaryPotential(ic.potential);
                hole->SetLabel(label);
                sgarbage.push_back(hole);
                geo.AddSolid(hole,&copper);
                if (ijump == 1) { break; }
            }
            break;
        }

        cmp.SetGeometry(&geo);
        cmp.Initialise();
    }

    Garfield::Sensor& sensor() { return sens; };

    virtual Garfield::ComponentBase& component() { return cmp; }


};

#include "drifires/factory.hpp"
static std::string
mytypename = drifires::factory<drifires::Component>("neBEMHoles3D").set_maker([] {
    return new Holes3d;
});
