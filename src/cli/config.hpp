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

        template< typename T >
        T& get( std::string_view name )
        {
            return options.at( name ).get< T >();
        }

       private:
        std::unordered_map< std::string_view, option_value > options = {
            { "inputFile", option_value{ std::string{} } },
            { "outputFile", option_value{ std::string{} } },
        };
    };
}  // namespace lorraine::cli
