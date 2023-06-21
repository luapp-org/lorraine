#pragma once

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>

#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "../node.hpp"
#include "table_descriptor.hpp"

namespace lorraine::ast::type
{
    struct type
    {
        /// @brief Main representation for primitive types
        enum class primitive_type
        {
            string,
            number,
            boolean,
            void_,
            any
        };

        /// @brief Compares the current type with a given primitive type
        /// @param t Primitive
        /// @return True if equal
        bool is( primitive_type t );

        /// @brief Compares the current type with a given table descriptor
        /// @param t Table descriptor
        /// @return True if equal
        bool is( table_descriptor t );

        /// @brief Compares the current type with another type
        /// @param t Other type
        /// @return True if equal
        bool is( std::shared_ptr< type > t );

        /// @brief Converts a primitive type to a string
        /// @param t Primitive type
        /// @return String
        static std::string to_string( primitive_type t );

        /// @brief Converts the current type to a string
        /// @return String
        [[nodiscard]] std::string to_string() const;

        /// @brief Converts the current type to a llvm type pointer
        /// @param context LLVM context
        /// @return LLVM type ptr
        llvm::Type *to_llvm_type( llvm::LLVMContext &context );

        std::variant< primitive_type, table_descriptor > value;

        type( primitive_type t ) : value( t )
        {
        }

        type( table_descriptor t ) : value( t )
        {
        }

        type() : value( primitive_type::any )
        {
        }
    };
}  // namespace lorraine::ast::type