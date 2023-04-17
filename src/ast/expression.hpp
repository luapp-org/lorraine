#pragma once

#include <memory>
#include <vector>

#include "node.hpp"
#include "type/type.hpp"

namespace lorraine::ast
{
    struct visitor;
    
    struct expression : node
    {
        type::type type;

        // Note: maybe change default type to 'primitive_type::unknown'? This could cause some
        // issues in the type checker/resolver later on...
        explicit expression(
            const utils::location& location,
            type::type type = type::type::primitive_type::any )
            : node( location ),
              type( type )
        {
        }

        virtual ~expression() = default;
    };

    using expression_list = std::vector< std::unique_ptr< expression > >;

    struct number_literal : expression
    {
        double value;

        explicit number_literal( const utils::location& location, double value )
            : expression( location, type::type::primitive_type::number ),
              value( value )
        {
        }

        void visit( visitor* v ) override;
    };

    struct string_literal : expression
    {
        std::wstring value;

        explicit string_literal( const utils::location& location, const std::wstring& value )
            : expression( location, type::type::primitive_type::string ),
              value( value )
        {
        }

        void visit( visitor* v ) override;
    };

    struct boolean_literal : expression
    {
        bool value;

        explicit boolean_literal( const utils::location& location, bool value )
            : expression( location, type::type::primitive_type::boolean ),
              value( value )
        {
        }

        void visit( visitor* v ) override;
    };

    // Represents identifiers that cannot initially be mapped to a definition in the codebase.
    // Unresolved identifiers can be user defined functions, library methods, or simply unknown
    // idenfiers.
    struct unresolved_identifier : expression
    {
        std::wstring value;

        explicit unresolved_identifier( const std::wstring& value )
            : expression( location ),
              value( value )
        {
        }
    };

    struct call : expression
    {
        std::unique_ptr< expression > function;
        expression_list arguments;

        explicit call(
            const utils::location& location,
            std::unique_ptr< expression > function,
            expression_list arguments )
            : expression( location ),
              function( std::move( function ) ),
              arguments( std::move( arguments ) )
        {
        }

        void visit( visitor* v ) override;
    };
}  // namespace lorraine::ast