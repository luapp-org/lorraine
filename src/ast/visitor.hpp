#pragma once

#include "expression.hpp"
#include "statement.hpp"

#define visitor_definition( root, derived )        \
    virtual bool visit( derived* a )               \
    {                                              \
        return visit( static_cast< root* >( a ) ); \
    }

namespace lorraine::ast
{
    struct visitor
    {
        virtual ~visitor() = default;

        virtual bool visit( node* node )
        {
            return true;
        }

        visitor_definition( node, expression );
        visitor_definition( node, statement );

        visitor_definition( expression, number_literal );
        visitor_definition( expression, string_literal );
        visitor_definition( expression, boolean_literal );
        visitor_definition( expression, unresolved_identifier );
        visitor_definition( expression, call );

        visitor_definition( statement, type_alias_definition );
        visitor_definition( statement, block );
    };
}  // namespace lorraine::ast