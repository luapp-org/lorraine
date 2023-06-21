#include "type.hpp"

#include "../../utils/error.hpp"
#include "table_descriptor.hpp"

namespace lorraine::ast::type
{
    bool type::is( primitive_type t )
    {
        if ( const auto prim = std::get_if< primitive_type >( &value ) )
            t == *prim;

        return false;
    }

    bool type::is( table_descriptor t )
    {
        if ( const auto table = std::get_if< table_descriptor >( &value ) )
        {
            if ( table->properties.size() != t.properties.size() )
                return false;

            // Iterate through each property and compare them. If any are not equal the descriptors
            // are not identical.
            for ( std::size_t i = 0; i < t.properties.size(); ++i )
                if ( !t.properties[ i ].is( table->properties[ i ] ) )
                    return false;

            return true;
        }

        return false;
    }

    bool type::is( std::shared_ptr< type > t )
    {
        if ( const auto prim = std::get_if< primitive_type >( &t->value ) )
            return is( *prim );

        if ( const auto table = std::get_if< table_descriptor >( &t->value ) )
            return is( *table );

        return false;
    }

    std::string type::to_string( primitive_type t )
    {
        switch ( t )
        {
            case primitive_type::string: return "string";
            case primitive_type::number: return "number";
            case primitive_type::boolean: return "boolean";
            case primitive_type::void_: return "void";
            case primitive_type::any: return "any";

            default: return "unknown";
        }
    }

    std::string type::to_string() const
    {
        if ( const auto prim = std::get_if< primitive_type >( &value ) )
            return type::to_string( *prim );

        if ( const auto table = std::get_if< table_descriptor >( &value ) )
            return ( *table ).to_string();

        return "unknown";
    }

    llvm::Type *type::to_llvm_type( llvm::LLVMContext &context )
    {
        if ( const auto prim = std::get_if< primitive_type >( &value ) )
        {
            switch ( *prim )
            {
                case primitive_type::number: return llvm::Type::getDoubleTy( context );
                case primitive_type::void_: return llvm::Type::getVoidTy( context );

                case primitive_type::boolean:
                    return reinterpret_cast< llvm::Type * >( llvm::Type::getInt1Ty( context ) );

                case primitive_type::string:
                    return reinterpret_cast< llvm::Type * >( llvm::Type::getInt8PtrTy( context ) );

                case primitive_type::any:
                    throw utils::compiler_error( "unable to convert 'any' to LLVM type" );
            }
        }
        else
            throw utils::compiler_error( "Descriptors are not supported yet" );
        
        return nullptr;
    }
}  // namespace lorraine::ast::type
