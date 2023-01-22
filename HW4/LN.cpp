//
// Created by 1 on 01.09.2022.
//
#include "LN.h"

#include <cmath>
#include <cstring>
#include <iostream>

void LN::showNum(LN &lhs)
{
	for (int i = lhs.digits.size() - 1; i >= 0; i--)
	{
		std::cout << lhs.digits[i];
	}
	std::cout << "\n";
}

std::string LN::toString()
{
	std::string res;
	if (NaN)
	{
		res = "NaN";
		return res;
	}
	if (minus)
		res += '-';

	for (int i = digits.size() - 1; i >= 0; i--)
	{
		res += (digits[i] + '0');
	}

	return res;
}

LN::LN(const char *C)
{
	for (int i = strlen(C) - 1; i >= 0; i--)
	{
		if (C[i] == '-' && i == 0)
		{
			minus = true;
			break;
		}
		digits.push_back(C[i] - 48);
	}
}

LN::LN(long long num)
{
	if (num < 0)
	{
		minus = true;
	}
	if (num == 0)
	{
		digits.push_back(num);
	}
	while (num != 0)
	{
		digits.push_back(num % 10);
		num /= 10;
	}
}

LN::LN(std::string_view num)
{
	for (int i = num.size() - 1; i >= 0; i--)
	{
		if (num[i] == '-')
		{
			minus = true;
			continue;
		}
		digits.push_back(num[i] - '0');
	}
}

LN::LN(std::string num)
{
	for (int i = num.size() - 1; i >= 0; i--)
	{
		if (num[i] == '-')
		{
			minus = true;
			continue;
		}
		digits.push_back(num[i] - '0');
	}
}

LN::LN(std::vector< int > digits)
{
	this->digits = digits;
}

LN::operator int() const {
	long check = INT32_MAX;
	int res = 0;
	for (int i = digits.size() - 1; i >= 0; i--) {
		res += (digits[i]) * pow(10, i);
	}
	if (res > check) {
		std::cerr << "OUT OF MEMORY";
		return 0;
	}

	if (minus) res *= -1;
	return res;
}

LN LN::negate()
{
	minus = !minus;
	return *this;
}

bool LN::operator>(LN &rhs)
{
	if (isNan(rhs))
		return false;

	if (isZeroes(rhs))
		return true;

	if (!minus && rhs.minus)
		return true;

	if (minus && !rhs.minus)
		return false;

	if (!minus)
	{
		if (digits.size() > rhs.digits.size())
			return true;

		if (digits.size() < rhs.digits.size())
			return false;

		for (int i = digits.size() - 1; i >= 0; i--)
		{
			if (digits[i] > rhs.digits[i])
				return true;

			if (digits[i] < rhs.digits[i])
				return false;
		}
		return false;
	}
	if (minus)
	{
		if (digits.size() < rhs.digits.size())
			return true;

		if (digits.size() > rhs.digits.size())
			return false;

		for (int i = digits.size() - 1; i >= 0; i--)
		{
			if (digits[i] < rhs.digits[i])
				return true;

			if (digits[i] > rhs.digits[i])
				return false;
		}
		return false;
	}
	return true;
}

bool LN::operator<(LN &rhs)
{
	if (isNan(rhs))
		return false;

	if (isZeroes(rhs))
		return true;

	return !(*this > rhs) && *this != rhs;
}

bool LN::operator<=(LN &rhs)
{
	if (isNan(rhs))
		return false;

	if (isZeroes(rhs))
		return true;

	return !(*this > rhs) || rhs == *this;
}
bool LN::operator>=(LN &rhs)
{
	if (isNan(rhs))
		return false;

	if (isZeroes(rhs))
		return true;

	return *this > rhs || rhs == *this;
}

bool LN::operator==(LN &rhs)
{
	if (isNan(rhs))
		return false;

	if (isZeroes(rhs))
		return true;

	return digits == rhs.digits && minus == rhs.minus;
}
bool LN::operator!=(LN &rhs)
{
	if (isNan(rhs))
		return false;

	if (isZeroes(rhs))
		return true;

	return !(rhs.digits == digits) || rhs.digits == digits && rhs.minus != minus;
}

