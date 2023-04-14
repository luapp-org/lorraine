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
        // app.add_option( "file", options[ "inputFile" ], "Input file." );

        app.callback( [ & ]() { callback(); } );
    }

    void cli::callback()
    {
        // input = utils::read_file( options[ "inputFile" ] );
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