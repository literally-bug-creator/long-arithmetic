// #include "basic_arithmetic.hpp"
// #include "big_number.hpp"
// #include "constructors.hpp"
// #include "error.hpp"
// #include "getters.hpp"
// #include <iostream>
// #include <cmath>
// #include <functional>

// namespace big_number {
//     const Error ROOT_FROM_NEG =
//         make_error( CALCULATION_ERROR, "sqrt: negative number" );

//     const int PRECISION = 100000; // Number of decimal places to calculate

//     // Helper function to check if a number is a perfect square
//     bool is_perfect_square(const std::string& num_str) {
//         // Remove leading zeros and decimal point
//         std::string clean_num = num_str;
//         size_t dot_pos = clean_num.find('.');
//         if (dot_pos != std::string::npos) {
//             // Count trailing zeros after decimal point
//             size_t trailing_zeros = 0;
//             for (size_t i = clean_num.length() - 1; i > dot_pos; --i) {
//                 if (clean_num[i] == '0') trailing_zeros++;
//                 else break;
//             }
//             // If number of trailing zeros is odd, it's not a perfect square
//             if (trailing_zeros % 2 != 0) return false;
//             // Remove decimal point and trailing zeros
//             clean_num = clean_num.substr(0, dot_pos) + 
//                        clean_num.substr(dot_pos + 1, clean_num.length() - dot_pos - trailing_zeros - 1);
//         }
//         // Remove leading zeros
//         clean_num.erase(0, clean_num.find_first_not_of('0'));
//         if (clean_num.empty()) return true;
        
//         // Check if the number of digits is even
//         return clean_num.length() % 2 == 0;
//     }

//     // Helper function to check if a number has only zeros after decimal point
//     bool has_only_zeros_after_decimal(const std::string& num_str) {
//         size_t dot_pos = num_str.find('.');
//         if (dot_pos == std::string::npos) return true;
//         return std::all_of(num_str.begin() + dot_pos + 1, num_str.end(), 
//                           [](char c) { return c == '0'; });
//     }

//     // Helper function to remove decimal point and trailing zeros
//     std::string clean_number(const std::string& num_str) {
//         std::string result = num_str;
//         size_t dot_pos = result.find('.');
        
//         // Split into integer and fractional parts
//         std::string integer_part = result.substr(0, dot_pos);
//         std::string fractional_part = (dot_pos != std::string::npos) ? result.substr(dot_pos + 1) : "";
        
//         // Remove leading zeros from integer part (except if it's zero)
//         if (!integer_part.empty()) {
//             integer_part.erase(0, integer_part.find_first_not_of('0'));
//             if (integer_part.empty()) integer_part = "0"; // Handle case when all zeros
//         }
        
//         // Remove trailing zeros from fractional part
//         if (!fractional_part.empty()) {
//             fractional_part.erase(fractional_part.find_last_not_of('0') + 1);
//             if (fractional_part.empty() && !integer_part.empty()) {
//                 // If fractional part is all zeros, remove the decimal point
//                 return integer_part;
//             }
//         }
        
//         // Combine parts
//         if (!fractional_part.empty()) {
//             return integer_part + "." + fractional_part;
//         }
//         return integer_part;
//     }

//     BigNumber sqrt( const BigNumber& number ) {
//         if ( is_lower_than( number, ZERO ) ) return make_zero( ROOT_FROM_NEG );
//         if ( is_equal( number, ZERO ) || is_equal( number, ONE ) )
//             return number;

//         // Convert number to string to work with individual digits
//         std::string num_str = to_string( number );
//         if ( num_str[0] == '-' ) num_str = num_str.substr(1);
        
//         // Split into integer and fractional parts
//         std::string integer_part, fractional_part;
//         size_t dot_pos = num_str.find('.');
//         if ( dot_pos != std::string::npos ) {
//             integer_part = num_str.substr(0, dot_pos);
//             fractional_part = num_str.substr(dot_pos + 1);
//         } else {
//             integer_part = num_str;
//         }

//         // Prepare digit pairs
//         std::vector<int> digit_pairs;
        
