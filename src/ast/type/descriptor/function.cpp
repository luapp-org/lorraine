#include "function.hpp"

#include <sstream>

#include "../type.hpp"

namespace lorraine::ast::type::descriptor
{
    std::string function::to_string() const
    {
        std::stringstream stream;

        stream << '(' << type::to_string( arguments ) << "): " << type::to_string( returns );

        return stream.str();
    }

    function::function( type_list arguments, type_list returns )
        : arguments( arguments ),
          returns( returns )
    {
    }
}  // namespace lorraine::ast::type