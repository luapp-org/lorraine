#pragma once

#include <codecvt>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

namespace lorraine::utils
{
    struct io
    {
        static std::optional< std::wstring > read_file( const std::string& filename );

        static std::wstring read_console();

        static std::string get_dir( const std::string& path );
    };

}  // namespace lorraine::utils
