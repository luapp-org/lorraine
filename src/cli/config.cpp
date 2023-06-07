#include "config.hpp"

#include <fstream>

#include "../utils/CLI11.hpp"

namespace lorraine::cli
{
    config config::load( const std::string& filename )
    {
        config cfg;
        std::ifstream in( filename.empty() ? default_config : filename );

        if ( !in.is_open() )
        {
            if ( !filename.empty() )
                throw CLI::FileError::Missing( filename );
            else
                return cfg;
        }

        std::string line;

        while ( std::getline( in, line ) )
        {
            if ( line.empty() || line[ 0 ] == '#' )
                continue;

            int colon;
            for ( colon = 0; line[ colon ] != ':'; ++colon )
                ;

            const auto& item = line.substr( 0, colon );

            // Skip whitespace after colon
            if (line[colon + 1] == ' ')
                ++colon;

            try
            {
                cfg.try_set( item, line.substr( colon + 1, line.length() ) );
            }
            catch ( const std::out_of_range& e )
            {
                throw CLI::ConfigError( item + ": unknown option; refer to configuration manual" );
            }
            catch ( const std::bad_variant_access& e )
            {
                throw CLI::ConfigError(
                    item + ": option value type mismatch; refer to configuration manual" );
            }
        }

        in.close();
        return cfg;
    }

    void config::try_set( std::string_view name, const std::string& value )
    {
        if ( value == "true" || value == "false" )
            get< bool >( name ) = value == "true";
        else
        {
            get< std::string >( name ) = value;
        }
    }
}  // namespace lorraine::cli