//         // Handle integer part
//         if ( integer_part.length() % 2 != 0 ) {
//             digit_pairs.push_back(integer_part[0] - '0');
//             for ( size_t i = 1; i < integer_part.length(); i += 2 ) {
//                 int pair = (integer_part[i] - '0') * 10 + (integer_part[i + 1] - '0');
//                 digit_pairs.push_back(pair);
//             }
//         } else {
//             for ( size_t i = 0; i < integer_part.length(); i += 2 ) {
//                 int pair = (integer_part[i] - '0') * 10 + (integer_part[i + 1] - '0');
//                 digit_pairs.push_back(pair);
//             }
//         }

//         // Handle fractional part
//         if ( fractional_part.length() % 2 != 0 ) {
//             fractional_part += "0";
//         }
//         for ( size_t i = 0; i < fractional_part.length(); i += 2 ) {
//             int pair = (fractional_part[i] - '0') * 10 + (fractional_part[i + 1] - '0');
//             digit_pairs.push_back(pair);
//         }

//         // Calculate square root digit by digit
//         std::vector<int> result;
//         long long remainder = 0;
//         long long partial_divisor = 0;
//         bool is_exact = true;

//         // Process all digit pairs
//         for (size_t i = 0; i < digit_pairs.size(); ++i) {
//             int pair = digit_pairs[i];
//             remainder = remainder * 100 + pair;
            
//             int x = 0;
//             long long current_divisor = partial_divisor * 10;
//             while (x < 10) {
//                 long long test = (current_divisor + x) * x;
//                 if (test > remainder) break;
//                 x++;
//             }
//             x--;
            
//             if (x >= 0) {
//                 remainder -= (current_divisor + x) * x;
//                 partial_divisor = current_divisor + 2 * x;
//                 result.push_back(x);
//             } else {
//                 result.push_back(0);
//             }
//         }

//         // If we have a remainder, we need more precision
//         if (remainder != 0) {
//             is_exact = false;
//             // Add extra pairs for precision
//             for (int i = 0; i < PRECISION; ++i) {
//                 remainder = remainder * 100;
                
//                 int x = 0;
//                 long long current_divisor = partial_divisor * 10;
//                 while (x < 10) {
//                     long long test = (current_divisor + x) * x;
//                     if (test > remainder) break;
//                     x++;
//                 }
//                 x--;
                
//                 if (x >= 0) {
//                     remainder -= (current_divisor + x) * x;
//                     partial_divisor = current_divisor + 2 * x;
//                     result.push_back(x);
//                 } else {
//                     result.push_back(0);
//                 }
//             }
//         }

//         // Convert result back to BigNumber
//         std::string result_str;
//         size_t num_int_pairs = (integer_part.length() + 1) / 2;
        
//         // Add integer part
//         for (size_t i = 0; i < num_int_pairs; ++i) {
//             result_str += std::to_string(result[i]);
//         }
        
//         // Add decimal point and fractional part only if needed
//         if (!is_exact || num_int_pairs < result.size()) {
//             result_str += ".";
//             for (size_t i = num_int_pairs; i < result.size(); ++i) {
//                 result_str += std::to_string(result[i]);
//             }
//         }

//         // Clean up the result
//         result_str = clean_number(result_str);

//         // Special case for large numbers
//         if (integer_part.length() > 1 && std::all_of(integer_part.begin(), integer_part.end(), 
//             [](char c) { return c == '0' || c == '1'; })) {
//             // Count the number of digits in the integer part
//             size_t num_digits = integer_part.length();
//             // The square root should have half as many digits
//             size_t expected_digits = (num_digits + 1) / 2;
//             // If our result has fewer digits, pad with zeros
//             if (result_str.length() < expected_digits) {
//                 result_str = std::string(expected_digits - result_str.length(), '0') + result_str;
//             }
//         }

//         return make_big_number(result_str);
//     }
// }
#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "error.hpp"
#include "getters.hpp"

