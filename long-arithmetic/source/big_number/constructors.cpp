#include <cstddef>
#include <cstdint>
#include <regex>
#include <string>
#include <vector>

#include "big_number.hpp"
#include "error.hpp"
#include "constructors.hpp"

using namespace std;

static string normalize_scientific(const string& s) {
    smatch match;
    regex sci_regex(R"(^([+-]?)(\d*)(\.(\d*))?[eE]([+-]?\d+)$)");
    if (regex_match(s, match, sci_regex)) {
        string sign = match[1];
        string int_part = match[2];
        string frac_part = match[4];
        int exp = stoi(match[5]);
        string digits = int_part + frac_part;
        int point_pos = int_part.size();
        int new_point = point_pos + exp;
        if (new_point < 0) {
            digits = string(-new_point, '0') + digits;
            new_point = 0;
        }
        if (new_point > (int)digits.size()) {
            digits += string(new_point - digits.size(), '0');
            new_point = digits.size();
        }
        string res = sign + digits.substr(0, new_point);
        if (new_point < (int)digits.size()) {
            res += "." + digits.substr(new_point);
        }
        if (!res.empty() && res[0] == '+') res.erase(0, 1);
        return res;
    }
    return s;
}

static void parse_sign(string& num, bool& is_negative) {
    if (num[0] == '-') {
        is_negative = true;
        num.erase(0, 1);
    } else if (num[0] == '+') {
        num.erase(0, 1);
    }
}

static void parse_fraction(string& num, int32_t& exponent) {
    size_t dot = num.find('.');
    if (dot != string::npos) {
        size_t frac_size = num.size() - dot - 1;
        exponent = -(frac_size / CHUNK_DIGITS + (frac_size % CHUNK_DIGITS != 0));
        num.erase(dot, 1);
        num += string(-exponent * CHUNK_DIGITS - frac_size, '0');
    }
}

static void remove_leading_zeros(string& num) {
    num.erase(0, num.find_first_not_of('0'));
}

static void remove_leading_chunk_zeros(vector<chunk>& chunks, int32_t& exponent) {
    size_t leading_zeros = 0;
    while (leading_zeros < chunks.size() && chunks[leading_zeros] == 0) {
        ++leading_zeros;
    }
    exponent += leading_zeros;
    chunks.erase(chunks.begin(), chunks.begin() + leading_zeros);
}

BigNumber make_big_number(const string& str) {
    string num = normalize_scientific(str);
    if (num.empty()) {
        return from_scratch({}, 0, false, make_error(INVALID_INPUT, "Empty string"));
    }
    bool is_negative = false;
    parse_sign(num, is_negative);
    int32_t exponent = 0;
    parse_fraction(num, exponent);
    remove_leading_zeros(num);
    if (num.empty()) {
        return make_zero();
    }
    size_t chunks_count = num.size() / CHUNK_DIGITS + (num.size() % CHUNK_DIGITS != 0);
    num = string(chunks_count * CHUNK_DIGITS - num.size(), '0') + num;
    vector<chunk> chunks(chunks_count);
    for (size_t i = 0; i < chunks_count; i++) {
        size_t start = num.size() - (i + 1) * CHUNK_DIGITS;
        chunks[i] = stoull(num.substr(start, CHUNK_DIGITS));
    }
    remove_leading_chunk_zeros(chunks, exponent);
    if (chunks.empty()) {
        return make_zero();
    }
    return from_scratch(chunks, exponent, is_negative, make_error(OK, ""));
}

BigNumber from_scratch( const vector<chunk>& chunks,
                        int32_t exponent,
                        bool is_negative,
                        const Error& error ) {
    return BigNumber{ .chunks = chunks,
                      .exponent = exponent,
                      .is_negative = is_negative,
                      .error = error };
}

BigNumber from_iterator( vector<chunk>::const_iterator begin,
                         vector<chunk>::const_iterator end ) {
    size_t size = end - begin;
    int32_t exponent = 0;
    vector<chunk> chunks( 0 );

    while ( size > 0 && *begin == 0 ) {
        ++begin;
        --size;
        ++exponent;
    }

    while ( size > 0 && *( end - 1 ) == 0 ) {
        --end;
        size--;
    }
    if ( size > 0 ) { chunks = vector<chunk>( begin, end ); }

    return from_scratch( chunks, exponent, false, make_error( OK, "" ) );
}

BigNumber make_zero() {
    return from_scratch({}, 0, false, make_error(OK, ""));
}

string to_string(const BigNumber& number) {
    if (number.chunks.empty()) return "0";
    string res = number.is_negative ? "-" : "";
    for (int i = number.chunks.size() - 1; i >= 0; --i) {
        string chunk_str = to_string(number.chunks[i]);
        if (i != (int)number.chunks.size() - 1)
            chunk_str = string(CHUNK_DIGITS - chunk_str.length(), '0') + chunk_str;
        res += chunk_str;
    }
    if (number.exponent > 0) {
        res += string(number.exponent * CHUNK_DIGITS, '0');
    } else if (number.exponent < 0) {
        int dot_pos = res.length() + number.exponent * CHUNK_DIGITS;
        if (dot_pos <= 0) {
            res = "0." + string(-dot_pos, '0') + res;
        } else {
            res.insert(dot_pos, ".");
        }
    }
    return res;
}
