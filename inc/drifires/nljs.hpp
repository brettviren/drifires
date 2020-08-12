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
        if (j.contains("type"))
            j.at("type").get_to(obj.type);    
        if (j.contains("name"))
            j.at("name").get_to(obj.name);    
    }
    // Converters for Binning
    // Partition a range into equal size bins
    inline void to_json(json& j, const Binning& obj) {
        j["lo"] = obj.lo;
        j["hi"] = obj.hi;
        j["nbins"] = obj.nbins;
    }
    inline void from_json(const json& j, Binning& obj) {
        if (j.contains("lo"))
            j.at("lo").get_to(obj.lo);    
        if (j.contains("hi"))
            j.at("hi").get_to(obj.hi);    
        if (j.contains("nbins"))
            j.at("nbins").get_to(obj.nbins);    
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
        if (j.contains("x"))
            j.at("x").get_to(obj.x);    
        if (j.contains("y"))
            j.at("y").get_to(obj.y);    
        if (j.contains("z"))
            j.at("z").get_to(obj.z);    
        if (j.contains("t"))
            j.at("t").get_to(obj.t);    
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
        if (j.contains("name"))
            j.at("name").get_to(obj.name);    
        if (j.contains("loc"))
            j.at("loc").get_to(obj.loc);    
        if (j.contains("pot"))
            j.at("pot").get_to(obj.pot);    
        if (j.contains("readout"))
            j.at("readout").get_to(obj.readout);    
        if (j.contains("nwires"))
            j.at("nwires").get_to(obj.nwires);    
        if (j.contains("nextra"))
            j.at("nextra").get_to(obj.nextra);    
        if (j.contains("pitch"))
            j.at("pitch").get_to(obj.pitch);    
        if (j.contains("dia"))
            j.at("dia").get_to(obj.dia);    
    }
    // Converters for Layered
    // Describe a layered geometry
    inline void to_json(json& j, const Layered& obj) {
        j["tn"] = obj.tn;
        j["periodicity"] = obj.periodicity;
        j["layers"] = obj.layers;
    }
    inline void from_json(const json& j, Layered& obj) {
        if (j.contains("tn"))
            j.at("tn").get_to(obj.tn);    
        if (j.contains("periodicity"))
            j.at("periodicity").get_to(obj.periodicity);    
        if (j.contains("layers"))
            j.at("layers").get_to(obj.layers);    
    }
    // Converters for Stepper
    // Stepper configuration
    inline void to_json(json& j, const Stepper& obj) {
        j["tn"] = obj.tn;
        j["accuracy"] = obj.accuracy;
        j["maxstep"] = obj.maxstep;
        j["throw_ok"] = obj.throw_ok;
    }
    inline void from_json(const json& j, Stepper& obj) {
        if (j.contains("tn"))
            j.at("tn").get_to(obj.tn);    
        if (j.contains("accuracy"))
            j.at("accuracy").get_to(obj.accuracy);    
        if (j.contains("maxstep"))
            j.at("maxstep").get_to(obj.maxstep);    
        if (j.contains("throw_ok"))
            j.at("throw_ok").get_to(obj.throw_ok);    
    }
    // Converters for MediumLarCfg
    // Configuration for MediumLar
    inline void to_json(json& j, const MediumLarCfg& obj) {
        j["temperature"] = obj.temperature;
        j["density"] = obj.density;
    }
    inline void from_json(const json& j, MediumLarCfg& obj) {
        if (j.contains("temperature"))
            j.at("temperature").get_to(obj.temperature);    
        if (j.contains("density"))
            j.at("density").get_to(obj.density);    
    }


} // namespace drifires

#endif