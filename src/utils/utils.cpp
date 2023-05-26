#include "utils.hpp"

namespace lorraine::utils
{
    std::wstring io::read_file( const std::string& filename )
    {
        if ( filename.empty() )
            return {};

        std::wifstream wif( filename );
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
}  // namespace lorraine::utils