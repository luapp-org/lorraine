#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "../utils/CLI11.hpp"

namespace lorraine::cli
{
    class cli final
    {
       public:
        /// @brief Creates a new command line interface
        /// @param argc The number of values in argv
        /// @param argv The array of arguments
        cli( int argc, char* argv[] );

        /// @brief Parses the command line arguments
        /// @return 1 for success 0 for failure
        int parse();

       private:
        CLI::App app;

        int argc;
        char** argv;

        std::wstring input;

        void callback();

    };
}  // namespace lorraine::cli