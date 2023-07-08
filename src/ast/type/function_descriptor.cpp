#include "function_descriptor.hpp"

#include <sstream>

#include "type.hpp"

namespace lorraine::ast::type
{
    std::string function_descriptor::to_string() const
    {
        std::stringstream stream;

        stream << '(' << type::to_string( arguments ) << "): " << type::to_string( returns );

        return stream.str();
    }

    function_descriptor::function_descriptor( type_list arguments, type_list returns )
        : arguments( arguments ),
          returns( returns )
    {
    }
}  // namespace lorraine::ast::type