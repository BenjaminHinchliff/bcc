#include <catch2/catch.hpp>

#include <bcc/bcc.hpp>

using namespace bcc::ast;

// TODO: expand this section

TEST_CASE("invalid programs parse", "[parser]") {
  std::vector<std::string> sources = {
      R"(
int main( {
    return 0;
}
)",
      R"(
int main() {
    return;
}
)",
      R"(
int main() {
    return 0;
)",
      R"(
int main() {
    return 0
}
)",
      R"(
int main() {
    return0;
}
)", R"(
int main() {
    RETURN 0;
}
)"};
  for (const std::string &source : sources) {
    REQUIRE_THROWS_AS(bcc::parser::parse(source), bcc::parser::Exception);
  }
}
