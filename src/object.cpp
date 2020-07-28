#include "drifires/object.hpp"

using drifires::maybe_to;

void drifires::to_json(object& j, const drifires::TypeName& tn)
{
    j = object({{"type",tn.type}});
    if (tn.name.empty()) { return; }
    j["name"] = tn.name;
}

void drifires::from_json(const object& j, drifires::TypeName& tn)
{ 
    j.at("type").get_to(tn.type);
    maybe_to(j, "name", tn.name);
}

std::vector<double> drifires::Binning::edges(bool last)
{
    std::vector<double> ret;
    if (last) ret.resize(nbins+1);
    else ret.resize(nbins);
    const double delta = binsize();
    for (int ibin = 0; ibin<nbins; ++ibin) {
        ret[ibin] = lo + delta*ibin;
    }
    if (last) {
        ret[nbins] = hi;
    }
    return ret;
}

void drifires::from_json(const drifires::object j, drifires::Binning& b)
{
    j.at("lo").get_to(b.lo);
    j.at("hi").get_to(b.hi);
    j.at("nbins").get_to(b.nbins);
}


