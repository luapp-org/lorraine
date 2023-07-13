#include "table.hpp"

#include <sstream>

#include "../type.hpp"

namespace lorraine::ast::type::descriptor
{
    bool table::table_property::is( table::table_property property ) const
    {
        return property.name == name && property.t->is( t );
    }

    std::string table::to_string() const
    {
        std::stringstream stream;
        stream << "{ ";

        for ( const auto& property : properties )
            stream << property.name << ( property.is_optional ? "?" : "" ) << ": " << property.t->to_string() << ", ";

        stream << '}';

        return stream.str();
    }

    std::optional< table::table_property > table::get_property( const std::string& name )
    {
        for ( const auto& property : properties )
        {
            if ( property.name == name )
                return property;
        }

        return std::nullopt;
    }
}  // namespace lorraine::ast::type::descriptor