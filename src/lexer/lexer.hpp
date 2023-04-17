#pragma once

#include <clocale>
#include <unordered_map>

#include "../compiler/compiler.hpp"
#include "token.hpp"

namespace lorraine::lexer
{
    class lexer final
    {
       public:
        /// @brief Constructs a new lexer instance based on a source string
        /// @param source The code to tokenize
        /// @param compiler Main compiler instance
        explicit lexer( const std::wstring_view& source, compiler::compiler* compiler )
            : source( source ),
              compiler( compiler )
        {
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

        /// @brief Dumps all tokens to the console
        /// @returns The output stream the tokens will be sent to
        std::wstringstream print_tokens();

       private:
        std::wstring_view source{};
        compiler::compiler* compiler;

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
        void read_number( const utils::position& start );
        void read_identifier( const utils::position& start );

        // Handle long strings/comments
        const bool read_long_string( const utils::position& start );
        std::size_t skip_long_delim();

        // Skip comments
        void skip_comment();

        // Keyword dictionary
        std::unordered_map< std::wstring_view, token_type > keyword_map = {
            { L"and", token_type::kw_and },
            { L"break", token_type::kw_break },
            { L"do", token_type::kw_do },
            { L"else", token_type::kw_else },
            { L"elseif", token_type::kw_elseif },
            { L"end", token_type::kw_end },
            { L"false", token_type::kw_false },
            { L"true", token_type::kw_true },
            { L"for", token_type::kw_for },
            { L"function", token_type::kw_function },
            { L"if", token_type::kw_if },
            { L"in", token_type::kw_in },
            { L"local", token_type::kw_local },
            { L"nil", token_type::kw_nil },
            { L"not", token_type::kw_not },
            { L"repeat", token_type::kw_repeat },
            { L"return", token_type::kw_return },
            { L"then", token_type::kw_then },
            { L"until", token_type::kw_until },
            { L"const", token_type::kw_const },
            { L"class", token_type::kw_class },
            { L"constructor", token_type::kw_constructor },
            { L"implicit", token_type::kw_implicit },
            { L"type", token_type::kw_type },
            { L"while", token_type::kw_while },
        };
    };
}  // namespace lorraine::lexer