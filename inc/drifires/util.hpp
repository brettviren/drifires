#ifndef DRIFIRES_UTIL_HPP
#define DRIFIRES_UTIL_HPP

#include <string>
namespace drifires {

    // A canonical name from a plane name and a relative number.
    std::string wire_name(std::string name, int nrel);

}

#endif
