#include "compiler.hpp"

#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/WithColor.h>

#include "../ast/type/validator.hpp"
#include "../code_generation/code_generation.hpp"
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"

namespace lorraine::compiler
{
    std::stringstream compiler::compile(
        const std::string& name,
        const std::string_view& source,
        compiler_stage stage )
    {
        this->source = source;

        if ( stage == compiler_stage::lexer )
        {
            lexer::lexer lexer( source, this );

            return lexer.print_tokens();
        }

        parser::parser parser( name, source, this );

        const auto main_module = parser.parse();

        if ( main_module )
        {
            ast::type::validator::validate( main_module.get(), this );

            code_generation::code_generation gen{ main_module.get() };
            std::shared_ptr< llvm::Module > main = gen.generate();
            main->print( llvm::errs(), nullptr );
        }

        return {};
    }

    void compiler::llvm_display_error(
        const std::string& name,
        const std::string_view& source,
        const utils::syntax_error& error )
    {
        llvm::errs() << name << ':' << error.location.start.line << ':'
                     << error.location.start.column + 1 << ": ";
        llvm::WithColor::error();

        std::cerr << error.msg;

        if ( cfg.get< bool >( "detailedErrors" ) )
        {
            std::size_t s = 0;
            llvm::errs() << ":\n" << error.get_snapshot( source, &s );

            llvm::WithColor color( llvm::errs(), llvm::raw_ostream::RED );
            color << error.get_underline( s );
        }

        llvm::errs() << '\n';
    }
}  // namespace lorraine::compiler