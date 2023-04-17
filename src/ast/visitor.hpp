#pragma once

#include "expression.hpp"

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

        visitor_definition( expression, number_literal );
    };
}  // namespace lorraine::ast