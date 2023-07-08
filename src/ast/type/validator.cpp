#include "validator.hpp"

namespace lorraine::ast::type
{
    bool validator::validate( module* root, compiler::compiler* compiler )
    {
        validator v;
        // Set the compiler instance
        v.compiler = compiler;

        try
        {
            root->visit( &v );
        }
        catch ( const utils::syntax_error& error )
        {
            compiler->llvm_display_error( root->info->absolute(), root->info->source, error );

            return false;
        }

        return true;
    }

    bool validator::visit( local_assignment* node )
    {
        const auto location = utils::location{
            node->values.front()->location.start,
            node->values.back()->location.end,
        };

        if ( node->values.size() > node->variables.size() )
        {
            throw utils::syntax_error( location, "Too many values in local assignment" );
            return false;
        }

        if ( node->values.size() < node->variables.size() )
        {
            if ( !compiler->cfg.get< bool >( "imbalancedLocalAssignments" ) )
            {
                throw utils::syntax_error( location, "Too few values in local assignment" );
                return false;
            }
            else
            {
                for ( std::size_t i = 0; i < ( node->variables.size() - node->values.size() ); ++i )
                    node->values.push_back( std::make_unique< nil_literal >( node->location ) );
            }
        }

        for ( std::size_t i = 0; i < node->variables.size(); ++i )
        {
            const auto& variable = node->variables[ i ];
            const auto& value = node->values[ i ];

            // Skip any variables that are of type any
            if ( variable->type->is( type::type::primitive_type::any ) )
                continue;

            if ( !variable->type->is( value->type ) )
            {
                std::stringstream stream;

                stream << "type mismatch; unable to assign variable of type '" << variable->type->to_string()
                       << "' a value of type '" << value->type->to_string() << "'";

                throw utils::syntax_error( variable->location, stream.str() );

                return false;
            }
        }

        return true;
    }

    bool validator::visit( external_decleration *node )
    {
        return true;
    }
}  // namespace lorraine::ast::type