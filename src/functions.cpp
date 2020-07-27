#include "drifires/functions.hpp"
#include "drifires/component.hpp"
#include "drifires/drifter.hpp"
#include "drifires/action.hpp"

struct Point { double x,y,z,t; };
void from_json(const drifires::object j, Point& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
    j.at("z").get_to(p.z);
    j.at("t").get_to(p.t);
}

drifires::object drifires::process(drifires::object cfg)
{
    auto& cmp = drifires::component(cfg["component"]);
    auto& dft = drifires::drifter(cfg["drifter"]);
    dft.set_sensor(cmp.sensor());

    drifires::object ret;

    auto& action = drifires::action(cfg["action"]);
    return action.act(cmp, dft);
}
