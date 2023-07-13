#pragma once

#include <memory>
#include <string>
#include <vector>

namespace lorraine::ast::type
{
    struct type;

    namespace descriptor
    {

        struct array
        {
            std::shared_ptr< type > t;

            explicit array( std::shared_ptr< type > t ) : t( t )
            {
            }

            array();

            std::string to_string() const;
        };
    }  // namespace descriptor
    
}  // namespace lorraine::ast::type