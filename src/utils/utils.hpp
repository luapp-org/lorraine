#pragma once

#include <codecvt>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace lorraine::utils
{
    struct io
    {
        static std::wstring read_file( const std::string& filename );

        static std::wstring read_console();
    };

}  // namespace lorraine::utils
