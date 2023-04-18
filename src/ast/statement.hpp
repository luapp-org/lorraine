#pragma once

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
        std::wstring name;
        type::type type;

        explicit type_alias_definition(
            const utils::location& location,
            const std::wstring& name,
            type::type type )
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
        std::unordered_map< std::wstring, type::type > types;

        explicit block( const utils::location& location, statement_list body )
            : statement( location ),
              body( std::move( body ) )
        {
        }

        explicit block( const utils::location& location ) : statement( location )
        {
        }

        std::optional< type::type > get_type( const std::wstring& name );

        void visit( visitor* v ) override;
    };
}  // namespace lorraine::ast