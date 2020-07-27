#include "drifires/action.hpp"
#include "drifires/factory.hpp"

drifires::Action& drifires::action(drifires::object cfg)
{
    return drifires::factory_getcfg<drifires::Action>(cfg);
}


