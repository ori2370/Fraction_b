#include "sources/Fraction.hpp"
#include "doctest.h"
#include <sstream>
using namespace std;
namespace ariel
{
    TEST_CASE("checking Ctor handling exceptions")
    {
        CHECK_THROWS(Fraction(1000, 0));
        CHECK_NOTHROW(Fraction(0, 1000));
    }
    TEST_CASE("float to fraction")
    {
        Fraction x(0.5);
        CHECK(x.getNumerator() == 1);
        CHECK(x.getDenominator() == 2);
        // now checking if 3 digits precision is correct
        Fraction y(0.9999999);
        CHECK(y.getNumerator() == 999);
        CHECK(y.getDenominator() == 1000);
        Fraction zero(0.0);
        CHECK(zero.getNumerator() == 0);
        CHECK(zero.getDenominator() == 1);

        Fraction z(1.333333);
        // checking if its 1333/1000 and not 4/3
        CHECK(z.getNumerator() == 1333);
        CHECK(z.getDenominator() == 1000);
    }
    TEST_CASE("checking reduction")
    {
        Fraction x(2, 4);
        CHECK(x.getNumerator() == 1);
        CHECK(x.getDenominator() == 2);
        Fraction y(3, 6), z(1, 6);
        // should give 2/3 and not 4/6
        CHECK((y + z).getNumerator() == 2);
    }
    TEST_CASE("checking basic operators")
    {
        Fraction a(5, 3), b(14, 21);
        Fraction c(7, 3);
        SUBCASE(" + operator")
        {
            CHECK((a + b) == c);
            Fraction x = a + 0.667774892; // will be 5/3 + 667/1000
            Fraction y(7001, 3000);
            CHECK(x == y);
            Fraction w = 0.667775893478 + a;
            CHECK(w == y);
        }
        SUBCASE(" - operator")
        {
            Fraction x(1, 1);
            Fraction y(-1, 1);
            CHECK((a - b) == x);
            CHECK((b - a) == y);
            Fraction z = a - 0.66777742427; // 5/3 - 667/1000
            Fraction u(2999, 3000);
            CHECK(z == u);
            Fraction w = 0.6677789958 - a; // 667/1000 - 5/3
            Fraction k(-2999, 3000);
            CHECK(w == k);
        }
        SUBCASE(" * operator")
        {
            Fraction x(10, 9);
            CHECK((a * b) == x); // 5/3 * 667/1000
            Fraction y = a * 0.6677774892;
            Fraction w(667, 600);
            CHECK(y == w);
            Fraction z = 0.6677777232 * a;
            CHECK(z == w);
        }
        SUBCASE(" / operator")
        {
            Fraction x(5, 2);
            CHECK((a / b) == x);
            Fraction y = a / 0.6677777424; // 5/3 * 1000/667
            Fraction w(5000, 2001);
            CHECK(y == w);
            Fraction z = 1.6670042421 / b; // 1667/1000 * 21/14
            Fraction u(5001, 2000);
            CHECK(z == u);
        }
    }
    TEST_CASE("divide by zero exception")
    {
        Fraction x(1, 2), y(0, 5);
        CHECK_THROWS(x / y);
        CHECK_THROWS(x / 0.0);
    }
    TEST_CASE("handling negative fractions")
    {
        Fraction x(-5, 3), y(14, -21);
        Fraction z(-7, 3);
        CHECK((x + y) == z);
        Fraction w(10, 9);
        CHECK((x * y) == w);
        Fraction a(2.5);
        CHECK(x / y == a);
    }
    TEST_CASE("fractions with 1 as denominator")
    {
        Fraction x(3, 1), y(2, 1);
        Fraction z(5, 1);
        CHECK((x + y) == z); // not 5/2
        Fraction w(6, 1);
        CHECK((x * y) == w);
        Fraction a(1.5);
        CHECK(x / y == a);
    }
    TEST_CASE("prefix and postfix operators")
    {
        Fraction x(1, 2);
        CHECK((x++) == Fraction(0.5));
        CHECK((x--) == Fraction(1.5));
        // now x back to 1/2
        CHECK((++x) == Fraction(3, 2));
        CHECK((--x) == Fraction(1, 2));
    }
    TEST_CASE("boolean operators")
    {
        Fraction x(1, 2), y(0.5), z(2, 1);
        CHECK(x == y);
        CHECK(x != z);
        CHECK(x < z);
        CHECK(x <= z);
        CHECK(x <= y);
        CHECK(z > x);
        CHECK(y >= x);
        CHECK(z >= x);
        // now with floats
        CHECK(x == 0.5);
        CHECK(x != 2.0);
        CHECK(x < 2.0);
        CHECK(x <= 2.0);
        CHECK(2.0 > x);
        CHECK(0.5 >= x);
    }
    TEST_CASE("IO operations")
    {
        Fraction x(3, 2);
        ostringstream os;
        os << x;
        CHECK(os.str() == "3/2");
        istringstream is("3 4");
        is >> x;
        CHECK(x == Fraction(3, 4));
    }

}