#include <codecvt>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "lexer/lexer.hpp"
#include "utils/CLI11.hpp"
#include "utils/utils.hpp"

using namespace lorraine;

int runMain( const std::wstring& input, const std::string& stage )
{
    lexer::lexer lexer( input );

    if ( stage == "lexer" )
    {
        lexer.print_tokens();
        return 1;
    }
    return 1;
}

int main( int argc, char* argv[] )
{
    // TODO: Add command line interface which allows UTF-8 support
    // https://cliutils.github.io/CLI11/book/

    CLI::App app{ "Lorraine. Performance based LLVM compiler for the Lua++ programming language." };

    std::string stage = "codegen";
    std::string input_file_name, output_file_name;
    bool utf8 = true;

    app.add_option(
        "-s,--stage",
        stage,
        "The last stage that the compiler will perform and display output for." );

    app.add_flag( "--utf8", utf8, "Will allow utf8 strings and identifiers." );
    app.add_option( "-o,--output", output_file_name, "The output file." );
    app.add_option( "file", input_file_name, "Input file." );

    CLI11_PARSE( app, argc, argv );

    if ( utf8 )
        std::locale::global( std::locale( "en_US.UTF-8" ) );

    std::wstring input = input_file_name.empty() ? utils::read_console()
                                                 : utils::read_file( input_file_name.c_str() );

    return runMain( input, stage );
}
