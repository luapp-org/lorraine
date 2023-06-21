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
        explicit lexer( const std::string_view& source, compiler::compiler* compiler )
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
        std::stringstream print_tokens();

       private:
        std::string_view source{};
        compiler::compiler* compiler;

        token t;

        std::uint32_t line = 1;
        std::uint32_t offset = 0;
        std::uint32_t line_offset = 0;

        void consume_character();
        [[nodiscard]] char peek_character( std::size_t count = 0 ) const;
        [[nodiscard]] char get_character();
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
        std::unordered_map< std::string_view, token_type > keyword_map = {
            { "and", token_type::kw_and },
            { "break", token_type::kw_break },
            { "do", token_type::kw_do },
            { "else", token_type::kw_else },
            { "elseif", token_type::kw_elseif },
            { "end", token_type::kw_end },
            { "false", token_type::kw_false },
            { "true", token_type::kw_true },
            { "for", token_type::kw_for },
            { "function", token_type::kw_function },
            { "if", token_type::kw_if },
            { "in", token_type::kw_in },
            { "local", token_type::kw_local },
            { "nil", token_type::kw_nil },
            { "not", token_type::kw_not },
            { "repeat", token_type::kw_repeat },
            { "return", token_type::kw_return },
            { "then", token_type::kw_then },
            { "until", token_type::kw_until },
            { "const", token_type::kw_const },
            { "class", token_type::kw_class },
            { "constructor", token_type::kw_constructor },
            { "implicit", token_type::kw_implicit },
            { "type", token_type::kw_type },
            { "import", token_type::kw_import },
            { "export", token_type::kw_export },
            { "from", token_type::kw_from },
            { "extern", token_type::kw_extern },
            { "while", token_type::kw_while },
        };

        std::unordered_map< char, token_type > symbol_map = {
            { '?', token_type::sym_question }, { '{', token_type::sym_lbrace },
            { '}', token_type::sym_rbrace },   { ':', token_type::sym_colon },
            { ',', token_type::sym_comma },    { '(', token_type::sym_lparen },
            { ')', token_type::sym_rparen }
        };
    };
}  // namespace lorraine::lexer