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

    void block::load_variable_list( variable_list variables )
    {
        for ( const auto& variable : variables )
            this->variables.emplace( variable->value, variable->type );
    }
}  // namespace lorraine::ast
