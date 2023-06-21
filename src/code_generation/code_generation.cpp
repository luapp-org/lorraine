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
        for ( const auto func : collector.external_functions )
            compile_function_prototype( func );

        const auto type = llvm::FunctionType::get( llvm::Type::getVoidTy( context ), false );

        const auto entry_function = llvm::Function::Create(
            type, llvm::GlobalValue::InternalLinkage, "entry", *llvm_module );

        const auto entry_block = llvm::BasicBlock::Create( context, "entry", entry_function );

        llvm::IRBuilder<> builder( entry_block );

        // Visit AST body
        llvm_visitor visitor( builder, *this );
        ast_module->visit( &visitor );

        builder.CreateRetVoid();

        return llvm_module;
    }

    llvm::Function* code_generation::compile_function_prototype(
        ast::function_prototype* function,
        bool is_extern )
    {
        llvm::Function* func = llvm_module->getFunction( function->name );

        if ( !func )
        {
            llvm::FunctionType* function_type = get_llvm_function_type( function );
            func = llvm::Function::Create(
                function_type, llvm::GlobalValue::ExternalLinkage, function->name, *llvm_module );
        }

        return func;
    }

    llvm::FunctionType* code_generation::get_llvm_function_type( ast::function_prototype* function )
    {
        const auto return_type = function->type->to_llvm_type( context );

        if ( !llvm::FunctionType::isValidReturnType( return_type ) )
            throw utils::compiler_error( "invalid return type" );

        std::vector< llvm::Type* > argument_types;

        for ( const auto arg : function->args )
        {
            const auto arg_type = arg->type->to_llvm_type( context );

            if ( !llvm::FunctionType::isValidArgumentType( arg_type ) )
                throw utils::compiler_error( "invalid argument type" );

            argument_types.push_back( arg_type );
        }

        return llvm::FunctionType::get(
            return_type, llvm::makeArrayRef( argument_types ), function->vararg );
    }
}  // namespace lorraine::code_generation