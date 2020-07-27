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
