#include "generic.hpp"

#include "type.hpp"

namespace lorraine::ast::type
{
    std::string generic::to_string() const
    {
        return t ? t->to_string() : name;
    }
}  // namespace lorraine::ast::type