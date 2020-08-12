#ifndef DRIFIRES_ACTION_HPP
#define DRIFIRES_ACTION_HPP

#include "drifires/component.hpp"
#include "drifires/drifter.hpp"

namespace drifires {

    struct Action {
        virtual object act(Component& cmp, Drifter& dft) = 0;
    };

    // Get a configured action
    Action& action(object);

}



#endif
