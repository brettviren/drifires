#ifndef DRIFIRES_DRIFIRES_NLJS_HPP
#define DRIFIRES_DRIFIRES_NLJS_HPP


#include "nlohmann/json.hpp"
#include "drifires_structs.hpp"

namespace drifires {

    using json = nlohmann::json;


    // Converters for TypeName
    // Describe an instance of a type
    inline void to_json(json& j, const TypeName& obj) {
        j["type"] = obj.type;
        j["name"] = obj.name;
    }
    inline void from_json(const json& j, TypeName& obj) {
        j.at("type").get_to(obj.type);    
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
        j.at("lo").get_to(obj.lo);    
        j.at("hi").get_to(obj.hi);    
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
        j.at("x").get_to(obj.x);    
        j.at("y").get_to(obj.y);    
        j.at("z").get_to(obj.z);    
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
        j.at("name").get_to(obj.name);    
        j.at("loc").get_to(obj.loc);    
        j.at("pot").get_to(obj.pot);    
        j.at("readout").get_to(obj.readout);    
        j.at("nwires").get_to(obj.nwires);    
        j.at("nextra").get_to(obj.nextra);    
        j.at("pitch").get_to(obj.pitch);    
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
        j.at("tn").get_to(obj.tn);    
        j.at("periodicity").get_to(obj.periodicity);    
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
        j.at("tn").get_to(obj.tn);    
        j.at("accuracy").get_to(obj.accuracy);    
        j.at("maxstep").get_to(obj.maxstep);    
        j.at("throw_ok").get_to(obj.throw_ok);    
    }


} // namespace drifires

#endif