LN LN::operator*(int b)
{
	LN cur = *this;
	if (b < 0)
		minus = !minus;

	for (int i = 0; i < cur.digits.size(); i++)
	{
		cur.digits[i] *= b;
	}
	for (int i = 0; i < cur.digits.size(); i++)
	{
		if (i == cur.digits.size() - 1 && cur.digits[i] > 9)
		{
			cur.digits.push_back(cur.digits[i] / 10);
			cur.digits[i] %= 10;
			continue;
		}
		cur.digits[i + 1] += cur.digits[i] / 10;
		cur.digits[i] %= 10;
	}
	return cur;
}

std::vector< int > defaultMul(std::vector< int > x, std::vector< int > y)
{
	int n = std::max(x.size(), y.size());

	std::vector< int > res(2 * n);

	for (int i = 0; i < x.size(); i++)
	{
		for (int j = 0; j < y.size(); j++)
		{
			res[i + j] += x[i] * y[j];
		}
	}

	for (int i = 0; i < res.size() - 1; i++)
	{
		res[i + 1] += res[i] / 10;
		res[i] %= 10;
	}
	killLeadingZeroes(res);

	return res;
}

LN LN::operator*(LN &rhs)
{
	if (isNan(rhs))
	{
		LN temp = 0_ln;
		temp.NaN = true;
		return temp;
	}
	LN res("");
	if (rhs.minus)
		minus = !minus;
	res.digits = defaultMul(digits, rhs.digits);
	return res;
}

LN LN::operator%(LN &rhs)
{
	if (isNan(rhs))
	{
		LN temp = 0_ln;
		temp.NaN = true;
		return temp;
	}

	minus = false;
	rhs.minus = false;
	if (*this < rhs)
	{
		return *this;
	}

	LN curValue = *this;
	LN cur("");
	LN curDel = rhs;
	LN zero("0");
	cur.digits = { digits.end() - rhs.digits.size(), digits.end() };

	for (int i = digits.size() - rhs.digits.size(); i >= 0; i--)
	{
		int l = 0;
		int r = 10;
		while (l < r - 1)
		{
			int m = (l + r) / 2;
			curDel = rhs * m;
			if (cur < curDel)
			{
				r = m;
			}
			else
			{
				l = m;
			}
		}
		curDel = rhs * l;

		cur = cur - curDel;
		if (cur == zero)
		{
			if (i - 1 >= 0)
			{
				cur.digits.pop_back();
				cur.digits.push_back(digits[i - 1]);
			}
		}
		else
		{
			if (i - 1 >= 0)
				cur.digits.insert(cur.digits.begin(), digits[i - 1]);
		}
	}

	return cur;
}

LN LN::operator/(LN &rhs)
{
	if (isNan(rhs))
	{
		LN temp = 0_ln;
		temp.NaN = true;
		return temp;
	}
	if (rhs == 0_ln)
	{
		NaN = true;
		return *this;
	}
	bool sign = false;
	bool changeSign = false;
	if (minus == true)
	{
		sign = true;
		minus = false;
	}
	if (rhs.minus == true)
	{
		changeSign = true;
		rhs.minus = false;
	}
	if (*this < rhs)
	{
		digits = std::vector< int >{ 0 };
		return *this;
	}

	LN cur("");
	LN curDel = rhs;
	LN res("");
	LN zero("0");

	cur.digits = { digits.end() - rhs.digits.size(), digits.end() };

	for (int i = digits.size() - rhs.digits.size(); i >= 0; i--)
	{
		int l = 0;
		int r = 10;
		while (l < r - 1)
		{
			int m = (l + r) / 2;
			curDel = rhs * m;
			if (cur < curDel)
			{
				r = m;
			}
			else
			{
				l = m;
			}
		}
		curDel = rhs * l;
		cur = cur - curDel;

		res.digits.insert(res.digits.begin(), l);
		if (cur == zero)
		{
			cur.digits.pop_back();
			cur.digits.push_back(digits[i - 1]);
		}
		else
		{
			cur.digits.insert(cur.digits.begin(), digits[i - 1]);
		}
	}

	killLeadingZeroes(res.digits);

	if (sign)
		res.minus = true;

	if (changeSign)
		res.minus = !res.minus;

	return res;
}

