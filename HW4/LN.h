//
// Created by 1 on 01.09.2022.
//

#ifndef MYBIGINTEGER_LN_H
#define MYBIGINTEGER_LN_H

#include <string_view>

#include <vector>

class LN
{
  private:
	std::vector< int > digits = {};
	bool minus = false;
	bool NaN = false;

  public:
	LN(const char *digits);
	LN(long long digits = 0);
	LN(std::vector< int > digits);
	LN(std::string_view digits);
	LN(std::string digits);
	operator int() const;
	LN root();
	bool operator==(LN &rhs);
	bool operator!=(LN &rhs);
	LN operator+(LN &rhs);
	LN operator+=(LN &rhs);
	LN operator-(LN &rhs);
	LN operator-=(LN &rhs);
	LN operator*(LN &rhs);
	LN operator*=(LN &rhs);
	LN operator*(int rhs);
	LN negate();
	LN operator/(LN &rhs);
	LN operator/=(LN &rhs);
	LN operator%(LN &rhs);
	LN operator%=(LN &rhs);
	LN& operator++();
	LN& operator--();
	LN operator++(int);
	LN operator--(int);
	bool isZeroes(LN &rhs);
	LN plusImpl(LN &rhs, bool changeSign);
	LN minusImpl(LN &rhs, bool changeSign);
	bool byAbs(LN &rhs);
	bool operator>(LN &rhs);
	bool operator<(LN &rhs);
	bool operator<=(LN &rhs);
	bool operator>=(LN &rhs);
	//    LN operator=(LN &rhs);
	void showNum(LN &lhs);
	std::string toString();
	bool isNan(LN &rhs);
};

LN operator""_ln(const char *C);
LN operator""_ln(unsigned long long num);
void killLeadingZeroes(std::vector< int > &rhs);

#endif	  // MYBIGINTEGER_BIGINTEGER_H
