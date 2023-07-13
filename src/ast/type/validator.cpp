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

    bool validator::visit( list_constructor* node )
    {
        // Visit all of our children
        for ( auto& expression : node->expressions )
            expression->visit( this );

        // If we have no expressions, we can't determine a type
        if ( node->expressions.empty() )
        {
            node->type = std::make_shared< type >( type::type::primitive_type::unknown );
            return false;
        }

        // Verify that there are only expressions or variable assignments in the list constructor.
        bool table_constructor = dynamic_cast< variable_assignment* >( node->expressions.front().get() );

        for ( const auto& expr : node->expressions )
        {
            if ( table_constructor != ( dynamic_cast< variable_assignment* >( expr.get() ) != nullptr ) )
            {
                throw utils::syntax_error( expr->location, "cannot mix table and array constructors" );
                return false;
            }
        }

        // Must be a table constructor
        if ( table_constructor )
        {
            table_descriptor table;

            for ( const auto& expr : node->expressions )
            {
                if ( const auto assignment = dynamic_cast< variable_assignment* >( expr.get() ) )
                    table.properties.push_back(
                        table_descriptor::table_property{ assignment->var->value, assignment->value->type, false } );
            }

            node->type = std::make_shared< type >( table );
        }
        // Must be an array constructor
        else
        {
            const auto primary_type = get_primary_type( std::move( node->expressions ) );

            node->type = std::make_shared< type >( array_descriptor{ primary_type } );
        }

        return false;
    }

    std::shared_ptr< type > validator::get_primary_type( expression_list expressions )
    {
        // If we have no expressions, we can't determine a type
        if ( expressions.empty() )
            return std::make_shared< type >( type::type::primitive_type::unknown );

        std::shared_ptr< type > rtype = nullptr;
        for ( const auto& expr : expressions )
        {
            if ( !rtype )
                rtype = expr->type;
            else
            {
                if ( !rtype->is( expr->type ) )
                    return std::make_shared< type >( type::type::primitive_type::any );
            }
        }

        return rtype;
    }

    bool validator::visit( local_assignment* node )
    {
        // Visit all of the expressions
        for ( auto& expression : node->values )
            expression->visit( this );

        const auto location = utils::location{
            node->values.front()->location.start,
            node->values.back()->location.end,
        };

        if ( node->values.size() > node->variables.size() )
        {
            throw utils::syntax_error( location, "too many values in local assignment" );
            return false;
        }

        if ( node->values.size() < node->variables.size() )
        {
            if ( !compiler->cfg.get< bool >( "imbalancedLocalAssignments" ) )
            {
                throw utils::syntax_error( location, "too few values in local assignment" );
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
            if ( variable->type->is( type::primitive_type::any ) )
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

    bool validator::visit( external_decleration* node )
    {
        return true;
    }
}  // namespace lorraine::ast::type