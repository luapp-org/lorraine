#pragma once

#include <memory>
#include <string>
#include <vector>

namespace lorraine::ast::type
{
    struct type;

    namespace descriptor
    {
        // Describes the type signature of a variable list (vararg). It contains a single type that all the variables
        // must be of. For example, in the function signature `void foo( a: number, ...string )`, the vararg descriptor
        // would be `...string`.
        struct vararg
        {
            std::shared_ptr< type > t;

            // Creates the vararg descriptor with a type
            explicit vararg( std::shared_ptr< type > t );

            /// @brief Converts the current descriptor to a string representation of its values.
            /// @return String representation
            std::string to_string() const;
        };
    }  // namespace descriptor

}  // namespace lorraine::ast::type::descriptor