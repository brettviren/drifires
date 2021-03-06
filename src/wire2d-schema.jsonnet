local moo = import "moo.jsonnet";
local oschema = import "object-schema.jsonnet";

function(schema) {
    local os = oschema(schema),

    local layer = schema.record("Layer", fields=[
        schema.field("name", os.letter, doc="Layer name as a letter"),
        schema.field("loc", os.double, doc="Layer location in Y"),
        schema.field("pot", os.double, doc="Layer potential"),
        schema.field("readout", os.bool, "false", doc="Layer is sensitive"),
        schema.field("nwires", os.int, 0, doc="Number of readout wires, 0 is plane"),
        schema.field("nextra", os.int, 0, doc="Number of extra wires"),
        schema.field("pitch", os.double, 0.0, doc="Distance between wires"),
        schema.field("dia", os.double, 0.0, doc="Wire diameter"),
    ], doc="Describe a wire or plane layer"),

    local layers = schema.sequence("Layers", layer),


    local layered = schema.record("Layered", fields=[
        schema.field("periodicity", os.double, -1.0, doc="X-periodic boundary, if positive nonzero"),
        schema.field("layers", layers, doc="The layers"),
        schema.field("medium", os.typename, doc="The medium type/instance identifier"),
    ], bases=[os.typename], doc="Describe a layered geometry"),

    types:[layer, layers, layered],
}
