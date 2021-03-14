#ifndef BCC_CODEGEN_HPP
#define BCC_CODEGEN_HPP

#include "ast.hpp"

namespace bcc {
namespace codegen {
void codegen(std::ostream &out, const ast::Program &program);
}
} // namespace bcc

#endif // !BCC_CODEGEN_HPP
