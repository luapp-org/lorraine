#pragma once

#include <memory>
#include <string>
#include <vector>

namespace lorraine::ast::type
{
    struct type;

    using type_list = std::vector< std::shared_ptr< type > >;

    namespace descriptor
    {
        // Describes the methods and attributes of a Lua++ interface.
        // TODO: Add support for super interfaces (inheritance).
        struct interface
        {
            // List of (optional) generic types.
            type_list generics;
            std::string name;

            explicit interface( const std::string& name, type_list generics );

            /// @brief Converts the current descriptor to a string representation of its values.
            /// @return String representation
            std::string to_string() const;
        };
    }  // namespace descriptor

}  // namespace lorraine::ast::type