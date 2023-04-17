#include "table_descriptor.hpp"

#include "type.hpp"

namespace lorraine::ast::type
{
    bool table_descriptor::table_property::is( table_descriptor::table_property property )
    {
        return property.name == name && property.is_optional == is_optional && property.t->is( *t );
    }
}  // namespace lorraine::ast::type