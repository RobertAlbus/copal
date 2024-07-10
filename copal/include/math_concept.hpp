#pragma once

#include <concepts>
#include <experimental/bits/simd.h>
#include <type_traits>

#include "stdx_definition.hpp"

namespace copal {

template<typename Impl>
concept CopalMath = requires {
    typename Impl::T;
    requires std::is_floating_point_v<typename Impl::T> || stdx::is_simd_v<typename Impl::T>;
    typename Impl::Float;
    requires std::is_floating_point_v<typename Impl::Float>;
    requires requires(Impl::T a, Impl::T b, Impl::T lerpFactor) {
        { Impl::sin_lookup(a)          } -> std::same_as<typename Impl::T>;
        { Impl::sin_taylor(a)          } -> std::same_as<typename Impl::T>;
        { Impl::sin_stdlib(a)          } -> std::same_as<typename Impl::T>;
        { Impl::fmod(a, b)             } -> std::same_as<typename Impl::T>;
        { Impl::lerp(a, b, lerpFactor) } -> std::same_as<typename Impl::T>;
        { Impl::floor(a)               } -> std::same_as<typename Impl::T>;
        { Impl::angle_normalization_pi_over_2(a) } -> std::same_as<std::pair<typename Impl::T, typename Impl::T>>;
    };
};

}