LN LN::root()
{
	if (minus == true)
	{
		NaN = true;
		return *this;
	}
	LN cur("");
	LN l("0");
	int badIdea = ceil(((double)digits.size()) / 2);
	std::vector< int > number;
	for (int i = 0; i < badIdea; i++)
	{
		number.push_back(9);
	}
	LN r(number);
	LN one(1);
	LN two(2);
	LN rl = r - l;
	//    std::cout << (r - l > one) << "\n";
	while (r - l > one)
	{
		LN m = (r + l) / two;
		LN mm = m * m;
		if (mm < *this)
		{
			l = m;
		}
		else if (mm == *this)
		{
			*this = m;
			return *this;
		}
		else
		{
			r = m;
		}
	}
	*this = l;
	return *this;
}

LN LN::plusImpl(LN &rhs, bool changeSign)
{
	int n = digits.size() < rhs.digits.size() ? digits.size() : rhs.digits.size();

	if (!byAbs(rhs))
	{
		std::vector< int > temp = rhs.digits;
		for (int i = 0; i < n; i++)
		{
			if (temp[i] < digits[i])
			{
				temp[i + 1] -= 1;
				temp[i] += 10;
			}
			temp[i] -= digits[i];
		}
		if (temp[n] == -1)
		{
			while (n + 1 < temp.size() && temp[n] == -1)
			{
				temp[n] += 10;
				temp[n + 1] -= 1;
				n++;
			}
		}
		minus = true;
		killLeadingZeroes(temp);
		LN res({ temp });
		res.minus = minus;
		if (!byAbs(rhs))
			res.minus = !res.minus;
		return res;
	}

	std::vector< int > temp = digits;
	for (int i = 0; i < n; i++)
	{
		if (temp[i] < rhs.digits[i])
		{
			temp[i + 1] -= 1;
			temp[i] += 10;
		}
		temp[i] -= rhs.digits[i];
	}
	if (temp[n] == -1)
	{
		while (n + 1 < temp.size() && temp[n] == -1)
		{
			temp[n] += 10;
			temp[n + 1] -= 1;
			n++;
		}
	}
	killLeadingZeroes(temp);
	LN res({ temp });
	res.minus = minus;
	if (!byAbs(rhs))
		res.minus = !res.minus;

	return res;
}

LN LN::minusImpl(LN &rhs, bool changeSign)
{
	int n = digits.size() < rhs.digits.size() ? digits.size() : rhs.digits.size();
	int shift = 0;

	std::vector< int > temp = digits;

	for (int i = 0; i < n; i++)
	{
		temp[i] += (rhs.digits[i] + shift);
		shift = 0;
		if (temp[i] >= 10)
		{
			shift = 1;
			temp[i] %= 10;
		}
	}

	if (temp.size() >= rhs.digits.size())
	{
		while (shift != 0 && n < temp.size())
		{
			temp[n] += shift;
			shift = temp[n] / 10;
			temp[n] %= 10;
			n++;
		}
		if (shift != 0)
			temp.push_back(1);
	}
	else
	{
		while (temp.size() < rhs.digits.size())
		{
			temp.push_back(rhs.digits[n] + shift);
			shift = temp[n] / 10;
			temp[n] %= 10;
			n++;
		}
		if (shift != 0)
			temp.push_back(1);
	}

	LN res({ temp });
	if (changeSign)
		res.minus = !res.minus;
	return res;
}

LN LN::operator-(LN &rhs)
{
	if (isNan(rhs))
	{
		LN temp = 0_ln;
		temp.NaN = true;
		return temp;
	}

	if (minus != rhs.minus)
	{
		if (*this < rhs)
			return minusImpl(rhs, true);

		else
			return minusImpl(rhs, false);
	}

	int n = digits.size() < rhs.digits.size() ? digits.size() : rhs.digits.size();

	if (!byAbs(rhs))
	{
		std::vector< int > temp = rhs.digits;
		for (int i = 0; i < n; i++)
		{
			if (temp[i] < digits[i])
			{
				temp[i + 1] -= 1;
				temp[i] += 10;
			}
			temp[i] -= digits[i];
		}
		if (temp[n] == -1)
		{
			while (n + 1 < temp.size() && temp[n] == -1)
			{
				temp[n] += 10;
				temp[n + 1] -= 1;
				n++;
			}
		}
		minus = true;
		killLeadingZeroes(temp);
		LN res({ temp });
		res.minus = minus;
		return res;
	}

	std::vector< int > temp = digits;
	for (int i = 0; i < n; i++)
	{
		if (temp[i] < rhs.digits[i])
		{
			temp[i + 1] -= 1;
			temp[i] += 10;
		}
		temp[i] -= rhs.digits[i];
	}
	if (temp[n] == -1)
	{
		while (n + 1 < temp.size() && temp[n] == -1)
		{
			temp[n] += 10;
			temp[n + 1] -= 1;
			n++;
		}
	}
	killLeadingZeroes(temp);
	LN res({ temp });
	res.minus = minus;
	return res;
}

