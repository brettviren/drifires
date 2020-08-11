// Base configuration schema used in object.hpp

local moo = import "moo.jsonnet";
local re = moo.schema.re;
function(schema) {
    ident: schema.string("Ident", pattern=re.ident),

    letter: schema.string("Letter", pattern='[a-zA-Z]'),
    tf: schema.boolean("Bool"),

    typename: schema.record("TypeName", fields=[
        schema.field("type", $.ident, doc="A registered type name"),
        schema.field("name", $.ident, "", doc="An instance name"),
    ], doc="Describe an instance of a type"),

    value: schema.number("Value", dtype="f4"),
    count: schema.number("Count", dtype="i4"),

    binning: schema.record("Binning", fields=[
        schema.field("lo", $.value, 0.0, doc="Low edge of a range"),
        schema.field("hi", $.value, 0.0, doc="High edge of a range"),
        schema.field("nbins", $.count, 0, doc="Number of bins covering a range"),
    ], doc="Partition a range into equal size bins"),

    point: schema.record("Point", fields=[
        schema.field("x", $.value, 0.0),
        schema.field("y", $.value, 0.0),
        schema.field("z", $.value, 0.0),
        schema.field("t", $.value, 0.0),
    ], doc="A four-space point"),

    types:[$.ident, $.letter, $.tf, $.typename, $.value, $.count, $.binning, $.point],
}
    
