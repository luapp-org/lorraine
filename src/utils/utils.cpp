#include "utils.hpp"

namespace lorraine::utils
{
    std::optional< std::string > io::read_file( const std::string& filename )
    {
        if ( filename.empty() )
            return std::nullopt;

        std::ifstream input( filename );

        if ( input.fail() )
            return std::nullopt;

        std::stringstream wss;
        wss << input.rdbuf();
        return wss.str();
    }

    std::string io::read_console()
    {
        std::string out;
        std::cout << "> ";
        std::getline( std::cin, out );

        return out;
    }

    std::string io::get_dir( const std::string& path )
    {
        const auto found = path.find_last_of( "/\\" );
        return path.substr( 0, found ) + '/';
    }

    std::filesystem::path system::get_source_dir()
    {
        std::filesystem::path compiler_path = std::filesystem::canonical( "/proc/self/exe" );
        return compiler_path.remove_filename();
    }

    std::filesystem::path system::get_working_dir()
    {
        return std::filesystem::current_path();
    }
}  // namespace lorraine::utils