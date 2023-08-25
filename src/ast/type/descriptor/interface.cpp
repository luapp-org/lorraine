#include "interface.hpp"

#include <sstream>

#include "../type.hpp"
#include "../generic.hpp"

namespace lorraine::ast::type::descriptor
{
    interface::interface( const std::string& name, generic_list generics ) : name( name ), generics( generics )
    {
    }

    std::string interface::to_string() const
    {
        std::stringstream ss;
        ss << name << "<";

        for ( std::size_t i = 0; i < generics.size(); ++i )
        {
            if (i > 0)
                ss << ", ";

            ss << generics[i].to_string();
        }
        
        ss << ">";
        return ss.str();
    }

    std::vector< interface::property > interface::get_properties( const std::string& name )
    {
        std::vector< interface::property > matches;

        for ( const auto& property : properties )
        {
            if ( property.name == name )
                matches.push_back( property );
        }

        return matches;
    }
}  // namespace lorraine::ast::type::descriptor