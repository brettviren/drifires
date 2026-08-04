local units = import "drifires/units.jsonnet";

function(det) {
    local xmax = +det.max_wire*det.wire_pitch,
    local xmin = -det.max_wire*det.wire_pitch,
    local xmax_big = +(det.nextra_wires + det.max_wire)*det.wire_pitch,
    local xmin_big = -(det.nextra_wires + det.max_wire)*det.wire_pitch,
    local ymax = det.response_plane,
    local ymin = -2*det.plane_gap,

    areas: [{xmin:-0.6*units.cm, ymin:-1.0*units.cm, xmax:0.6*units.cm, ymax:2.0*units.cm},
            {xmin:xmin,     ymin:ymin, xmax:xmax,     ymax:ymax},
            //{xmin:xmin_big, ymin:ymin, xmax:xmax_big, ymax:ymax},
           ],
}
