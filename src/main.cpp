#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "lexer/lexer.hpp"

using namespace lorraine;

int main( int argc, char* argv[] )
{
    lexer::lexer lexer( L"   [", "C.UTF-8" );

    lexer::token token;

    do
    {
        token = lexer.current();

        std::wcout << std::left << std::setw( 15 ) << token.to_string() << std::right
                  << std::setw( 12 ) << token.location.start.line << ':'
                  << token.location.start.column << "-" << token.location.end.line << ':'
                  << token.location.end.column << '\n';

        lexer.next();
    } while ( token.type != lexer::token_type::eof );

    return 1;
}