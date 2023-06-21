#include "parser.hpp"

#include <charconv>

#include "../utils/utils.hpp"

namespace lorraine::parser
{
    std::unique_ptr< ast::module > parser::parse()
    {
        std::unique_ptr< ast::block > root = nullptr;

        try
        {
            root = parse_block();

            expect( lexer::token_type::eof );

            return std::make_unique< ast::module >( info, std::move( root ) );
        }
        catch ( const utils::syntax_error& e )
        {
            compiler->llvm_display_error( info->absolute(), source, e );
            return nullptr;
        }
    }

    std::unique_ptr< ast::block > parser::parse_block()
    {
        std::unique_ptr< ast::block > block =
            std::make_unique< ast::block >( lexer.current().location );

        // Register primitive types in the new block if no parent is provided.
        if ( last_block == nullptr )
            register_primitives( block.get() );
        else
            block->parent = last_block;  // Inherit

        // Update our last block
        last_block = block.get();

        // Parse statements until we encounter the end of the program
        while ( lexer.current().type != lexer::token_type::eof )
            block->body.push_back( parse_statement() );

        return block;
    }

    std::unique_ptr< ast::statement > parser::parse_statement()
    {
        const auto& current = lexer.current();

        // Switch current token type
        switch ( current.type )
        {
            case lexer::token_type::kw_type:
            {
                std::unique_ptr< ast::type_alias_definition > alias = parse_type_alias();

                return std::make_unique< ast::expression_statement >( std::move( alias ) );
            }
            case lexer::token_type::kw_local: return parse_local_assignment();
            case lexer::token_type::kw_import: return parse_import();
            case lexer::token_type::kw_export: return parse_export();
            case lexer::token_type::kw_extern: return parse_extern();

            default:
            {
                std::stringstream msg;
                msg << "unexpected " << current.to_string() << " when parsing statement";

                throw utils::syntax_error( current.location, msg.str() );
            }
        }

        return nullptr;
    }

    std::unique_ptr< ast::import > parser::parse_import()
    {
        const auto& start = lexer.current().location.start;

        expect( lexer::token_type::kw_import, true );

        // TODO: Add syntax that imports all exported items (e.g. '*')
        bool import_all;

        if ( import_all = ( lexer.current().type == lexer::token_type::sym_mul ) )
            lexer.next();
        else
        {
            expect( lexer::token_type::sym_lbrace, true );

            auto identifiers = parse_identifier_list();

            expect( lexer::token_type::sym_rbrace, true );
            expect( lexer::token_type::kw_from, true );

            expect( lexer::token_type::string );

            const auto& current = lexer.current();

            std::unique_ptr< ast::module > module = get_module( current.location, current.value );
            lexer.next();

            // Add all identifiers to an expression list
            ast::expression_list imports;
            for ( const auto& identifier : identifiers )
            {
                const auto name = identifier.value;

                // If we are importing a variable
                if ( auto type = module->body->get_export_variable_type( name ) )
                {
                    std::shared_ptr< ast::variable > var = std::make_shared< ast::variable >(
                        identifier.location, std::string{ name.begin(), name.end() }, type );

                    imports.push_back(
                        std::make_unique< ast::variable_reference >( identifier.location, var ) );

                    last_block->variables.emplace( name, type );
                }
                // If we are importing a type definition
                else if ( auto type = module->body->get_export_type( name ) )
                {
                    imports.push_back( std::make_unique< ast::type_wrapper >(
                        identifier.location, std::string{ name.begin(), name.end() }, type ) );

                    last_block->types.emplace( name, type );
                }
                else
                {
                    std::stringstream msg;
                    msg << "unable to find export of '" << name << "' in "
                        << module->info->filename.c_str();

                    throw utils::syntax_error( identifier.location, msg.str() );
                }
            }
        }

        return nullptr;
    }

    std::unique_ptr< ast::export_item > parser::parse_export()
    {
        if ( last_block->parent != nullptr )
            throw utils::syntax_error(
                lexer.current().location,
                "exports are only allowed in the lowest scope level of the module" );

        const auto& start = lexer.current().location.start;

        expect( lexer::token_type::kw_export, true );

        const auto current = lexer.current();

        switch ( current.type )
        {
            case lexer::token_type::kw_type:
            {
                std::unique_ptr< ast::type_alias_definition > type_alias = parse_type_alias();

                last_block->export_types.emplace( type_alias->name, type_alias->type );

                return std::make_unique< ast::export_item >(
                    utils::location{ start, lexer.current().location.end },
                    std::move( type_alias ) );
            }
        }

        std::stringstream msg;
        msg << "unexpected " << current.to_string() << " when parsing export statement";

        throw utils::syntax_error( current.location, msg.str() );
    }

