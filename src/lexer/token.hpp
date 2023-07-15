#pragma once

#include <string>

#include "../utils/location.hpp"

namespace lorraine::lexer
{
    enum class token_type
    {
        // Keywords
        kw_and,
        kw_break,
        kw_do,
        kw_else,
        kw_elseif,
        kw_end,
        kw_false,
        kw_for,
        kw_function,
        kw_if,
        kw_in,
        kw_local,
        kw_nil,
        kw_not,
        kw_or,
        kw_repeat,
        kw_return,
        kw_then,
        kw_true,
        kw_until,
        kw_const,
        kw_class,
        kw_constructor,
        kw_implicit,
        kw_type,
        kw_import,
        kw_export,
        kw_from,
        kw_extern,
        kw_interface,
        kw_while,

        // Symbols
        sym_plus,      // +
        sym_min,       // -
        sym_mul,       // *
        sym_div,       // /
        sym_mod,       // %
        sym_pow,       // ^
        sym_g,         // >
        sym_l,         // <
        sym_len,       // #
        sym_dot,       // .
        sym_colon,     // :
        sym_equals,    // =
        sym_comma,     // ,
        sym_lparen,    // (
        sym_rparen,    // )
        sym_rbracket,  // ]
        sym_lbracket,  // [
        sym_lbrace,    // {
        sym_rbrace,    // }
        sym_wiggle,    // ~
        sym_question,  // ?
        sym_pipe,      // |

        // Combinations
        cmb_eq,       // ==
        cmb_ge,       // >=
        cmb_le,       // <=
        cmb_ne,       // ~=
        cmb_concat,   // ..
        cmb_vararg,   // ...
        cmb_inc,      // ++
        cmb_dec,      // --
        cmb_comppe,   // +=
        cmb_compse,   // -=
        cmb_compme,   // *=
        cmb_compde,   // /=
        cmb_compmoe,  // %=
        cmb_comppoe,  // ^=
        cmb_compce,   // ..=
        cmb_arrow,    // =>

        // Literals
        string,
        number,
        boolean,

        // Other
        identifier,
        eof
    };

    struct token
    {
        token_type type = token_type::eof;
        std::string value{};  
        utils::location location{};

        /// @brief Returns a string representation of the token type provided
        /// @param type Token type
        /// @return String representation
        static std::string to_string( const token_type type )
        {
            switch ( type )
            {
                case token_type::string: return "<string>";
                case token_type::number: return "<number>";
                case token_type::boolean: return "<boolean>";
                case token_type::identifier: return "<identifier>";
                case token_type::eof: return "<eof>";

                case token_type::kw_and: return "and";
                case token_type::kw_break: return "break";
                case token_type::kw_do: return "do";
                case token_type::kw_else: return "else";
                case token_type::kw_elseif: return "elseif";
                case token_type::kw_end: return "end";
                case token_type::kw_false: return "false";
                case token_type::kw_for: return "for";
                case token_type::kw_function: return "function";
                case token_type::kw_if: return "if";
                case token_type::kw_in: return "in";
                case token_type::kw_local: return "local";
                case token_type::kw_nil: return "nil";
                case token_type::kw_not: return "not";
                case token_type::kw_or: return "or";
                case token_type::kw_repeat: return "repeat";
                case token_type::kw_return: return "return";
                case token_type::kw_then: return "then";
                case token_type::kw_true: return "true";
                case token_type::kw_const: return "const";
                case token_type::kw_until: return "until";
                case token_type::kw_while: return "while";
                case token_type::kw_import: return "import";
                case token_type::kw_export: return "export";
                case token_type::kw_from: return "from";
                case token_type::kw_type: return "type";
                case token_type::kw_extern: return "extern";
                case token_type::kw_interface: return "interface";

                case token_type::sym_plus: return "+";
                case token_type::sym_min: return "-";
                case token_type::sym_mul: return "*";
                case token_type::sym_div: return "/";
                case token_type::sym_mod: return "%";
                case token_type::sym_pow: return "^";
                case token_type::sym_g: return ">";
                case token_type::sym_l: return "<";
                case token_type::sym_len: return "#";
                case token_type::sym_dot: return ".";
                case token_type::sym_colon: return ":";
                case token_type::sym_equals: return "=";
                case token_type::sym_comma: return ",";
                case token_type::sym_lparen: return "(";
                case token_type::sym_rparen: return ")";
                case token_type::sym_rbracket: return "]";
                case token_type::sym_lbracket: return "[";
                case token_type::sym_lbrace: return "{";
                case token_type::sym_rbrace: return "}";
                case token_type::sym_wiggle: return "~";
                case token_type::sym_question: return "?";
                case token_type::sym_pipe: return "|";

                case token_type::cmb_eq: return "==";
                case token_type::cmb_ge: return ">=";
                case token_type::cmb_le: return "<=";
                case token_type::cmb_ne: return "~=";
                case token_type::cmb_concat: return "..";
                case token_type::cmb_vararg: return "...";
                case token_type::cmb_inc: return "++";
                case token_type::cmb_dec: return "--";
                case token_type::cmb_comppe: return "+=";
                case token_type::cmb_compse: return "-=";
                case token_type::cmb_compme: return "*=";
                case token_type::cmb_compde: return "/=";
                case token_type::cmb_compmoe: return "%=";
                case token_type::cmb_comppoe: return "^=";
                case token_type::cmb_compce: return "..=";
                case token_type::cmb_arrow: return "=>";

                default: return "<unknown>";
            }
        }

        /// @brief Returns a string representation of the current token
        /// @return String representation, must accept return value
        [[nodiscard]] std::string to_string() const
        {
            switch ( type )
            {
                case token_type::number:
                case token_type::boolean:
                {
                    return value;
                }
                case token_type::string:
                {
                    return "\"" + value + "\"";
                }
                case token_type::identifier:
                {
                    return "'" + value + "'";
                }
            }

            const auto str = to_string( type );
            return { str.cbegin(), str.cend() };
        }
    };

}  // namespace lorraine::lexer