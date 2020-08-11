local moo = import "moo.jsonnet";
local oschema = import "object-schema.jsonnet";
local units = import "../cfg/drifires/units.jsonnet";

function(schema) {
    local os = oschema(schema),

    local stepper = schema.record("Stepper", fields=[
        schema.field("tn", os.typename, doc="The type and instance names"),
        schema.field("accuracy", os.value, 1e-9*units.cm, doc="Max (in)accuracy distance"),
        schema.field("maxstep", os.value, 1*units.mm, doc="Max (in)accuracy distance"),
        schema.field("throw_ok", os.tf),
    ], doc = "Stepper configuration"),

    types: [stepper]

}
