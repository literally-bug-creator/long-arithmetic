#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "error.hpp"
#include "getters.hpp"
#include <iostream>
#include <cmath>
#include <functional>
#include <algorithm>

namespace big_number {
    
    static int32_t compute_max_exp(const BigNumber& number) {
        return get_exponent(number) + get_size(number) - 1;
    }

    const Error ROOT_FROM_NEG = make_error(CALCULATION_ERROR, "sqrt: negative number");
    
    // Оптимизированная функция для больших чисел
    BigNumber sqrt(const BigNumber& number) {
        static const BigNumber ZERO = from_long_long(0);
        static const BigNumber ONE = from_long_long(1);
        static const BigNumber HALF = make_big_number("0.5");
        static const BigNumber TWO = from_long_long(2);
        
        if (is_lower_than(number, ZERO)) 
            return make_zero(ROOT_FROM_NEG);
        if (is_equal(number, ZERO) || is_equal(number, ONE))
            return number;

        // Вычисляем начальное приближение на основе двоичного представления
        int32_t max_exp = compute_max_exp(number);
        int32_t initial_exp = max_exp / 2;
        
        // Создаем начальное приближение как 2^(max_exp/2)
        std::vector<chunk> initial_chunks;
        initial_chunks.push_back(1);
        BigNumber x = from_scratch(initial_chunks, initial_exp, false, DEFAULT_ERROR);
        
        // Адаптивная точность в зависимости от размера числа
        int32_t target_precision = std::max(1, static_cast<int32_t>(get_size(number) / 2));
        int32_t current_precision = 1;
        
        // Максимальное количество итераций зависит от размера числа
        int max_iterations = std::min(50, 10 + static_cast<int>(std::log2(get_size(number))));
        
        BigNumber last = ZERO;
        int iterations = 0;
        
        while (iterations < max_iterations) {
            if (is_equal(x, last)) break;
            last = x;
            
            // Вычисляем 1/x с увеличивающейся точностью
            BigNumber inv_x = div(ONE, x, current_precision);
            
            // Ограничиваем размер промежуточных результатов
            size_t inv_size = get_size(inv_x);
            if (inv_size > 5555) {
                size_t excess = inv_size - 5555;
                std::vector<chunk> new_inv_chunks(
                    inv_x.chunks.begin() + excess,
                    inv_x.chunks.end()
                );
                inv_x = from_scratch(
                    new_inv_chunks,
                    get_exponent(inv_x),
                    is_negative(inv_x),
                    get_error(inv_x)
                );
            }
            
            // Вычисляем следующее приближение
            BigNumber term = mul(number, inv_x);
            BigNumber sum = add(x, term);
            x = mul(sum, HALF);
            
            // Ограничиваем размер результата
            size_t x_size = get_size(x);
            if (x_size > 5555) {
                size_t excess = x_size - 5555;
                std::vector<chunk> new_x_chunks(
                    x.chunks.begin() + excess,
                    x.chunks.end()
                );
                x = from_scratch(
                    new_x_chunks,
                    get_exponent(x),
                    is_negative(x),
                    get_error(x)
                );
            }
            
            // Увеличиваем точность для следующей итерации
            current_precision = std::min(current_precision * 2, target_precision);
            iterations++;
            
            // Проверяем сходимость
            if (iterations > 1) {
                BigNumber diff = sub(x, last);
                if (is_lower_than(abs(diff), shift(ONE, -target_precision))) {
                    break;
                }
            }
        }
        
        return x;
    }
}
