#ifndef DRIFIERS_COMPONENT_HPP
#define DRIFIERS_COMPONENT_HPP

#include "drifires/object.hpp"

#include "Garfield/ComponentBase.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/ViewCell.hh"

namespace drifires {

    using Range = std::pair<double, double>;
    using BoundingBox = std::vector<Range>;

    // fixme: the name "Component" doesn't quite match the semantics
    // of garfield as it also populates the sensor.
    struct Component {

        // Return reference to built component.
        virtual Garfield::ComponentBase& component() = 0;
        // Return reference to built sensor
        virtual Garfield::Sensor& sensor() = 0;

        // This only works on a subset of concrete components so we
        // have to dance it inside to call ViewCell::SetComponent() on
        // the concrete component....  Return true if handled
        virtual void enable_view(Garfield::ViewCell& vc) {
            throw std::runtime_error("enable_view not implemented");
        }

        //virtual BoundingBox bounds() = 0;

        // Return all the readout labels
        virtual std::vector<std::string> readout_labels() = 0;

    };

    // Get A configured component
    Component& component(object);

}

#endif
