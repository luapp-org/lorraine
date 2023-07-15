#include "statement.hpp"

#include <iostream>

#include "../utils/utils.hpp"
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

    std::shared_ptr< type::type > block::get_export_type( const std::string& name )
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

    std::shared_ptr< type::type > block::get_export_variable_type( const std::string& name )
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

    std::shared_ptr< module::information > module::information::get( const std::string& path, std::string_view source )
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
            std::filesystem::path current = utils::system::get_working_dir();

            info->filename = path;
            info->name = std::filesystem::path{ info->filename }.replace_extension( "" );
            info->directory = current.string() + "/";
        }

        return info;
    }

    std::string module::information::absolute() const
    {
        return directory + filename;
    }

    std::shared_ptr< module::information > module::get_information(
        std::shared_ptr< information > relative,
        std::string name )
    {
        std::shared_ptr< module::information > info = std::make_shared< module::information >();

        // Absolute path to our module
        std::filesystem::path absolute = utils::system::get_source_dir().append( name + ".lua" );

        info->name = name;
        info->filename = absolute.filename();
        info->directory = absolute.remove_filename();

        return info;
    }

    void external_decleration::visit( visitor* v )
    {
        v->visit( this );
    }

    void interface_definition::visit( visitor* v )
    {
        v->visit( this );
    }

}  // namespace lorraine::ast
