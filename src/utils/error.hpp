#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

#include "location.hpp"

namespace lorraine::utils
{
    /// @brief Base error class
    struct error : std::runtime_error
    {
        std::wstring msg;

        explicit error( const std::wstring& msg )
            : std::runtime_error( std::string{ msg.begin(), msg.end() } ),
              msg( msg )
        {
        }

        virtual ~error() = default;
    };

    /// @brief Lorraine syntax error class. Is thrown when an unexpected error during
    /// syntax parsing occurs.
    struct syntax_error : error
    {
        utils::location location;

        explicit syntax_error( utils::location location, const std::wstring& msg )
            : error( msg ),
              location( location )
        {
        }

        /// @brief Gets the lines that the syntax error applies to
        /// @param source The whole script as a string
        /// @return The "snapshot" or snippet
        std::string get_snapshot( const std::wstring_view& source, std::size_t* size = nullptr )
            const
        {
            int start, end;
            int newlines = 1;
            std::stringstream snippet;

            for ( start = 0; newlines < location.start.line; ++start )
                if ( source[ start ] == L'\n' )
                    newlines++;

            std::wstringstream line;
            for ( end = start; end < source.size() && source[ end ] != L'\n'; ++end )
                line << source[ end ];

            // Add line number and extra spacing
            snippet << "  " << location.start.line << " | ";
            const int offset = snippet.str().size();

            // Add the actual code snippet
            const auto& line_str = line.str();
            snippet << std::string{ line_str.cbegin(), line_str.cend() } << '\n';

            // Set snippet size
            if ( size )
                *size = line_str.size();

            // Align error underline with the snippet
            for ( int i = 0; i < offset - 2; ++i )
                snippet << " ";

            snippet << "| ";

            return snippet.str();
        }

        /// @brief Gets the underline for the syntax error
        /// @param size The size of the line printed
        /// @return Underline string
        std::string get_underline( std::size_t size ) const
        {
            std::stringstream str;

            const int end_col =
                location.start.line == location.end.line ? location.end.column : size;

            // Print the underline/spaces with color
            for ( int i = 0; i < location.end.column + ( size == 0 ? 1 : 0 ); ++i )
            {
                if ( i < location.start.column )
                    str << ' ';
                else if ( i >= location.start.column && i <= end_col )
                {
                    if ( i == location.start.column )
                        str << '^';
                    else
                        str << '~';
                }
            }

            return str.str();
        }
    };
}  // namespace lorraine::utils
