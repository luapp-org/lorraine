#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "../compiler/compiler.hpp"
#include "../utils/CLI11.hpp"
#include "config.hpp"

namespace lorraine::cli
{
    class cli final
    {
       public:
        /// @brief Creates a new command line interface
        /// @param argc The number of values in argv
        /// @param argv The array of arguments
        explicit cli( int argc, char* argv[] );

        /// @brief Parses the command line arguments
        /// @return 1 for success 0 for failure
        int parse();

       private:
        CLI::App app;
        config cfg;

        int argc;
        char** argv;

        std::string source;

        std::string input_file, output_file, stage = "codegen";
        std::string config_file;

        void callback();
        compiler::compiler_stage get_stage();
        
        std::string get_input();

        std::unordered_map< std::string_view, compiler::compiler_stage > stage_map = {
            { "lexer", compiler::compiler_stage::lexer },
            { "parser", compiler::compiler_stage::parser },
            { "type", compiler::compiler_stage::type },
            { "ir", compiler::compiler_stage::ir },
            { "codegen", compiler::compiler_stage::codegen },
        };
    };
}  // namespace lorraine::cli