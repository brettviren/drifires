#ifndef DRIFIRES_FUNCTIONS_HPP
#define DRIFIRES_FUNCTIONS_HPP

#include "nlohmann/json.hpp"

namespace drifires {
    using json = nlohmann::json;

    // Process directives and return results
    json process(json cfg);
}

#endif
