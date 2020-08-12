local moo = import "moo.jsonnet";
local oschema = import "object-schema.jsonnet";
local units = import "../cfg/drifires/units.jsonnet";

function(schema) {
    local os = oschema(schema),

    local lar = schema.record('MediumLarCfg', fields=[
        schema.field("temperature", os.double, 89.0*units.K, doc="Temperature"),
        schema.field("density", os.double, 1.3954*units.gram/units.cm3, doc="Density"),
        // FIXME: need to add a bunch more that are still hard coded!  
    ], doc="Configuration for MediumLar"),

    types: [lar],
}
