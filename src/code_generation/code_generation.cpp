#include "code_generation.hpp"

#include "../utils/error.hpp"
#include "llvm_visitor.hpp"

namespace lorraine::code_generation
{
    std::shared_ptr< llvm::Module > code_generation::generate()
    {
        // Collect external functions
        llvm_collector collector;
        ast_module->body->visit( &collector );

        // Compile external function declerations
        for ( const auto func : collector.external_declerations )
            compile_external_decleration( func );

        const auto type = llvm::FunctionType::get( llvm::Type::getVoidTy( context ), false );

        const auto entry_function =
            llvm::Function::Create( type, llvm::GlobalValue::InternalLinkage, "entry", *llvm_module );

        const auto entry_block = llvm::BasicBlock::Create( context, "entry", entry_function );

        llvm::IRBuilder<> builder( entry_block );

        // Visit AST body
        llvm_visitor visitor( builder, *this );
        ast_module->visit( &visitor );

        builder.CreateRetVoid();

        return llvm_module;
    }

    llvm::Function* code_generation::get_or_create_function( std::shared_ptr< ast::variable > variable, bool external )
    {
        if ( const auto function_type =
                 llvm::dyn_cast< llvm::FunctionType >( variable->type->to_llvm_type( context ) ) )
        {
            llvm::Function* func = llvm_module->getFunction( variable->value );

            if ( !func )
            {
                func = llvm::Function::Create(
                    function_type,
                    external ? llvm::GlobalValue::ExternalLinkage : llvm::GlobalValue::InternalLinkage,
                    variable->value,
                    *llvm_module );
            }

            return func;
        }
        else
            throw utils::compiler_error( "Invalid function type" );
    }

    llvm::Function* code_generation::compile_external_decleration( std::shared_ptr< ast::variable > variable )
    {
        return get_or_create_function( variable, true );
    }
}  // namespace lorraine::code_generation