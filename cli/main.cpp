#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include <bcc/bcc.hpp>

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
  auto tokens = bcc::lexer::lex(inputStrStream.str());

  bcc::ast::Program program;
  try {
    program = bcc::parser::parseTokens(tokens);
  } catch (const bcc::parser::Exception &e) {
    std::cerr << "Parsing error:\n";
    std::cerr << '\t' << e.what() << '\n';
    return 1;
  }

  std::ofstream outputFile(outputPath);
  bcc::codegen::codegen(outputFile, program);
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
