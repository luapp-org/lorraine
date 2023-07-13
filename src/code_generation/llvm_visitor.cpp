#include "llvm_visitor.hpp"

#include "../utils/error.hpp"

namespace lorraine::code_generation
{
    bool llvm_collector::visit( ast::external_decleration* item )
    {
        external_declerations.push_back( item->var );
        return false;
    }

    bool llvm_visitor::visit( ast::number_literal* node )
    {
        value = llvm::ConstantFP::get( builder.getContext(), llvm::APFloat( node->value ) );
        return false;
    }

    bool llvm_visitor::visit( ast::string_literal* node )
    {
        value = builder.CreateGlobalStringPtr( llvm::StringRef( node->value.c_str() ) );
        return false;
    }

    bool llvm_visitor::visit( ast::boolean_literal* node )
    {
        value = llvm::ConstantInt::get( builder.getContext(), llvm::APInt( 1, node->value ) );
        return false;
    }

    bool llvm_visitor::visit( ast::call* node )
    {
        node->function->visit( this );
        
        if (const auto func = llvm::dyn_cast< llvm::Function >( value ))
        {
            std::vector< llvm::Value* > args;
            for ( const auto& arg : node->arguments )
            {
                arg->visit( this );
                args.push_back( value );
            }
            value = builder.CreateCall( func, args );
        }
        else
            throw utils::compiler_error( "Invalid function type" );
        return false;
    }

    bool llvm_visitor::visit( ast::variable_reference* node )
    {
        if ( const auto function = std::get_if< ast::type::descriptor::function >( &node->var->type->value ) )
        {
            value = gen.get_or_create_function( node->var );
        }
        else
        {
            // TODO: Implement
        }
        return false;
    }
}  // namespace lorraine::code_generation