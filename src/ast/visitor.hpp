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

        // Register visitor patterns for expressionss
        visitor_definition( expression, number_literal );
        visitor_definition( expression, string_literal );
        visitor_definition( expression, boolean_literal );
        visitor_definition( expression, unresolved_identifier );
        visitor_definition( expression, type_wrapper );
        visitor_definition( expression, variable_reference );
        visitor_definition( expression, call );
        visitor_definition( expression, type_alias_definition );

        // Register visitor patterns for statements
        visitor_definition( statement, block );
        visitor_definition( statement, expression_statement );
        visitor_definition( statement, local_assignment );
        visitor_definition( statement, export_item );
        visitor_definition( statement, module );
        visitor_definition( statement, import );
    };
}  // namespace lorraine::ast