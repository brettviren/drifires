local units = import "drifires/units.jsonnet";

local ind_plane = {
    hole_pitch: 10.0*units.mm/3.0,
    strip_pitch: 5.0*units.mm,
    strip_width: 4.5*units.mm,
    depth: 0.32*units.cm,
    thickness: 0.150*units.mm,
    potential: 0.0*units.volt
};    
local col_plane = {
    hole_pitch: 10.0*units.mm/3.0,
    strip_pitch: 5.0*units.mm,
    strip_width: 4.7*units.mm,
    depth: 0.00*units.cm,
    thickness: 0.150*units.mm,
    potential: 2000*units.volt
};

{

    // for hole3d geom

    component: {
        type: "neBEMHoles3D",

        medium: {
            "type": "MediumLar",
            temperature : 89*units.K,   // fixme: factor to own file
        },

        diameter: 2.0*units.mm,
        neighbors: 5,                       
        ind: ind_plane,
        col: col_plane,
    },

    drifter: {
        // type: names the type to use, the rest are type-specfic params
        type: "DriftLineRKF",
        
        integration_accuracy : 0.000001*units.cm,
        maxstep : 1*units.mm,

        throw_ok: true,        // false=ignore garfield++ failures
    },

    action: {
        type: 'PlotField3D',
    }
}
