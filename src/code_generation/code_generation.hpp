#pragma once

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/TargetSelect.h>

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
            llvm::InitializeAllTargetInfos();
            llvm::InitializeAllTargets();
            llvm::InitializeAllTargetMCs();
            llvm::InitializeAllAsmParsers();
            llvm::InitializeAllAsmPrinters();

            llvm_module->setSourceFileName( ast_module->info->absolute() );
        }

        /// @brief Generates an LLVM module from the AST module
        /// @return New LLVM module
        std::shared_ptr< llvm::Module > generate();

        /// @brief Gets or creates a function from a variable
        /// @param variable Variable
        /// @param external If the function needs to be externally linked
        /// @return New or existing function
        llvm::Function* get_or_create_function( std::shared_ptr< ast::variable > variable, bool external = false );

       private:
        llvm::LLVMContext context;

        std::shared_ptr< llvm::Module > llvm_module;
        ast::module* ast_module;

        llvm::Function* compile_external_decleration( std::shared_ptr< ast::variable > variable );
    };
}  // namespace lorraine::code_generation
