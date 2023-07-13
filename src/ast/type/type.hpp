#pragma once

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>


#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "../node.hpp"
#include "array_descriptor.hpp"
#include "function_descriptor.hpp"
#include "table_descriptor.hpp"
#include "vararg_descriptor.hpp"

namespace lorraine::ast::type
{
    using type_list = std::vector< std::shared_ptr< type > >;

    struct type
    {
        /// @brief Main representation for primitive types
        enum class primitive_type
        {
            // Annotatable types
            string,
            number,
            boolean,
            any,

            // Non-annotatable types
            nil,
            unknown,

            // Annotatable, but only for function return values
            void_,
        };

        /// @brief Compares the current type with a given primitive type
        /// @param t Primitive
        /// @return True if equal
        bool is( primitive_type t );

        /// @brief Compares the current type with a given table descriptor
        /// @param t Table descriptor
        /// @return True if equal
        bool is( table_descriptor t );

        /// @brief Compares the current type with a given function descriptor
        /// @param t Function descriptor
        /// @return True if equal
        bool is( function_descriptor t );

        /// @brief Compares the current type with a given vararg descriptor
        /// @param t Vararg descriptor
        /// @return True if equal
        bool is( vararg_descriptor t );

        /// @brief Compares the current type with a given array descriptor
        /// @param t Vararg descriptor
        /// @return True if equal
        bool is( array_descriptor t );

        /// @brief Compares the current type with another type
        /// @param t Other type
        /// @return True if equal
        bool is( std::shared_ptr< type > t );

        /// @brief Converts a primitive type to a string
        /// @param t Primitive type
        /// @return String
        static std::string to_string( primitive_type t );

        /// @brief Converts a list of types to a string
        /// @param types Type list
        /// @return String
        static std::string to_string( type_list types );

        /// @brief Converts the current type to a string
        /// @return String
        [[nodiscard]] std::string to_string() const;

        /// @brief Converts the current type to a llvm type pointer
        /// @param context LLVM context
        /// @return LLVM type ptr
        llvm::Type *to_llvm_type( llvm::LLVMContext &context);

        /// @brief Validates the current type. Basically makes sure everything has been legally assigned.
        /// @return True if valid
        [[nodiscard]] bool validate( const utils::location &location ) const;

        // @brief Validates a list of types
        // @return True if valid
        static bool validate( type_list types, utils::location location );

        std::variant< primitive_type, table_descriptor, function_descriptor, vararg_descriptor, array_descriptor >
            value;

        type( primitive_type t ) : value( t )
        {
        }

        type( array_descriptor t ) : value( t )
        {
        }

        type( table_descriptor t ) : value( t )
        {
        }

        type( function_descriptor t ) : value( t )
        {
        }

        type( vararg_descriptor t ) : value( t )
        {
        }

        type() : value( primitive_type::any )
        {
        }
    };
}  // namespace lorraine::ast::type