#include "catch.hpp"
#include "drifires/factory.hpp"

#include <iostream>

struct MyType {
    int a{42};
};
struct YourType {
    float x{6.9};
};

static std::string
mytypename = drifires::factory<MyType>("mytype").set_maker([] {
    std::cerr << "making type 'mytype' which is C++ type MyType" << std::endl;
    return new MyType;
});

TEST_CASE( "Default maker" ) {
    auto ytfac = drifires::factory<YourType>("yourtype");
    SECTION ( "making some instances" ) {
        REQUIRE ( ytfac.get() );
        REQUIRE ( ytfac.get("your instance") );
    };
}
TEST_CASE( "Custom maker" ) {
    auto mtfac = drifires::factory<MyType>("mytype");
    SECTION ( "making some instances" ) {
        REQUIRE ( mtfac.get() );
        REQUIRE ( mtfac.get("my instance") );
    };
}
