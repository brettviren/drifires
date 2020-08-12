#include "drifires/functions.hpp"
#include "drifires/component.hpp"
#include "drifires/drifter.hpp"
#include "drifires/action.hpp"
#include "drifires/medium.hpp"


drifires::object drifires::process(drifires::object cfg)
{
    for (auto med : cfg["media"]) {
        drifires::medium(med);  // make sure constructed/configured
    }

    auto& cmp = drifires::component(cfg["component"]);
    auto& dft = drifires::drifter(cfg["drifter"]);
    dft.set_sensor(cmp.sensor());

    drifires::object ret;

    auto& action = drifires::action(cfg["action"]);
    return action.act(cmp, dft);
}
