#include "parser.hpp"

namespace lorraine::parser
{
    std::unique_ptr< ast::block > parser::parse()
    {
        auto root = parse_block();

        expect( lexer::token_type::eof );
        return root;
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
            case lexer::token_type::kw_type: return parse_type_alias();
            case lexer::token_type::kw_local: return parse_local_assignment();

            default:
            {
                std::wstringstream msg;
                msg << "unexpected " << current.to_string() << " when parsing statement";

                throw utils::syntax_error( current.location, msg.str() );
            }
        }
    }

    std::unique_ptr< ast::type_alias_definition > parser::parse_type_alias()
    {
        const auto& start = lexer.current().location.start;

        expect( lexer::token_type::kw_type, true );

        // Make sure we actually have an identifier
        expect( lexer::token_type::identifier );

        // Get the name and continue
        const auto& name = lexer.current().value;
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
                std::wstringstream msg;
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

        std::wstringstream msg;
        msg << "the type '" << current.value << "' does not exist in the current context";

        throw utils::syntax_error( current.location, msg.str() );
    }

    void parser::register_primitives( ast::block* block )
    {
        // Note: doing this so that the formatter doesn't multiline
        auto& types = block->types;

        types.emplace(
            L"string",
            std::make_shared< ast::type::type >( ast::type::type::primitive_type::string ) );
        types.emplace(
            L"number",
            std::make_shared< ast::type::type >( ast::type::type::primitive_type::number ) );
        types.emplace(
            L"boolean",
            std::make_shared< ast::type::type >( ast::type::type::primitive_type::boolean ) );
        types.emplace(
            L"void",
            std::make_shared< ast::type::type >( ast::type::type::primitive_type::void_ ) );
        types.emplace(
            L"any", std::make_shared< ast::type::type >( ast::type::type::primitive_type::any ) );
    }

    std::shared_ptr< ast::variable > parser::parse_variable()
    {
        std::shared_ptr< ast::type::type > type = any_type;

        expect( lexer::token_type::identifier );
        const auto& name = lexer.current().value;

        lexer.next();

        if ( lexer.current().type == lexer::token_type::sym_colon )
        {
            lexer.next();

            type = parse_type();
        }

        return std::make_shared< ast::variable >( std::wstring{ name.begin(), name.end() }, type );
    }

    ast::variable_list parser::parse_variable_list()
    {
        ast::variable_list variables;

        do
            variables.push_back( parse_variable() );
        while ( lexer.current().type == lexer::token_type::sym_comma );

        return variables;
    }

    ast::expression_list parser::parse_expression_list()
    {
        ast::expression_list expressions;

        do
            expressions.push_back( parse_expression() );
        while ( lexer.current().type == lexer::token_type::sym_comma );

        return expressions;
    }

    std::unique_ptr< ast::expression > parser::parse_expression()
    {
        const auto start = lexer.current().location.start;

        switch ( lexer.current().type )
        {
            case lexer::token_type::number:
            {
                // omg ew why would anyone use c stuff
                errno = 0;
                wchar_t* end;

                const wchar_t* data = lexer.current().value.data();
                lexer.next();

                double value = wcstof32x( data, &end );

                // Check for errors
                if ( errno || *data == L'\0' || *end != L'\0' )
                {
                    std::wstringstream message;
                    message << L"Unable to convert '" << data << "' to a number";

                    throw utils::syntax_error(
                        utils::location{ start, lexer.current().location.end }, message.str() );

                }

                return std::make_unique< ast::number_literal >( lexer.current().location, value );
            }

            default: break;
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
            std::wstringstream message;
            message << L"expected " << lexer::token::to_string( type ) << L", got "
                    << lexer.current().to_string();

            throw utils::syntax_error( current_token.location, message.str() );
        }

        if ( consume )
            lexer.next();
    }
}  // namespace lorraine::parser