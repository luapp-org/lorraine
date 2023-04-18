#include "parser.hpp"

namespace lorraine::parser
{
    std::unique_ptr< ast::block > parser::parse()
    {
        auto root = parse_block();

        expect( lexer::token_type::eof );
        return root;
    }

    std::unique_ptr< ast::block > parser::parse_block( ast::block* parent )
    {
        std::unique_ptr< ast::block > block =
            std::make_unique< ast::block >( lexer.current().location );

        // Register primitive types in the new block if no parent is provided.
        if ( !parent )
            register_primitives( block.get() );
        else
            block->parent = parent;
    }

    void parser::register_primitives( ast::block* block )
    {
        // Note: doing this so that the formatter doesn't multiline
        auto& types = block->types;

        types.emplace( L"string", ast::type::type{ ast::type::type::primitive_type::string } );
        types.emplace( L"number", ast::type::type{ ast::type::type::primitive_type::number } );
        types.emplace( L"boolean", ast::type::type{ ast::type::type::primitive_type::boolean } );
        types.emplace( L"void", ast::type::type{ ast::type::type::primitive_type::void_ } );
        types.emplace( L"any", ast::type::type{ ast::type::type::primitive_type::any } );
    }

    void parser::expect( const lexer::token_type type, const bool consume )
    {
        const lexer::token& current_token = lexer.current();

        if ( current_token.type != type )
        {
            std::wstringstream message;
            message << L"expected " << lexer::token::to_string( type ) << L", got "
                    << lexer.current().to_string();

            compiler->error< utils::syntax_error >( current_token.location, message.str() );
        }
    }
}  // namespace lorraine::parser