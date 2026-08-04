local units = import "drifires/units.jsonnet";
local util2d = import "util2d.jsonnet";

function(det, filename) {

    local u = util2d(det),

    type: "PlotDiags",
    pdf: filename,
    areas: u.areas,
    medium : { type: "MediumLar" },

}
