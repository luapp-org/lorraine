#include "array.hpp"

#include "../type.hpp"

namespace lorraine::ast::type::descriptor
{
    std::string array::to_string() const
    {
        return t->to_string() + "[]";
    }

    array::array() : t( std::make_shared< type >( type::primitive_type::any ) )
    {
    }

}  // namespace lorraine::ast::type