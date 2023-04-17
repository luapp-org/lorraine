#pragma once

#include "../utils/location.hpp"

namespace lorraine::ast
{
    struct visitor;

    struct node
    {
        utils::location location;

        virtual ~node() = default;

        /// @brief Visitor method for the visitor pattern
        /// @param v visitor
        virtual void visit( visitor* v ) = 0;

       protected:
        /// @brief Basic node constructor inherited by expressions and statements
        /// @param location Where the node occurs in the code
        explicit node( const utils::location& location ) : location( location )
        {
        }
    };
}  // namespace lorraine::ast