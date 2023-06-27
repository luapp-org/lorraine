#pragma once

#include <memory>
#include <string>
#include <vector>

namespace lorraine::ast::type
{
    struct type;

    using type_list = std::vector< std::shared_ptr< type > >;

    // Describes the type signature of a function. This descriptor is very useful when building a type safe environment
    // for function variables. A function descriptor consists of a single return type and a list of types for each
    // argument in the function.
    struct function_descriptor
    {
        type_list arguments;
        std::shared_ptr< type > return_type;
    };
}  // namespace lorraine::ast::type