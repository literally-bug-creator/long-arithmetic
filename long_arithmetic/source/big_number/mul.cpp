#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "getters.hpp"

namespace big_number {
#define MULTIPLY_THRESHOLD 100

    BigNumber simple_mul( const BigNumber& multiplicand,
                          const BigNumber& multiplier ) {
        if ( is_zero( multiplicand ) || is_zero( multiplier ) )
            return make_zero();
        std::vector<chunk> chunks(
            get_size( multiplicand ) + get_size( multiplier ), 0 );
        chunk carry = 0;
        for ( int32_t i = 0; i < get_size( multiplicand ); ++i ) {
            carry = 0;
            for ( int32_t j = 0; j < get_size( multiplier ); ++j ) {
                mul_chunk l_chunk =
                    (mul_chunk)get_chunk_direct( multiplicand, i );
                mul_chunk r_chunk =
                    (mul_chunk)get_chunk_direct( multiplier, j );
                mul_chunk product = l_chunk * r_chunk + chunks[i + j] + carry;
                carry = product / CHUNK_BASE;
                chunks[i + j] = product % CHUNK_BASE;
            }
            chunks[i + get_size( multiplier )] = carry;
        }
        return from_scratch(
            chunks,
            get_exponent( multiplicand ) + get_exponent( multiplier ),
            is_negative( multiplicand ) != is_negative( multiplier ),
            get_error( multiplicand ) );
    }

//     BigNumber mul( const BigNumber& multiplicand,
//                    const BigNumber& multiplier ) {
//         if ( is_zero( multiplicand ) || is_zero( multiplier ) )
//             return make_zero();
//         if ( get_size( multiplicand ) <= MULTIPLY_THRESHOLD ||
//              get_size( multiplier ) <= MULTIPLY_THRESHOLD )
//             return simple_mul( multiplicand, multiplier );
//         size_t m =
//             std::max( get_size( multiplicand ), get_size( multiplier ) ) / 2;
//         BigNumber a_0, a_1, b_0, b_1;
//         if ( m >= get_size( multiplicand ) ) {
//             a_0 = make_zero();
//             a_1 = from_scratch( get_chunks( multiplicand ),
//                                 get_exponent( multiplicand ),
//                                 is_negative( multiplicand ),
//                                 get_error( multiplicand ) );
//         } else {
//             a_0 = from_iterator( get_chunks( multiplicand ).begin() + m,
//                                  get_chunks( multiplicand ).end() );
//             a_1 = from_iterator( get_chunks( multiplicand ).begin(),
//                                  get_chunks( multiplicand ).begin() + m );
//         }
//         if ( m >= get_size( multiplier ) ) {
//             b_0 = make_zero();
//             b_1 = from_scratch( get_chunks( multiplier ),
//                                 get_exponent( multiplier ),
//                                 is_negative( multiplier ),
//                                 get_error( multiplier ) );
//         } else {
//             b_0 = from_iterator( get_chunks( multiplier ).begin() + m,
//                                  get_chunks( multiplier ).end() );
//             b_1 = from_iterator( get_chunks( multiplier ).begin(),
//                                  get_chunks( multiplier ).begin() + m );
//         }
//         BigNumber z0 = mul( a_0, b_0 );
//         BigNumber z1 = mul( add( a_0, a_1 ), add( b_0, b_1 ) );
//         BigNumber z2 = mul( a_1, b_1 );
//         BigNumber z0_shifted = shift( z0, 2 * m );
//         BigNumber z1_shifted = shift( sub( sub( z1, z0 ), z2 ), m );
//         BigNumber result = add( add( z0_shifted, z1_shifted ), z2 );
//         bool is_neg = is_negative( multiplicand ) != is_negative( multiplier );
//         int32_t exp = get_exponent( multiplicand ) + get_exponent( multiplier );
//         return from_scratch(
//             get_chunks( result ), exp, is_neg, get_error( result ) );
//     }


//     // Параметры NTT
// static const uint32_t MOD = 998244353;             // 119 * 2^23 + 1
// static const uint32_t PRIMITIVE_ROOT = 3;

// // Быстрая экспонентация по модулю
// static uint32_t mod_pow(uint32_t a, uint32_t e) {
//     uint64_t res = 1, base = a;
//     while (e) {
//         if (e & 1) res = (res * base) % MOD;
//         base = (base * base) % MOD;
//         e >>= 1;
//     }
//     return (uint32_t)res;
// }

// // NTT: inplace преобразование
// static void ntt(std::vector<uint32_t>& a, bool invert) {
//     size_t n = a.size();
//     // битовая реверсия
//     for (size_t i = 1, j = 0; i < n; ++i) {
//         size_t bit = n >> 1;
//         for (; j & bit; bit >>= 1) j ^= bit;
//         j |= bit;
//         if (i < j) std::swap(a[i], a[j]);
//     }
    
//     for (size_t len = 2; len <= n; len <<= 1) {
//         uint32_t wlen = mod_pow(PRIMITIVE_ROOT, (MOD - 1) / len);
//         if (invert) wlen = mod_pow(wlen, MOD - 2);
//         for (size_t i = 0; i < n; i += len) {
//             uint32_t w = 1;
//             for (size_t j = 0; j < (len >> 1); ++j) {
//                 uint32_t u = a[i + j];
//                 uint32_t v = (uint64_t)a[i + j + (len >> 1)] * w % MOD;
//                 a[i + j] = u + v < MOD ? u + v : u + v - MOD;
//                 a[i + j + (len >> 1)] = u >= v ? u - v : u + MOD - v;
//                 w = (uint64_t)w * wlen % MOD;
//             }
//         }
//     }
//     if (invert) {
//         uint32_t inv_n = mod_pow(n, MOD - 2);
//         for (auto & x : a) x = (uint64_t)x * inv_n % MOD;
//     }
// }

// // Преобразовать BigNumber.chunks (base 1e18) -> вектор base 1e9
// static std::vector<uint32_t> to_base1e9(const std::vector<chunk>& c) {
//     std::vector<uint32_t> d;
//     d.reserve(c.size() * 2);
//     for (chunk x : c) {
//         uint32_t low = x % 1000000000ULL;
//         uint32_t high = x / 1000000000ULL;
//         d.push_back(low);
//         d.push_back(high);
//     }
//     return d;
// }

// // Преобразовать вектор base1e9 -> BigNumber.chunks base1e18
// static std::vector<chunk> from_base1e9(std::vector<uint32_t>& d) {
//     // carry по base1e9
//     uint64_t carry = 0;
//     for (size_t i = 0; i < d.size(); ++i) {
//         uint64_t val = (uint64_t)d[i] + carry;
//         d[i] = val % 1000000000;
//         carry = val / 1000000000;
//     }
//     // конвертация в base1e18
//     std::vector<chunk> c;
//     for (size_t i = 0; i < d.size(); i += 2) {
//         uint64_t low = d[i];
//         uint64_t high = (i + 1 < d.size() ? d[i+1] : 0);
//         chunk v = high * 1000000000ULL + low;
//         c.push_back(v);
//     }
//     while (!c.empty() && c.back() == 0) c.pop_back();
//     return c;
// }

// // Быстрое умножение через NTT
// BigNumber ntt_mul(const BigNumber &A, const BigNumber &B) {
//     if (is_zero(A) || is_zero(B)) return make_zero();
//     // Векторы коэффициентов
//     auto a = to_base1e9(get_chunks(A));
//     auto b = to_base1e9(get_chunks(B));
//     size_t n = 1;
//     while (n < a.size() + b.size()) n <<= 1;
//     a.resize(n); b.resize(n);
//     ntt(a, false);
//     ntt(b, false);
//     for (size_t i = 0; i < n; ++i) {
//         a[i] = (uint64_t)a[i] * b[i] % MOD;
//     }
//     ntt(a, true);
//     auto chunks = from_base1e9(a);
//     int32_t exp = get_exponent(A) + get_exponent(B);
//     bool sign = is_negative(A) != is_negative(B);
//     return from_scratch(chunks, exp, sign, get_error(A));
// }

// BigNumber mul(const BigNumber& A, const BigNumber& B) {
//     if (is_zero(A) || is_zero(B)) return make_zero();
//     if ((int)get_size(A) <= MULTIPLY_THRESHOLD || (int)get_size(B) <= MULTIPLY_THRESHOLD) {
//         return simple_mul(A, B);
//     }
//     return ntt_mul(A, B);
// }
// Четыре модуля вида k*2^m+1 для NTT с большим динамическим диапазоном
static const uint32_t MOD1 = 998244353;     // 119 * 2^23 + 1
static const uint32_t MOD2 = 1004535809;    // 479 * 2^21 + 1
static const uint32_t MOD3 = 469762049;     // 7 * 2^26 + 1
static const uint32_t MOD4 = 1224736769;    // 73 * 2^24 + 1
static const uint32_t ROOT = 3;

static uint32_t mod_pow(uint64_t a, uint64_t e, uint32_t mod) {
    uint64_t res = 1, base = a % mod;
    while (e) {
        if (e & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    return (uint32_t)res;
}

static void ntt_mod(std::vector<uint32_t>& a, bool invert, uint32_t mod) {
    size_t n = a.size();
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j |= bit;
        if (i < j) std::swap(a[i], a[j]);
    }
    for (size_t len = 2; len <= n; len <<= 1) {
        uint32_t wlen = mod_pow(ROOT, (mod - 1) / len, mod);
        if (invert) wlen = mod_pow(wlen, mod - 2, mod);
        for (size_t i = 0; i < n; i += len) {
            uint32_t w = 1;
            for (size_t j = 0; j < (len >> 1); ++j) {
                uint32_t u = a[i + j];
                uint32_t v = (uint64_t)a[i + j + (len >> 1)] * w % mod;
                a[i + j] = u + v < mod ? u + v : u + v - mod;
                a[i + j + (len >> 1)] = u >= v ? u - v : u + mod - v;
                w = (uint64_t)w * wlen % mod;
            }
        }
    }
    if (invert) {
        uint32_t inv_n = mod_pow(n, mod - 2, mod);
        for (auto & x : a) x = (uint64_t)x * inv_n % mod;
    }
}

// base1e18 -> base1e9
static std::vector<uint32_t> to_base1e9(const std::vector<chunk>& c) {
    std::vector<uint32_t> d;
    d.reserve(c.size() * 2);
    for (chunk x : c) {
        d.push_back(x % 1000000000ULL);
        d.push_back(x / 1000000000ULL);
    }
    return d;
}

// CRT-сборка из четырех модулей
static std::vector<chunk> from_ntt_crt4(
    const std::vector<uint32_t>& r1,
    const std::vector<uint32_t>& r2,
    const std::vector<uint32_t>& r3,
    const std::vector<uint32_t>& r4)
{
    size_t n = r1.size();
    
    const __int128 m1 = MOD1, m2 = MOD2, m3 = MOD3, m4 = MOD4;
    const __int128 m12 = m1 * m2;
    const __int128 m123 = m12 * m3;
    
    // Вычисление CRT-коэффициентов в base 1e9
    std::vector<__int128> coeff(n);
    uint64_t inv_m1_mod2 = mod_pow((uint64_t)(m1 % m2), (uint64_t)(m2 - 2), MOD2);
    uint64_t inv_m12_mod3 = mod_pow((uint64_t)(m12 % m3), (uint64_t)(m3 - 2), MOD3);
    uint64_t inv_m123_mod4 = mod_pow((uint64_t)(m123 % m4), (uint64_t)(m4 - 2), MOD4);
    for (size_t i = 0; i < n; ++i) {
        __int128 t = ((__int128)r2[i] - r1[i] + m2) % m2;
        t = t * inv_m1_mod2 % m2;
        __int128 x12 = r1[i] + t * m1;
        t = ((__int128)r3[i] - (x12 % m3) + m3) % m3;
        t = t * inv_m12_mod3 % m3;
        __int128 x123 = x12 + t * m12;
        t = ((__int128)r4[i] - (x123 % m4) + m4) % m4;
        t = t * inv_m123_mod4 % m4;
        coeff[i] = x123 + t * m123;
    }
    
    // Нормализация разрядов: собираем все цифры base 1e9
    std::vector<uint32_t> digits;
    digits.reserve(n + 1);
    __int128 carry = 0;
    for (size_t i = 0; i < n; ++i) {
        __int128 val = coeff[i] + carry;
        digits.push_back((uint32_t)(val % 1000000000));
        carry = val / 1000000000;
    }
    while (carry > 0) {
        digits.push_back((uint32_t)(carry % 1000000000));
        carry /= 1000000000;
    }

    // Формирование чанков base 1e18 (по две цифры base 1e9)
    std::vector<chunk> out;
    for (size_t i = 0; i < digits.size(); i += 2) {
        uint64_t low = digits[i];
        uint64_t high = (i + 1 < digits.size()) ? digits[i + 1] : 0;
        out.push_back((chunk)(high * 1000000000ULL + low));
    }
    
    // Удаление ведущих нулей
    while (!out.empty() && out.back() == 0) out.pop_back();
    return out;
}


BigNumber ntt_mul(const BigNumber &A, const BigNumber &B) {
    if (is_zero(A) || is_zero(B)) return make_zero();
    auto a = to_base1e9(get_chunks(A));
    auto b = to_base1e9(get_chunks(B));
    size_t n = 1;
    while (n < a.size() + b.size()) n <<= 1;
    a.resize(n); b.resize(n);
    // копии для четырех модулей
    auto a1 = a, b1 = b;
    auto a2 = a, b2 = b;
    auto a3 = a, b3 = b;
    auto a4 = a, b4 = b;
    ntt_mod(a1, false, MOD1); ntt_mod(b1, false, MOD1);
    ntt_mod(a2, false, MOD2); ntt_mod(b2, false, MOD2);
    ntt_mod(a3, false, MOD3); ntt_mod(b3, false, MOD3);
    ntt_mod(a4, false, MOD4); ntt_mod(b4, false, MOD4);
    for (size_t i = 0; i < n; ++i) {
        a1[i] = (uint64_t)a1[i] * b1[i] % MOD1;
        a2[i] = (uint64_t)a2[i] * b2[i] % MOD2;
        a3[i] = (uint64_t)a3[i] * b3[i] % MOD3;
        a4[i] = (uint64_t)a4[i] * b4[i] % MOD4;
    }
    ntt_mod(a1, true, MOD1);
    ntt_mod(a2, true, MOD2);
    ntt_mod(a3, true, MOD3);
    ntt_mod(a4, true, MOD4);
    auto chunks = from_ntt_crt4(a1, a2, a3, a4);
    int32_t exp = get_exponent(A) + get_exponent(B);
    bool sign = is_negative(A) != is_negative(B);
    return from_scratch(chunks, exp, sign, get_error(A));
}

BigNumber mul(const BigNumber& A, const BigNumber& B) {
    if (is_zero(A) || is_zero(B)) return make_zero();
    if ((int)get_size(A) <= MULTIPLY_THRESHOLD || (int)get_size(B) <= MULTIPLY_THRESHOLD) {
        return simple_mul(A, B);
    }
    return ntt_mul(A, B);
}
}
