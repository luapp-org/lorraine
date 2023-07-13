#pragma once

#include <memory>
#include <string>
#include <vector>

namespace lorraine::ast::type
{
    struct type;

    struct array_descriptor
    {
        std::shared_ptr< type > t;

        explicit array_descriptor( std::shared_ptr< type > t ) : t( t )
        {
        }

        array_descriptor();

        std::string to_string() const;
    };

}  // namespace lorraine::ast::type