#include "constructors.hpp"

#include <cstdint>
#include <cstring>

#include "error.hpp"
#include "getters.hpp"

void strip_zeros(BigNumber& num) {
    auto& chunks = num.chunks;
    size_t sz = chunks.size();

    if (sz == 0) return;

    size_t start = 0;
    while (start < sz && chunks[start] == 0) {
        ++start;
    }

    size_t end = sz;
    while (end > start && chunks[end - 1] == 0) {
        --end;
    }

    if (start == end) {
        num.chunks.clear();
        num.exponent = 0;
        num.is_negative = false;
        return;
    }

    num.exponent += static_cast<int32_t>(start);
    chunks = std::vector<chunk>(chunks.begin() + start, chunks.begin() + end);
}

BigNumber make_big_number(const std::string& number) {
    std::string num = number;
    bool isNegative = false;
    int64_t expAdjust = 0;
    Error errorObj;                   
    std::vector<chunk> chunksVec;     
    int32_t exponentValue = 0;        

    auto ePos = num.find_first_of("eE");
    if (ePos != std::string::npos) {
        std::string expPart = num.substr(ePos + 1);
        expAdjust = std::stoll(expPart);
        num.erase(ePos);
    }

    if (!num.empty() && num[0] == '-') {
        isNegative = true;
        num.erase(0, 1);
    }

    size_t frac_len = 0;
    auto dotPos = num.find('.');
    if (dotPos != std::string::npos) {
        frac_len = num.size() - dotPos - 1;
        num.erase(dotPos, 1);
    }

    if (expAdjust >= 0) {
        if (expAdjust > static_cast<int64_t>(frac_len)) {
            num.append(static_cast<size_t>(expAdjust - frac_len), '0');
            frac_len = 0;
        } 
        else {
            frac_len -= static_cast<size_t>(expAdjust);
        }
    } 
    else {
        num = std::string(static_cast<size_t>(-expAdjust), '0') + num;
        frac_len += static_cast<size_t>(-expAdjust);
    }

    num.erase(0, num.find_first_not_of('0'));
    if (num.empty()) {
        num = "0";
    }

    exponentValue = -(
        static_cast<int32_t>(frac_len / CHUNK_DIGITS) +
        static_cast<int32_t>((frac_len % CHUNK_DIGITS != 0) ? 1 : 0)
    );

    if (frac_len > 0) {
        num += std::string(
            static_cast<size_t>(-exponentValue) * CHUNK_DIGITS - frac_len,
            '0'
        );
    }

    size_t chunksCount = num.size() / CHUNK_DIGITS
                         + (num.size() % CHUNK_DIGITS != 0 ? 1 : 0);

    size_t totalLen = chunksCount * CHUNK_DIGITS;
    if (num.size() < totalLen) {
        num = std::string(totalLen - num.size(), '0') + num;
    }

    chunksVec.resize(chunksCount);
    for (size_t i = 0; i < chunksCount; ++i) {
        size_t startIdx = num.size() - (i + 1) * CHUNK_DIGITS;
        std::string chunkSubstring = num.substr(startIdx, CHUNK_DIGITS);
        chunksVec[i] = static_cast<chunk>(std::stoull(chunkSubstring));
    }

    BigNumber temp = from_scratch(chunksVec,
                                  exponentValue,
                                  isNegative,
                                  errorObj);

    strip_zeros(temp);
    return temp;
}

BigNumber from_scratch( const std::vector<chunk>& chunks,
                        int32_t exponent,
                        bool is_negative,
                        const Error& error ) {

    BigNumber res = { chunks, exponent, is_negative, error };
    strip_zeros( res );
    return res;
}

BigNumber from_iterator( std::vector<chunk>::const_iterator begin,
                         std::vector<chunk>::const_iterator end ) {
    std::vector<chunk> chunks;
    int32_t exponent = 0;
    size_t new_size = end - begin;
    while ( new_size > 0 && *begin == 0 ) {
        ++begin;
        --new_size;
        exponent++;
    }
    while ( new_size > 0 && *( end - 1 ) == 0 ) {
        --end;
        new_size--;
    }
    if ( new_size > 0 ) { chunks = std::vector<chunk>( begin, end ); }
    return from_scratch(
        chunks, exponent, false, make_error( OK, "" ) ); // TODO: Fix
}

BigNumber make_zero() {
    return from_scratch( {}, 0, false, make_error( OK, "" ) ); // TODO: Fix
}

BigNumber from_long_long( long long v ) {
    return make_big_number( std::to_string( v ) );
}

static void trimTrailingZeros(std::string& s) {
    size_t suff_zeros = 0;
    for (size_t i = s.size(); i > 0; --i) {
        if (s[i - 1] == '0') {
            ++suff_zeros;
        }
        else if (s[i - 1] == '.') {
            break;
        }
        else {
            break;
        }
    }
    if (suff_zeros > 0) {
        s.erase(s.size() - suff_zeros, suff_zeros);
    }
    if (!s.empty() && s.back() == '.') {
        s.pop_back();
    }
}
std::string to_string( const BigNumber& number ) {
    std::vector<chunk> chunks = get_chunks(number);
    size_t chunks_size = chunks.size();
    if (chunks.empty() || (chunks.size() == 1 && chunks[0] == 0))
        return "0";

    std::string result;
    result.reserve(1 + 1 + chunks_size * CHUNK_DIGITS);
    if (is_negative(number)) {
        result.push_back('-');
    }

    int32_t exp = get_exponent(number);
    int32_t max_exp = std::max(exp + static_cast<int32_t>(chunks_size) - 1, 0);
    int32_t min_exp = std::min(exp, 0);

    for (int32_t i = max_exp; i >= min_exp; --i) {
        if (i == -1) {
            result += '.';
        }
        std::string chunk_str;
        int32_t chunk_idx = i - exp;
        if (chunk_idx >= 0 && static_cast<size_t>(chunk_idx) < chunks_size) {
            chunk_str = std::to_string(chunks[chunk_idx]);
        } else {
            chunk_str = "0";
        }
        if (i != max_exp) {
            chunk_str = std::string(CHUNK_DIGITS - chunk_str.size(), '0') + chunk_str;
        }
        result.append(chunk_str);
    }

    if (min_exp < 0) {
        trimTrailingZeros(result);
    }

    return result;
}
