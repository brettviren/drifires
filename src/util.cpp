#include "drifires/util.hpp"

#include <sstream>

using namespace drifires;

std::string drifires::wire_name(std::string name, int nrel)
{
    std::string sign = "0";
    if (nrel < 0) sign = "-";
    if (nrel > 0) sign = "+";
    
    std::stringstream ss;
    ss << name << sign << std::abs(nrel);
    return ss.str();
}

double drifires::binsize(const Binning& binning)
{
    return (binning.hi-binning.lo)/binning.nbins;
}
std::vector<double> drifires::edges(const Binning& binning, bool last)
{
    std::vector<double> ret;
    if (last) ret.resize(binning.nbins+1);
    else ret.resize(binning.nbins);
    const double delta = binsize(binning);
    for (int ibin = 0; ibin<binning.nbins; ++ibin) {
        ret[ibin] = binning.lo + delta*ibin;
    }
    if (last) {
        ret[binning.nbins] = binning.hi;
    }
    return ret;
}
