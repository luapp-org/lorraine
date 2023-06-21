#pragma once

#include <iostream>
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
        std::unordered_map< std::string_view, std::shared_ptr< type::type > > variables;
        std::unordered_map< std::string_view, std::shared_ptr< type::type > > types;

        // Exportable data types
        std::unordered_map< std::string_view, std::shared_ptr< type::type > > export_variables;
        std::unordered_map< std::string_view, std::shared_ptr< type::type > > export_types;

        explicit block( const utils::location& location, statement_list body )
            : statement( location ),
              body( std::move( body ) )
        {
        }

        explicit block( const utils::location& location ) : statement( location )
        {
        }

        std::shared_ptr< type::type > get_type( const std::string_view& name );
        std::shared_ptr< type::type > get_variable_type( const std::string_view& name );

        std::shared_ptr< type::type > get_export_type( const std::string_view& name );
        std::shared_ptr< type::type > get_export_variable_type( const std::string_view& name );

        void load_variable_list( variable_list variables );

        void visit( visitor* v ) override;
    };

    struct local_assignment : statement
    {
        variable_list variables;
        expression_list values;

        explicit local_assignment( const utils::location& location, variable_list variables, expression_list values )
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
        struct information
        {
            std::string directory, filename, name;
            std::string_view source;

            std::string absolute() const;

            static std::shared_ptr< information > get( const std::string& path, std::string_view source );
        };

        std::shared_ptr< information > info;
        std::unique_ptr< block > body;

        explicit module( std::shared_ptr< information > info, std::unique_ptr< block > body )
            : statement( body->location ),
              info( info ),
              body( std::move( body ) )
        {
        }

        static std::shared_ptr< information > get_information(
            std::shared_ptr< information > relative,
            const std::string& name );

        void visit( visitor* v ) override;
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

    struct extern_item : statement
    {
        std::unique_ptr< ast::function_prototype > function;

        explicit extern_item( const utils::location& location, std::unique_ptr< ast::function_prototype > function )
            : statement( location ),
              function( std::move( function ) )
        {
        }

        void visit( visitor* v ) override;
    };
}  // namespace lorraine::ast