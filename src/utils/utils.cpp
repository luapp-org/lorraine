#include "utils.hpp"

namespace lorraine::utils
{
    std::optional< std::wstring > io::read_file( const std::string& filename )
    {
        if ( filename.empty() )
            return std::nullopt;

        std::wifstream wif( filename );

        if (wif.fail())
            return std::nullopt;

        std::wstringstream wss;
        wss << wif.rdbuf();
        return wss.str();
    }

    std::wstring io::read_console()
    {
        std::wstring out;
        std::cout << "> ";
        std::getline( std::wcin, out );

        return out;
    }

    std::string io::get_dir( const std::string& path )
    {
        const auto found = path.find_last_of( "/\\" );
        return path.substr( 0, found ) + '/';
    }
}  // namespace lorraine::utils