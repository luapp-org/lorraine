#pragma once

#include <cstddef>

namespace lorraine::utils
{
    /// @brief Contains line and column information.
    struct position
    {
        std::size_t line;
        std::size_t column;
    };

    /// @brief Contains start and end positions (represents a selection)
    struct location
    {
        position start;
        position end;
    };
}  // namespace lorraine::utils