    std::unique_ptr< ast::extern_item > parser::parse_extern()
    {
        const auto start = lexer.current().location.start;

        // Expect and consume the initial 'extern' statement
        expect( lexer::token_type::kw_extern, true );

        auto p = parse_function_prototype();

        return std::make_unique< ast::extern_item >(
            utils::location{ start, p->location.end }, std::move( p ) );
    }

    std::unique_ptr< ast::function_prototype > parser::parse_function_prototype()
    {
        const auto start = lexer.current().location.start;

        expect( lexer::token_type::identifier );
        const auto name = lexer.current().value;

        lexer.next();

        expect( lexer::token_type::sym_lparen, true );

        const auto variable_list = parse_variable_list( true );

        expect( lexer::token_type::sym_rparen, true );

        std::shared_ptr< ast::type::type > type = void_type;

        // Check if there is an annotation for the main prototype
        if ( lexer.current().type == lexer::token_type::sym_colon )
        {
            lexer.next();

            type = parse_type();
        }

        const auto end = lexer.current().location.end;

        return std::make_unique< ast::function_prototype >(
            utils::location{ start, end },
            std::string{ name.cbegin(), name.cend() },
            std::move( variable_list ),
            type );
    }

    std::vector< lexer::token > parser::parse_identifier_list()
    {
        std::vector< lexer::token > identifiers;

        do
        {
            expect( lexer::token_type::identifier );
            identifiers.push_back( lexer.current() );

            lexer.next();

        } while ( lexer.current().type == lexer::token_type::sym_comma );

        return identifiers;
    }

    std::unique_ptr< ast::module > parser::get_module(
        const utils::location& loc,
        const std::string_view& name )
    {
        std::shared_ptr< ast::module::information > info =
            ast::module::get_information( this->info, std::string{ name.begin(), name.end() } );

        if ( !info )
            throw utils::syntax_error(
                loc, "There was an issue parsing the module name. Use './' for local files." );

        // Check if module actually exists
        auto source = utils::io::read_file( info->absolute() );

        if ( !source.has_value() )
        {
            std::stringstream msg;
            msg << "unable to open file '" << info->absolute().c_str() << "'";

            throw utils::syntax_error( loc, msg.str() );
        }

        // Set the source of the module
        info->source = source.value();

        parser parser{ info->absolute(), *source, compiler };

        return parser.parse();
    }

    std::unique_ptr< ast::type_alias_definition > parser::parse_type_alias()
    {
        const auto& start = lexer.current().location.start;

        expect( lexer::token_type::kw_type, true );

        // Make sure we actually have an identifier
        expect( lexer::token_type::identifier );

        // Get the name and continue
        const auto name = lexer.current().value;
        lexer.next();

        expect( lexer::token_type::sym_equals, true );

        std::shared_ptr< ast::type::type > type = parse_type();

        last_block->types.emplace( name, type );

        return std::make_unique< ast::type_alias_definition >(
            utils::location{ start, lexer.current().location.end }, name, type );
    }

    std::shared_ptr< ast::type::type > parser::parse_type()
    {
        const auto& current = lexer.current();

        switch ( current.type )
        {
            case lexer::token_type::identifier: return parse_named_type();
            case lexer::token_type::sym_lbrace: return parse_table_type();

            default:
            {
                std::stringstream msg;
                msg << "unexpected " << current.to_string() << " when parsing type reference";

                throw utils::syntax_error( current.location, msg.str() );
            }
        }

        return nullptr;
    }

    std::shared_ptr< ast::type::type > parser::parse_table_type()
    {
        expect( lexer::token_type::sym_lbrace, true );

        ast::type::table_descriptor descriptor;

        while ( lexer.current().type != lexer::token_type::sym_rbrace )
        {
            expect( lexer::token_type::identifier );
            const auto name = lexer.current().value;

            lexer.next();

            bool is_optional = false;

            if ( lexer.current().type == lexer::token_type::sym_question )
            {
                is_optional = true;
                lexer.next();
            }

            expect( lexer::token_type::sym_colon, true );

            std::shared_ptr< ast::type::type > type = parse_named_type();

            descriptor.properties.emplace_back( name, type, is_optional );

            if ( lexer.current().type == lexer::token_type::sym_comma )
                lexer.next();
        }

        lexer.next();

        return std::make_shared< ast::type::type >( descriptor );
    }

