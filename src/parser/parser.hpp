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
        explicit parser(
            const std::string& name,
            const std::string_view& source,
            compiler::compiler* compiler )
            : source( source ),
              lexer( source, compiler ),
              compiler( compiler )
        {
            info = ast::module::information::get( name, source );
        }

        /// @brief Parses the source code into an abstract syntax tree
        /// @return A new block
        std::unique_ptr< ast::module > parse();

       private:
        lexer::lexer lexer;
        compiler::compiler* compiler;

        std::shared_ptr< ast::module::information > info = nullptr;
        std::string_view source;

        /// @brief Last block created and entered. Type and variable definitions will get added.
        ast::block* last_block = nullptr;

        /// @brief  Basic 'any' type. Used for unresolved types or when none are specified. This is
        /// also a member variable for convenience, as a type instance of 'any' is commonly
        /// referenced in the code.
        std::shared_ptr< ast::type::type > any_type = std::make_shared< ast::type::type >();

        /// @brief Expects the current token to be of a certain type. Acts similarly to `assert`.
        /// @param type The expected type
        /// @param consume Tells the function to consume the following token
        void expect( const lexer::token_type type, const bool consume = false );

        /// @brief Opens a new module and parses it into an AST
        /// @param loc The location of the import statement invoking this function
        /// @param name The module name
        /// @return The new module
        std::unique_ptr< ast::module > get_module(
            const utils::location& loc,
            const std::string_view& name );

        /// @brief Parses a block (list of statements)
        /// @return New block
        std::unique_ptr< ast::block > parse_block();

        /// @brief Registers all primitive types to a block
        /// @param block Our block
        void register_primitives( ast::block* block );

        /// @brief Parses a statement while making sure grammar is correct
        /// @return A new AST statement
        std::unique_ptr< ast::statement > parse_statement();

        /// @brief Parses an expression from the source code.
        /// @return Expression
        std::unique_ptr< ast::expression > parse_expression();

        /// @brief Parses a list of expressions seperated by a comma (',')
        /// @return Expression list
        ast::expression_list parse_expression_list();

        /// @brief Parses a new type alias definition and adds it to the top level block
        /// @return New type alias definition
        std::unique_ptr< ast::type_alias_definition > parse_type_alias();

        /// @brief Parses a type reference in the source code. This does not return a node in the
        /// AST but a type that can be assigned to a node. Used in type aliases and annotations.
        /// @return Type structure
        std::shared_ptr< ast::type::type > parse_type();

        /// @brief Parses a named type, types that are referenced by names (primitives & classes)
        /// @return Type structure
        std::shared_ptr< ast::type::type > parse_named_type();

        /// @brief Parses a table type reference.
        /// @return Table descriptor
        std::shared_ptr< ast::type::type > parse_table_type();

        /// @brief Parses a local assignment in the source code.
        /// @return Local assignment
        std::unique_ptr< ast::local_assignment > parse_local_assignment();

        /// @brief Parses source code to a list of variable pointers
        /// @return List of variable pointers
        ast::variable_list parse_variable_list();

        /// @brief Parses a single variable from source code. Assigns type 'any' if not annotated.
        /// @return Variable pointer
        std::shared_ptr< ast::variable > parse_variable();

        /// @brief Parses an import statement and loads the referenced module.
        /// @return Import statement
        std::unique_ptr< ast::import > parse_import();

        /// @brief Parses a list of identifiers. These can be either variables or types.
        /// @return List of identifiers
        std::vector< lexer::token > parse_identifier_list();

        /// @brief Parses an export statement.
        /// @return Export statement
        std::unique_ptr< ast::export_item > parse_export();
    };
}  // namespace lorraine::parser