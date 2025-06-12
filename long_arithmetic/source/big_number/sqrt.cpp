#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "error.hpp"
#include "getters.hpp"
#include <iostream>
#include <cmath>
#include <functional>


namespace big_number {
    
    static int32_t compute_max_exp( const BigNumber& number ) {
        return get_exponent( number ) + get_size( number ) - 1;
    }

    
    const Error ROOT_FROM_NEG =
    make_error( CALCULATION_ERROR, "sqrt: negative number" );
    BigNumber sqrt(const BigNumber& number) {
    // Константы
    static const BigNumber ZERO = from_long_long(0);
    static const BigNumber ONE = from_long_long(1);
    static const BigNumber HALF = make_big_number("0.5");
    static const int PRECISION = 20; // Точность для вычисления обратного
    
    // Проверка особых случаев
    if (is_lower_than(number, ZERO)) 
        return make_zero(ROOT_FROM_NEG);
    if (is_equal(number, ZERO) || is_equal(number, ONE))
        return number;

    BigNumber x = number;
    BigNumber last = ZERO;

    for (int i = 0; i < 30; ++i) { // Фиксированное число итераций
        // Проверка сходимости
        if (is_equal(x, last)) break;
        last = x;

        // Вычисление обратного значения (1/x)
        BigNumber inv_x = div_old(ONE, x, PRECISION);
        
        // Обновление по формуле Ньютона: x = 0.5 * (x + number * (1/x))
        BigNumber term = mul(number, inv_x);
        BigNumber sum = add(x, term);
        x = mul(sum, HALF);
    }
    return x;
}
    
}
