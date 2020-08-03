function(det) {
    type: "HalfRegionResponse",

    // number of wires beyond the central wire.  Use no more
    // than were used to make the component.
    neighbors: det.max_wire,
    
    // where to start the paths, in cm
    ystart: det.response_plane,

    local pln(name,id) = {
        name:name,
        PlaneResponse: {
            planeid:id,
            location: det.plane_locations[name].loc,
            pitch: det.wire_pitch}},

    // skeleton of WCT field response.  
    FieldResponse : {
        planes: [ pln("u",0), pln("v",1), pln("w",2)],
        axis: [1,0,0],
        origin: det.response_plane,
        tstart: 0,
        period: det.sample_period,
        speed: det.bulk_drift_speed,
    },
    

    // The time range of the responses
    trange: det.trange,
    // the impacts to drift, they are always below the wire....
    // irange: { lo:0, hi:0.5*wire_pitch, nbins:6-1},
    irange: { lo:-0.5*det.wire_pitch, hi:0.0, nbins:6-1},
    // how much to nudge first and last
    nudge: det.nudge,

}
