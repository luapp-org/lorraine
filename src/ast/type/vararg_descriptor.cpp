#include "vararg_descriptor.hpp"

#include "type.hpp"

namespace lorraine::ast::type
{
    vararg_descriptor::vararg_descriptor( std::shared_ptr< type > t ) : t( t )
    {
    }

    std::string vararg_descriptor::to_string() const
    {
        return "..." + t->to_string();
    }
}  // namespace lorraine::ast::type