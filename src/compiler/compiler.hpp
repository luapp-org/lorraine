#pragma once

#include <memory>
#include <sstream>
#include <string_view>
#include <vector>

#include "../cli/config.hpp"
#include "../utils/error.hpp"

namespace lorraine::compiler
{
    enum class compiler_stage
    {
        lexer,
        parser,
        type,
        ir,
        codegen
    };

    class compiler final
    {
       public:
        explicit compiler( const cli::config& cfg ) : cfg( cfg )
        {
        }

        /// @brief Compiles the given source
        /// @param name The name of the module we are currentlyu compiling
        /// @param source Code to compile
        /// @param stage Stage the compiler will stop at and generate output for
        /// @return Output stream
        std::wstringstream compile(
            const std::string& name,
            const std::wstring_view& source,
            compiler_stage stage = compiler_stage::codegen );

        void llvm_display_error(
            const std::string& name,
            const std::wstring_view& source,
            const utils::syntax_error& error );

       private:
        cli::config cfg;
        std::wstring_view source;

        std::vector< std::shared_ptr< utils::error > > errors;
    };
}  // namespace lorraine::compiler