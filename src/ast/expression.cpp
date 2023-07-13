#include "expression.hpp"

#include "visitor.hpp"

namespace lorraine::ast
{
    void number_literal::visit( visitor* v )
    {
        v->visit( this );
    }

    void string_literal::visit( visitor* v )
    {
        v->visit( this );
    }

    void unresolved_identifier::visit( visitor* v )
    {
        v->visit( this );
    }

    void variable_reference::visit( visitor* v )
    {
        v->visit( this );
    }

    void type_wrapper::visit( visitor* v )
    {
        v->visit( this );
    }

    void call::visit( visitor* v )
    {
        if ( v->visit( this ) )
        {
            function->visit( v );

            for ( const auto& arg : arguments )
                arg->visit( v );
        }
    }

    void nil_literal::visit( visitor* v )
    {
        v->visit( this );
    }

    void boolean_literal::visit( visitor* v )
    {
        v->visit( this );
    }

    void function_prototype::visit( visitor* v )
    {
        v->visit( this );
    }

    void name_index::visit( visitor* v )
    {
        if ( v->visit( this ) )
            variable->visit( v );
    }

    void expression_index::visit( visitor* v )
    {
        if ( v->visit( this ) )
        {
            variable->visit( v );
            index->visit( v );
        }
    }

    // Visitor body for the expression group
    void expression_group::visit( visitor* v )
    {
        if ( v->visit( this ) )
            value->visit( v );
    }

    void list_constructor::visit( visitor* v )
    {
        if ( v->visit( this ) )
        {
            for ( const auto& expr : expressions )
                expr->visit( v );
        }
    }

    void variable_assignment::visit( visitor* v )
    {
        if ( v->visit( this ) )
        {
            value->visit( v );
        }
    }
}  // namespace lorraine::ast