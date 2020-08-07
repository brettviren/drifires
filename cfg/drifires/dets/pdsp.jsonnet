local units = import "drifires/units.jsonnet";

{
    temperature : 89*units.K,

    integration_accuracy : 0.000001*units.cm,

    // 1mm is too fast to allow nudges smaller than 1 radius
    //local maxstep = 0.1*units.mm;
    maxstep : 1*units.mm,

    // How to sample response. 100us in 0.1us bins is usual
    // This counts time from the drift paths start.
    trange : { lo:0.0*units.us, hi: self.lo + self.nbins*0.1*units.us, nbins: 1000 },

    // Distance between anode wire planes
    plane_gap:4.71*units.mm,

    // Where in Y the drifts start
    drift_start: 10.0*units.cm + self.plane_gap,

    // Where in Z the mesh plane is
    mesh_plane_y: -4.79*units.mm,

    // A virtual 'cathode' plane where the responses start and which
    // holds the potential setting the drift field.  Adding plane_gap
    // closely corresponds to the time shift Wenqiang added to the
    // dune-garfield-1d565.json.bz2 fields.  This is a time shifted
    // version of garfield-1d-boundary-path-rev-dune.json.bz2.
    // Removing the plane_gap will be w/in ~1us of this unshifted
    // result.  There is still a ~5% time scale difference.
    // response_plane : 10.0*units.cm + self.plane_gap,
    response_plane : 20.0*units.cm + self.plane_gap,

    // Set the nominal drift field.
    drift_field : 500.0*units.volt/units.cm,

    // The "bulk" drift speed is NOT used by drifires and is merely copied
    // to the output WCT .json.bz2 file.  Detailed drift velocity is
    // copied inside the MediumLar drifires/Garfield++ class.  Wenqiang
    // found PDSP is somewhat less than nominal 1.6mm/us.
    bulk_drift_speed : 1.565*units.mm/units.us,

    // The sampling for the produced responses.
    sample_period : 0.1*units.us,

    // Number of wires beyond the central wire-of-interest to calculate the fields.
    max_wire : 10,
    nwires : 2 * self.max_wire + 1,
    nextra_wires : 40,
    max_dead_wire : self.max_wire + self.nextra_wires,
    
    // A REPRESENTATIVE distance between neighboring wires in an "average" plane.
    // larsoft U/V: 4.762mm, W: 4.792mm
    // DUNE TDR: U/V: 4.669mm, W: 4.790mm
    // original GARFIELD U/V: 4.667, W:4.79
    wire_pitch : 4.71*units.mm,

    // Diameter of wires.
    wire_diameter : 0.150*units.mm,

    // Distance to offset the first and last impact position so as to not
    // sample unlikely and anomolous paths which occur exactly along a
    // line of symmetry.  A nudge of 0.01*wire_diameter is needed to
    // reproduce the delayed "prong" of the central response function.
    //local nudge = 0.01*wire_diameter;
    nudge : 0.5 * self.wire_diameter,

    // Make a 2D wire plane specification
    wspec(pname, pit, loc, pot, nextra=self.nextra_wires, readout=true) :: {
        name:pname,
        loc:loc,                // in Y direction
        pot:pot,                // potential
        nwires: $.nwires,       // of sensitive central wires
        pitch: pit,             // wire pitch
        dia: $.wire_diameter,   // wire diameter
        readout: readout,       // if this plane has central wires read out
        nextra:nextra,          // additional wires further beyond central
    },    

    // Make a simple, infinite plane specification
    pspec(name, loc, pot, readout=false) :: {
        name:name,
        loc:loc,
        pot:pot,
        nwires: 0,                  // marks as simple plane, not wire plane
        readout: readout
    },

    plane_locations : {
        c: $.pspec("c", $.response_plane, -$.drift_field*$.response_plane),
        g: $.wspec("g", 4.790*units.mm, 3*$.plane_gap, -665.0*units.volt, readout=false),
        u: $.wspec("u", 4.667*units.mm, 2*$.plane_gap, -370.0*units.volt),
        v: $.wspec("v", 4.667*units.mm, 1*$.plane_gap,    0.0*units.volt),
        w: $.wspec("w", 4.790*units.mm, 0*$.plane_gap,  820.0*units.volt),
        m: $.pspec("m",                $.mesh_plane_y,    0.0*units.volt),
    },
}
