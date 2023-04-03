#include "lexer.hpp"

#include <cwctype>

#include "../utils/exception.hpp"

namespace lorraine::lexer
{
    void lexer::next()
    {
        consume_space_or_comment();
        const utils::position start = current_position();

        switch ( peek_character() )
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
                        throw utils::syntax_exception(
                            { start, current_position() }, "unfinished long string near <eof>" );
                }
                else
                {
                    t.type = token_type::sym_lbracket;
                    t.location = { start, start };
                    consume_character();
                }
                break;

            default: break;
        }
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
                throw utils::syntax_exception(
                    { start, current_position() }, "unfinished long comment near <eof>" );
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
                    throw utils::syntax_exception(
                        { start, current_position() },
                        "unfinished string, expected closing " + quote );
            }
            consume_character();
        }

        t.type = token_type::string;
        t.value = source.substr( start_offset, offset - start_offset );
        t.location = { start, current_position() };
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
}  // namespace lorraine::lexer