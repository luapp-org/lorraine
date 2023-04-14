#pragma once

#include <iostream>
#include <string_view>
#include <unordered_map>
#include <variant>

namespace lorraine::cli
{
    enum class config_type
    {
        json
    };

    class option_value final
    {
       public:
        template< typename T >
        option_value( T value ) : value( value )
        {
        }

        template< typename T >
        T& get()
        {
            return std::get< T >( value );
        }

       private:
        std::variant< bool, std::string > value;
    };

    class config final
    {
       public:
        static config load( const std::string& data, config_type type = config_type::json );

       private:
    };
}  // namespace lorraine::cli
