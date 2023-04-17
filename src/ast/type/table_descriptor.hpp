#pragma once

#include <string>
#include <vector>

namespace lorraine::ast::type
{
    struct type;

    // Note: A table descriptor in Lua++ is similar to an object type in typescript. It has a list
    // of properties with their types. In traditional Lua, tables represent a hashmap and an object,
    // but in Lua++ tables represent objects and hashmaps have their own class.
    struct table_descriptor
    {
        // A table descriptor consists of a list of properties. These are defined similarly to a
        // local variable where the type proceeds a ':'.
        struct table_property
        {
            std::wstring name;
            type* t;

            // Optional properties can be specified with a '?' after the property name.
            bool is_optional;

            explicit table_property( const std::wstring& name, type* t, bool is_optional )
                : name( name ),
                  t( t ),
                  is_optional( is_optional )
            {
            }

            /// @brief Compares the current table property with another
            /// @param property Table property
            /// @return True if equal
            bool is( table_property property );
        };

        std::vector< table_property > properties;
    };
}  // namespace lorraine::ast::type