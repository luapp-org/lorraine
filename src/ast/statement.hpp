#pragma once

#include <memory>
#include <optional>
#include <unordered_map>

#include "expression.hpp"
#include "node.hpp"

namespace lorraine::ast
{
    struct statement : node
    {
        explicit statement( const utils::location& location ) : node( location )
        {
        }

        virtual ~statement() = default;
    };

    using statement_list = std::vector< std::unique_ptr< statement > >;

    /// @brief Represents a type alias definition in the AST. `type' Name = type
    struct type_alias_definition : statement
    {
        std::wstring_view name;
        std::shared_ptr< ast::type::type > type;

        explicit type_alias_definition(
            const utils::location& location,
            const std::wstring_view& name,
            std::shared_ptr< ast::type::type > type )
            : statement( location ),
              name( name ),
              type( type )
        {
        }

        void visit( visitor* v ) override;
    };

    struct block : statement
    {
        block* parent;
        statement_list body;

        // Data structures for the block's content
        std::unordered_map< std::wstring_view, std::shared_ptr< type::type > > variables;
        std::unordered_map< std::wstring_view, std::shared_ptr< type::type > > types;

        explicit block( const utils::location& location, statement_list body )
            : statement( location ),
              body( std::move( body ) )
        {
        }

        explicit block( const utils::location& location ) : statement( location )
        {
        }

        std::shared_ptr< type::type > get_type( const std::wstring_view& name );
        void load_variable_list( variable_list variables );

        void visit( visitor* v ) override;
    };

    struct local_assignment : statement
    {
        variable_list variables;
        expression_list values;

        explicit local_assignment(
            const utils::location& location,
            variable_list variables,
            expression_list values )
            : statement( location ),
              variables( std::move( variables ) ),
              values( std::move( values ) )

        {
        }

        void visit( visitor* v ) override;
    };
}  // namespace lorraine::ast