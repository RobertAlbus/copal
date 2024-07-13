#include <concepts>
#include <type_traits>

#include "vector_stdx.hpp"
#include "num.hpp"
#include "sin_lut.hpp"

namespace copal {

template<std::floating_point F>
template<size_t terms>
stdx::native_simd<F> VectorStdx<F>::sin_taylor_terms(T xIn) {
  auto [x, sign] = VectorStdx<F>::angle_normalization_pi_over_2(xIn);
  stdx::native_simd<F> term = x;
  stdx::native_simd<F> sum  = term;
  
  constexpr size_t termsEnd = terms * 2;
  for (size_t m = 2; m <= termsEnd; m += 2) {
    term *= -x * x / static_cast<F>(m * (m + 1));
    sum += term;
  }
  return sum * sign;
}

template<std::floating_point F>
stdx::native_simd<F> VectorStdx<F>::sin_taylor(T xIn) {
  return VectorStdx<F>::sin_taylor_terms<5>(xIn);
}

template<std::floating_point F>
stdx::native_simd<F> VectorStdx<F>::sin_lookup(T xIn) {
  using Int = std::conditional_t<std::is_same_v<F, float>, int,
        std::conditional_t<std::is_same_v<F, double>, long int, long long>>;

  auto [x, sign] = VectorStdx<F>::angle_normalization_pi_over_2(xIn);

  stdx::native_simd<F>   index_f  = x / copal::num::pi_over_2<F> * F{lut::max_index};
  stdx::native_simd<Int> index_A  = stdx::static_simd_cast<stdx::native_simd<Int>>(index_f);
  stdx::native_simd<F>   index_Af = stdx::static_simd_cast<stdx::native_simd<Float>>(index_A);
  stdx::native_simd<Int> index_B  = ++index_A;

  stdx::native_simd_mask<Int> invalidIndex_i = index_A  >= lut::max_index_i;
  stdx::native_simd_mask<F>   invalidIndex   = index_Af >= lut::max_index_i;
  if (stdx::any_of(invalidIndex_i)) {
    stdx::where(invalidIndex,   index_f ) = F{lut::max_index_i};
    stdx::where(invalidIndex,   index_Af) = F{lut::max_index_i};
    stdx::where(invalidIndex_i, index_A ) = Int{lut::max_index_i};
    stdx::where(invalidIndex_i, index_B ) = Int{lut::max_index_i};
  }
  
  alignas(stdx::native_simd<F>)
    std::array<F, stdx::native_simd<F>::size()> val_A_array {};
  alignas(stdx::native_simd<F>)
    std::array<F, stdx::native_simd<F>::size()> val_B_array {};

  // gather/scatter not yet implemented in stdx::simd as of July 2024
  // https://github.com/VcDevel/std-simd/issues/4
  for (size_t i = 0; i < stdx::native_simd<F>::size(); ++i) {
    val_A_array[i] = lut::get<F>()[index_A[i]];
    val_B_array[i] = lut::get<F>()[index_B[i]];
  }

  stdx::native_simd<F> val_A(&val_A_array[0], stdx::vector_aligned);
  stdx::native_simd<F> val_B(&val_B_array[0], stdx::vector_aligned);

  T lerpAmount = index_f - index_Af;
  return lerp(val_A, val_B, lerpAmount);
}

template<std::floating_point F>
stdx::native_simd<F> VectorStdx<F>::sin_stdlib(const stdx::native_simd<F> xIn) {
  return stdx::sin(xIn);
}

template<std::floating_point F>
stdx::native_simd<F> VectorStdx<F>::fabs(const stdx::native_simd<F> x) {
  return stdx::fabs(x);
}

template<std::floating_point F>
stdx::native_simd<F> VectorStdx<F>::fmod(const stdx::native_simd<F> a, const stdx::native_simd<F> b) {
  stdx::native_simd<F> result = {stdx::fmod(a, b)};

  // always positive, unlike std::fmod
  stdx::native_simd_mask<F> lt_zero = result < 0;
  if(stdx::any_of(lt_zero)) {
    where(lt_zero, result) = result + b;
  }

  return result;
}

template<std::floating_point F>
stdx::native_simd<F> VectorStdx<F>::lerp(const stdx::native_simd<F> a, const stdx::native_simd<F> b, const stdx::native_simd<F> lerpFactor) {
  // No overload for std::lerp in stdx.
  stdx::native_simd<F> result;
  stdx::native_simd_mask<F> infinite = stdx::isinf<>(a) || stdx::isinf<>(b) || stdx::isinf<>(lerpFactor);

  // for uniform behaviour with infinity across copal::Stdlib, copal::Scalar, copal::VectorImpl, copal::VectorStdx
  if (stdx::any_of(infinite))
    stdx::where(infinite, result) = a*b*lerpFactor;

  stdx::where(!infinite, result) = (1 - lerpFactor) * a + lerpFactor * b;
  return result;
}

template<std::floating_point F>
stdx::native_simd<F> VectorStdx<F>::floor(const stdx::native_simd<F> a) {
  return stdx::floor<F>(a);
}

template<std::floating_point F>
std::pair<stdx::native_simd<F>, stdx::native_simd<F>>
VectorStdx<F>::angle_normalization_pi_over_2(stdx::native_simd<F> xIn) {
  constexpr F halfPi = num::pi_over_2<F>;
  constexpr F pi     = num::pi_x_1<F>;
  constexpr F twoPi  = num::pi_x_2<F>;

  stdx::native_simd<F> x(xIn);

  stdx::native_simd_mask<F> gt_2pi = x >= twoPi || x <= -twoPi;
  if(stdx::any_of(gt_2pi)) {
    where(gt_2pi, x) = VectorStdx<F>::fmod(x, twoPi);
  }

  stdx::native_simd_mask<F> lt_zero = x < 0;
  if(stdx::any_of(lt_zero)) {
    where(lt_zero, x) = x + twoPi;
  }

  stdx::native_simd<F> sign = 1;
  stdx::native_simd_mask<F> gte_pi = x >= pi;
  if(stdx::any_of(gte_pi)) {
    where(gte_pi, x) = x - pi;
    where(gte_pi, sign) = -1;
  }

  stdx::native_simd_mask<F> gt_halfPi = x >= halfPi;
  if(stdx::any_of(gt_halfPi)) {
    where(gt_halfPi, x) = pi - x;
  }

  return { x, sign };
}

template class VectorStdx<float>;
template class VectorStdx<double>;

}
