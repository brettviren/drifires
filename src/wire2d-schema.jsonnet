local moo = import "moo.jsonnet";
local oschema = import "object-schema.jsonnet";

function(schema) {
    local os = oschema(schema),

    local layer = schema.record("Layer", fields=[
        schema.field("name", os.letter, doc="Layer name as a letter"),
        schema.field("loc", os.value, doc="Layer location in Y"),
        schema.field("pot", os.value, doc="Layer potential"),
        schema.field("readout", os.tf, false, doc="Layer is sensitive"),
        schema.field("nwires", os.count, 0, doc="Number of readout wires, 0 is plane"),
        schema.field("nextra", os.count, 0, doc="Number of extra wires"),
        schema.field("pitch", os.value, 0.0, doc="Distance between wires"),
        schema.field("dia", os.value, 0.0, doc="Wire diameter"),
    ], doc="Describe a wire or plane layer"),

    local layers = schema.sequence("Layers", layer),

    local layered = schema.record("Layered", fields=[
        schema.field("tn", os.typename, doc="The type and instance names"),
        schema.field("periodicity", os.value, -1.0, doc="X-periodic boundary, if positive nonzero"),
        schema.field("layers", layers, doc="The layers"),
    ], doc="Describe a layered geometry"),

    types:[layer, layers, layered],
}
