#ifndef TCC_CODEGEN_HPP
#define TCC_CODEGEN_HPP

#include "ast.hpp"

namespace tcc {
namespace codegen {
void codegen(std::ostream &out, const ast::Program &program);
}
} // namespace tcc

#endif // !TCC_CODEGEN_HPP
