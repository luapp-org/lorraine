#include "type.hpp"

#include "../../utils/error.hpp"

namespace lorraine::ast::type
{
    bool type::is( primitive_type t )
    {
        if ( const auto prim = std::get_if< primitive_type >( &value ) )
            return t == *prim;

        return false;
    }

    bool type::is( generic t )
    {
        if ( const auto gen = std::get_if< generic >( &value ) )
            return t.name == gen->name;

        return false;
    }

    bool type::is( descriptor::table t )
    {
        if ( const auto table = std::get_if< descriptor::table >( &value ) )
        {
            // Make sure that all properties in the table are present in the other table.
            for ( const auto &property : table->properties )
            {
                if ( const auto other_property = t.get_property( property.name ) )
                {
                    // If we get a property make sure that the types are equal.
                    if ( !property.is( *other_property ) )
                        return false;
                }
                else
                {
                    if ( !property.is_optional )
                        return false;
                }
            }

            return true;
        }

        return false;
    }

    bool type::is( descriptor::function t )
    {
        if ( const auto function = std::get_if< descriptor::function >( &value ) )
        {
            if ( function->arguments.size() != t.arguments.size() || function->returns.size() != t.returns.size() )
                return false;

            // First we need to make sure that the return types are all equal. If any of them differ from eachother we
            // can immediately return false as the descriptors are not equal.
            for ( std::size_t i = 0; i < t.returns.size(); ++i )
                if ( !t.returns[ i ]->is( function->returns[ i ] ) )
                    return false;

            // Now just do the same for the function arguments
            for ( std::size_t i = 0; i < t.arguments.size(); ++i )
                if ( !t.arguments[ i ]->is( function->arguments[ i ] ) )
                    return false;

            return true;
        }

        return false;
    }

    bool type::is( descriptor::vararg t )
    {
        if ( const auto vararg = std::get_if< descriptor::vararg >( &value ) )
            return vararg->t->is( t.t );

        return false;
    }

    bool type::is( descriptor::array t )
    {
        if ( const auto array = std::get_if< descriptor::array >( &value ) )
            return array->t->is( t.t );

        return false;
    }

    bool type::is( descriptor::interface t )
    {
        if ( const auto interface = std::get_if< descriptor::interface >( &value ) )
        {
            if ( interface->name != t.name )
                return false;

            if ( interface->generics.size() != t.generics.size() )
                return false;

            return true;
        }

        return false;
    }

    bool type::is( std::shared_ptr< type > t )
    {
        if ( const auto prim = std::get_if< primitive_type >( &t->value ) )
            return is( *prim );

        if ( const auto gen = std::get_if< generic >( &t->value ) )
            return is( *gen );

        if ( const auto table = std::get_if< descriptor::table >( &t->value ) )
            return is( *table );

        if ( const auto function = std::get_if< descriptor::function >( &t->value ) )
            return is( *function );

        if ( const auto vararg = std::get_if< descriptor::vararg >( &t->value ) )
            return is( *vararg );

        if ( const auto array = std::get_if< descriptor::array >( &t->value ) )
            return is( *array );

        if ( const auto interface = std::get_if< descriptor::interface >( &t->value ) )
            return is( *interface );

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
            case primitive_type::nil: return "nil";

            default: return "unknown";
        }
    }

    std::string type::to_string() const
    {
        if ( const auto prim = std::get_if< primitive_type >( &value ) )
            return type::to_string( *prim );

        if ( const auto gen = std::get_if< generic >( &value ) )
            return gen->to_string();

        if ( const auto table = std::get_if< descriptor::table >( &value ) )
            return ( *table ).to_string();

        if ( const auto array = std::get_if< descriptor::array >( &value ) )
            return ( *array ).to_string();

        if ( const auto function = std::get_if< descriptor::function >( &value ) )
            return ( *function ).to_string();

        if ( const auto vararg = std::get_if< descriptor::vararg >( &value ) )
            return ( *vararg ).to_string();

        if ( const auto interface = std::get_if< descriptor::interface >( &value ) )
            return ( *interface ).to_string();

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
                case primitive_type::boolean: return llvm::Type::getInt1Ty( context );
                case primitive_type::string: return llvm::PointerType::getUnqual( llvm::Type::getInt8Ty( context ) );

                default:
                    throw utils::compiler_error(
                        "An LLVM Type does not exist for the provided Lua++ primitive '" + to_string() + "'" );
            }
        }
        else if ( const auto func = std::get_if< descriptor::function >( &value ) )
        {
            llvm::Type *return_type = nullptr;

            if ( func->returns.size() > 1 )
            {
                std::vector< llvm::Type * > return_types;

                for ( const auto ret : func->returns )
                    return_types.push_back( ret->to_llvm_type( context ) );

                return_type = llvm::StructType::get( context, llvm::makeArrayRef( return_types ) );
            }
            else
                return_type = func->returns.front()->to_llvm_type( context );

            if ( !llvm::FunctionType::isValidReturnType( return_type ) )
                throw utils::compiler_error( "invalid return type" );

            bool is_vararg = false;
            std::vector< llvm::Type * > argument_types;

            for ( const auto arg : func->arguments )
            {
                // We have already validated that '...' is the last type in a type list so if we encounter a vararg, we
                // can just set the flag and return.
                if ( is_vararg = std::get_if< descriptor::vararg >( &arg->value ) != nullptr )
                    break;

                const auto arg_type = arg->to_llvm_type( context );

                if ( !llvm::FunctionType::isValidArgumentType( arg_type ) )
                    throw utils::compiler_error( "invalid argument type" );

                argument_types.push_back( arg_type );
            }

            return llvm::FunctionType::get( return_type, llvm::makeArrayRef( argument_types ), is_vararg );
        }
        else
            throw utils::compiler_error( "Unsupported Lua++ type" );

        return nullptr;
    }

    std::string type::to_string( type_list types )
    {
        std::stringstream stream;

        for ( std::size_t i = 0; i < types.size(); ++i )
        {
            if ( i > 0 )
                stream << ", ";

            stream << types[ i ]->to_string();
        }

        return stream.str();
    }

    bool type::validate( type_list types, utils::location location )
    {
        for ( std::size_t i = 0; i < types.size(); ++i )
        {
            if ( const auto vararg = std::get_if< descriptor::vararg >( &types[ i ]->value ) )
                if ( i != types.size() - 1 )
                {
                    throw utils::syntax_error( location, "vararg must be the last type in a type list" );
                    return false;
                }
        }

        return true;
    }
}  // namespace lorraine::ast::type
