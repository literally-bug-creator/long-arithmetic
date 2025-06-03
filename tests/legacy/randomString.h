#pragma once
#include <string>
#include <random>
#include <sstream>
#include <iomanip>

// Генерирует случайную строку, представляющую float-число без незначащих нулей
inline std::string randomFloatString() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> sign_dist(0, 1);
    std::uniform_int_distribution<int> int_dist(0, 999999);
    std::uniform_int_distribution<int> frac_len_dist(0, 6);
    std::uniform_int_distribution<int> exp_dist(-10, 10);

    // Генерируем знак
    std::string sign = sign_dist(gen) ? "-" : "";

    // Генерируем целую часть без лидирующих нулей
    int int_part = int_dist(gen);
    std::string int_str = std::to_string(int_part);
    // Удаляем лидирующие нули
    int_str.erase(0, int_str.find_first_not_of('0'));
    if (int_str.empty()) int_str = "0";

    // Генерируем дробную часть без лишних нулей
    int frac_len = frac_len_dist(gen);
    std::string frac_str;
    if (frac_len > 0) {
        std::uniform_int_distribution<int> frac_digit_dist(0, 9);
        frac_str = ".";
        for (int i = 0; i < frac_len; ++i) {
            frac_str += std::to_string(frac_digit_dist(gen));
        }
        // Удаляем незначащие нули справа
        while (frac_str.size() > 1 && frac_str.back() == '0') frac_str.pop_back();
        if (frac_str == ".") frac_str.clear();
    }

    // Генерируем экспоненту без плюса и незначащих нулей
    int exp = exp_dist(gen);
    std::string exp_str;
    if (exp != 0) {
        exp_str = "e" + std::to_string(exp);
    }

    return sign + int_str + frac_str + exp_str;
}
