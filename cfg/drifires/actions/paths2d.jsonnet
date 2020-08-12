local units = import "drifires/units.jsonnet";
local util2d = import "util2d.jsonnet";
function(det, filename) {

    local u = util2d(det),

    type: "PlotDrifts",
    pdf: filename,
    areas: u.areas,

    ystart: det.drift_start,
    trange: det.trange,

    // Hit nominal and "nudged" impacts across two wire regions.
    local impact_pitch = det.wire_pitch/10.0,
    impacts: std.sort( [ 0.0, -det.nudge, +det.nudge,
                         -det.wire_pitch+det.nudge, det.wire_pitch-det.nudge,
                         -0.5*det.wire_pitch-det.nudge, -0.5*det.wire_pitch+det.nudge, 
                         +0.5*det.wire_pitch+det.nudge, +0.5*det.wire_pitch-det.nudge ]
                       + [ -impact_pitch*n for n in std.range(1,10) ]
                       + [ impact_pitch*n for n in std.range(1,10)])

}
