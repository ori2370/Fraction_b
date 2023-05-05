#include "Fraction.hpp"
#include <limits>
#include <cmath>
using namespace std;

// flags for overflow check
#define PLUS_OPERATOR 1
#define MINUS_OPERATOR 2
#define MULTI_OPERATOR 3
#define MAX_VALUE numeric_limits<int>::max()
#define MIN_VALUE numeric_limits<int>::min()
namespace ariel
{
    // helper function: gcd implementiation: https://www.tutorialspoint.com/cplusplus-program-to-find-gcd
    void Fraction::fracReduction()
    {
        // check if some of the fraction is negative
        bool negativenumer = numerator < 0;
        bool negativedenom = denominator < 0;
        numerator = abs(numerator);
        denominator = abs(denominator);
        int a = numerator;
        int b = denominator;
        while (b != 0)
        {
            int t = b;
            b = a % b;
            a = t;
        }
        int gcd = a;

        if (gcd != 0)
        {
            numerator = numerator / gcd;
            denominator = denominator / gcd;
        }
        if (negativenumer || negativedenom)
        { // if one of them is negative we need to bring back the -
            if (negativenumer && negativedenom)
            {
                return; // both negative so no need to bring - back
            }
            numerator = -numerator;
        }
    }
    //  for checking overflow i used this https://stackoverflow.com/a/1514309
    // for multi check i used this: https://stackoverflow.com/questions/54318815/integer-overflow-w-multiplication-in-c the last example.
    bool Fraction::isOverflow(const Fraction &frac1, const Fraction &frac2, int operator_flag) const
    {

        switch (operator_flag)
        {
        case (PLUS_OPERATOR):
        {
            double a = frac1.numerator * frac2.denominator;
            double b = frac2.numerator * frac1.denominator;
            return (a > 0 && b > MAX_VALUE - a) || (a < 0 && b < MIN_VALUE - a);
        }
        case (MINUS_OPERATOR):
        {
            double a = frac1.numerator * frac2.denominator;
            double b = frac2.numerator * frac1.denominator;
            return (b < 0 && a > MAX_VALUE + b) || (b > 0 && a < MIN_VALUE + b);
        }
        case (MULTI_OPERATOR):
        {
            double a = frac1.numerator, b = frac2.numerator;
            double c = frac2.denominator, d = frac1.denominator;
            return ((a * b > MAX_VALUE) || (a * b < MIN_VALUE) || (c * d > MAX_VALUE) || (c * d < MIN_VALUE));
        }
        }
        return false;
    }

    // ctors
    Fraction::Fraction(int numerator, int denominator)
    {
        if (denominator == 0)
        {
            throw invalid_argument("you can't divide by zero");
        }
        if (numerator == 0)
        {
            this->numerator = 0;
            this->denominator = 1;
            return; // no need to reduce
        }
        this->numerator = numerator;
        this->denominator = denominator;
        fracReduction();
    }
    Fraction::Fraction(float fraction)
    {
        bool negativeflag = fraction < 0;
        fraction = abs(fraction);
        int numer = floor(fraction * 1000);
        if (numer < 0.001 && numer >= 0) // after multiplying still less than 0.001 so its 0
        {
            this->numerator = 0;
            this->denominator = 1;
            return; // no need to reduce
        }
        this->denominator = 1000;
        if (negativeflag)
        {
            this->numerator = -numer;
        }
        else
        {
            this->numerator = numer;
        }

        fracReduction();
    }
    // operators
    //  (-)
    Fraction Fraction::operator-(const Fraction &other) const
    {

        if (isOverflow(*this, other, MINUS_OPERATOR))
        {
            throw overflow_error("int overflow");
        }
        int tempnumer = (numerator * other.denominator) - (other.numerator * denominator);
        int tempdenom = denominator * other.denominator;
        return Fraction(tempnumer, tempdenom);
    }

    // (+)
    Fraction Fraction::operator+(const Fraction &other) const
    {
        if (isOverflow(*this, other, PLUS_OPERATOR))
        {
            throw overflow_error("int overflow");
        }
        int tempnumer = (numerator * other.denominator) + (other.numerator * denominator);
        int tempdenom = denominator * other.denominator;
        return Fraction(tempnumer, tempdenom);
    }

    // (*)
    Fraction Fraction::operator*(const Fraction &other) const
    {
        if (isOverflow(*this, other, MULTI_OPERATOR))
        {
            throw overflow_error("int overflow");
        }
        int tempnumer = numerator * other.numerator;
        int tempdenom = denominator * other.denominator;
        return Fraction(tempnumer, tempdenom);
    }

    // (/)
    Fraction Fraction::operator/(const Fraction &other) const
    {

        if (other.numerator == 0)
        {
            throw runtime_error("dividing by zero is not allowed");
        }
        Fraction temp(other.denominator, other.numerator);
        if (isOverflow(*this, temp, MULTI_OPERATOR))
        {
            throw overflow_error("int overflow");
        }
        return *this * temp;
    }

    // comparison operators
    // (==)
    bool Fraction::operator==(const Fraction &other) const
    {
        return ((numerator * other.denominator) == (other.numerator * denominator));
    }

    // (!=)
    bool Fraction::operator!=(const Fraction &other) const
    {
        return !(*this == other);
    }

    // (>)
    bool Fraction::operator>(const Fraction &other) const
    {
        int frac1 = numerator * other.denominator;
        int frac2 = other.numerator * denominator;
        // wont work if the minus in the denmonator so we need to check first
        if ((denominator < 0 && other.denominator > 0) || (denominator > 0 && other.denominator < 0)) // checking if one of the denom is negative while the other is not
        {
            frac1 *= -1;
        }
        return frac1 > frac2;
    }

    // (<)
    bool Fraction::operator<(const Fraction &other) const
    {
        return !(*this > other);
    }

    // (>=)
    bool Fraction::operator>=(const Fraction &other) const
    {
        return ((*this > other) || (*this == other));
    }

    // (<=)
    bool Fraction::operator<=(const Fraction &other) const
    {
        return ((*this < other) || (*this == other));
    }
    // prefix and postfix
    Fraction Fraction::operator++(int)
    {
        Fraction value_holder(numerator, denominator);
        numerator += denominator; // +1 equals to adding whole cylce of the fraction
        return value_holder;
    }
    Fraction Fraction::operator--(int)
    {
        Fraction value_holder(numerator, denominator);
        numerator -= denominator;
        return value_holder;
    }
    Fraction &Fraction::operator++()
    {
        numerator += denominator; // +1 equals to adding whole cylce of the fraction
        return *this;
    }
    Fraction &Fraction::operator--()
    {
        numerator -= denominator;
        return *this;
    }

    // input and output
    ostream &operator<<(ostream &os, const Fraction &fraction)
    {
        if (fraction < 0.0)
        { // negative fraction so the minus is on top
            int numer = abs(fraction.numerator);
            int denom = abs(fraction.denominator);

            os << -numer << "/" << denom;
            return os;
        }
        os << fraction.numerator << "/" << fraction.denominator;
        return os;
    }

    istream &operator>>(istream &is, Fraction &fraction)
    {
        int tempnumerator, tempdenominator;
        if (!(is >> tempnumerator >> tempdenominator))
        {
            throw runtime_error("must enter two numbers");
        }
        if (tempdenominator == 0)
        {
            throw runtime_error("dividing by zero is not allowed");
        }
        fraction.numerator = tempnumerator;
        fraction.denominator = tempdenominator;
        return is;
    }
}