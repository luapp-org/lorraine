#include "compiler.hpp"

#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/WithColor.h>

#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"

namespace lorraine::compiler
{
    std::wstringstream compiler::compile( const std::wstring_view& source, compiler_stage stage )
    {
        this->source = source;

        if ( stage == compiler_stage::lexer )
        {
            lexer::lexer lexer( source, this );

            return lexer.print_tokens();
        }

        parser::parser parser( source, this );

        parser.parse();

        return {};
    }

    bool compiler::report_errors( const std::string& filename )
    {
        for ( const auto& error : errors )
        {
            if ( auto ptr = dynamic_cast< utils::syntax_error* >( error.get() ) )
            {
                llvm::errs() << filename << ':' << ptr->location.start.line << ':'
                             << ptr->location.start.column + 1 << ": ";
                llvm::WithColor::error();

                std::wcerr << ptr->msg;

                if ( cfg.get< bool >( "detailedErrors" ) )
                {
                    std::size_t s = 0;
                    llvm::errs() << ":\n" << ptr->get_snapshot( source, &s );

                    llvm::WithColor color( llvm::errs(), llvm::raw_ostream::RED );
                    color << ptr->get_underline( s );
                }

                llvm::errs() << '\n';
            }
        }

        return errors.size();
    }
}  // namespace lorraine::compiler