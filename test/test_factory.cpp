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
yourtypename = drifires::factory<YourType>("yourtype").set_maker([] {
    return new YourType;
});

static std::string
mytypename = drifires::factory<MyType>("mytype").set_maker([] {
    return new MyType;
});

TEST_CASE( "Default maker" ) {
    auto ytfac = drifires::factory<YourType>("yourtype");
    SECTION ( "making some instances" ) {
        REQUIRE ( nullptr != ytfac.get() );
        REQUIRE ( nullptr != ytfac.get("your instance") );
    };
}
TEST_CASE( "Custom maker" ) {
    auto mtfac = drifires::factory<MyType>("mytype");
    SECTION ( "making some instances" ) {
        REQUIRE ( nullptr != mtfac.get() );
        REQUIRE ( nullptr != mtfac.get("my instance") );
    };
}

TEST_CASE( "Make MediumLar" ){
    
}
