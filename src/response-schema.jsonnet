local moo = import "moo.jsonnet";
local oschema = import "object-schema.jsonnet";
local units = import "../cfg/drifires/units.jsonnet";

function(schema) {
    local os = oschema(schema),

    local prstub = schema.record("PlaneResponseStub", fields=[
        schema.field("planeid", os.int, doc="wire plane name as letter"),
        schema.field("location", os.double, doc="X-location of wire plane"),
        schema.field("pitch", os.double, doc="Distance between neighboring wires"),
    ], doc="A stub of the PlaneResponse object"),

    local prwrap = schema.record("PlaneResponseWrap", fields=[
        schema.field("name", os.letter),
        schema.field("PlaneResponse", prstub),
    ]),
    local vector_prwrap = schema.sequence("VectorPlaneResponseWrap", prwrap),

    local frstub = schema.record("FieldResponseStub", fields=[
        schema.field("planes", vector_prwrap, doc="Vector of plane response stubs"),
        schema.field("axis", os.vector_double, doc="The anti-drift axis"),
        schema.field("tstart", os.double, 0.0, doc="When the response starts"),        
        schema.field("origin", os.double, doc="Y-location of start of the field response"),
        schema.field("period", os.double, doc="Field response sampling period"),
        schema.field("speed", os.double, doc="Bulk drift speed outside of response region"),
    ], doc="A stub for FieldResponse object"),

    local hrr = schema.record("HalfRegionResponseCfg", fields=[
        schema.field("trange", os.binning, doc="Bins spanning time period"),
        schema.field("irange", os.binning, doc="Bins spanning transverse impacts"),
        schema.field("ystart", os.double, 10*units.cm, doc="Y-location to start drifts"),
        schema.field("neighbors", os.int, 10, doc="Number of wires on either side of central wire"),
        schema.field("nudge", os.double, 0.0, doc="Distance from nominal path to avoid saddle points"),
        schema.field("FieldResponse", frstub, doc="Field response stub object"),
    ], doc="Configuration for HalfRegionResponse"),

    types: [prstub, prwrap, vector_prwrap, frstub, hrr],
}