void killLeadingZeroes(std::vector< int > &rhs)
{
	while (rhs[rhs.size() - 1] == 0 && rhs.size() != 1)
		rhs.pop_back();
}

LN LN::operator+(LN &rhs)
{
	if (isNan(rhs))
	{
		LN temp = 0_ln;
		temp.NaN = true;
		return temp;
	}

	if (minus != rhs.minus)
	{
		if (byAbs(rhs))
			return plusImpl(rhs, false);

		else
			return plusImpl(rhs, true);
	}

	int n = digits.size() < rhs.digits.size() ? digits.size() : rhs.digits.size();
	int shift = 0;

	std::vector< int > temp = digits;

	for (int i = 0; i < n; i++)
	{
		temp[i] += (rhs.digits[i] + shift);
		shift = 0;
		if (temp[i] >= 10)
		{
			shift = 1;
			temp[i] %= 10;
		}
	}

	if (temp.size() >= rhs.digits.size())
	{
		while (shift != 0 && n < temp.size())
		{
			temp[n] += shift;
			shift = temp[n] / 10;
			temp[n] %= 10;
			n++;
		}
		if (shift != 0)
			temp.push_back(1);
	}
	else
	{
		while (temp.size() < rhs.digits.size())
		{
			temp.push_back(rhs.digits[n] + shift);
			shift = temp[n] / 10;
			temp[n] %= 10;
			n++;
		}
		if (shift != 0)
			temp.push_back(1);
	}

	LN res({ temp });
	res.minus = minus;
	return res;
}

LN& LN::operator++() {
	LN temp = *this;
	LN incr(1);
	*this = *this + incr;
	return *this;
}

LN LN::operator++(int)
{
	LN temp = *this;
	LN incr(1);
	*this = *this + incr;
	return temp;
}

LN& LN::operator--() {
	LN temp = *this;
	LN incr(1);
	*this = *this - incr;
	return *this;
}

LN LN::operator--(int) {
	LN temp = *this;
	LN incr(1);
	*this = *this - incr;
	return temp;
}


LN LN::operator*=(LN &rhs)
{
	return *this * rhs;
}

LN LN::operator+=(LN &rhs)
{
	return *this + rhs;
}

LN LN::operator-=(LN &rhs)
{
	return *this - rhs;
}

LN LN::operator/=(LN &rhs)
{
	return *this / rhs;
}
LN LN::operator%=(LN &rhs)
{
	return *this % rhs;
}
bool LN::isNan(LN &rhs)
{
	return NaN || rhs.NaN;
}
bool LN::byAbs(LN &rhs)
{
	if (digits.size() > rhs.digits.size())
		return true;
	else if (digits.size() == rhs.digits.size())

		for (int i = digits.size() - 1; i >= 0; i--)
		{
			if (digits[i] != rhs.digits[i])
				return digits[i] > rhs.digits[i];
		}

	else
		return false;
}
bool LN::isZeroes(LN &rhs)
{
	if (rhs.digits == digits && rhs.digits == std::vector{ 0 })
		return true;
}
// LN LN::operator=(LN &rhs) {
//     if (&rhs != this)
//     {
//         minus = rhs.minus;
//         NaN = rhs.NaN;
//         digits = rhs.digits;
//     }
//     return *this;
// }

LN operator""_ln(const char *C)
{
	return LN(C);
}

LN operator""_ln(unsigned long long C)
{
	return LN(C);
}
