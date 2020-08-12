#ifndef DRIFIRES_MEDIUM_HPP
#define DRIFIRES_MEDIUM_HPP


#include "drifires/object.hpp"

#include "Garfield/Medium.hh"

namespace drifires {
    struct Medium {
        virtual Garfield::Medium& medium() = 0;
    };

    // Get a configured medium.
    Medium& medium(object);

}


#endif
