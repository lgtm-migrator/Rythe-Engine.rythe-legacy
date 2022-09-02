#include <core/math/interpolation/quintic.hpp>
#pragma once

namespace legion::core::math::detail
{
    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto _quintic_impl_(T&& v) noexcept
    {
        using scalar = remove_cvr_t<T>;
        return v * v * v * (v * (v * static_cast<scalar>(6) - static_cast<scalar>(15)) + static_cast<scalar>(10));
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto _quintic_derivative_impl_(T&& v) noexcept
    {
        using scalar = remove_cvr_t<T>;
        return static_cast<scalar>(30) * v * v * (v * (v - static_cast<scalar>(2)) + static_cast<scalar>(1));
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE auto _inverse_quintic_impl_(T&& v) noexcept
    {
        using scalar = remove_cvr_t<T>;
        return static_cast<scalar>(0.5) - sin(asin(static_cast<scalar>(1) - static_cast<scalar>(2) * v) / static_cast<scalar>(3));
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE auto _inverse_quintic_derivative_impl_(T&& v) noexcept
    {
        using scalar = remove_cvr_t<T>;
        auto denom = static_cast<scalar>(3) / sqrt(-(v - 1) * v);
        return cos(asin(static_cast<scalar>(2)*v-static_cast<scalar>(1))/static_cast<scalar>(3)) / denom;
    }
}

#include <core/math/interpolation/quintic_vector.inl>
//#include <core/math/interpolation/quintic_matrix.inl>
//#include <core/math/interpolation/quintic_quaternion.inl>

namespace legion::core::math
{
    // quintic
    template<typename TypeMin, typename TypeMax, typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto quintic(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
    {
        using A = remove_cvr_t<TypeMin>;
        using B = remove_cvr_t<TypeMax>;
        using T = remove_cvr_t<ValueType>;

        if constexpr (is_quat_v<A> && is_quat_v<B> && is_quat_v<T>)
        {
            using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
            return detail::compute_quintic<quaternion<scalar>>::compute(::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax), ::std::forward<ValueType>(value));
        }
        if constexpr (is_matrix_v<A> && is_matrix_v<B> && is_matrix_v<T>)
        {
            using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
            return detail::compute_quintic<matrix<scalar, A::row_count, A::col_count>>::compute(::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax), ::std::forward<ValueType>(value));
        }
        if constexpr (is_vector_v<A> && is_vector_v<B> && is_vector_v<T>)
        {
            using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
            constexpr size_type size = min(A::size, min(B::size, T::size))));
            return detail::compute_quintic<vector<scalar, size>>::compute(::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax), ::std::forward<ValueType>(value));
        }
        else
        {
            return detail::_quintic_impl_(saturate(map01(::std::forward<ValueType>(value), ::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax))));
        }
    }

    // quintic
    template<typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto quintic(ValueType&& value) noexcept
    {
        using T = remove_cvr_t<ValueType>;

        if constexpr (is_quat_v<T>)
        {
            return detail::compute_quintic<quaternion<T::scalar>>::compute(::std::forward<ValueType>(value));
        }
        if constexpr (is_matrix_v<T>)
        {
            return detail::compute_quintic<matrix<T::scalar, T::row_count, T::col_count>>::compute(::std::forward<ValueType>(value));
        }
        if constexpr (is_vector_v<T>)
        {
            return detail::compute_quintic<vector<T::scalar, T::size>>::compute(::std::forward<ValueType>(value));
        }
        else
        {
            return detail::_quintic_impl_(::std::forward<ValueType>(value));
        }
    }

    // quintic_derivative
    template<typename TypeMin, typename TypeMax, typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto quintic_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
    {
        using A = remove_cvr_t<TypeMin>;
        using B = remove_cvr_t<TypeMax>;
        using T = remove_cvr_t<ValueType>;

        if constexpr (is_quat_v<A> && is_quat_v<B> && is_quat_v<T>)
        {
            using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
            return detail::compute_quintic<quaternion<scalar>>::compute_derivative(::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax), ::std::forward<ValueType>(value));
        }
        if constexpr (is_matrix_v<A> && is_matrix_v<B> && is_matrix_v<T>)
        {
            using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
            return detail::compute_quintic<matrix<scalar, A::row_count, A::col_count>>::compute_derivative(::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax), ::std::forward<ValueType>(value));
        }
        if constexpr (is_vector_v<A> && is_vector_v<B> && is_vector_v<T>)
        {
            using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
            constexpr size_type size = min(A::size, min(B::size, T::size))));
            return detail::compute_quintic<vector<scalar, size>>::compute_derivative(::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax), ::std::forward<ValueType>(value));
        }
        else
        {
            return detail::_quintic_derivative_impl_(saturate(map01(::std::forward<ValueType>(value), ::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax))));
        }
    }

    // quintic_derivative
    template<typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto quintic_derivative(ValueType&& value) noexcept
    {
        using T = remove_cvr_t<ValueType>;

        if constexpr (is_quat_v<T>)
        {
            return detail::compute_quintic<quaternion<T::scalar>>::compute_derivative(::std::forward<ValueType>(value));
        }
        if constexpr (is_matrix_v<T>)
        {
            return detail::compute_quintic<matrix<T::scalar, T::row_count, T::col_count>>::compute_derivative(::std::forward<ValueType>(value));
        }
        if constexpr (is_vector_v<T>)
        {
            return detail::compute_quintic<vector<T::scalar, T::size>>::compute_derivative(::std::forward<ValueType>(value));
        }
        else
        {
            return detail::_quintic_derivative_impl_(::std::forward<ValueType>(value));
        }
    }

    // inverse_quintic
    template<typename TypeMin, typename TypeMax, typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE auto inverse_quintic(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
    {
        using A = remove_cvr_t<TypeMin>;
        using B = remove_cvr_t<TypeMax>;
        using T = remove_cvr_t<ValueType>;

        if constexpr (is_quat_v<A> && is_quat_v<B> && is_quat_v<T>)
        {
            using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
            return detail::compute_quintic<quaternion<scalar>>::compute_inverse(::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax), ::std::forward<ValueType>(value));
        }
        if constexpr (is_matrix_v<A> && is_matrix_v<B> && is_matrix_v<T>)
        {
            using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
            return detail::compute_quintic<matrix<scalar, A::row_count, A::col_count>>::compute_inverse(::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax), ::std::forward<ValueType>(value));
        }
        if constexpr (is_vector_v<A> && is_vector_v<B> && is_vector_v<T>)
        {
            using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
            constexpr size_type size = min(A::size, min(B::size, T::size))));
            return detail::compute_quintic<vector<scalar, size>>::compute_inverse(::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax), ::std::forward<ValueType>(value));
        }
        else
        {
            return detail::_inverse_quintic_impl_(saturate(map01(::std::forward<ValueType>(value), ::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax))));
        }
    }

    // inverse_quintic
    template<typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE auto inverse_quintic(ValueType&& value) noexcept
    {
        using T = remove_cvr_t<ValueType>;

        if constexpr (is_quat_v<T>)
        {
            return detail::compute_quintic<quaternion<T::scalar>>::compute_inverse(::std::forward<ValueType>(value));
        }
        if constexpr (is_matrix_v<T>)
        {
            return detail::compute_quintic<matrix<T::scalar, T::row_count, T::col_count>>::compute_inverse(::std::forward<ValueType>(value));
        }
        if constexpr (is_vector_v<T>)
        {
            return detail::compute_quintic<vector<T::scalar, T::size>>::compute_inverse(::std::forward<ValueType>(value));
        }
        else
        {
            return detail::_inverse_quintic_impl_(::std::forward<ValueType>(value));
        }
    }

    // inverse_quintic_derivative
    template<typename TypeMin, typename TypeMax, typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE auto inverse_quintic_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
    {
        using A = remove_cvr_t<TypeMin>;
        using B = remove_cvr_t<TypeMax>;
        using T = remove_cvr_t<ValueType>;

        if constexpr (is_quat_v<A> && is_quat_v<B> && is_quat_v<T>)
        {
            using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
            return detail::compute_quintic<quaternion<scalar>>::compute_inverse_derivative(::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax), ::std::forward<ValueType>(value));
        }
        if constexpr (is_matrix_v<A> && is_matrix_v<B> && is_matrix_v<T>)
        {
            using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
            return detail::compute_quintic<matrix<scalar, A::row_count, A::col_count>>::compute_inverse_derivative(::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax), ::std::forward<ValueType>(value));
        }
        if constexpr (is_vector_v<A> && is_vector_v<B> && is_vector_v<T>)
        {
            using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
            constexpr size_type size = min(A::size, min(B::size, T::size))));
            return detail::compute_quintic<vector<scalar, size>>::compute_inverse_derivative(::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax), ::std::forward<ValueType>(value));
        }
        else
        {
            return detail::_inverse_quintic_derivative_impl_(saturate(map01(::std::forward<ValueType>(value), ::std::forward<TypeMin>(edgeMin), ::std::forward<TypeMax>(edgeMax))));
        }
    }

    // inverse_quintic_derivative
    template<typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE auto inverse_quintic_derivative(ValueType&& value) noexcept
    {
        using T = remove_cvr_t<ValueType>;

        if constexpr (is_quat_v<T>)
        {
            return detail::compute_quintic<quaternion<T::scalar>>::compute_inverse_derivative(::std::forward<ValueType>(value));
        }
        if constexpr (is_matrix_v<T>)
        {
            return detail::compute_quintic<matrix<T::scalar, T::row_count, T::col_count>>::compute_inverse_derivative(::std::forward<ValueType>(value));
        }
        if constexpr (is_vector_v<T>)
        {
            return detail::compute_quintic<vector<T::scalar, T::size>>::compute_inverse_derivative(::std::forward<ValueType>(value));
        }
        else
        {
            return detail::_inverse_quintic_derivative_impl_(::std::forward<ValueType>(value));
        }
    }
}
