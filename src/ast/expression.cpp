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

    void function_prototype::visit( visitor* v )
    {
        v->visit( this );
    }
}  // namespace lorraine::ast