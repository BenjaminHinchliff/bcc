#include <iostream>
#include <fstream>
#include <sstream>

#include <tcc/tcc.hpp>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "no input file specified" << '\n';
    return 1;
  }

  std::string inputPath = argv[1];
  std::string inputBase = inputPath.substr(0, inputPath.rfind("."));
  std::string outputPath = inputBase + ".s";

  std::ifstream inputFile(inputPath);
  std::stringstream inputStrStream;
  inputStrStream << inputFile.rdbuf();
  auto tokens = tcc::lexer::lex(inputStrStream.str());

  tcc::ast::Program program;
  try {
program =  tcc::parser::parse(tokens);
  } catch (const tcc::parser::exceptions::UnexpectedToken &e) {
    std::cerr << "Parsing error:\n";
    std::cerr << '\t' << e.what() << '\n';
    return 1;
  }

  std::ofstream outputFile(outputPath);
  tcc::codegen::codegen(outputFile, program);

  return 0;
}
