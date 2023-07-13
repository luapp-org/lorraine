#include "vararg.hpp"

#include "../type.hpp"

namespace lorraine::ast::type::descriptor
{
    vararg::vararg( std::shared_ptr< type > t ) : t( t )
    {
    }

    std::string vararg::to_string() const
    {
        return "..." + t->to_string();
    }
}  // namespace lorraine::ast::type