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

        switch ( char c = peek_character() )
        {
            case EOF:
                t.type = token_type::eof;
                t.location = { start, start };
                break;
            case '\'':
            case '"':
                read_string( start );

                consume_character();
                break;
            case '[':
                // Start long string if next char is '['
                if ( peek_character( 1 ) == '[' )
                {
                    if ( !read_long_string( start ) )
                        throw utils::syntax_error(
                            utils::location{ start, current_position() }, "unfinished long string near <eof>" );
                }
                else
                {
                    t.type = token_type::sym_lbracket;
                    t.location = { start, start };
                    consume_character();
                }
                break;
            case '=':
                consume_character();

                if ( peek_character() == '=' )
                {
                    consume_character();
                    t.type = token_type::cmb_eq;
                    t.location = { start, current_position() };
                    break;
                }
                else if ( peek_character() == '>' )
                {
                    consume_character();
                    t.type = token_type::cmb_arrow;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_equals;
                t.location = { start, start };
                break;
            case '>':
                consume_character();

                if ( peek_character() == '=' )
                {
                    consume_character();
                    t.type = token_type::cmb_ge;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_g;
                t.location = { start, start };
                break;
            case '<':
                consume_character();

                if ( peek_character() == '=' )
                {
                    consume_character();
                    t.type = token_type::cmb_le;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_l;
                t.location = { start, start };
                break;
            case '~':
                consume_character();

                if ( peek_character() == '=' )
                {
                    consume_character();
                    t.type = token_type::cmb_ne;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_wiggle;
                t.location = { start, start };
                break;
            case '.':
                // Check if this is a decimal
                if ( std::isdigit( peek_character( 1 ) ) )
                {
                    read_number( start );
                    break;
                }

                consume_character();

                if ( peek_character() == '.' )
                {
                    consume_character();

                    if ( peek_character() == '.' )
                    {
                        consume_character();

                        t.type = token_type::cmb_vararg;
                        t.location = { start, current_position() };
                        break;
                    }
                    else if ( peek_character() == '=' )
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
            case '+':
                consume_character();

                if ( peek_character() == '+' )
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
            case '-':
                if ( std::isdigit( peek_character( 1 ) ) )
                {
                    read_number( start );
                    break;
                }

                if ( peek_character() == '-' )
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

                t.type = token_type::sym_min;
                t.location = { start, start };
                break;
            case '*':
                consume_character();

                if ( peek_character() == '=' )
                {
                    consume_character();
                    t.type = token_type::cmb_compme;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_mul;
                t.location = { start, start };
                break;
            case '/':
                consume_character();

                if ( peek_character() == '=' )
                {
                    consume_character();
                    t.type = token_type::cmb_compde;
                    t.location = { start, current_position() };
                    break;
                }

                t.type = token_type::sym_div;
                t.location = { start, start };
                break;
            case '^':
                consume_character();

                if ( peek_character() == '=' )
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
                if ( std::isdigit( c ) )
                {
                    read_number( start );
                    break;
                }
                else if ( std::isalpha( c ) || c == '_' )
                {
                    read_identifier( start );
                    break;
                }
                else
                {
                    const auto it = symbol_map.find( c );

                    if ( it != symbol_map.end() )
                    {
                        t.type = it->second;
                        t.location = { start, start };

                        consume_character();
                        break;
                    }

                    std::stringstream msg;
                    msg << "unrecognized character '" << c << "'";

                    throw utils::syntax_error( utils::location{ start, current_position() }, msg.str() );

                    consume_character();
                    return;
                }
            }
        }
    }

    token lexer::peek( std::size_t count )
    {
        // Save the old offsets as we will reset them later
        const std::size_t old_offset = offset, old_line = line, old_line_offset = line_offset;
        const token old_token = current();

        for ( std::size_t i = 0; i < count; ++i )
            next();

        const token t = current();

        // Reset the offsets
        offset = old_offset;
        line = old_line;
        line_offset = old_line_offset;

        // Reset the token
        this->t = old_token;

        return t;
    }

    void lexer::read_identifier( const utils::position& start )
    {
        const std::size_t start_offset = offset;

        // Consume all characters that qualify
        while ( std::isalnum( peek_character() ) || peek_character() == '_' )
            consume_character();

        t.location = { start, current_position() };
        t.value = source.substr( start_offset, offset - start_offset );

        const auto kw_iterator = keyword_map.find( t.value );

        if ( kw_iterator == keyword_map.end() )
            t.type = token_type::identifier;
        else
            t.type = kw_iterator->second;
    }

    char lexer::peek_character( std::size_t count ) const
    {
        if ( offset + count >= source.length() )
            return -1;
        return source[ offset + count ];
    }

    char lexer::get_character()
    {
        const char c = peek_character();
        consume_character();

        return c;
    }

    void lexer::consume_character()
    {
        if ( peek_character() == '\n' )
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
        return peek_character() == '-' && peek_character( 1 ) == '-';
    }

    void lexer::consume_space_or_comment()
    {
        bool comment;

        while ( ( comment = is_comment() ) || std::isspace( peek_character() ) )
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

        if ( peek_character() == '[' )
        {
            const utils::position start = current_position();

            if ( !read_long_string( start ) )
                throw utils::syntax_error(
                    utils::location{ start, current_position() }, "unfinished long comment near <eof>" );
        }

        while ( peek_character() != '\n' && peek_character() != '\r' && peek_character() != EOF )
            consume_character();
    }
    void lexer::read_string( const utils::position& start )
    {
        const char quote = get_character();
        const auto start_offset = offset;

        std::string value;

        while ( peek_character() != quote )
        {
            switch ( peek_character() )
            {
                case EOF:
                case '\n':
                case '\r':
                {
                    throw utils::syntax_error(
                        utils::location{ start, current_position() }, "unfinished string, expected closing quote" );
                    return;
                }
                case '\\':
                {
                    consume_character();

                    switch ( peek_character() )
                    {
                        case '\\': value.push_back( '\\' ); break;
                        case 'a': value.push_back( '\a' ); break;
                        case 'b': value.push_back( '\b' ); break;
                        case 'f': value.push_back( '\f' ); break;
                        case 'n': value.push_back( '\n' ); break;
                        case 'r': value.push_back( '\r' ); break;
                        case 't': value.push_back( '\t' ); break;
                        case 'v': value.push_back( '\v' ); break;
                        case '"': value.push_back( '"' ); break;
                        case '\'': value.push_back( '\'' ); break;
                    }

                    break;  // Added break statement
                }
                default: value.push_back( peek_character() ); break;  // Added break statement
            }

            consume_character();
        }

        t.type = token_type::string;
        t.value = value;
        t.location = { start, current_position() };
    }

    void lexer::read_number( const utils::position& start )
    {
        const std::size_t start_offset = offset;
        consume_character();

        while ( std::isdigit( peek_character() ) || peek_character() == '.' )
            consume_character();

        if ( peek_character() == 'e' || peek_character() == 'E' )
        {
            consume_character();

            if ( peek_character() == '+' || peek_character() == '-' )
                consume_character();
        }

        // Added for hexadecimal support, simplifies work in the parser
        while ( std::isdigit( peek_character() ) || std::isalpha( peek_character() ) )
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

        while ( peek_character() != EOF )
        {
            // Find first closing ']'
            if ( peek_character() == ']' )
            {
                consume_character();

                if ( skip_long_delim() == delim_count )
                {
                    consume_character();

                    t.type = token_type::string;
                    t.value = source.substr( start_offset, ( ( offset - start_offset ) - delim_count ) - 2 );
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
        } while ( peek_character() == '=' );

        return delims;
    }

    std::stringstream lexer::print_tokens()
    {
        std::stringstream out;
        token token;

        do
        {
            token = current();

            out << std::left << std::setw( 15 ) << token.to_string() << std::right << std::setw( 12 )
                << token.location.start.line << ':' << token.location.start.column << "-" << token.location.end.line
                << ':' << token.location.end.column << '\n';

            next();
        } while ( token.type != token_type::eof );

        return out;
    }
}  // namespace lorraine::lexer