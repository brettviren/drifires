local moo = import "moo.jsonnet";
local schemas = [
    import "object-schema.jsonnet",
    import "wire2d-schema.jsonnet",
    import "drifter-schema.jsonnet",
];
local cg = moo.schema.avro.codegen;

local multischema(ss) = function(schema) {
    types: std.flattenArrays([s(schema).types for s in ss])
};

//multischema([os])(moo.schema.avro)
cg("drifires", multischema(schemas), "drifires")
