#pragma once

#include "../../compiler/compiler.hpp"
#include "../visitor.hpp"

namespace lorraine::ast::type
{
    class validator : public visitor
    {
       public:
        /// @brief Ensures that all types in the AST have been assigned and used correctly.
        /// @param root The root node of the AST
        /// @param compiler The compiler instance
        /// @return True if the AST is valid, false otherwise.
        static bool validate( module *root, compiler::compiler *compiler );

        bool visit( local_assignment *node ) override;

       private:
       compiler::compiler *compiler;
    };
}  // namespace lorraine::ast::type