#include "catch.hpp"
#include "drifires/object.hpp"

int return_foo(drifires::object o, int ind) {
    return o["foo"][ind].get<int>();
}

TEST_CASE("references","[object]") {
    SECTION ( "making some instances" ) {
        
        drifires::object o;
        o["foo"][0] = 0;
        auto& foo = o["foo"];
        foo.push_back(1);
        REQUIRE( o["foo"].size() == 2);
        REQUIRE( o["foo"][1] == 1);
        REQUIRE( return_foo(o,1) == 1);


    }
}

