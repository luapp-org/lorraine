#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>

#include "../ast/visitor.hpp"
#include "code_generation.hpp"

namespace lorraine::code_generation
{
    class llvm_visitor : public ast::visitor
    {
       public:
        /// @brief Creates a new instance of the llvm visitor
        explicit llvm_visitor( llvm::IRBuilder<>& builder, code_generation& gen )
            : builder( builder ),
              gen( gen )
        {
        }

        bool visit( ast::number_literal* node ) override;
        bool visit( ast::string_literal* node ) override;
        bool visit( ast::boolean_literal* node ) override;

       private:
        llvm::IRBuilder<>& builder;
        llvm::Value *value;
        code_generation& gen;
    };

    class llvm_collector : public ast::visitor
    {
       public:
        std::vector< ast::function_prototype* > external_functions;

        bool visit( ast::extern_item* item ) override;
    };
}  // namespace lorraine::code_generation