#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>
#include <bcc/bcc.hpp>

int main(int argc, char **argv) {
  CLI::App app{"A Small and Simple C Compiler for Windows and Linux"};

  std::string inputPath;
  app.add_option("input-path", inputPath, "the path to the input file")->required();
  bool dump_tokens = false;
  app.add_flag("-t,--tokens", dump_tokens, "dump tokens to stderr");
  bool dump_ast = false;
  app.add_flag("-a,--ast", dump_ast, "dump ast to stderr");

  CLI11_PARSE(app, argc, argv);

  std::string inputBase = inputPath.substr(0, inputPath.rfind("."));
  std::string outputPath = inputBase + ".s";

  std::ifstream inputFile(inputPath);
  std::stringstream inputStrStream;
  inputStrStream << inputFile.rdbuf();
  auto tokens = bcc::lexer::lex(inputStrStream.str());

  if (dump_tokens) {
    std::cerr << "Tokens:\n";
    std::cerr << tokens << '\n';
  }

  std::unique_ptr<bcc::ast::Program> program;
  try {
    program = std::make_unique<bcc::ast::Program>(bcc::parser::parseTokens(tokens));
  } catch (const bcc::parser::Exception &e) {
    std::cerr << "Parsing error:\n";
    std::cerr << '\t' << e.what() << '\n';
    return 1;
  }

  if (dump_ast) {
    std::cerr << "Ast:\n";
    std::cerr << *program << '\n';
  }

  std::ofstream outputFile(outputPath);
  bcc::codegen::codegen(outputFile, *program);
  outputFile.close();

#ifdef _WIN32
  std::string linker = "clang.exe";
  std::string postfix = ".exe";
#else
  std::string linker = "gcc";
  std::string postfix = "";
#endif // _WIN32

  std::string command = linker + " \"" + outputPath + "\"" + " -o \"" +
                        inputBase + postfix + "\"";

  std::system(command.c_str());

  return 0;
}
