#ifndef DRIFIRES_CONFIGURABLE_HPP
#define DRIFIRES_CONFIGURABLE_HPP

#include "drifires/object.hpp"

namespace drifires {

    struct ConfigurableBase {
        virtual void configure(object obj) = 0;
    };

    template<class Cfg>
    struct Configurable : public ConfigurableBase {
        Cfg cfg;
        virtual void configure(object obj) {
            cfg = obj;
            initialize();
        }
        virtual void initialize() {}
        
    };

}

#endif
