#ifndef DRIFIRES_UTIL_HPP
#define DRIFIRES_UTIL_HPP

#include <string>
namespace drifires {

    // A canonical name from a plane name and a relative number.
    std::string wire_name(std::string name, int nrel);

}

// A minimal copy of WCT system of units
namespace units {
    static const double mm = 1.0;
    static const double cm = 10*mm;
    static const double ns = 1.0;
    static const double us = 1e3*ns;

    static const double eplus = 1.0;
    static const double MeV = 1.0;
    static const double megavolt = MeV/eplus;
    static const double volt = 1e-6*megavolt;
    static const double e_SI = 1.602176487e-19;  // positron charge in coulomb
    static const double coulomb = eplus / e_SI;  // coulomb = 6.24150 e+18 * eplus
    static const double fC = 1e-15 * coulomb;
}

namespace gfunits {
    static const double length = units::cm;
    static const double time = units::ns;
    static const double pot = units::volt;
    static const double current = units::fC/units::ns;
}
#endif
