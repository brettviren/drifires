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
};

local integration_accuracy = 0.000001*units.cm;
// 1mm is fast and probably okay.  0.1mm is also doable
local maxstep = 1*units.mm;
// How to sample response. 100us in 0.1us bins is usual
local trange = { lo:0.0, hi: 100.0*units.us, nbins: 1000 };
local response_plane = 10.0*units.cm;    // where the 'cathode' is
local drift_field = 500.0*units.volt/units.cm;      // V/cm, nominal drift field
local plane_gap=0.5*units.cm;   // between wire planes
local max_wire = 10;
local nwires = 2*max_wire + 1;
local wire_pitch = 0.5*units.cm;         // between wires
local nudge = 0.01*wire_pitch;       // nudge away from saddles

local wspec(name,loc,pot, readout=true) = {
    name:name,
    loc:loc,
    pot:pot,
    nwires: nwires,             // in a plane
    pitch: wire_pitch,
    dia: 0.015,                 // cm
    readout: readout,
};    

local pspec(name, loc, pot, readout=false) = {
    name:name,
    loc:loc,
    pot:pot,
    nwires: 0,
    readout: readout
};


{
    // A drifires component constructs a Garfield ComponentBase.
    // Except for type: and optional name: the rest of the parameters
    // are specific to the type.
    component: {
        type:  "AnalyticWire2D", 
        // 2D cathode + wire anode planes and maybe "grid" and "mesh" planes

        // Medium is intrinsic to the component
        medium : { type: "MediumLar" },

        periodicity: nwires*wire_pitch,
        layers: [
            pspec("c", response_plane, -drift_field*response_plane),
            wspec("g", 2*plane_gap, -665.0*units.volt, false),
            wspec("u", 1*plane_gap, -370.0*units.volt),
            wspec("v", 0*plane_gap,    0.0*units.volt),
            wspec("w",-1*plane_gap,  820.0*units.volt),
            pspec("m",-2*plane_gap,    0.0*units.volt),
        ],

    },
    
    // Specify drift information
    drifter: {
        // type: names the type to use, the rest are type-specfic params
        type: "DriftLineRKF",
        
        accuracy: integration_accuracy, // cm
        maxstep: maxstep,       // cm

    },

    action: self.actions.response,

    actions: {
        plot_drifts : {
            type: "PlotDrifts",
            local xmax = +max_wire*wire_pitch,
            local xmin = -max_wire*wire_pitch,
            local ymax = response_plane,
            local ymin = -2*plane_gap,
            pdf: "pdsp-drifts.pdf",
            areas: [{xmin:-0.6*units.cm, ymin:-0.6*units.cm, xmax:0.6*units.cm, ymax:1.5*units.cm},
                    {xmin:xmin, ymin:ymin, xmax:xmax, ymax:ymax}],
            ystart: response_plane,
            trange: trange,
            drange: { lo:-wire_pitch, hi:+wire_pitch, nbins:42},
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
                    location: (1-id)*plane_gap,
                    pitch: wire_pitch}},

            // skeleton of WCT field response.  
            FieldResponse : {
                planes: [ pln("u",0), pln("v",1), pln("w",2)],
                axis: [1,0,0],
                origin: response_plane,
                tstart: 0,
                period: 0.1*units.us,
                speed: 1.6*units.mm/units.us,
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


