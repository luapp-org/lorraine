#include "lexer.hpp"

#include <cwctype>
#include <iomanip>
#include <iostream>

namespace lorraine::lexer
{
    void lexer::next()
    {
        consume_space_or_comment();
        const utils::position start = current_position();

        switch ( wchar_t c = peek_character() )
        {
            case WEOF:
                t.type = token_type::eof;
                t.location = { start, start };
                break;
            case L'\'':
            case L'"':
                read_string( start );

                consume_character();
                break;
            case L'[':
                // Start long string if next char is '['
                if ( peek_character( 1 ) == L'[' )
                {
                    if ( !read_long_string( start ) )
                        compiler->error< utils::syntax_error >(
                            utils::location{ start, current_position() },
                            "unfinished long string near <eof>" );
                }
                else
                {
                    t.type = token_type::sym_lbracket;
                    t.location = { start, start };
                    consume_character();
                }
                break;
            case L'=':
                consume_character();

                if ( peek_character() == L'=' )
                {
                    consume_character();
                    t.type = token_type::cmb_eq;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_rbracket;
                t.location = { start, start };
                break;
            case L'>':
                consume_character();

                if ( peek_character() == L'=' )
                {
                    consume_character();
                    t.type = token_type::cmb_ge;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_g;
                t.location = { start, start };
                break;
            case L'<':
                consume_character();

                if ( peek_character() == L'=' )
                {
                    consume_character();
                    t.type = token_type::cmb_le;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_l;
                t.location = { start, start };
                break;
            case L'~':
                consume_character();

                if ( peek_character() == L'=' )
                {
                    consume_character();
                    t.type = token_type::cmb_ne;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_wiggle;
                t.location = { start, start };
                break;
            case L'.':
                // Check if this is a decimal
                if ( std::iswdigit( peek_character( 1 ) ) )
                {
                    read_number( start );
                    break;
                }

                consume_character();

                if ( peek_character() == L'.' )
                {
                    consume_character();

                    if ( peek_character() == L'.' )
                    {
                        consume_character();

                        t.type = token_type::cmb_vararg;
                        t.location = { start, current_position() };
                        break;
                    }
                    else if ( peek_character() == L'=' )
                    {
                        consume_character();

                        t.type = token_type::cmb_compce;
                        t.location = { start, current_position() };
                        break;
                    }

                    t.type = token_type::cmb_concat;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_dot;
                t.location = { start, start };
                break;
            case L'+':
                consume_character();

                if ( peek_character() == L'+' )
                {
                    consume_character();
                    t.type = token_type::cmb_inc;
                    t.location = { start, current_position() };
                    break;
                }
                else if ( peek_character() == '=' )
                {
                    consume_character();
                    t.type = token_type::cmb_comppe;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_plus;
                t.location = { start, start };
                break;
            case L'-':
                if ( std::iswdigit( peek_character( 1 ) ) )
                {
                    read_number( start );
                    break;
                }

                if ( peek_character() == L'-' )
                {
                    consume_character();
                    t.type = token_type::cmb_dec;
                    t.location = { start, current_position() };
                    break;
                }
                else if ( peek_character() == '=' )
                {
                    consume_character();
                    t.type = token_type::cmb_compme;
                    t.location = { start, current_position() };
                    break;
                }
                else if ( peek_character() == L'>' )
                {
                    consume_character();
                    t.type = token_type::cmb_arrow;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_min;
                t.location = { start, start };
                break;
            case L'*':
                consume_character();

                if ( peek_character() == L'=' )
                {
                    consume_character();
                    t.type = token_type::cmb_compme;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_mul;
                t.location = { start, start };
                break;
            case L'/':
                consume_character();

                if ( peek_character() == L'=' )
                {
                    consume_character();
                    t.type = token_type::cmb_compde;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_div;
                t.location = { start, start };
                break;
            case L'^':
                consume_character();

                if ( peek_character() == L'=' )
                {
                    consume_character();
                    t.type = token_type::cmb_comppoe;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_pow;
                t.location = { start, start };
                break;
            default:
            {
                if ( std::iswdigit( c ) )
                {
                    read_number( start );
                    break;
                }
                else if ( std::iswalpha( c ) || c == '_' )
                {
                    read_identifier( start );
                    break;
                }
                else
                {
                    compiler->error< utils::syntax_error >(
                        utils::location{ start, current_position() }, "unrecognized character" );
                    
                    consume_character();
                    return;
                }
            }
        }
    }

    void lexer::read_identifier( const utils::position& start )
    {
        const std::size_t start_offset = offset;

        // Consume all characters that qualify
        while ( std::iswalnum( peek_character() ) || peek_character() == L'_' )
            consume_character();

        t.location = { start, current_position() };
        t.value = source.substr( start_offset, offset - start_offset );

        const auto kw_iterator = keyword_map.find( t.value );

        if ( kw_iterator == keyword_map.end() )
            t.type = token_type::identifier;
        else
            t.type = kw_iterator->second;
    }

    wchar_t lexer::peek_character( std::size_t count ) const
    {
        if ( offset + count >= source.length() )
            return -1;
        return source[ offset + count ];
    }

    wchar_t lexer::get_character()
    {
        const wchar_t c = peek_character();
        consume_character();

        return c;
    }

    void lexer::consume_character()
    {
        if ( peek_character() == L'\n' )
        {
            ++line;
            line_offset = ++offset;
            return;
        }

        ++offset;
    }

    utils::position lexer::current_position() const
    {
        return { line, offset - line_offset };
    }

    const bool lexer::is_comment() const
    {
        return peek_character() == L'-' && peek_character( 1 ) == L'-';
    }

    void lexer::consume_space_or_comment()
    {
        bool comment;

        while ( ( comment = is_comment() ) || std::iswspace( peek_character() ) )
        {
            if ( !comment )
                consume_character();
            else
                skip_comment();

            comment = false;
        }
    }

    void lexer::skip_comment()
    {
        consume_character();
        consume_character();

        if ( peek_character() == L'[' )
        {
            const utils::position start = current_position();

            if ( !read_long_string( start ) )
                compiler->error< utils::syntax_error >(
                    utils::location{ start, current_position() },
                    "unfinished long comment near <eof>" );
        }

        while ( peek_character() == L'\n' || peek_character() == L'\r' && peek_character() != WEOF )
            consume_character();
    }

    void lexer::read_string( const utils::position& start )
    {
        const wchar_t quote = get_character();
        const auto start_offset = offset;

        while ( peek_character() != quote )
        {
            switch ( peek_character() )
            {
                case WEOF:
                case L'\n':
                case L'\r':
                {
                    compiler->error< utils::syntax_error >(
                        utils::location{ start, current_position() },
                        "unfinished string, expected closing quote" );
                    return;
                }
            }
            consume_character();
        }

        t.type = token_type::string;
        t.value = source.substr( start_offset, offset - start_offset );
        t.location = { start, current_position() };
    }

    void lexer::read_number( const utils::position& start )
    {
        const std::size_t start_offset = offset;
        consume_character();

        while ( std::iswdigit( peek_character() ) || peek_character() == L'.' )
            consume_character();

        if ( peek_character() == 'e' || peek_character() == 'E' )
        {
            consume_character();

            if ( peek_character() == '+' || peek_character() == '-' )
                consume_character();
        }

        // Added for hexadecimal support, simplifies work in the parser
        while ( std::iswdigit( peek_character() ) || std::iswalpha( peek_character() ) )
            consume_character();

        t.location = { start, current_position() };
        t.type = token_type::number;
        t.value = source.substr( start_offset, offset - start_offset );
    }

    const bool lexer::read_long_string( const utils::position& start )
    {
        consume_character();

        const std::size_t delim_count = skip_long_delim();
        const std::size_t start_offset = offset;

        while ( peek_character() != WEOF )
        {
            // Find first closing ']'
            if ( peek_character() == ']' )
            {
                consume_character();

                if ( skip_long_delim() == delim_count )
                {
                    consume_character();

                    t.type = token_type::string;
                    t.value = source.substr(
                        start_offset, ( ( offset - start_offset ) - delim_count ) - 2 );
                    t.location = { start, current_position() };
                    return true;
                }
            }

            consume_character();
        }

        return false;
    }

    std::size_t lexer::skip_long_delim()
    {
        std::size_t delims = 0;
        do
        {
            consume_character();
            ++delims;
        } while ( peek_character() == L'=' );

        return delims;
    }

    std::wstringstream lexer::print_tokens()
    {
        std::wstringstream out;
        token token;

        do
        {
            token = current();

            out << std::left << std::setw( 15 ) << token.to_string() << std::right
                << std::setw( 12 ) << token.location.start.line << ':'
                << token.location.start.column << "-" << token.location.end.line << ':'
                << token.location.end.column << '\n';

            next();
        } while ( token.type != token_type::eof );

        return out;
    }
}  // namespace lorraine::lexer