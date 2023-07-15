#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../generic.hpp"

namespace lorraine::ast::type
{
    struct type;
    
    using generic_list = std::vector< generic >;

    namespace descriptor
    {
        // Describes the methods and attributes of a Lua++ interface.
        // TODO: Add support for super interfaces (inheritance).
        struct interface
        {
            // List of (optional) generic types.
            generic_list generics;
            std::string name;

            struct property
            {
                std::string name;
                std::shared_ptr< type > t;

                // Optional properties can be specified with a '?' after the property name.
                bool is_optional;

                explicit property( const std::string& name, std::shared_ptr< type > t, bool is_optional )
                    : name( name ),
                      t( t ),
                      is_optional( is_optional )
                {
                }
            };

            std::vector< property > properties;

            explicit interface( const std::string& name, generic_list generics );

            /// @brief Converts the current descriptor to a string representation of its values.
            /// @return String representation
            std::string to_string() const;

            /// @brief Gets all the interface properties with the provided name
            /// @param name Property name
            /// @return List of properties
            std::vector< property > get_properties( const std::string& name );
        };
    }  // namespace descriptor

}  // namespace lorraine::ast::type