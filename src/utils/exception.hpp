#pragma once

#include <stdexcept>
#include <string>

#include "location.hpp"

namespace lorraine::utils
{
    /// @brief Lorraine syntax exception class. Is thrown when an unexpected error during
    /// syntax parsing occurs.
    struct syntax_exception : std::runtime_error
    {
        utils::location location;

        explicit syntax_exception( utils::location location, const std::string& msg )
            : std::runtime_error( msg.c_str() ),
              location( location )
        {
        }
    };
}  // namespace lorraine::utils
