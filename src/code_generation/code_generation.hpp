#pragma once

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "../ast/statement.hpp"

namespace lorraine::code_generation
{
    class code_generation
    {
       public:
        /// @brief Creates a new instance of the LLVM code generator
        /// @param ast_module
        code_generation( ast::module* ast_module )
            : llvm_module( std::make_unique< llvm::Module >( ast_module->info->name, context ) ),
              ast_module( ast_module )
        {
        }

        /// @brief Generates an LLVM module from the AST module
        /// @return New LLVM module
        std::shared_ptr< llvm::Module > generate();

       private:
        llvm::LLVMContext context;

        std::shared_ptr< llvm::Module > llvm_module;
        ast::module* ast_module;

        llvm::Function* compile_function_prototype(
            ast::function_prototype* function,
            bool is_extern = false );

        llvm::FunctionType* get_llvm_function_type( ast::function_prototype* function );
    };
}  // namespace lorraine::code_generation
