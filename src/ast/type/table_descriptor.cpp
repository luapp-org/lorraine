#include "table_descriptor.hpp"

#include <sstream>

#include "type.hpp"

namespace lorraine::ast::type
{
    bool table_descriptor::table_property::is( table_descriptor::table_property property )
    {
        return property.name == name && property.is_optional == is_optional && property.t->is( t );
    }

    std::string table_descriptor::to_string() const
    {
        std::stringstream stream;
        stream << '{' << '\n';

        for ( const auto& property : properties )
            stream << "   " << property.name << ": " << property.t->to_string() << ",\n";

        stream << '}';

        return stream.str();
    }
}  // namespace lorraine::ast::type