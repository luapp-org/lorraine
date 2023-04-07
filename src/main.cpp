#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/WithColor.h>

#include <codecvt>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "lexer/lexer.hpp"
#include "utils/CLI11.hpp"
#include "utils/exception.hpp"
#include "utils/timer.hpp"
#include "utils/utils.hpp"

using namespace lorraine;

std::wstringstream run_compiler( const std::wstring& input, const std::string& stage, bool elapsed )
{
    std::wstringstream out;

    lexer::lexer lexer( input );

    if ( stage == "lexer" )
    {
        lexer.print_tokens( out );
        return out;
    }

    return out;
}

void write_content( const std::string& filename, const std::wstring& out )
{
    if ( filename.empty() )
        std::wcout << out;
    else
    {
        std::wofstream output( filename );
        output << out;
    }
}

int main( int argc, char* argv[] )
{
    CLI::App app{
        "Lorraine, a performance based LLVM compiler for the Lua++ programming language."
    };

    std::string stage = "codegen";
    std::string input_file_name, output_file_name;
    bool utf8 = true;
    bool elapsed_time = false;
    bool errors_detailed = true;

    app.add_option(
        "-s,--stage",
        stage,
        "The last stage that the compiler will perform and display output for." );

    app.add_flag( "--utf8", utf8, "Will allow utf8 strings and identifiers." );
    app.add_option( "-o,--output", output_file_name, "The output file." );
    app.add_option( "file", input_file_name, "Input file." );
    app.add_flag( "-e,--elapsed", elapsed_time, "Display elapsed time for the compiler stage." );
    app.add_flag(
        "--errors-detailed", errors_detailed, "Show a snippet of the code surrounding errors." );

    // Set configuration file (optional)
    app.set_config( "--config", "lorraineConfig.ini", "Read configuration file." );

    CLI11_PARSE( app, argc, argv );

    if ( utf8 )
        std::locale::global( std::locale( "en_US.UTF-8" ) );

    std::wstring input = input_file_name.empty() ? utils::read_console()
                                                 : utils::read_file( input_file_name.c_str() );

    utils::timer timer;
    std::wstring out;
    timer.start();

    try
    {
        out = run_compiler( input, stage, elapsed_time ).str();
    }
    catch ( const utils::syntax_exception& e )
    {
        llvm::errs() << ( input_file_name.empty() ? "stdin" : input_file_name ) << ':'
                     << e.location.start.line << ':' << e.location.start.column + 1 << ": ";
        llvm::WithColor::error() << e.what();

        if ( errors_detailed )
        {
            std::size_t s = 0;
            llvm::errs() << ":\n" << e.get_snapshot( input, &s );

            llvm::WithColor color( llvm::errs(), llvm::raw_ostream::RED );
            color << e.get_underline( s );
        }

        llvm::errs() << '\n';

        return 1;
    }

    timer.stop();

    write_content( output_file_name, out );
}
