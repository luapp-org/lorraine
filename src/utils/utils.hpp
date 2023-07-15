#pragma once

#include <codecvt>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

namespace lorraine::utils
{
    struct io
    {
        static std::optional< std::string > read_file( const std::string& filename );

        static std::string read_console();

        static std::string get_dir( const std::string& path );
    };

    struct system
    {
        static std::filesystem::path get_source_dir();

        static std::filesystem::path get_working_dir();
    };

}  // namespace lorraine::utils
