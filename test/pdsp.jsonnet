local integration_accuracy = 0.000001; // cm
local maxstep = 0.1;       // cm

local response_plane = 10.0;    // cm, where the 'cathode' is
local drift_field = 500.0;      // V/cm, nominal drift field
local plane_gap=0.5;            // between wire planes
local nwires = 21;
local wire_pitch = 0.5;         // between wires
local impact_pitch = wire_pitch * 0.1; // 0.05cm
local impeps = 0.00001;                // nudge away from saddles
local impact_pitches = [impeps, 0.05, 0.10, 0.15, 0.20, 0.25-impeps];
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

// subset of WCT units
local units = {
    mm: 1.0,
    cm : 10*self.mm,
    ns: 1.0,
    us: 1e-3*self.ns,
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
            wspec("g", 2*plane_gap, -665.0, false),
            wspec("u", 1*plane_gap, -370.0),
            wspec("v", 0*plane_gap,    0.0),
            wspec("w",-1*plane_gap,  820.0),
            pspec("m",-2*plane_gap,    0.0),
        ],

    },
    
    // Specify drift information
    drifter: {
        // type: names the type to use, the rest are type-specfic params
        type: "DriftLineRKF",
        
        accuracy: integration_accuracy, // cm
        maxstep: maxstep,       // cm

    },

    actions: [
        // {
        //     type: "PlotDrifts",
        //     local xmax = +10*wire_pitch,
        //     local xmin = -10*wire_pitch,
        //     local ymax = response_plane,
        //     local ymin = -2*plane_gap,
        //     pdf: "pdsp-drifts.pdf",
        //     areas: [{xmin:-0.6, ymin:-0.6, xmax:0.6, ymax:1.5},
        //             {xmin:xmin, ymin:ymin, xmax:xmax, ymax:ymax}],
        //     ystart: response_plane,
        //     trange: { lo:0, hi: self.nbins*1000, nbins: 100 },
        //     drange: { lo:-wire_pitch, hi:+wire_pitch, nbins:42},
        // } ,
        {
            type: "HalfRegionResponse",

            filename: "pdsp-response.json",

            // number of wires beyond the central wire.  Use no more
            // than were used to make the component.
            neighbors: 1,
            
            // where to start the paths, in cm
            ystart: response_plane,

            local pln(name,id) = {
                name:name, planeid:id,
                location: (1-id)*plane_gap*units.cm,
                pitch: wire_pitch*units.cm},

            // skeleton of WCT field response.  All quanties MUST be
            // in WCT system of units.
            FieldResponse : {
                planes: [ pln("u",0), pln("v",1), pln("w",2)],
                axis: [1,0,0],
                origin: response_plane*units.cm,
                tstart: 0,
                period: 0.1*units.us,
                speed: 1.6*units.mm/units.us,
            },
                    

            // The time range of the responses
            trange: { lo:0, hi: self.nbins*1000, nbins: 100 },
            // the impacts to drift
            irange: { lo:0, hi:0.5*wire_pitch, nbins:6-1},
            // how much to nudge first and last
            nudge: 0.00001*wire_pitch, 
            
        }
    ],
            
        

    //     {
    //     type: "HalfRegionResponse",
    //     // sensor stepings in time
    //     tbeg: 0,                // ns
    //     tstep: 100,             // ns
    //     nsteps: 1000,
    // }
}


