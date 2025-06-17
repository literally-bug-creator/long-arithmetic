#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "error.hpp"
#include "getters.hpp"
#include <iostream>
#include <cmath>
#include <functional>
#include <flint/fmpz.h>

#include <flint/flint.h>
namespace big_number {
    const Error ROOT_FROM_NEG =
        make_error( CALCULATION_ERROR, "sqrt: negative number" );

    const int PRECISION = 100000; // Number of decimal places to calculate

    // Helper function to check if a number is a perfect square
    bool is_perfect_square(const std::string& num_str) {
        // Реализация без изменений
        std::string clean_num = num_str;
        size_t dot_pos = clean_num.find('.');
        if (dot_pos != std::string::npos) {
            size_t trailing_zeros = 0;
            for (size_t i = clean_num.length() - 1; i > dot_pos; --i) {
                if (clean_num[i] == '0') trailing_zeros++;
                else break;
            }
            if (trailing_zeros % 2 != 0) return false;
            clean_num = clean_num.substr(0, dot_pos) + 
                       clean_num.substr(dot_pos + 1, clean_num.length() - dot_pos - trailing_zeros - 1);
        }
        clean_num.erase(0, clean_num.find_first_not_of('0'));
        if (clean_num.empty()) return true;
        return clean_num.length() % 2 == 0;
    }

    // Helper function to check if a number has only zeros after decimal point
    bool has_only_zeros_after_decimal(const std::string& num_str) {
        size_t dot_pos = num_str.find('.');
        if (dot_pos == std::string::npos) return true;
        return std::all_of(num_str.begin() + dot_pos + 1, num_str.end(), 
                          [](char c) { return c == '0'; });
    }

    // Helper function to remove decimal point and trailing zeros
    std::string clean_number(const std::string& num_str) {
        // Реализация без изменений
        std::string result = num_str;
        size_t dot_pos = result.find('.');
        
        std::string integer_part = result.substr(0, dot_pos);
        std::string fractional_part = (dot_pos != std::string::npos) ? result.substr(dot_pos + 1) : "";
        
        if (!integer_part.empty()) {
            integer_part.erase(0, integer_part.find_first_not_of('0'));
            if (integer_part.empty()) integer_part = "0";
        }
        
        if (!fractional_part.empty()) {
            fractional_part.erase(fractional_part.find_last_not_of('0') + 1);
            if (fractional_part.empty() && !integer_part.empty()) {
                return integer_part;
            }
        }
        
        if (!fractional_part.empty()) {
            return integer_part + "." + fractional_part;
        }
        return integer_part;
    }

