#ifndef ENTT_CORE_MEMORY_HPP
#define ENTT_CORE_MEMORY_HPP


#include <memory>
#include <type_traits>
#include <utility>
#include "../config/config.h"


namespace entt {


/**
 * @brief Unwraps fancy pointers, does nothing otherwise (waiting for C++20).
 * @tparam Type Pointer type.
 * @param ptr Fancy or raw pointer.
 * @return A raw pointer that represents the address of the original pointer.
 */
template<typename Type>
[[nodiscard]] constexpr auto to_address(Type &&ptr) ENTT_NOEXCEPT {
    if constexpr(std::is_pointer_v<std::remove_const_t<std::remove_reference_t<Type>>>) {
        return ptr;
    } else {
        return to_address(std::forward<Type>(ptr).operator->());
    }
}


/**
 * @brief Utility function to design allocation-aware containers.
 * @tparam Allocator Type of allocator.
 * @param lhs A valid allocator.
 * @param rhs Another valid allocator.
 */
template<typename Allocator>
constexpr void propagate_on_container_copy_assignment([[maybe_unused]] Allocator &lhs, [[maybe_unused]] Allocator &rhs) ENTT_NOEXCEPT {
    if constexpr(std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value) {
        lhs = rhs;
    }
}


/**
 * @brief Utility function to design allocation-aware containers.
 * @tparam Allocator Type of allocator.
 * @param lhs A valid allocator.
 * @param rhs Another valid allocator.
 */
template<typename Allocator>
constexpr void propagate_on_container_move_assignment([[maybe_unused]] Allocator &lhs, [[maybe_unused]] Allocator &rhs) ENTT_NOEXCEPT {
    if constexpr(std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value) {
        lhs = std::move(rhs);
    }
}


/**
 * @brief Utility function to design allocation-aware containers.
 * @tparam Allocator Type of allocator.
 * @param lhs A valid allocator.
 * @param rhs Another valid allocator.
 */
template<typename Allocator>
constexpr void propagate_on_container_swap(Allocator &lhs, Allocator &rhs) ENTT_NOEXCEPT {
    constexpr auto pocs = std::allocator_traits<Allocator>::propagate_on_container_swap::value;
    ENTT_ASSERT(pocs || lhs == rhs, "Cannot swap the containers");

    if constexpr(pocs) {
        using std::swap;
        swap(lhs, rhs);
    }
}


/**
 * @brief Utility class to check whether a value is a power of two or not.
 * @tparam Value A value that may or may not be a power of two.
 */
template<std::size_t Value>
using is_power_of_two = std::bool_constant<Value && ((Value & (Value - 1)) == 0)>;


/**
 * @brief Helper variable template.
 * @tparam Value A value that may or may not be a power of two.
 */
template<std::size_t Value>
inline constexpr bool is_power_of_two_v = is_power_of_two<Value>::value;


/**
 * @brief Fast module utility function (powers of two only).
 * @tparam Value Compile-time page size, it must be a power of two.
 * @param value A value for which to calculate the modulus.
 * @return Remainder of division.
 */
template<std::size_t Value>
[[nodiscard]] constexpr std::size_t fast_mod(const std::size_t value) ENTT_NOEXCEPT {
    static_assert(is_power_of_two_v<Value>, "Value must be a power of two");
    return value & (Value - 1u);
}


}


#endif