#ifndef DRIFIRES_OBJECT_HPP
#define DRIFIRES_OBJECT_HPP

#include "nlohmann/json.hpp"

namespace drifires {
    using object = nlohmann::json;

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
