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
        std::string in_name;

        app.add_option( "file", in_name, "Input file." );

        app.callback(
            [ & ]()
            {
                // Set input content
                input =
                    in_name.empty() ? utils::read_console() : utils::read_file( in_name.c_str() );
            } );
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
    }
}  // namespace lorraine::cli