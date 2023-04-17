#pragma once

#include <iostream>
#include <string_view>
#include <unordered_map>
#include <variant>

namespace lorraine::cli
{
    constexpr const char* default_config = "./lorraine.cfg";

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
        static config load( const std::string& filename );

        template< typename T >
        T& get( std::string_view name )
        {
            try
            {
                return options.at( name ).get< T >();
            }
            catch ( const std::out_of_range& e )
            {
                throw std::runtime_error(
                    "Error retrieving value from option '" + std::string{ name } + "'" );
            }
        }

        void try_set( std::string_view name, const std::string& value );

       private:
        std::unordered_map< std::string_view, option_value > options = {
            { "locale", option_value{ std::string{ "en_US.UTF-8" } } },
            { "elapsedTime", option_value{ true } },
            { "detailedErrors", option_value{ true } },
        };
    };
}  // namespace lorraine::cli
