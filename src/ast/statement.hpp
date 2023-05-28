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

    struct expression_statement : statement
    {
        std::unique_ptr< expression > expr;

        explicit expression_statement( std::unique_ptr< ast::expression > expr )
            : statement( expr->location ),
              expr( std::move( expr ) )
        {
        }

        void visit( visitor* v ) override;
    };

    using statement_list = std::vector< std::unique_ptr< statement > >;

    struct block : statement
    {
        block* parent;
        statement_list body;

        // Data structures for the block's content
        std::unordered_map< std::wstring_view, std::shared_ptr< type::type > > variables;
        std::unordered_map< std::wstring_view, std::shared_ptr< type::type > > types;

        // Exportable data types
        std::unordered_map< std::wstring_view, std::shared_ptr< type::type > > export_variables;
        std::unordered_map< std::wstring_view, std::shared_ptr< type::type > > export_types;

        explicit block( const utils::location& location, statement_list body )
            : statement( location ),
              body( std::move( body ) )
        {
        }

        explicit block( const utils::location& location ) : statement( location )
        {
        }

        std::shared_ptr< type::type > get_type( const std::wstring_view& name );
        std::shared_ptr< type::type > get_variable_type( const std::wstring_view& name );

        std::shared_ptr< type::type > get_export_type( const std::wstring_view& name );
        std::shared_ptr< type::type > get_export_variable_type( const std::wstring_view& name );

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

    /// @brief An expression (type or variable) that will be exported for accessability
    struct export_item : statement
    {
        std::unique_ptr< expression > item;

        explicit export_item( const utils::location& location, std::unique_ptr< expression > item )
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
        std::string path, name;
        std::unique_ptr< block > body;

        explicit module( const std::string& path, std::unique_ptr< block > body )
            : statement( body->location ),
              path( path ),
              body( std::move( body ) )
        {
            name = get_module_name( path );
        }

        void visit( visitor* v ) override;

        static std::string get_module_name( std::string filename );
    };

    struct import : statement
    {
        expression_list name_list;
        std::unique_ptr< ast::module > module;

        explicit import(
            const utils::location& location,
            expression_list name_list,
            std::unique_ptr< ast::module > module )
            : statement( location ),
              name_list( std::move( name_list ) ),
              module( std::move( module ) )
        {
        }

        void visit( visitor* v ) override;
    };
}  // namespace lorraine::ast