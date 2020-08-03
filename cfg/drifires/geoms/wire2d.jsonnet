// Configure drifires-cli to generate 2D wire responses.
//
// run like jsonnet -A action=response -A det=pdsp.jsonnet

// The result should correspond to dune-garfield-1d565.json.bz2

local units = import "drifires/units.jsonnet";
local util = import "drifires/util.jsonnet";

function(det) 
{
    // A drifires component constructs a Garfield ComponentBase.
    // Except for type: and optional name: the rest of the parameters
    // are specific to the type.
    component: {
        type:  "AnalyticWire2D", 
        // 2D cathode + wire anode planes and maybe "grid" and "mesh" planes

        // Medium is intrinsic to the component
        medium : { type: "MediumLar", temperature: det.temperature},

        periodicity: det.nwires*det.wire_pitch,

        layers: util.values(det.plane_locations),
    },
    
    // Specify drift information
    drifter: {
        // type: names the type to use, the rest are type-specfic params
        type: "DriftLineRKF",
        
        accuracy: det.integration_accuracy, // cm
        maxstep: det.maxstep,       // cm
        throw_ok: true,        // false=ignore garfield++ failures
    },

}


