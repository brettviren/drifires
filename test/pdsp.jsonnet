// Configure drifires-cli to generate PDSP / DUNE APA responses.
// The result should correspond to dune-garfield-1d565.json.bz2


// A subset of WCT units.  These are NOT the same as Garfield (in
// general).  ALL numbers accessible to users must be expressed in WCT
// system of units.  drifiers will convert between WCT and Garfield
// when needed.
local units = {
    mm: 1.0,
    cm : 10*self.mm,
    ns: 1.0,
    us: 1e3*self.ns,
    eplus: 1.0,
    MeV: 1.0,
    megavolt: self.MeV/self.eplus,
    volt: 1e-6*self.megavolt, 
    K: 1.0,
};

local temperature = 89*units.K;

local integration_accuracy = 0.000001*units.cm;
// 1mm is too fast to allow nudges smaller than 1 radius
//local maxstep = 0.1*units.mm;
local maxstep = 1*units.mm;
// How to sample response. 100us in 0.1us bins is usual
local trange = { lo:0.0, hi: 100.0*units.us, nbins: 1000 };

// Distance between anode wire planes
local plane_gap=4.76*units.mm;

// A virtual 'cathode' plane where the responses start and which holds
// the potential setting the drift field.  Adding plane_gap closely
// corresponds to the time shift Wenqiang added to the
// dune-garfield-1d565.json.bz2 fields.  This is a time shifted
// version of garfield-1d-boundary-path-rev-dune.json.bz2.  Removing
// the plane_gap will be w/in ~1us of this unshifted result.  There is
// still a ~5% time scale difference.
local response_plane = 10.0*units.cm+plane_gap;

// Set the nominal drift field.
local drift_field = 500.0*units.volt/units.cm;

// The "bulk" drift speed is NOT used by drifires and is merely copied
// to the output WCT .json.bz2 file.  Detailed drift velocity is
// copied inside the MediumLar drifires/Garfield++ class.  Wenqiang
// found PDSP is somewhat less than nominal 1.6mm/us.
local bulk_drift_speed = 1.565*units.mm/units.us;

// The sampling for the produced responses.
local sample_period = 0.1*units.us;

// Number of wires beyond the central wire-of-interest to calculate the fields.
local max_wire = 10;
local nwires = 2*max_wire + 1;

// distance between neighboring wires in a plane
// larsoft U/V: 4.762mm, W: 4.792mm
// DUNE TDR: U/V: 4.669mm, W: 4.790mm
// original GARFIELD 4.71
local wire_pitch = 4.71*units.mm;

// Diameter of wires
local wire_diameter = 0.150*units.mm;

// Distance to offset the first and last impact position so as to not
// sample unlikely and anomolous paths which occur exactly along a
// line of symmetry.  A nudge of 0.01*wire_diameter is needed to
// reproduce the delayed "prong" of the central response function.
//local nudge = 0.01*wire_diameter;
local nudge = 0.5*wire_diameter;

// Make a wire plane specification
local wspec(name,loc,pot, readout=true) = {
    name:name,
    loc:loc,
    pot:pot,
    nwires: nwires,             // in a plane
    pitch: wire_pitch,
    dia: wire_diameter,
    readout: readout,
};    

// Make a simple, infinite plane specification
local pspec(name, loc, pot, readout=false) = {
    name:name,
    loc:loc,
    pot:pot,
    nwires: 0,                  // marks as simple plane, not wire plane
    readout: readout
};

local plane_locations = {
    c: pspec("c", response_plane, -drift_field*response_plane),
    g: wspec("g", 3*plane_gap, -665.0*units.volt, false),
    u: wspec("u", 2*plane_gap, -370.0*units.volt),
    v: wspec("v", 1*plane_gap,    0.0*units.volt),
    w: wspec("w", 0*plane_gap,  820.0*units.volt),
    m: pspec("m",-1*plane_gap,    0.0*units.volt),
};


function(action = "response") 
{
    // A drifires component constructs a Garfield ComponentBase.
    // Except for type: and optional name: the rest of the parameters
    // are specific to the type.
    component: {
        type:  "AnalyticWire2D", 
        // 2D cathode + wire anode planes and maybe "grid" and "mesh" planes

        // Medium is intrinsic to the component
        medium : { type: "MediumLar", temperature: temperature},

        periodicity: nwires*wire_pitch,
        layers: [ plane_locations[l] for l in ["c","g","u","v","w","m"]],
    },
    
    // Specify drift information
    drifter: {
        // type: names the type to use, the rest are type-specfic params
        type: "DriftLineRKF",
        
        accuracy: integration_accuracy, // cm
        maxstep: maxstep,       // cm
        throw_ok: true,        // false=ignore garfield++ failures
    },

    action: self.actions[action],

    actions: {
        plot_drifts : {
            type: "PlotDrifts",
            local xmax = +max_wire*wire_pitch,
            local xmin = -max_wire*wire_pitch,
            local ymax = response_plane,
            local ymin = -2*plane_gap,
            pdf: "pdsp-drifts.pdf",
            areas: [{xmin:-0.6*units.cm, ymin:-0.1*units.cm, xmax:0.6*units.cm, ymax:2.0*units.cm},
                    {xmin:xmin, ymin:ymin, xmax:xmax, ymax:ymax}],
            ystart: response_plane,
            trange: trange,

            // Hit nominal and "nudged" impacts across two wire regions.
            local impact_pitch = wire_pitch/10.0,
            impacts: std.sort( [ 0.0, -nudge, +nudge,
                                 -wire_pitch+nudge, wire_pitch-nudge,
                                 -0.5*wire_pitch-nudge, -0.5*wire_pitch+nudge, 
                                 +0.5*wire_pitch+nudge, +0.5*wire_pitch-nudge ]
                               + [ -impact_pitch*n for n in std.range(1,10) ]
                               + [ impact_pitch*n for n in std.range(1,10)])


        } ,
        response : {
            type: "HalfRegionResponse",

            filename: "pdsp-response.json",

            // number of wires beyond the central wire.  Use no more
            // than were used to make the component.
            neighbors: max_wire,
            
            // where to start the paths, in cm
            ystart: response_plane,

            local pln(name,id) = {
                name:name,
                PlaneResponse: {
                    planeid:id,
                    location: plane_locations[name].loc,
                    pitch: wire_pitch}},

            // skeleton of WCT field response.  
            FieldResponse : {
                planes: [ pln("u",0), pln("v",1), pln("w",2)],
                axis: [1,0,0],
                origin: response_plane,
                tstart: 0,
                period: sample_period,
                speed: bulk_drift_speed,
            },
                    

            // The time range of the responses
            trange: trange,
            // the impacts to drift, they are always below the wire....
            // irange: { lo:0, hi:0.5*wire_pitch, nbins:6-1},
            irange: { lo:-0.5*wire_pitch, hi:0.0, nbins:6-1},
            // how much to nudge first and last
            nudge: nudge,
            
        }
    },
}