    std::shared_ptr< ast::type::type > parser::parse_named_type()
    {
        expect( lexer::token_type::identifier );

        const auto current = lexer.current();

        lexer.next();

        if ( const auto type = last_block->get_type( current.value ) )
            return std::make_shared< ast::type::type >( *type );

        std::stringstream msg;
        msg << "the type '" << current.value << "' does not exist in the current context";

        throw utils::syntax_error( current.location, msg.str() );
    }

    void parser::register_primitives( ast::block* block )
    {
        // Note: doing this so that the formatter doesn't multiline
        auto& types = block->types;

        types.emplace(
            "string",
            std::make_shared< ast::type::type >( ast::type::type::primitive_type::string ) );
        types.emplace(
            "number",
            std::make_shared< ast::type::type >( ast::type::type::primitive_type::number ) );
        types.emplace(
            "boolean",
            std::make_shared< ast::type::type >( ast::type::type::primitive_type::boolean ) );
        types.emplace(
            "void", std::make_shared< ast::type::type >( ast::type::type::primitive_type::void_ ) );
        types.emplace(
            "any", std::make_shared< ast::type::type >( ast::type::type::primitive_type::any ) );
    }

    std::shared_ptr< ast::variable > parser::parse_variable( bool allow_variadic )
    {
        const auto start = lexer.current().location.start;

        std::shared_ptr< ast::type::type > type = any_type;

        bool is_variadic = false;
        if ( allow_variadic && lexer.current().type == lexer::token_type::cmb_vararg )
            is_variadic = true;
        else
            expect( lexer::token_type::identifier );

        const auto& name = lexer.current().value;

        lexer.next();

        if ( lexer.current().type == lexer::token_type::sym_colon )
        {
            lexer.next();

            type = parse_type();
        }

        const auto end = lexer.current().location.end;

        if ( !is_variadic )
            return std::make_shared< ast::variable >(
                utils::location{ start, end }, std::string{ name.begin(), name.end() }, type );
        else
            return std::make_shared< ast::variadic >( utils::location{ start, end }, type );
    }

    ast::variable_list parser::parse_variable_list( bool allow_variadic )
    {
        ast::variable_list variables;

        do
        {
            if ( variables.size() )
                lexer.next();

            variables.push_back( parse_variable( allow_variadic ) );
        } while ( lexer.current().type == lexer::token_type::sym_comma );

        return variables;
    }

    ast::expression_list parser::parse_expression_list()
    {
        ast::expression_list expressions;

        do
        {
            if ( expressions.size() )
                lexer.next();

            expressions.push_back( parse_expression() );
        } while ( lexer.current().type == lexer::token_type::sym_comma );

        return expressions;
    }

    std::unique_ptr< ast::expression > parser::parse_expression()
    {
        const auto current = lexer.current();
        const auto start = current.location.start;

        switch ( current.type )
        {
            case lexer::token_type::number:
            {
                double value;

                const std::string data = std::string{ current.value };

                try
                {
                    value = std::stod( data );
                }
                catch ( const std::invalid_argument& e )
                {
                    std::stringstream message;
                    message << "Unable to convert '" << data << "' to a number";

                    throw utils::syntax_error(
                        utils::location{ start, lexer.current().location.end }, message.str() );
                }

                lexer.next();

                return std::make_unique< ast::number_literal >( current.location, value );
            }

            default:
            {
                std::stringstream stream;
                stream << "unexpected token '" << current.to_string()
                       << "' when parsing expression";

                throw utils::syntax_error( current.location, stream.str() );
            }
        }

        return nullptr;
    }

    std::unique_ptr< ast::local_assignment > parser::parse_local_assignment()
    {
        const utils::position& start = lexer.current().location.start;

        expect( lexer::token_type::kw_local, true );

        ast::variable_list variables = parse_variable_list();

        last_block->load_variable_list( variables );

        expect( lexer::token_type::sym_equals, true );

        ast::expression_list expressions = parse_expression_list();

        return std::make_unique< ast::local_assignment >(
            utils::location{ start, lexer.current().location.end },
            variables,
            std::move( expressions ) );
    }

    void parser::expect( const lexer::token_type type, const bool consume )
    {
        const lexer::token& current_token = lexer.current();

        if ( current_token.type != type )
        {
            std::stringstream message;
            message << "expected '" << lexer::token::to_string( type ) << "', got '"
                    << lexer.current().to_string() << "'";

            throw utils::syntax_error( current_token.location, message.str() );
        }

        if ( consume )
            lexer.next();
    }
}  // namespace lorraine::parser