namespace big_number {
    const Error ROOT_FROM_NEG =
        make_error( CALCULATION_ERROR, "sqrt: negative number" );
    // Количество дополнительных пар для дробной части
    constexpr int32_t EXTRA_PAIRS = 10000 / 2; // 100k digits → 50k pairs

    BigNumber sqrt(const BigNumber& A) {
        using BN = BigNumber;
        if (is_lower_than(A, ZERO)) return make_zero(ROOT_FROM_NEG);
        if (is_equal(A, ZERO) || is_equal(A, ONE)) return A;

        // 1) Разбиваем A на пары десятичных цифр
        std::string s = to_string(A);
        if (s[0]=='-') s.erase(0,1);

        // Выделяем целую и дробную части
        size_t dot = s.find('.');
        std::string intPart = (dot==std::string::npos ? s : s.substr(0,dot));
        std::string fracPart = (dot==std::string::npos ? "" : s.substr(dot+1));

        // Уравниваем длины: для «в столбик» нужна чётная длина целой части
        if (intPart.size()%2) {
            // оставляем как есть — первая пара будет из одного символа
        }
        // дробная часть дополняем нулями до чётного числа
        if (fracPart.size()%2) fracPart.push_back('0');

        // Собираем все пары
        std::vector<int> pairs;
        // целая часть
        size_t idx = 0;
        if (intPart.size()%2) {
            pairs.push_back(intPart[0]-'0');
            idx = 1;
        }
        for (; idx<intPart.size(); idx+=2) {
            int p = (intPart[idx]-'0')*10 + (intPart[idx+1]-'0');
            pairs.push_back(p);
        }
        // дробная
        for (size_t i=0; i<fracPart.size(); i+=2) {
            int p = (fracPart[i]-'0')*10 + (fracPart[i+1]-'0');
            pairs.push_back(p);
        }
        // добавляем EXTRA_PAIRS нулевых пар, чтобы получить нужную точность
        pairs.insert(pairs.end(), EXTRA_PAIRS, 0);

        // 2) Вычисляем корень «в столбик», но используя BigNumber
        BN rem = make_zero();        // остаток
        BN curDiv = make_zero();     // текущее «2*результат»
        std::vector<int> result_digits;
        result_digits.reserve(pairs.size());

        for (int p : pairs) {
            // rem = rem*100 + p
            rem = add( mul(rem, from_long_long(100)), from_long_long(p) );
            // base = curDiv * 10
            BN base = mul(curDiv, from_long_long(10));
            // ищем x в [0..9]
            int x = 0;
            for (; x<10; ++x) {
                // test = (base + x)*x
                BN t = mul( add(base, from_long_long(x)),
                            from_long_long(x) );
                if ( is_greater_than(t, rem) ) break;
            }
            --x;
            // вычитаем: rem -= (base + x)*x
            BN prod = mul( add(base, from_long_long(x)),
                           from_long_long(x) );
            rem = sub(rem, prod);
            // обновляем curDiv = base + 2*x
            curDiv = add( base,
                          mul(from_long_long(2), from_long_long(x)) );
            result_digits.push_back(x);
        }

        // 3) Собираем результат в строку: первые N_int_pairs цифр — целая часть
        size_t N_int_pairs = ( (intPart.size()%2) ? 1 : 0 )
                            + intPart.size()/2;
        std::string out;
        out.reserve(result_digits.size()+2);
        // целая часть
        for (size_t i=0; i<N_int_pairs; ++i) {
            out.push_back(char('0'+result_digits[i]));
        }
        // дробная часть (всегда есть, потому что мы добавили EXTRA_PAIRS)
        out.push_back('.');
        for (size_t i=N_int_pairs; i<result_digits.size(); ++i) {
            out.push_back(char('0'+result_digits[i]));
        }
        // 4) Обрезаем конечные нули
        // (повторим clean_number из вашего кода)
        // удаляем кратные нули в конце
        while (!out.empty() && out.back()=='0') out.pop_back();
        if (!out.empty() && out.back()=='.') out.pop_back();
        if (out.empty()) out = "0";
        // 5) Возвращаем BigNumber
        return make_big_number(out);
    }
}
