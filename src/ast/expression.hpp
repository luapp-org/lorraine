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
        std::shared_ptr< type::type > type;

        explicit expression( const utils::location& location ) : node( location )
        {
        }

        virtual ~expression() = default;
    };

    using expression_list = std::vector< std::unique_ptr< expression > >;

    struct number_literal : expression
    {
        double value;

        explicit number_literal( const utils::location& location, double value )
            : expression( location ),
              value( value )
        {
        }

        void visit( visitor* v ) override;
    };

    struct string_literal : expression
    {
        std::wstring value;

        explicit string_literal( const utils::location& location, const std::wstring& value )
            : expression( location ),
              value( value )
        {
        }

        void visit( visitor* v ) override;
    };

    struct boolean_literal : expression
    {
        bool value;

        explicit boolean_literal( const utils::location& location, bool value )
            : expression( location ),
              value( value )
        {
        }

        void visit( visitor* v ) override;
    };

    struct nil_literal : expression
    {
        explicit nil_literal( const utils::location& location )
            : expression( location )
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

        explicit unresolved_identifier( const utils::location& location, const std::wstring& value )
            : expression( location ),
              value( value )
        {
        }

        void visit( visitor* v ) override;
    };

    struct type_wrapper : expression
    {
        std::wstring name;

        explicit type_wrapper(
            const utils::location& location,
            const std::wstring& name,
            std::shared_ptr< type::type > type )
            : expression( location ),
              name( name )
        {
            this->type = type;
        }

        void visit( visitor* v ) override;
    };

    /// @brief Does not represent a node in the tree but wraps a name and type for a variable
    /// definition. Used in local assignments and references.
    struct variable
    {
        std::wstring value;
        std::shared_ptr< type::type > type;

        explicit variable( const std::wstring& value, std::shared_ptr< type::type > type )
            : value( value ),
              type( type )
        {
        }
    };

    using variable_list = std::vector< std::shared_ptr< variable > >;

    struct variable_reference : expression
    {
        std::shared_ptr< variable > var;

        explicit variable_reference(
            const utils::location& location,
            std::shared_ptr< variable > var )
            : expression( location ),
              var( var )
        {
        }

        void visit( visitor* v ) override;
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

    /// @brief Represents a type alias definition in the AST. `type' Name = type
    struct type_alias_definition : expression
    {
        std::wstring_view name;
        std::shared_ptr< ast::type::type > type;

        explicit type_alias_definition(
            const utils::location& location,
            const std::wstring_view& name,
            std::shared_ptr< ast::type::type > type )
            : expression( location ),
              name( name ),
              type( type )
        {
        }

        void visit( visitor* v ) override;
    };

}  // namespace lorraine::ast