 BigNumber sqrt(const BigNumber& number) {
        if (is_lower_than(number, ZERO)) return make_zero(ROOT_FROM_NEG);
        if (is_equal(number, ZERO) || is_equal(number, ONE)) return number;

        // Convert to string and strip sign
        std::string num_str = to_string(number);
        if (!num_str.empty() && num_str[0] == '-') num_str.erase(0, 1);

        // Split into integer and fractional parts
        std::string integer_part, fractional_part;
        size_t dot_pos = num_str.find('.');
        if (dot_pos != std::string::npos) {
            integer_part = num_str.substr(0, dot_pos);
            fractional_part = num_str.substr(dot_pos + 1);
        } else {
            integer_part = num_str;
        }

        // Prepare digit pairs
        std::vector<int> digit_pairs;
        if (integer_part.size() % 2 != 0) {
            digit_pairs.push_back(integer_part[0] - '0');
            for (size_t i = 1; i < integer_part.size(); i += 2) {
                digit_pairs.push_back((integer_part[i] - '0') * 10 + (integer_part[i+1] - '0'));
            }
        } else {
            for (size_t i = 0; i < integer_part.size(); i += 2) {
                digit_pairs.push_back((integer_part[i] - '0') * 10 + (integer_part[i+1] - '0'));
            }
        }
        if (fractional_part.size() % 2 != 0) fractional_part.push_back('0');
        for (size_t i = 0; i < fractional_part.size(); i += 2) {
            digit_pairs.push_back((fractional_part[i] - '0') * 10 + (fractional_part[i+1] - '0'));
        }

        // FLINT raw variables for maximum performance
        fmpz_t rem, divr, temp, test_val, M10, M100;
        fmpz_init(rem); fmpz_zero(rem);
        fmpz_init(divr); fmpz_zero(divr);
        fmpz_init(temp);
        fmpz_init(test_val);
        fmpz_init(M10); fmpz_set_ui(M10, 10);
        fmpz_init(M100); fmpz_set_ui(M100, 100);

        std::vector<int> result;
        result.reserve(digit_pairs.size() + PRECISION + 10);
        bool is_exact = true;

        // Precompute constants
        fmpz_t const_2;
        fmpz_init(const_2); fmpz_set_ui(const_2, 2);

        // Main digit-by-digit extraction with binary search optimization
        for (int pair : digit_pairs) {
            // rem = rem * 100 + pair
            fmpz_mul(rem, rem, M100);
            fmpz_add_ui(rem, rem, pair);
            
            // divr = divr * 10
            fmpz_mul(divr, divr, M10);

            // Binary search for x in [0, 9]
            int low = 0, high = 9;
            int x = 0;
            while (low <= high) {
                int mid = (low + high) / 2;
                
                // test_val = (divr + mid) * mid
                fmpz_set_ui(temp, mid);
                fmpz_add(test_val, divr, temp);
                fmpz_mul_ui(test_val, test_val, mid);
                
                if (fmpz_cmp(test_val, rem) <= 0) {
                    x = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
            
            // Compute final test value
            fmpz_set_ui(temp, x);
            fmpz_add(test_val, divr, temp);
            fmpz_mul_ui(test_val, test_val, x);
            
            // Update remainder and divisor
            fmpz_sub(rem, rem, test_val);
            
            // divr = divr + 2*x
            fmpz_set_ui(temp, 2 * x);
            fmpz_add(divr, divr, temp);
            
            result.push_back(x);
        }

        // Handle fractional precision
        if (!fmpz_is_zero(rem)) {
            is_exact = false;
            for (int i = 0; i < PRECISION; ++i) {
                fmpz_mul(rem, rem, M100);
                fmpz_mul(divr, divr, M10);
                
                // Binary search for x
                int low = 0, high = 9;
                int x = 0;
                while (low <= high) {
                    int mid = (low + high) / 2;
                    
                    fmpz_set_ui(temp, mid);
                    fmpz_add(test_val, divr, temp);
                    fmpz_mul_ui(test_val, test_val, mid);
                    
                    if (fmpz_cmp(test_val, rem) <= 0) {
                        x = mid;
                        low = mid + 1;
                    } else {
                        high = mid - 1;
                    }
                }
                
                fmpz_set_ui(temp, x);
                fmpz_add(test_val, divr, temp);
                fmpz_mul_ui(test_val, test_val, x);
                fmpz_sub(rem, rem, test_val);
                
                fmpz_set_ui(temp, 2 * x);
                fmpz_add(divr, divr, temp);
                
                result.push_back(x);
            }
        }

        // Clean up FLINT variables
        fmpz_clear(rem);
        fmpz_clear(divr);
        fmpz_clear(temp);
        fmpz_clear(test_val);
        fmpz_clear(M10);
        fmpz_clear(M100);
        fmpz_clear(const_2);

        // Build result string
        std::string result_str;
        size_t num_int_pairs = (integer_part.length() + 1) / 2;
        result_str.reserve(result.size() + 2); // +2 for '.' and sign if needed

        for (size_t i = 0; i < num_int_pairs; ++i)
            result_str += '0' + result[i];

        if (!is_exact || result.size() > num_int_pairs) {
            result_str += '.';
            for (size_t i = num_int_pairs; i < result.size(); ++i)
                result_str += '0' + result[i];
        }

        result_str = clean_number(result_str);
        return make_big_number(result_str);
    }
}