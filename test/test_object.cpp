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

TEST_CASE("precision","[object]") {
    SECTION ( "cpp to json" ) {
        drifires::object o;
        const double wire_pitch = 47.1;
        const double imp_pitch = wire_pitch / 10.0;
        for (int ind=0; ind<6; ++ind) {
            const double here = ind*imp_pitch;
            o[ind] = here;
            const double there = o[ind];
            INFO("o: cpp:" << here << " json as cpp:" << there << " json as json:" << o[ind]);
            REQUIRE (std::abs(here - there) < 1e-9);
        }
        
        std::string text = o.dump(4);
        auto o2 = drifires::object::parse(text);
        for (int ind=0; ind<6; ++ind) {
            double d1 = o[ind];
            double d2 = o2[ind];
            INFO("o: d1:" << d1 << " d2:" << d2);
            REQUIRE (std::abs(d1 - d2) < 1e-9);
        }
    }

}
