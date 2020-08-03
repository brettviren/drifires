local det = import "drifires/dets/pdsp.jsonnet";
local w2d = import "drifires/geoms/wire2d.jsonnet";

local r2d = import "drifires/actions/resp2d.jsonnet";
local d2d = import "drifires/actions/paths2d.jsonnet";

local actions = {
    response: r2d(det),
    drifts: d2d(det)
};

function(action="response", filename=null) w2d(det) {
    actions: {
        response: r2d(det),
        drifts: d2d(det, filename)
    },

    action: self.actions[action]
}
