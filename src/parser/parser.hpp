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
        explicit parser( const std::string& name, const std::string_view& source, compiler::compiler* compiler )
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

        /// @brief  Basic 'void' type. Used for functions that don't have an annoation for their
        /// return type. This is also a member veriable for convenience, as a type instance of
        /// 'void' is commonly referenced in the code.
        std::shared_ptr< ast::type::type > void_type =
            std::make_shared< ast::type::type >( ast::type::type::primitive_type::void_ );

        /// @brief Expects the current token to be of a certain type. Acts similarly to `assert`.
        /// @param type The expected type
        /// @param consume Tells the function to consume the following token
        void expect( const lexer::token_type type, const bool consume = false );

        /// @brief Opens a new module and parses it into an AST
        /// @param loc The location of the import statement invoking this function
        /// @param name The module name
        /// @return The new module
        std::unique_ptr< ast::module > get_module( const utils::location& loc, const std::string_view& name );

        /// @brief Creates a type list from the types referenced in an expression list
        /// @param expressions The expression list
        /// @return The type list
        ast::type::type_list get_type_list( ast::expression_list expressions );

        /// @brief Creates a type list from the types referenced in a variable list
        /// @param variables The variable list
        /// @return The type list
        ast::type::type_list get_type_list( ast::variable_list variables );

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

        /// @brief Parses a type reference in the source code. It does exactly what `parse_type` does but does not throw
        /// an exception if the current token does not match any of the tokens.
        /// @return Type structure
        std::shared_ptr< ast::type::type > unsafe_parse_type();

        /// @brief Parses a type list reference.
        /// @return Type list
        ast::type::type_list parse_type_list();

        /// @brief Parses a named type, types that are referenced by names (primitives & classes)
        /// @return Type structure
        std::shared_ptr< ast::type::type > parse_named_type();

        /// @brief Parses a table type reference.
        /// @return Table descriptor
        std::shared_ptr< ast::type::type > parse_table_type();

        /// @brief Parses a function type reference.
        /// @return Function type
        std::shared_ptr< ast::type::type > parse_function_type();

        /// @brief Parses a vararg type reference.
        /// @return Vararg type
        std::shared_ptr< ast::type::type > parse_vararg_type();

        /// @brief Parses a local assignment in the source code.
        /// @return Local assignment
        std::unique_ptr< ast::local_assignment > parse_local_assignment();

        /// @brief Parses source code to a list of variable pointers
        /// @return List of variable pointers
        ast::variable_list parse_variable_list( bool allow_variadic = false );

        /// @brief Parses a single variable from source code. Assigns type 'any' if not annotated.
        /// @return Variable pointer
        std::shared_ptr< ast::variable > parse_variable( bool allow_variadic = false );

        /// @brief Parses an import statement and loads the referenced module.
        /// @return Import statement
        std::unique_ptr< ast::import > parse_import();

        /// @brief Parses a list of identifiers. These can be either variables or types.
        /// @return List of identifiers
        std::vector< lexer::token > parse_identifier_list();

        /// @brief Parses an export statement.
        /// @return Export statement
        std::unique_ptr< ast::export_item > parse_export();

        /// @brief Parses an extern statement (imports c functions)
        /// @return New extern statement
        std::unique_ptr< ast::external_decleration > parse_extern();

        /// @brief Parses function prototype
        /// @return New function prototype
        std::unique_ptr< ast::function_prototype > parse_function_prototype();

        /// @brief Parses a prefix expression. These consist of an expression group or a name expression.
        /// @return Prefix expression
        std::unique_ptr< ast::expression > parse_prefix_expression();

        /// @brief Parses an expression group (parenthesis around an expression)
        /// @return Expression group
        std::unique_ptr< ast::expression_group > parse_expression_group();

        /// @brief Parses a name expression. This is an identifier that can be a variable or a global.
        /// @return Name expression
        std::unique_ptr< ast::expression > parse_name_expression();

        /// @brief Parses a primary expression. Basically anything that can be referenced by a name.
        /// @return Primary expression
        std::unique_ptr< ast::expression > parse_primary_expression();

        /// @brief Parses a call expression. This can be a call to a function or class constructor.
        /// @return Function call expression
        std::unique_ptr< ast::expression > parse_call_expression( std::unique_ptr< ast::expression > function );
    };
}  // namespace lorraine::parser