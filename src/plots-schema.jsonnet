local moo = import "moo.jsonnet";
local oschema = import "object-schema.jsonnet";
local units = import "../cfg/drifires/units.jsonnet";

function(schema) {
    local os = oschema(schema),

    local drifts = schema.record("PlotDriftsCfg", fields=[
        schema.field("type", os.ident, '"PlotDrifts"', doc="The type name for the plotter"),
        schema.field("name", os.ident, '""', doc="The instance name for the plotter"),
        schema.field("ystart", os.double, 10.0*units.cm, doc="Y-location to start drifts"),
        schema.field("trange", os.binning, doc="Time range binning"),
        schema.field("impacts", os.vector_double, doc="X-locations of drifts"),
        schema.field("areas", os.vector_area2d, doc="2D ranges to plot"),
        schema.field("pdf", os.filepath, '"drifires-drifts.pdf"', doc="PDF file name"),
    ], doc="Configuration for PlotDrifts"),

    local diags = schema.record("PlotDiagCfg", fields=[
        schema.field("type", os.ident, '"PlotDiag"', doc="The type name for the plotter"),
        schema.field("name", os.ident, '""', doc="The instance name for the plotter"),
        schema.field("areas", os.vector_area2d, doc="2D ranges to plot"),
        schema.field("pdf", os.filepath, '"drifires-drifts.pdf"', doc="PDF file name"),
        schema.field("medium", os.typename, doc="The medium type/instance identifier"),
    ], doc="Configuration for PlotDiags"),


    types: [drifts, diags]
}
