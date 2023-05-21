#include "cli.hpp"

#include "../compiler/compiler.hpp"
#include "../utils/CLI11.hpp"
#include "../utils/error.hpp"
#include "../utils/utils.hpp"

namespace lorraine::cli
{
    cli::cli( int argc, char* argv[] )
        : argc( argc ),
          argv( argv ),
          app( "A multipurpose optimizing compiler for the Lua++ programming language", "lorraine" )
    {
        app.add_option( "file", input_file, "Input file." );
        app.add_option( "-o,--output", output_file, "Output file." );
        app.add_option( "-s,--stage", stage, "Terminates the compiler after the stage." );
        app.add_option( "--config", config_file, "Location of a compiler configuration file." );

        app.add_flag(
            "--detailedErrors",
            cfg.get< bool >( "detailedErrors" ),
            "Prints snippets of code associated with a syntax error." );
        app.add_option(
            "--locale,-l",
            cfg.get< std::string >( "locale" ),
            "Sets the global locale for the compiler (for unicode)." );
        app.add_flag(
            "--elapsedTime",
            cfg.get< bool >( "elapsedTime" ),
            "Prints the elapsed time it took for a stage to complete." );

        app.callback( [ & ]() { callback(); } );
    }

    void cli::callback()
    {
        if ( !config_file.empty() )
            cfg = config::load( config_file );

        source = input_file.empty() ? utils::read_console() : utils::read_file( input_file );

        std::locale::global( std::locale( cfg.get< std::string >( "locale" ) ) );
    }

    int cli::parse()
    {
        try
        {
            app.parse( argc, argv );
        }
        catch ( const CLI::ParseError& e )
        {
            return app.exit( e );
        }

        compiler::compiler compiler( cfg );
        std::wstring output;

        try
        {
            output = compiler.compile( source, get_stage() ).str();
        }
        catch ( const utils::syntax_error& e )
        {
            compiler.llvm_display_error(input_file.empty() ? "stdin" : input_file, e);
        }

        if ( output_file.empty() )
            std::wcout << output;
        else
        {
            std::wofstream out( output_file );
            out << output;
        }

        return 1;
    }

    compiler::compiler_stage cli::get_stage()
    {
        const auto it = stage_map.find( stage );
        if ( it != stage_map.end() )
            return it->second;

        throw CLI::InvalidError(
            "Unknown compiler stage, possible stages: lexer, parser, type, ir, and codegen" );
    }
}  // namespace lorraine::cli

int main( int argc, char* argv[] )
{
    lorraine::cli::cli cli{ argc, argv };

    return cli.parse();
}