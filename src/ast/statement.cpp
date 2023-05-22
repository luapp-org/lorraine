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
        if (filename != "stdin")
            return filename.erase(filename.size() - 3);
        
        return filename;
    }
}  // namespace lorraine::ast
