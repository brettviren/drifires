#include "drifires/functions.hpp"
#include "drifires/component.hpp"
#include "drifires/drifter.hpp"
#include "drifires/action.hpp"


drifires::object drifires::process(drifires::object cfg)
{
    auto& cmp = drifires::component(cfg["component"]);
    auto& dft = drifires::drifter(cfg["drifter"]);
    dft.set_sensor(cmp.sensor());

    drifires::object ret;

    auto& action = drifires::action(cfg["action"]);
    return action.act(cmp, dft);
}
