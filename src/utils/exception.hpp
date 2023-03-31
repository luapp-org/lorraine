#pragma once

#include <stdexcept>
#include <string>

#include "location.hpp"

namespace lorraine::utils
{
    /// @brief Lorraine compiler exception class. Represents where an error during compilation could
    /// have occurred.
    struct compiler_exception : std::runtime_error
    {
        location location;

        explicit compiler_exception( utils::location location, const std::string& msg )
            : std::runtime_error( msg.c_str() ),
              location( location )
        {
        }
    };
}  // namespace lorraine::utils
