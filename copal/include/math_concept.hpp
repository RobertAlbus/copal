#pragma once

#include <concepts>
#include <experimental/bits/simd.h>
#include <concepts>
#include <type_traits>


#include "scalar/math.hpp"
#include "scalar/sin.hpp"

#include "stdlib/math.hpp"
#include "stdlib/sin.hpp"

#include "stdlib/sin_stdlib.hpp"
#include "vector_impl/math.hpp"
#include "vector_impl/sin.hpp"

#include "vector_stdx/math.hpp"
#include "vector_stdx/sin.hpp"

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

template<std::floating_point Type>
struct Scalar {
    using T     = Type;
    using Float = Type;

    static T sin_lookup(T x)              { return copal::scalar::sin_lookup(x);          }
    static T sin_taylor(T x)              { return copal::scalar::sin_taylor(x);          }
    static T sin_stdlib(T x)              { return copal::stdlib::sin_stdlib(x);          }
    static T fabs(T x)                    { return copal::scalar::fabs(x);                }
    static T fmod(T a, T b)               { return copal::scalar::fmod(a, b);             }
    static T lerp(T a, T b, T lerpFactor) { return copal::scalar::lerp(a, b, lerpFactor); }
    static T floor(T x)                   { return copal::scalar::floor(x);               }
    static std::pair<T, T> angle_normalization_pi_over_2(T x) { return copal::scalar::angle_normalization_pi_over_2(x); }
};

template<std::floating_point Type>
struct Stdlib {
    using T     = Type;
    using Float = Type;

    static T sin_lookup(T x)              { return copal::stdlib::sin_lookup(x);          }
    static T sin_taylor(T x)              { return copal::stdlib::sin_taylor(x);          }
    static T sin_stdlib(T x)              { return copal::stdlib::sin_stdlib(x);          }
    static T fabs(T x)                    { return copal::stdlib::fabs(x);                }
    static T fmod(T a, T b)               { return copal::stdlib::fmod(a, b);             }
    static T lerp(T a, T b, T lerpFactor) { return copal::stdlib::lerp(a, b, lerpFactor); }
    static T floor(T x)                   { return copal::stdlib::floor(x);               }
    static std::pair<T, T> angle_normalization_pi_over_2(T x) { return copal::scalar::angle_normalization_pi_over_2(x); }
};

template<std::floating_point Type>
struct VectorImpl {
    using T = typename stdx::native_simd<Type>;
    using Float = Type;

    static T sin_lookup(T x)              { return copal::vector_impl::sin_lookup(x);          }
    static T sin_taylor(T x)              { return copal::vector_impl::sin_taylor(x);          }
    static T sin_stdlib(T x)              { return copal::vector_stdx::sin_stdlib(x);          }
    static T fabs(T x)                    { return copal::vector_impl::fabs(x);                }
    static T fmod(T a, T b)               { return copal::vector_impl::fmod(a, b);             }
    static T lerp(T a, T b, T lerpFactor) { return copal::vector_impl::lerp(a, b, lerpFactor); }
    static T floor(T x)                   { return copal::vector_impl::floor(x);               }
    static std::pair<T, T> angle_normalization_pi_over_2(T x) { return copal::vector_impl::angle_normalization_pi_over_2(x); }
};

template<std::floating_point Type>
struct VectorStdx {
    using T = stdx::native_simd<Type>;
    using Float = Type;

    static T sin_lookup(T x)              { return copal::vector_stdx::sin_lookup(x);          }
    static T sin_taylor(T x)              { return copal::vector_stdx::sin_taylor(x);          }
    static T sin_stdlib(T x)              { return copal::vector_stdx::sin_stdlib(x);          }
    static T fabs(T x)                    { return copal::vector_stdx::fabs(x);                }
    static T fmod(T a, T b)               { return copal::vector_stdx::fmod(a, b);             }
    static T lerp(T a, T b, T lerpFactor) { return copal::vector_stdx::lerp(a, b, lerpFactor); }
    static T floor(T x)                   { return copal::vector_stdx::floor(x);               }
    static std::pair<T, T> angle_normalization_pi_over_2(T x)  { return copal::vector_stdx::angle_normalization_pi_over_2(x); }
};

}
