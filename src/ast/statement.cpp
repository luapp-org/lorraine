#include "statement.hpp"

#include <iostream>

#include "visitor.hpp"

namespace lorraine::ast
{
    void type_alias_definition::visit( visitor* v )
    {
        v->visit( this );
    }

    void block::visit( visitor* v )
    {
        if ( v->visit( this ) )
        {
            for ( const auto& s : body )
                s->visit( v );
        }
    }

    std::shared_ptr< type::type > block::get_type( const std::wstring_view& name )
    {
        const auto it = types.find( name );

        if ( it == types.end() )
            if ( parent )
                return parent->get_type( name );
            else
                return nullptr;

        return it->second;
    }

    std::shared_ptr< type::type > block::get_export_type( const std::wstring_view& name )
    {
        const auto it = export_types.find( name );

        if ( it == export_types.end() )
            return nullptr;

        return it->second;
    }

    void local_assignment::visit( visitor* v )
    {
        if ( v->visit( this ) )
        {
            // Visit values
            for ( const auto& val : values )
                val->visit( v );
        }
    }

    void export_item::visit( visitor* v )
    {
        if ( v->visit( this ) )
            item->visit( v );
    }

    void module::visit( visitor* v )
    {
        if ( v->visit( this ) )
            body->visit( v );
    }

    void block::load_variable_list( variable_list variables )
    {
        for ( const auto& variable : variables )
            this->variables.emplace( variable->value, variable->type );
    }

    std::string module::get_module_name( std::string filename )
    {
        if ( filename != "stdin" )
        {
            const auto last = filename.find_last_of( "/\\" ) + 1;
            const auto len = filename.find_last_of( "." ) - last;

            return filename.substr( last, len );
        }

        return filename;
    }

    void import::visit( visitor* v )
    {
        if ( v->visit( this ) )
        {
            for ( const auto& name : name_list )
                name->visit( v );

            module->visit( v );
        }
    }

    std::shared_ptr< type::type > block::get_variable_type( const std::wstring_view& name )
    {
        const auto it = variables.find( name );

        if ( it == variables.end() )
            if ( parent )
                return parent->get_variable_type( name );
            else
                return nullptr;

        return it->second;
    }

    std::shared_ptr< type::type > block::get_export_variable_type( const std::wstring_view& name )
    {
        const auto it = export_variables.find( name );

        if ( it == export_variables.end() )
            return nullptr;

        return it->second;
    }

    void expression_statement::visit( visitor* v )
    {
        if ( v->visit( this ) )
            expr->visit( v );
    }

}  // namespace lorraine::ast
