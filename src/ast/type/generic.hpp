#pragma once

#include <string>

namespace lorraine::ast::type
{
    // Special. This type doesn't actually have a value. It is used to represent a generic type that gets
    // evaluated in the validator pass. It is found in custom types (interfaces, classes, etc.) and method
    // definitions.
    struct generic
    {
        std::string name;

        explicit generic( const std::string& name ) : name( name )
        {
        }

        std::string to_string() const
        {
            return name;
        }
    };
}  // namespace lorraine::ast::type