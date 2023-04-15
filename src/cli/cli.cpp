#include "cli.hpp"

#include "../utils/CLI11.hpp"
#include "../utils/utils.hpp"

namespace lorraine::cli
{
    cli::cli( int argc, char* argv[] )
        : argc( argc ),
          argv( argv ),
          app( "A multipurpose optimizing compiler for the Lua++ programming language", "Lorraine" )
    {
        app.add_option( "file", cfg.get< std::string >( "inputFile" ), "Input file." );
        app.add_option( "-o,--output", cfg.get< std::string >( "outputFile" ), "Output file." );
        app.add_option( "--config", config_file, "Location of a compiler configuration file.");

        app.callback( [ & ]() { callback(); } );
    }

    void cli::callback()
    {
        input = utils::read_file( cfg.get< std::string >( "inputFile" ) );
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

        return 1;
    }
}  // namespace lorraine::cli

int main( int argc, char* argv[] )
{
    lorraine::cli::cli cli{ argc, argv };

    return cli.parse();
}