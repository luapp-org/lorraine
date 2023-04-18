#pragma once

#include <string>
#include <string_view>

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
        cmb_comppoe,   // ^=
        cmb_compce,   // ..=
        cmb_arrow,    // ->

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
        std::wstring_view value{};  // wstring for unicode support
        utils::location location{};

        /// @brief Returns a string representation of the token type provided
        /// @param type Token type
        /// @return String representation
        static std::wstring to_string( const token_type type )
        {
            switch ( type )
            {
                case token_type::string: return L"<string>";
                case token_type::number: return L"<number>";
                case token_type::boolean: return L"<boolean>";
                case token_type::identifier: return L"<identifier>";
                case token_type::eof: return L"<eof>";

                case token_type::kw_and: return L"and";
                case token_type::kw_break: return L"break";
                case token_type::kw_do: return L"do";
                case token_type::kw_else: return L"else";
                case token_type::kw_elseif: return L"elseif";
                case token_type::kw_end: return L"end";
                case token_type::kw_false: return L"false";
                case token_type::kw_for: return L"for";
                case token_type::kw_function: return L"function";
                case token_type::kw_if: return L"if";
                case token_type::kw_in: return L"in";
                case token_type::kw_local: return L"local";
                case token_type::kw_nil: return L"nil";
                case token_type::kw_not: return L"not";
                case token_type::kw_or: return L"or";
                case token_type::kw_repeat: return L"repeat";
                case token_type::kw_return: return L"return";
                case token_type::kw_then: return L"then";
                case token_type::kw_true: return L"true";
                case token_type::kw_const: return L"const";
                case token_type::kw_until: return L"until";
                case token_type::kw_while: return L"while";

                case token_type::sym_plus: return L"+";
                case token_type::sym_min: return L"-";
                case token_type::sym_mul: return L"*";
                case token_type::sym_div: return L"/";
                case token_type::sym_mod: return L"%";
                case token_type::sym_pow: return L"^";
                case token_type::sym_g: return L">";
                case token_type::sym_l: return L"<";
                case token_type::sym_len: return L"#";
                case token_type::sym_dot: return L".";
                case token_type::sym_colon: return L":";
                case token_type::sym_equals: return L"=";
                case token_type::sym_comma: return L",";
                case token_type::sym_lparen: return L"(";
                case token_type::sym_rparen: return L")";
                case token_type::sym_rbracket: return L"]";
                case token_type::sym_lbracket: return L"[";
                case token_type::sym_lbrace: return L"{";
                case token_type::sym_rbrace: return L"}";
                case token_type::sym_wiggle: return L"~";

                case token_type::cmb_eq: return L"==";
                case token_type::cmb_ge: return L">=";
                case token_type::cmb_le: return L"<=";
                case token_type::cmb_ne: return L"~=";
                case token_type::cmb_concat: return L"..";
                case token_type::cmb_vararg: return L"...";
                case token_type::cmb_inc: return L"++";
                case token_type::cmb_dec: return L"--";
                case token_type::cmb_comppe: return L"+=";
                case token_type::cmb_compse: return L"-=";
                case token_type::cmb_compme: return L"*=";
                case token_type::cmb_compde: return L"/=";
                case token_type::cmb_compmoe: return L"%=";
                case token_type::cmb_comppoe: return L"^=";
                case token_type::cmb_compce: return L"..=";
                case token_type::cmb_arrow: return L"->";

                default: return L"<unknown>";
            }
        }

        /// @brief Returns a string representation of the current token
        /// @return String representation, must accept return value
        [[nodiscard]] std::wstring to_string() const
        {
            switch ( type )
            {
                case token_type::number:
                case token_type::boolean:
                {
                    return std::wstring{ value.cbegin(), value.cend() };
                }
                case token_type::string:
                {
                    return L"\"" + std::wstring{ value.cbegin(), value.cend() } + L"\"";
                }
            }

            const auto str = to_string( type );
            return { str.cbegin(), str.cend() };
        }
    };

}  // namespace lorraine::lexer