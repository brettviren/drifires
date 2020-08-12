#ifndef DRIFIRES_DRIFIRES_NLJS_HPP
#define DRIFIRES_DRIFIRES_NLJS_HPP

#include "nlohmann/json.hpp"
#include "drifires/structs.hpp"

namespace drifires {

    using json = nlohmann::json;


    // Converters for TypeName
    // Describe an instance of a type
    inline void to_json(json& j, const TypeName& obj) {
        j["type"] = obj.type;
        j["name"] = obj.name;
    }
    inline void from_json(const json& j, TypeName& obj) {
        if (j.contains("type")) {
            j.at("type").get_to(obj.type);
        }
        else {
            throw std::runtime_error("required field not provided: \"TypeName::type\"");
        }
        if (j.contains("name")) {
            j.at("name").get_to(obj.name);
        }
        else {
            obj.name = "";
        }
    }
    // Converters for Area2d
    // A 2D area bounding box
    inline void to_json(json& j, const Area2d& obj) {
        j["xmin"] = obj.xmin;
        j["ymin"] = obj.ymin;
        j["xmax"] = obj.xmax;
        j["ymax"] = obj.ymax;
    }
    inline void from_json(const json& j, Area2d& obj) {
        if (j.contains("xmin")) {
            j.at("xmin").get_to(obj.xmin);
        }
        else {
            obj.xmin = 0;
        }
        if (j.contains("ymin")) {
            j.at("ymin").get_to(obj.ymin);
        }
        else {
            obj.ymin = 0;
        }
        if (j.contains("xmax")) {
            j.at("xmax").get_to(obj.xmax);
        }
        else {
            obj.xmax = 0;
        }
        if (j.contains("ymax")) {
            j.at("ymax").get_to(obj.ymax);
        }
        else {
            obj.ymax = 0;
        }
    }
    // Converters for Binning
    // Partition a range into equal size bins
    inline void to_json(json& j, const Binning& obj) {
        j["lo"] = obj.lo;
        j["hi"] = obj.hi;
        j["nbins"] = obj.nbins;
    }
    inline void from_json(const json& j, Binning& obj) {
        if (j.contains("lo")) {
            j.at("lo").get_to(obj.lo);
        }
        else {
            obj.lo = 0;
        }
        if (j.contains("hi")) {
            j.at("hi").get_to(obj.hi);
        }
        else {
            obj.hi = 0;
        }
        if (j.contains("nbins")) {
            j.at("nbins").get_to(obj.nbins);
        }
        else {
            obj.nbins = 0;
        }
    }
    // Converters for Point
    // A four-space point
    inline void to_json(json& j, const Point& obj) {
        j["x"] = obj.x;
        j["y"] = obj.y;
        j["z"] = obj.z;
        j["t"] = obj.t;
    }
    inline void from_json(const json& j, Point& obj) {
        if (j.contains("x")) {
            j.at("x").get_to(obj.x);
        }
        else {
            obj.x = 0;
        }
        if (j.contains("y")) {
            j.at("y").get_to(obj.y);
        }
        else {
            obj.y = 0;
        }
        if (j.contains("z")) {
            j.at("z").get_to(obj.z);
        }
        else {
            obj.z = 0;
        }
        if (j.contains("t")) {
            j.at("t").get_to(obj.t);
        }
        else {
            obj.t = 0;
        }
    }
    // Converters for Layer
    // Describe a wire or plane layer
    inline void to_json(json& j, const Layer& obj) {
        j["name"] = obj.name;
        j["loc"] = obj.loc;
        j["pot"] = obj.pot;
        j["readout"] = obj.readout;
        j["nwires"] = obj.nwires;
        j["nextra"] = obj.nextra;
        j["pitch"] = obj.pitch;
        j["dia"] = obj.dia;
    }
    inline void from_json(const json& j, Layer& obj) {
        if (j.contains("name")) {
            j.at("name").get_to(obj.name);
        }
        else {
            throw std::runtime_error("required field not provided: \"Layer::name\"");
        }
        if (j.contains("loc")) {
            j.at("loc").get_to(obj.loc);
        }
        else {
            throw std::runtime_error("required field not provided: \"Layer::loc\"");
        }
        if (j.contains("pot")) {
            j.at("pot").get_to(obj.pot);
        }
        else {
            throw std::runtime_error("required field not provided: \"Layer::pot\"");
        }
        if (j.contains("readout")) {
            j.at("readout").get_to(obj.readout);
        }
        else {
            obj.readout = false;
        }
        if (j.contains("nwires")) {
            j.at("nwires").get_to(obj.nwires);
        }
        else {
            obj.nwires = 0;
        }
        if (j.contains("nextra")) {
            j.at("nextra").get_to(obj.nextra);
        }
        else {
            obj.nextra = 0;
        }
        if (j.contains("pitch")) {
            j.at("pitch").get_to(obj.pitch);
        }
        else {
            obj.pitch = 0;
        }
        if (j.contains("dia")) {
            j.at("dia").get_to(obj.dia);
        }
        else {
            obj.dia = 0;
        }
    }
    // Converters for Layered
    // Describe a layered geometry
    inline void to_json(json& j, const Layered& obj) {
        to_json(j, (const TypeName&)obj);
        j["periodicity"] = obj.periodicity;
        j["layers"] = obj.layers;
        j["medium"] = obj.medium;
    }
    inline void from_json(const json& j, Layered& obj) {
        from_json(j, (TypeName&)obj);
        if (j.contains("periodicity")) {
            j.at("periodicity").get_to(obj.periodicity);
        }
        else {
            obj.periodicity = -1;
        }
        if (j.contains("layers")) {
            j.at("layers").get_to(obj.layers);
        }
        else {
            throw std::runtime_error("required field not provided: \"Layered::layers\"");
        }
        if (j.contains("medium")) {
            j.at("medium").get_to(obj.medium);
        }
        else {
            throw std::runtime_error("required field not provided: \"Layered::medium\"");
        }
    }
    // Converters for Stepper
    // Stepper configuration
    inline void to_json(json& j, const Stepper& obj) {
        to_json(j, (const TypeName&)obj);
        j["accuracy"] = obj.accuracy;
        j["maxstep"] = obj.maxstep;
        j["throw_ok"] = obj.throw_ok;
    }
    inline void from_json(const json& j, Stepper& obj) {
        from_json(j, (TypeName&)obj);
        if (j.contains("accuracy")) {
            j.at("accuracy").get_to(obj.accuracy);
        }
        else {
            obj.accuracy = 1e-08;
        }
        if (j.contains("maxstep")) {
            j.at("maxstep").get_to(obj.maxstep);
        }
        else {
            obj.maxstep = 1;
        }
        if (j.contains("throw_ok")) {
            j.at("throw_ok").get_to(obj.throw_ok);
        }
        else {
            throw std::runtime_error("required field not provided: \"Stepper::throw_ok\"");
        }
    }
    // Converters for MediumLarCfg
    // Configuration for MediumLar
    inline void to_json(json& j, const MediumLarCfg& obj) {
        to_json(j, (const TypeName&)obj);
        j["temperature"] = obj.temperature;
        j["density"] = obj.density;
    }
    inline void from_json(const json& j, MediumLarCfg& obj) {
        from_json(j, (TypeName&)obj);
        if (j.contains("temperature")) {
            j.at("temperature").get_to(obj.temperature);
        }
        else {
            obj.temperature = 89;
        }
        if (j.contains("density")) {
            j.at("density").get_to(obj.density);
        }
        else {
            obj.density = 8709402561591828480;
        }
    }
    // Converters for PlotDriftsCfg
    // Configuration for PlotDrifts
    inline void to_json(json& j, const PlotDriftsCfg& obj) {
        j["type"] = obj.type;
        j["name"] = obj.name;
        j["ystart"] = obj.ystart;
        j["trange"] = obj.trange;
        j["impacts"] = obj.impacts;
        j["areas"] = obj.areas;
        j["pdf"] = obj.pdf;
    }
    inline void from_json(const json& j, PlotDriftsCfg& obj) {
        if (j.contains("type")) {
            j.at("type").get_to(obj.type);
        }
        else {
            obj.type = "PlotDrifts";
        }
        if (j.contains("name")) {
            j.at("name").get_to(obj.name);
        }
        else {
            obj.name = "";
        }
        if (j.contains("ystart")) {
            j.at("ystart").get_to(obj.ystart);
        }
        else {
            obj.ystart = 100;
        }
        if (j.contains("trange")) {
            j.at("trange").get_to(obj.trange);
        }
        else {
            throw std::runtime_error("required field not provided: \"PlotDriftsCfg::trange\"");
        }
        if (j.contains("impacts")) {
            j.at("impacts").get_to(obj.impacts);
        }
        else {
            throw std::runtime_error("required field not provided: \"PlotDriftsCfg::impacts\"");
        }
        if (j.contains("areas")) {
            j.at("areas").get_to(obj.areas);
        }
        else {
            throw std::runtime_error("required field not provided: \"PlotDriftsCfg::areas\"");
        }
        if (j.contains("pdf")) {
            j.at("pdf").get_to(obj.pdf);
        }
        else {
            obj.pdf = "drifires-drifts.pdf";
        }
    }
    // Converters for PlotDiagCfg
    // Configuration for PlotDiags
    inline void to_json(json& j, const PlotDiagCfg& obj) {
        j["type"] = obj.type;
        j["name"] = obj.name;
        j["areas"] = obj.areas;
        j["pdf"] = obj.pdf;
        j["medium"] = obj.medium;
    }
    inline void from_json(const json& j, PlotDiagCfg& obj) {
        if (j.contains("type")) {
            j.at("type").get_to(obj.type);
        }
        else {
            obj.type = "PlotDiag";
        }
        if (j.contains("name")) {
            j.at("name").get_to(obj.name);
        }
        else {
            obj.name = "";
        }
        if (j.contains("areas")) {
            j.at("areas").get_to(obj.areas);
        }
        else {
            throw std::runtime_error("required field not provided: \"PlotDiagCfg::areas\"");
        }
        if (j.contains("pdf")) {
            j.at("pdf").get_to(obj.pdf);
        }
        else {
            obj.pdf = "drifires-drifts.pdf";
        }
        if (j.contains("medium")) {
            j.at("medium").get_to(obj.medium);
        }
        else {
            throw std::runtime_error("required field not provided: \"PlotDiagCfg::medium\"");
        }
    }
    // Converters for PlaneResponseStub
    // A stub of the PlaneResponse object
    inline void to_json(json& j, const PlaneResponseStub& obj) {
        j["planeid"] = obj.planeid;
        j["location"] = obj.location;
        j["pitch"] = obj.pitch;
    }
    inline void from_json(const json& j, PlaneResponseStub& obj) {
        if (j.contains("planeid")) {
            j.at("planeid").get_to(obj.planeid);
        }
        else {
            throw std::runtime_error("required field not provided: \"PlaneResponseStub::planeid\"");
        }
        if (j.contains("location")) {
            j.at("location").get_to(obj.location);
        }
        else {
            throw std::runtime_error("required field not provided: \"PlaneResponseStub::location\"");
        }
        if (j.contains("pitch")) {
            j.at("pitch").get_to(obj.pitch);
        }
        else {
            throw std::runtime_error("required field not provided: \"PlaneResponseStub::pitch\"");
        }
    }
    // Converters for PlaneResponseWrap
    // 
    inline void to_json(json& j, const PlaneResponseWrap& obj) {
        j["name"] = obj.name;
        j["PlaneResponse"] = obj.PlaneResponse;
    }
    inline void from_json(const json& j, PlaneResponseWrap& obj) {
        if (j.contains("name")) {
            j.at("name").get_to(obj.name);
        }
        else {
            throw std::runtime_error("required field not provided: \"PlaneResponseWrap::name\"");
        }
        if (j.contains("PlaneResponse")) {
            j.at("PlaneResponse").get_to(obj.PlaneResponse);
        }
        else {
            throw std::runtime_error("required field not provided: \"PlaneResponseWrap::PlaneResponse\"");
        }
    }
    // Converters for FieldResponseStub
    // A stub for FieldResponse object
    inline void to_json(json& j, const FieldResponseStub& obj) {
        j["planes"] = obj.planes;
        j["axis"] = obj.axis;
        j["tstart"] = obj.tstart;
        j["origin"] = obj.origin;
        j["period"] = obj.period;
        j["speed"] = obj.speed;
    }
    inline void from_json(const json& j, FieldResponseStub& obj) {
        if (j.contains("planes")) {
            j.at("planes").get_to(obj.planes);
        }
        else {
            throw std::runtime_error("required field not provided: \"FieldResponseStub::planes\"");
        }
        if (j.contains("axis")) {
            j.at("axis").get_to(obj.axis);
        }
        else {
            throw std::runtime_error("required field not provided: \"FieldResponseStub::axis\"");
        }
        if (j.contains("tstart")) {
            j.at("tstart").get_to(obj.tstart);
        }
        else {
            obj.tstart = 0;
        }
        if (j.contains("origin")) {
            j.at("origin").get_to(obj.origin);
        }
        else {
            throw std::runtime_error("required field not provided: \"FieldResponseStub::origin\"");
        }
        if (j.contains("period")) {
            j.at("period").get_to(obj.period);
        }
        else {
            throw std::runtime_error("required field not provided: \"FieldResponseStub::period\"");
        }
        if (j.contains("speed")) {
            j.at("speed").get_to(obj.speed);
        }
        else {
            throw std::runtime_error("required field not provided: \"FieldResponseStub::speed\"");
        }
    }
    // Converters for HalfRegionResponseCfg
    // Configuration for HalfRegionResponse
    inline void to_json(json& j, const HalfRegionResponseCfg& obj) {
        j["trange"] = obj.trange;
        j["irange"] = obj.irange;
        j["ystart"] = obj.ystart;
        j["neighbors"] = obj.neighbors;
        j["nudge"] = obj.nudge;
        j["FieldResponse"] = obj.FieldResponse;
    }
    inline void from_json(const json& j, HalfRegionResponseCfg& obj) {
        if (j.contains("trange")) {
            j.at("trange").get_to(obj.trange);
        }
        else {
            throw std::runtime_error("required field not provided: \"HalfRegionResponseCfg::trange\"");
        }
        if (j.contains("irange")) {
            j.at("irange").get_to(obj.irange);
        }
        else {
            throw std::runtime_error("required field not provided: \"HalfRegionResponseCfg::irange\"");
        }
        if (j.contains("ystart")) {
            j.at("ystart").get_to(obj.ystart);
        }
        else {
            obj.ystart = 100;
        }
        if (j.contains("neighbors")) {
            j.at("neighbors").get_to(obj.neighbors);
        }
        else {
            obj.neighbors = 10;
        }
        if (j.contains("nudge")) {
            j.at("nudge").get_to(obj.nudge);
        }
        else {
            obj.nudge = 0;
        }
        if (j.contains("FieldResponse")) {
            j.at("FieldResponse").get_to(obj.FieldResponse);
        }
        else {
            throw std::runtime_error("required field not provided: \"HalfRegionResponseCfg::FieldResponse\"");
        }
    }


} // namespace drifires

#endif