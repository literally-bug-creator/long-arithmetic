#include "big_number.hpp"
#include "getters.hpp"
#include <sstream>
#include <iomanip>

namespace big_number{
    std::string to_string(const BigNumber& number){
        // Handle special cases
        if (is_nan(number)) {
            return "NaN";
        }
        
        if (is_inf(number)) {
            return is_negative(number) ? "-inf" : "inf";
        }
        
        if (is_zero(number)) {
            return "0.0e+0";
        }
        
        std::ostringstream result;
        
        // Add sign
        if (is_negative(number)) {
            result << "-";
        }
        
        const chunks& mantissa = get_mantissa(number);
        int32_t shift = get_shift(number);
        
        // Find the most significant chunk (mantissa is stored in reverse order)
        int32_t most_significant_chunk_index = static_cast<int32_t>(mantissa.size()) - 1;
        
        // Convert the most significant chunk to string to find first significant digit
        chunk most_significant_chunk = mantissa[most_significant_chunk_index];
        std::string chunk_str = std::to_string(most_significant_chunk);
        
        // Calculate the actual exponent
        // The position of the most significant chunk in the number
        int32_t chunk_position = shift + most_significant_chunk_index;
        // The position of the most significant digit
        int32_t exponent = chunk_position * BASE + static_cast<int32_t>(chunk_str.length()) - 1;
        
        // Output the first significant digit
        result << chunk_str[0] << ".";
        
        // Output remaining digits from the most significant chunk
        for (size_t i = 1; i < chunk_str.length(); ++i) {
            result << chunk_str[i];
        }
        
        // Output digits from remaining chunks (going from high to low)
        for (int32_t chunk_idx = most_significant_chunk_index - 1; chunk_idx >= 0; --chunk_idx) {
            chunk current_chunk = mantissa[chunk_idx];
            std::ostringstream chunk_stream;
            chunk_stream << std::setfill('0') << std::setw(BASE) << current_chunk;
            result << chunk_stream.str();
        }
        
        // Remove trailing zeros from the fractional part
        std::string temp_result = result.str();
        size_t last_non_zero = temp_result.find_last_not_of('0');
        if (last_non_zero != std::string::npos && temp_result[last_non_zero] == '.') {
            temp_result = temp_result.substr(0, last_non_zero + 2); // Keep one zero after decimal point
        } else if (last_non_zero != std::string::npos) {
            temp_result = temp_result.substr(0, last_non_zero + 1);
        }
        
        // Add exponent
        std::ostringstream final_result;
        final_result << temp_result << "e";
        if (exponent >= 0) {
            final_result << "+";
        }
        final_result << exponent;
        
        return final_result.str();
    }
}
