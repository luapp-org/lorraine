#include "llvm_visitor.hpp"

namespace lorraine::code_generation
{
    bool llvm_collector::visit( ast::extern_item* item )
    {
        external_functions.push_back( item->function.get() );
        return false;
    }

    bool llvm_visitor::visit( ast::number_literal* node )
    {
        value = llvm::ConstantFP::get( builder.getContext(), llvm::APFloat( node->value ) );
        return false;
    }

    bool llvm_visitor::visit( ast::string_literal* node )
    {
        value = builder.CreateGlobalString( llvm::StringRef( node->value.c_str() ) );
        return false;
    }
    
    bool llvm_visitor::visit( ast::boolean_literal* node )
    {
        value = llvm::ConstantInt::get( builder.getContext(), llvm::APInt( 1, node->value ) );
        return false;
    }
}  // namespace lorraine::code_generation