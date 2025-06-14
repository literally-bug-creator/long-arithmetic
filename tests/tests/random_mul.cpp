#include <cstdint>
#include <gmpxx.h>
#include <gtest/gtest.h>
#include <random>
#include <string>

#include "big_number.hpp"

using namespace big_number;

typedef std::vector<chunk> chunks;
typedef std::vector<int> digits;

const u_long SEED = 42;
const int32_t TEST_F_CASES = 10000;
const double EPS = 1e-6;

class BigNumberMulTest : public ::testing::Test {
public:
    digits random_digits( int min_length, int max_length ) {
        digits value;

        std::uniform_int_distribution<int> digit_count_dist( min_length,
                                                             max_length );
        int digit_count = digit_count_dist( rng );

        std::uniform_int_distribution<int> digit_dist( 1, 9 );
        value.reserve( digit_count );

        for ( int i = 0; i < digit_count; ++i ) {
            value.push_back( digit_dist( rng ) );
        }

        if ( !value.empty() && value[0] == 0 ) { value[0] = 1; }

        return value;
    }

    int32_t random_exp( int32_t min_exponent, int32_t max_exponent ) {
        std::uniform_int_distribution<int32_t> exp_dist( min_exponent,
                                                         max_exponent );
        return exp_dist( rng );
    }

    bool random_sign() {
        std::uniform_int_distribution<int> sign_dist( 0, 1 );
        return static_cast<bool>( sign_dist( rng ) );
    }

    BigNumber random_big_number( int min_chunks = 1,
                                 int max_chunks = 5,
                                 int32_t min_exponent = -5,
                                 int32_t max_exponent = 5 ) {
        digits value = random_digits( min_chunks, max_chunks );
        int32_t exponent = random_exp( min_exponent, max_exponent );
        bool is_negative = random_sign();

        return make_big_number( value, exponent, is_negative );
    }

    std::pair<std::string, int32_t> parse_parts( const std::string& str ) {
        size_t ePos = str.find_first_of( "eE" );

        if ( ePos == std::string::npos ) { return { str, 0 }; }

        std::string mantissa = str.substr( 0, ePos );
        int exponent = std::stoi( str.substr( ePos + 1 ) );

        return { mantissa, exponent };
    }

    mpz_class long_int_from_big_number( const BigNumber& number ) {
        auto mantissa_exp = parse_parts( to_string( number ) );
        std::string str =
            mantissa_exp.first + std::string( mantissa_exp.second, '0' );

        return mpz_class( str );
    }

    mpf_class long_float_from_big_number( const BigNumber& number ) {
        return mpf_class( to_string( number ), 1280 );
    }

    bool is_equal( const BigNumber& number, const mpz_class right ) {
        mpz_class left = long_int_from_big_number( number );
        return left.get_str() == right.get_str();
    }

    bool is_equal( const BigNumber& number, const mpf_class right ) {
        mpf_class left = long_float_from_big_number( number );
        return ( left - right ) < EPS;
    }

protected:
    void SetUp() override { rng.seed( SEED ); }

    std::mt19937 rng;
};

TEST_F( BigNumberMulTest, Random ) {
    for ( int i = 0; i < TEST_F_CASES; i++ ) {
        BigNumber a = random_big_number();
        BigNumber b = random_big_number();
        mpf_class expected =
            long_float_from_big_number( a ) * long_float_from_big_number( b );

        BigNumber real = mul( a, b );

        EXPECT_TRUE( is_equal( real, expected ) );
    }
}
