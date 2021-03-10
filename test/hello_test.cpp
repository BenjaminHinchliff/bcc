#include <catch2/catch.hpp>
#include <tcc/tcc.hpp>

TEST_CASE("hello test", "[hello]") {
  REQUIRE(tcc::hello("world") == "hello, world");
}
