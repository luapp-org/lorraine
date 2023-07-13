#include "array_descriptor.hpp"

#include "type.hpp"

namespace lorraine::ast::type
{
    std::string array_descriptor::to_string() const
    {
        return t->to_string() + "[]";
    }

    array_descriptor::array_descriptor() : t( std::make_shared< type >( type::primitive_type::any ) )
    {
    }

}  // namespace lorraine::ast::type