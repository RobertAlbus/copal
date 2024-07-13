#include <concepts>
#include <experimental/bits/simd.h>
#include <type_traits>

#include "vector_impl.hpp"
#include "vector_stdx.hpp"
#include "num.hpp"
#include "sin_lut.hpp"

namespace copal {

template<std::floating_point F>
template<size_t terms>
stdx::native_simd<F> VectorImpl<F>::sin_taylor_terms(stdx::native_simd<F> xIn) {
  auto [x, sign] = VectorImpl<F>::angle_normalization_pi_over_2(xIn);
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
stdx::native_simd<F> VectorImpl<F>::sin_taylor(stdx::native_simd<F> xIn) {
  return VectorImpl<F>::sin_taylor_terms<5>(xIn);
}

template<std::floating_point F>
stdx::native_simd<F> VectorImpl<F>::sin_lookup(stdx::native_simd<F> xIn) {
  using Int = std::conditional_t<std::is_same_v<F, float>, int,
        std::conditional_t<std::is_same_v<F, double>, long int, long long>>;

  auto [x, sign] = VectorImpl<F>::angle_normalization_pi_over_2(xIn);

  stdx::native_simd<F>   index_f  = x / copal::num::pi_over_2<F> * F{lut::max_index};
  stdx::native_simd<Int> index_A  = stdx::static_simd_cast<stdx::native_simd<Int>>(index_f);
  stdx::native_simd<F>   index_Af = stdx::static_simd_cast<stdx::native_simd<F>>(index_A);
  stdx::native_simd<Int> index_B  = ++index_A;

  stdx::native_simd_mask<Int> invalidIndex_i = index_A  >= lut::max_index_i;
  stdx::native_simd_mask<F>   invalidIndex   = index_Af >= lut::max_index_i;
  if (stdx::any_of(invalidIndex_i)) {
    stdx::where(invalidIndex,   index_f ) = F{lut::max_index_i};
    stdx::where(invalidIndex,   index_Af) = F{lut::max_index_i};
    stdx::where(invalidIndex_i, index_A ) = F{lut::max_index_i};
    stdx::where(invalidIndex_i, index_B ) = F{lut::max_index_i};
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
stdx::native_simd<F> VectorImpl<F>::sin_stdlib(stdx::native_simd<F> xIn) {
  return stdx::sin(xIn);
}

template<std::floating_point F>
stdx::native_simd<F> VectorImpl<F>::fabs(stdx::native_simd<F> data) {
  stdx::native_simd<F> result = data;
  stdx::native_simd_mask<F> lt_zero = data < 0;
  if (stdx::any_of(lt_zero)) {
    stdx::where(lt_zero, result) = -data;
  }
  return result;
}

template<std::floating_point F>
stdx::native_simd<F> VectorImpl<F>::floor(stdx::native_simd<F> x) {

  // recursive dependency between VectorImpl::floor and VectorImpl::fmod.
  // defer to VectorStdx 
  return VectorStdx<F>::floor(x);
}

template<std::floating_point F>
stdx::native_simd<F> VectorImpl<F>::fmod(stdx::native_simd<F> a, stdx::native_simd<F> b) {
  stdx::native_simd<F> sign = 1;
  stdx::native_simd_mask<F> a_lt_zero = a < 0;
  if (stdx::any_of(a_lt_zero)) {
    stdx::where(a_lt_zero, sign) = -1;
  }

  // always positive, unlike std::fmod
  auto a_abs = VectorImpl<F>::fabs(a);
  auto b_abs = VectorImpl<F>::fabs(b);

  stdx::native_simd<F> result {a};

  stdx::native_simd_mask<F> a_eq_b = a_abs == b_abs;
  if (stdx::any_of(a_eq_b)) {
    stdx::where(a_eq_b, result) = 0;
  }

  stdx::native_simd_mask<F> a_gt_b = a_abs > b_abs;
  if (stdx::any_of(a_gt_b)) {
    stdx::native_simd<F> fitTimes = a_abs / b_abs;
    stdx::native_simd<F> remainder = fitTimes - VectorImpl<F>::floor(fitTimes);
    stdx::where(a_gt_b, result) = remainder * b_abs;
  }
  return result;
}

template<std::floating_point F>
stdx::native_simd<F> VectorImpl<F>::lerp(stdx::native_simd<F> a, stdx::native_simd<F> b, stdx::native_simd<F> lerpFactor) {
  stdx::native_simd<F> result;
  stdx::native_simd_mask<F> infinite = stdx::isinf<>(a) || stdx::isinf<>(b) || stdx::isinf<>(lerpFactor);

  // for uniform behaviour with infinity across copal::Stdlib, copal::Scalar, copal::VectorImpl, copal::VectorStdx
  if (stdx::any_of(infinite))
    stdx::where(infinite, result) = a*b*lerpFactor;

  stdx::where(!infinite, result) = (1 - lerpFactor) * a + lerpFactor * b;
  return result;
}

template<std::floating_point F>
std::pair<stdx::native_simd<F>, stdx::native_simd<F>>
VectorImpl<F>::angle_normalization_pi_over_2(stdx::native_simd<F> xIn) {
  constexpr F halfPi = num::pi_over_2<F>;
  constexpr F pi     = num::pi_x_1<F>;
  constexpr F twoPi  = num::pi_x_2<F>;

  stdx::native_simd<F> x(xIn);

  stdx::native_simd_mask<F> gt_2pi = x >= twoPi || x <= -twoPi;
  if(stdx::any_of(gt_2pi)) {
    where(gt_2pi, x) = VectorImpl<F>::fmod(x, twoPi);
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

template class VectorImpl<float>;
template class VectorImpl<double>;

}