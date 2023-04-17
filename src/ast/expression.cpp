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

    void call::visit( visitor* v )
    {
        if ( v->visit( this ) )
        {
            function->visit( v );

            for ( const auto& arg : arguments )
                arg->visit( v );
        }
    }
}  // namespace lorraine::ast