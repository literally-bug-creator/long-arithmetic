#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "error.hpp"
#include "getters.hpp"
#include <iostream>
#include <cmath>
#include <functional>
#include "core.hpp"

namespace big_number {
    const Error ROOT_FROM_NEG =
        make_error( CALCULATION_ERROR, "sqrt: negative number" );

    std::string clean_number(const std::string& num_str) {
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

        std::string num_str = to_string(number);
        if (!num_str.empty() && num_str[0] == '-') num_str.erase(0, 1);

        std::string integer_part, fractional_part;
        size_t dot_pos = num_str.find('.');
        if (dot_pos != std::string::npos) {
            integer_part = num_str.substr(0, dot_pos);
            fractional_part = num_str.substr(dot_pos + 1);
        } else {
            integer_part = num_str;
        }

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

        apa::bint rem = 0;
        apa::bint divr = 0;
        apa::bint temp;
        apa::bint test_val;
        const apa::bint M10 = 10;
        const apa::bint M100 = 100;
        const apa::bint TWO = 2;

        std::vector<int> result;
        result.reserve(digit_pairs.size() + 100000 + 10);
        bool is_exact = true;
        for (int pair : digit_pairs) {
            rem = rem * M100 + pair;
            divr = divr * M10;

            int low = 0, high = 9;
            int x = 0;
            while (low <= high) {
                int mid = (low + high) / 2;
                temp = mid;
                test_val = (divr + temp) * mid;
                if (test_val <= rem) {
                    x = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
            temp = x;
            test_val = (divr + temp) * x;
            rem = rem - test_val;
            divr = divr + TWO * x;
            result.push_back(x);
        }
        size_t PRECISION = std::max((size_t)1,100000 - result.size()); 
        if (rem != 0) {
            is_exact = false;
            for (int i = 0; i < PRECISION; ++i) {
                rem = rem * M100;
                divr = divr * M10;
                int low = 0, high = 9;
                int x = 0;
                while (low <= high) {
                    int mid = (low + high) / 2;
                    temp = mid;
                    test_val = (divr + temp) * mid;
                    if (test_val <= rem) {
                        x = mid;
                        low = mid + 1;
                    } else {
                        high = mid - 1;
                    }
                }
                temp = x;
                test_val = (divr + temp) * x;
                rem = rem - test_val;
                divr = divr + TWO * x;
                result.push_back(x);
            }
        }

        std::string result_str;
        size_t num_int_pairs = (integer_part.length() + 1) / 2;
        result_str.reserve(result.size() + 2);

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