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

    /// @brief An expression (type or function) that will be exported for accessability
    struct export_item : statement
    {
        std::unique_ptr< statement > item;

        explicit export_item( const utils::location& location, std::unique_ptr< statement > item )
            : statement( location ),
              item( std::move( item ) )
        {
        }

        void visit( visitor* v ) override;
    };

    using export_list = std::vector< std::unique_ptr< export_item > >;

    /// @brief An object that can be referenced from different files
    struct module : statement
    {
        std::string name;
        std::unique_ptr< block > body;

        explicit module(
            const utils::location& location,
            std::string name,
            std::unique_ptr< block > body )
            : statement( location ),
              name( std::move( name ) ),
              body( std::move( body ) )
        {
        }

        void visit( visitor* v ) override;

        static std::string get_module_name( std::string filename );
    };
}  // namespace lorraine::ast