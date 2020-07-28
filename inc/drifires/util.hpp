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
    static const double m = 1e3*mm;
    static const double cm2 = cm*cm;
    static const double cm3 = cm*cm*cm;
    static const double ns = 1.0;
    static const double us = 1e3*ns;
    static const double s = 1.e+9 * ns;
    
    static const double eplus = 1.0;
    static const double MeV = 1.0;
    static const double eV = 1.e-6 * MeV;
    static const double megavolt = MeV/eplus;
    static const double volt = 1e-6*megavolt;
    static const double e_SI = 1.602176487e-19;  // positron charge in coulomb
    static const double coulomb = eplus / e_SI;  // coulomb = 6.24150 e+18 * eplus
    static const double fC = 1e-15 * coulomb;

    static const double K = 1.0;

    static const double joule = eV / e_SI;  // joule = 6.24150 e+12 * MeV
    static const double kilogram = joule * s * s / (m * m);
    static const double gram = 1.e-3 * kilogram;    
}

namespace gfunits {
    static const double length = units::cm;
    static const double time = units::ns;
    static const double pot = units::volt;
    static const double current = units::fC/units::ns;
    static const double temperature = units::K;
    static const double density = units::gram/units::cm3;
}
#endif
