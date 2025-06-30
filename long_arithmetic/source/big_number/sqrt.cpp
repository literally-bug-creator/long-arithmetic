#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "error.hpp"
#include "getters.hpp"
#include <iostream>
#include <cmath>
#include <functional>
#include "core.hpp"
#include <gmp.h>

namespace big_number {
    const Error ROOT_FROM_NEG =
        make_error(CALCULATION_ERROR, "sqrt: negative number");

    // Предварительно инициализированные константы
    struct GMPConstants {
        mpz_t M100;
        mpz_t M10;
        mpz_t TWO;
        
        GMPConstants() {
            mpz_init_set_ui(M100, 100);
            mpz_init_set_ui(M10, 10);
            mpz_init_set_ui(TWO, 2);
        }
        
        ~GMPConstants() {
            mpz_clears(M100, M10, TWO, nullptr);
        }
    };

    const GMPConstants gmp_constants;

    BigNumber sqrt(const BigNumber& number) {
        if (is_lower_than(number, ZERO)) return make_zero(ROOT_FROM_NEG);
        if (is_equal(number, ZERO) || is_equal(number, ONE)) return number;

        std::string num_str = to_string(number);
        if (!num_str.empty() && num_str[0] == '-') num_str.erase(0, 1);

        mpz_t rem, divr, temp, test_val;
        mpz_inits(rem, divr, temp, test_val, nullptr);
        
        // Обработка целой и дробной частей
        std::string integer_part, fractional_part;
        size_t dot_pos = num_str.find('.');
        if (dot_pos != std::string::npos) {
            integer_part = num_str.substr(0, dot_pos);
            fractional_part = num_str.substr(dot_pos + 1);
        } else {
            integer_part = num_str;
        }

        // Формирование пар цифр
        std::vector<int> digit_pairs;
        digit_pairs.reserve(integer_part.size() + fractional_part.size());
        
        auto process_digits = [](const std::string& str, std::vector<int>& pairs) {
            size_t len = str.length();
            if (len % 2 != 0) {
                pairs.push_back(str[0] - '0');
                for (size_t i = 1; i < len; i += 2) {
                    pairs.push_back(10 * (str[i] - '0') + (str[i+1] - '0'));
                }
            } else {
                for (size_t i = 0; i < len; i += 2) {
                    pairs.push_back(10 * (str[i] - '0') + (str[i+1] - '0'));
                }
            }
        };
        
        process_digits(integer_part, digit_pairs);
        if (fractional_part.size() % 2 != 0) fractional_part += '0';
        process_digits(fractional_part, digit_pairs);

        // Основной алгоритм
        std::vector<int> result;
        result.reserve(digit_pairs.size() + 100000);
        bool is_exact = true;

        mpz_set_ui(rem, 0);
        mpz_set_ui(divr, 0);

        // Восстановление оригинальной логики обновления divr
        for (int pair : digit_pairs) {
            // rem = rem * 100 + pair
            mpz_mul(rem, rem, gmp_constants.M100);
            mpz_add_ui(rem, rem, pair);
            
            // divr = divr * 10 (оригинальная операция)
            mpz_mul(divr, divr, gmp_constants.M10);
            
            // Оптимизированный бинарный поиск
            int x = 0;
            int low = 0, high = 9;
            
            // Проверяем верхнюю границу для оптимизации
            mpz_set_ui(temp, high);
            mpz_add(test_val, divr, temp);
            mpz_mul_ui(test_val, test_val, high);
            if (mpz_cmp(test_val, rem) <= 0) {
                x = high;
            } else {
                // Бинарный поиск при необходимости
                while (low <= high) {
                    int mid = (low + high) / 2;
                    mpz_set_ui(temp, mid);
                    mpz_add(test_val, divr, temp);
                    mpz_mul_ui(test_val, test_val, mid);
                    
                    if (mpz_cmp(test_val, rem) <= 0) {
                        x = mid;
                        low = mid + 1;
                    } else {
                        high = mid - 1;
                    }
                }
            }
            
            // Обновление значений
            mpz_set_ui(temp, x);
            mpz_add(test_val, divr, temp);
            mpz_mul_ui(test_val, test_val, x);
            mpz_sub(rem, rem, test_val);
            
            // divr = divr + 2 * x (оригинальная операция)
            mpz_mul_ui(temp, gmp_constants.TWO, x);
            mpz_add(divr, divr, temp);
            
            result.push_back(x);
        }
        
        // Дополнительные итерации для дробной части
        size_t PRECISION = std::max<size_t>(1, 100000 - result.size());
        if (mpz_sgn(rem) != 0) {
            is_exact = false;
            
            for (size_t i = 0; i < PRECISION; ++i) {
                mpz_mul(rem, rem, gmp_constants.M100);
                mpz_mul(divr, divr, gmp_constants.M10);
                
                int x = 0;
                int low = 0, high = 9;
                
                // Проверка верхней границы
                mpz_set_ui(temp, high);
                mpz_add(test_val, divr, temp);
                mpz_mul_ui(test_val, test_val, high);
                if (mpz_cmp(test_val, rem) <= 0) {
                    x = high;
                } else {
                    // Бинарный поиск
                    while (low <= high) {
                        int mid = (low + high) / 2;
                        mpz_set_ui(temp, mid);
                        mpz_add(test_val, divr, temp);
                        mpz_mul_ui(test_val, test_val, mid);
                        
                        if (mpz_cmp(test_val, rem) <= 0) {
                            x = mid;
                            low = mid + 1;
                        } else {
                            high = mid - 1;
                        }
                    }
                }
                
                mpz_set_ui(temp, x);
                mpz_add(test_val, divr, temp);
                mpz_mul_ui(test_val, test_val, x);
                mpz_sub(rem, rem, test_val);
                
                mpz_mul_ui(temp, gmp_constants.TWO, x);
                mpz_add(divr, divr, temp);
                
                result.push_back(x);
            }
        }

        // Восстановление оригинального формирования результата
        std::string result_str;
        size_t num_int_pairs = (integer_part.length() + 1) / 2;
        result_str.reserve(result.size() + 2);

        // Целая часть
        for (size_t i = 0; i < num_int_pairs; ++i) {
            result_str += '0' + result[i];
        }
        
        // Дробная часть (если есть)
        if (!is_exact || result.size() > num_int_pairs) {
            result_str += '.';
            for (size_t i = num_int_pairs; i < result.size(); ++i) {
                result_str += '0' + result[i];
            }
        }

        // Освобождение ресурсов
        mpz_clears(rem, divr, temp, test_val, nullptr);
        
        return make_big_number(result_str);
    }
}