#include <codecvt>
#include <fstream>
#include <iostream>
#include <sstream>

namespace lorraine::utils
{
    std::wstring read_file( const char* filename )
    {
        std::wifstream wif( filename );
        std::wstringstream wss;
        wss << wif.rdbuf();
        return wss.str();
    }

    std::wstring read_console()
    {
        std::wstring out;
        std::cout << "> ";
        std::getline( std::wcin, out );

        return out;
    }
}  // namespace lorraine::utils
