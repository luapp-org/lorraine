#include "table_descriptor.hpp"

#include <sstream>

#include "type.hpp"

namespace lorraine::ast::type
{
    bool table_descriptor::table_property::is( table_descriptor::table_property property )
    {
        return property.name == name && property.is_optional == is_optional && property.t->is( t );
    }

    std::wstring table_descriptor::to_string() const
    {
        std::wstringstream stream;
        stream << L'{' << '\n';

        for ( const auto& property : properties )
            stream << L"   " << property.name << L": " << property.t->to_string() << ",\n";

        stream << L'}';

        return stream.str();
    }
}  // namespace lorraine::ast::type