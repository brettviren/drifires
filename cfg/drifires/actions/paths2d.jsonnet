local units = import "drifires/units.jsonnet";
function(det, filename) {

    type: "PlotDrifts",
    local xmax = +det.max_wire*det.wire_pitch,
    local xmin = -det.max_wire*det.wire_pitch,
    local xmax_big = +(det.nextra_wires + det.max_wire)*det.wire_pitch,
    local xmin_big = -(det.nextra_wires + det.max_wire)*det.wire_pitch,
    local ymax = det.response_plane,
    local ymin = -2*det.plane_gap,
    pdf: filename,
    areas: [{xmin:-0.6*units.cm, ymin:-0.1*units.cm, xmax:0.6*units.cm, ymax:2.0*units.cm},
            {xmin:xmin,     ymin:ymin, xmax:xmax,     ymax:ymax},
            {xmin:xmin_big, ymin:ymin, xmax:xmax_big, ymax:ymax},
           ],
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
