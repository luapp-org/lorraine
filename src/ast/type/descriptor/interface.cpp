#include "interface.hpp"

#include <sstream>

#include "../type.hpp"

namespace lorraine::ast::type::descriptor
{
    interface::interface( const std::string& name, type_list generics ) : name( name ), generics( generics )
    {
    }

    std::string interface::to_string() const
    {
        std::stringstream ss;
        ss << name << " <";
        for ( const auto& generic : generics )
        {
            ss << generic->to_string() << ", ";
        }
        ss << ">";
        return ss.str();
    }
}  // namespace lorraine::ast::type::descriptor