#pragma once

#include <string>
#include <memory>

namespace lorraine::ast::type
{
    struct type;

    // Special. This type doesn't actually have a value. It is used to represent a generic type that gets
    // evaluated in the validator pass. It is found in custom types (interfaces, classes, etc.) and method
    // definitions.
    struct generic
    {
        std::string name;
        std::shared_ptr< type > t;

        explicit generic( const std::string& name ) : name( name )
        {
        }

        std::string to_string() const;
    };
}  // namespace lorraine::ast::type