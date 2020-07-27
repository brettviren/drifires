#include "drifires/component.hpp"
#include "drifires/factory.hpp"

drifires::Component& drifires::component(drifires::object cfg)
{
    return drifires::factory_getcfg<drifires::Component>(cfg);
}


