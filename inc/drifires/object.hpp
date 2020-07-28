#ifndef DRIFIRES_OBJECT_HPP
#define DRIFIRES_OBJECT_HPP

#include "nlohmann/json.hpp"

namespace drifires {
    using object = nlohmann::json;

    // like o.at("key").get_to(var) but is no-op if key d.n.e.
    template<typename Type>
    bool maybe_to(const object& o, std::string key, Type& val) {
        if (o.contains(key)) {
            o.at(key).get_to(val);
            return true;
        }
        return false;
    }


    struct TypeName {
        std::string type;
        std::string name{""};
    };
    void to_json(object& j, const TypeName& tn);
    void from_json(const object& j, TypeName& tn);


    struct Binning {
        double lo, hi; int nbins;

        double binsize() { return (hi-lo)/nbins; }

        std::vector<double> edges(bool last=true);
    };
    void from_json(const object j, Binning& b);
}

#endif
