#pragma once

#include "../ast/expression.hpp"
#include "../ast/statement.hpp"
#include "../ast/type/type.hpp"
#include "../lexer/lexer.hpp"

namespace lorraine::parser
{
    class parser final
    {
       public:
        /// @brief Initializes a new parser class instance
        explicit parser( const std::wstring_view& source, compiler::compiler* compiler )
            : lexer( source, compiler ),
              compiler( compiler )
        {
        }

        /// @brief Parses the source code into an abstract syntax tree
        /// @return A new block
        std::unique_ptr< ast::block > parse();

       private:
        lexer::lexer lexer;
        compiler::compiler* compiler;

        /// @brief Expects the current token to be of a certain type. Acts similarly to `assert`.
        /// @param type The expected type
        /// @param consume Tells the function to consume the following token
        void expect( const lexer::token_type type, const bool consume = false );

        /// @brief Parses a block (list of statements)
        /// @param parent Parent block
        /// @return New block
        std::unique_ptr< ast::block > parse_block( ast::block* parent = nullptr );

        /// @brief Registers all primitive types to a block
        /// @param block Our block
        void register_primitives( ast::block* block );
    };
}  // namespace lorraine::parser