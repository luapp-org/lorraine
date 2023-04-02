#pragma once
#include <clocale>

#include "token.hpp"

namespace lorraine::lexer
{
    class lexer final
    {
       public:
        /// @brief Constructs a new lexer instance based on a source string
        /// @param source The code to tokenize
        /// @param locale The locale to use (default C)
        explicit lexer( const std::wstring_view& source, const char* const locale = "C" )
            : source( source )
        {
            std::setlocale( LC_ALL, locale );

            next();
        }

        /// @brief Gets the current token (last set by 'next')
        /// @return The last token tokenized
        const token& current() const
        {
            return t;
        }

        /// @brief Gets the next token from the source (updates current)
        void next();

        /// @brief Gets the next token without updating the source pointer
        /// @return Next token
        token peek();

       private:
        std::wstring_view source{};

        token t;

        std::uint32_t line = 1;
        std::uint32_t offset = 0;
        std::uint32_t line_offset = 0;

        void consume_character();
        [[nodiscard]] wchar_t peek_character( std::size_t count = 0 ) const;
        [[nodiscard]] wchar_t get_character();
        [[nodiscard]] utils::position current_position() const;

        const bool is_comment() const;
        void consume_space_or_comment();

        void read_string( const utils::position& start );

        // Handle long strings/comments
        const bool read_long_string( const utils::position& start );
        std::size_t skip_long_delim();

        // Skip comments
        void skip_comment();
    };
}  // namespace lorraine::lexer