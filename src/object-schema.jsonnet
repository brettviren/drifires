// Base configuration schema used in object.hpp

local moo = import "moo.jsonnet";
local re = moo.schema.re;
function(schema) {
    ident: schema.string("Ident", pattern=re.ident),

    letter: schema.string("Letter", pattern='[a-zA-Z]'),
    filepath: schema.string("FilePath", pattern=re.hierpath),

    bool: schema.boolean("Bool"),

    typename: schema.record("TypeName", fields=[
        schema.field("type", $.ident, doc="A registered type name"),
        schema.field("name", $.ident, "", doc="An instance name"),
    ], doc="Describe an instance of a type"),

    double: schema.number("Double", dtype="f8"),
    float: schema.number("Float", dtype="f4"),
    int: schema.number("Int", dtype="i4"),

    vector_double: schema.sequence("VectorDouble", $.double),
    vector_int: schema.sequence("VectorInt", $.int),

    binning: schema.record("Binning", fields=[
        schema.field("lo", $.double, 0.0, doc="Low edge of a range"),
        schema.field("hi", $.double, 0.0, doc="High edge of a range"),
        schema.field("nbins", $.int, 0, doc="Number of bins covering a range"),
    ], doc="Partition a range into equal size bins"),

    area2d: schema.record("Area2d", fields=[
        schema.field("xmin", $.double, 0.0),
        schema.field("ymin", $.double, 0.0),
        schema.field("xmax", $.double, 0.0),
        schema.field("ymax", $.double, 0.0),
    ], doc="A 2D area bounding box"),
    vector_area2d: schema.sequence("VectorArea2D", $.area2d),

    point: schema.record("Point", fields=[
        schema.field("x", $.double, 0.0),
        schema.field("y", $.double, 0.0),
        schema.field("z", $.double, 0.0),
        schema.field("t", $.double, 0.0),
    ], doc="A four-space point"),

    types:[$.ident, $.filepath, $.letter, $.bool, $.typename,
           $.double, $.float, $.int,
           $.vector_double, $.vector_int,
           $.area2d, $.vector_area2d,
           $.binning, $.point],
}
    
