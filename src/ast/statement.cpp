#include "statement.hpp"

#include <iostream>

#include "visitor.hpp"

namespace lorraine::ast
{
    void type_alias_definition::visit( visitor* v )
    {
        v->visit( this );
    }

    void block::visit( visitor* v )
    {
        if ( v->visit( this ) )
        {
            for ( const auto& s : body )
                s->visit( v );
        }
    }

    std::shared_ptr< type::type > block::get_type( const std::string_view& name )
    {
        const auto it = types.find( name );

        if ( it == types.end() )
            if ( parent )
                return parent->get_type( name );
            else
                return nullptr;

        return it->second;
    }

    std::shared_ptr< type::type > block::get_export_type( const std::string_view& name )
    {
        const auto it = export_types.find( name );

        if ( it == export_types.end() )
            return nullptr;

        return it->second;
    }

    void local_assignment::visit( visitor* v )
    {
        if ( v->visit( this ) )
        {
            // Visit values
            for ( const auto& val : values )
                val->visit( v );
        }
    }

    void export_item::visit( visitor* v )
    {
        if ( v->visit( this ) )
            item->visit( v );
    }

    void module::visit( visitor* v )
    {
        if ( v->visit( this ) )
            body->visit( v );
    }

    void block::load_variable_list( variable_list variables )
    {
        for ( const auto& variable : variables )
            this->variables.emplace( variable->value, variable->type );
    }

    void import::visit( visitor* v )
    {
        if ( v->visit( this ) )
        {
            for ( const auto& name : name_list )
                name->visit( v );

            module->visit( v );
        }
    }

    std::shared_ptr< type::type > block::get_variable_type( const std::string_view& name )
    {
        const auto it = variables.find( name );

        if ( it == variables.end() )
            if ( parent )
                return parent->get_variable_type( name );
            else
                return nullptr;

        return it->second;
    }

    std::shared_ptr< type::type > block::get_export_variable_type( const std::string_view& name )
    {
        const auto it = export_variables.find( name );

        if ( it == export_variables.end() )
            return nullptr;

        return it->second;
    }

    void expression_statement::visit( visitor* v )
    {
        if ( v->visit( this ) )
            expr->visit( v );
    }

    std::shared_ptr< module::information > module::information::get(
        const std::string& path,
        std::string_view source )
    {
        std::shared_ptr< module::information > info = std::make_shared< module::information >();
        info->source = source;

        if ( path == "stdin" )
        {
            info->name = path;
            info->filename = path;
            info->directory = path;
        }
        else
        {
            const std::size_t last_slash = path.find_last_of( "/\\" ) + 1;
            const std::size_t last_period = path.find_last_of( ".\\" );

            info->directory = path.substr( 0, last_slash );
            info->filename = path.substr( last_slash, path.size() - last_slash );
            info->name = path.substr( last_slash, last_period - last_slash );
        }

        return info;
    }

    std::string module::information::absolute() const
    {
        return directory + filename;
    }

    std::shared_ptr< module::information > module::get_information(
        std::shared_ptr< information > relative,
        const std::string& name )
    {
        std::shared_ptr< module::information > info = std::make_shared< module::information >();

        // TODO: Add support for packages. These will just be referenced by their names not './' or
        // '../'
        if ( name[ 0 ] != '.' )
            return nullptr;

        const std::size_t last_slash = name.find_last_of( "/\\" ) + 1;
        const auto file_directory = name.substr( 0, last_slash );

        info->directory = relative->directory + ( file_directory == "./" ? "" : file_directory );
        info->name = name.substr( last_slash, name.size() - last_slash );
        info->filename = info->name + ".lua";
        

        return info;
    }

}  // namespace lorraine::ast
