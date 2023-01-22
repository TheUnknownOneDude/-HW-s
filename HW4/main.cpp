#include "LN.h"
#include "fstream"
#include "return_codes.h"

#include <cstring>
#include <iostream>
#include <map>
#include <stack>

int main(int argc, char *args[])
{
	if (argc != 3)
	{
		std::cout << "The parameter (count of parameters) is incorrect";
		return ERROR_INVALID_PARAMETER;
	}

	std::map< std::string, int > operations;

	operations["+"] = 0;
	operations["-"] = 0;
	operations["*"] = 0;
	operations["/"] = 0;
	operations["%"] = 0;
	operations["~"] = 0;
	operations["_"] = 0;
	operations["<"] = 0;
	operations[">"] = 0;
	operations["<="] = 0;
	operations[">="] = 0;
	operations["=="] = 0;
	operations["!="] = 0;

	std::ifstream in(args[1]);

	if (!in.is_open())
	{
		std::cerr << "No file with such name";
		return ERROR_FILE_NOT_FOUND;
	}

//	std::stack< LN > stack;
	std::string num;
//	while (in >> num)
//	{
//		try
//		{
//			if (!operations.contains(num))
//			{
//				bool isDecr = false;
//				bool isIncr = true;
//				if (num[0] == '-' && num[1] == '-')
//				{
//					isDecr = true;
//				}
//
//				if (num[0] == '+' && num[1] == '+')
//				{
//					isIncr = true;
//				}
//
//				LN number(num);
//				if (isDecr)
//				{
//					--number;
//				}
//				if (isIncr)
//				{
//					++number;
//				}
//				try
//				{
//					stack.push(number);
//				} catch (std::bad_alloc &e)
//				{
//					std::cerr << "OUT OF MEMORY";
//					return ERROR_OUTOFMEMORY;
//				}
//			}
//			else if (num == "+")
//			{
//				LN temp = stack.top();
//				temp++; temp--;
//				stack.pop();
//				stack.top()++;
//				temp = stack.top() + temp;
//				stack.pop();
//				stack.push(temp);
//			}
//			else if (num == "-")
//			{
//				LN temp = stack.top();
//				stack.pop();
//				temp = stack.top() - temp;
//				stack.pop();
//				stack.push(temp);
//			}
//			else if (num == "*")
//			{
//				LN temp = stack.top();
//				if (!stack.empty())
//				{
//					stack.pop();
//					temp = stack.top() * temp;
//					stack.pop();
//					stack.push(temp);
//				}
//			}
//			else if (num == "/")
//			{
//				LN temp = stack.top();
//				stack.pop();
//				temp = stack.top() / temp;
//				stack.pop();
//				stack.push(temp);
//			}
//			else if (num == "%")
//			{
//				LN temp = stack.top();
//				stack.pop();
//				temp = stack.top() % temp;
//				stack.pop();
//				stack.push(temp);
//			}
//			else if (num == "~")
//			{
//				LN temp = stack.top();
//				stack.pop();
//				temp = temp.root();
//				stack.push(temp);
//			}
//			else if (num == "_")
//			{
//				LN temp = stack.top();
//				stack.pop();
//				temp = temp.negate();
//				stack.push(temp);
//			}
//			else if (num == "<")
//			{
//				LN temp = stack.top();
//				stack.pop();
//				if (stack.top() < temp)
//				{
//					stack.pop();
//					stack.push(1_ln);
//				}
//				else
//				{
//					stack.pop();
//					stack.push(0_ln);
//				}
//			}
//			else if (num == ">")
//			{
//				LN temp = stack.top();
//				stack.pop();
//				if (stack.top() > temp)
//				{
//					stack.pop();
//					stack.push(1_ln);
//				}
//				else
//				{
//					stack.pop();
//					stack.push(0_ln);
//				}
//			}
//			else if (num == "<=")
//			{
//				LN temp = stack.top();
//				stack.pop();
//				if (stack.top() <= temp)
//				{
//					stack.pop();
//					stack.push(1_ln);
//				}
//				else
//				{
//					stack.pop();
//					stack.push(0_ln);
//				}
//			}
//			else if (num == ">=")
//			{
//				LN temp = stack.top();
//				stack.pop();
//				if (stack.top() >= temp)
//				{
//					stack.pop();
//					stack.push(1_ln);
//				}
//				else
//				{
//					stack.pop();
//					stack.push(0_ln);
//				}
//			}
//			else if (num == "==")
//			{
//				LN temp = stack.top();
//				stack.pop();
//				if (stack.top() == temp)
//				{
//					stack.pop();
//					stack.push(1_ln);
//				}
//				else
//				{
//					stack.pop();
//					stack.push(0_ln);
//				}
//			}
//			else if (num == "!=")
//			{
//				LN temp = stack.top();
//				stack.pop();
//				if (stack.top() != temp)
//				{
//					stack.pop();
//					stack.push(1_ln);
//				}
//				else
//				{
//					stack.pop();
//					stack.push(0_ln);
//				}
//			}
//		} catch (std::bad_alloc &e)
//		{
//			std::cerr << "OUT OF MEMORY";
//			return ERROR_OUTOFMEMORY;
//		}
//	}
//
//	in.close();
//	std::ofstream out(args[2]);
//
//	if (!out.is_open())
//	{
//		std::cerr << "FILE IS NOT OPEN FOR SOME REASONS";
//		return ERROR_FILE_NOT_FOUND;
//	}
//	while (!stack.empty())
//	{
//		out << stack.top().toString() << "\n";
//		stack.pop();
//	}
//
//	out.close();


//	std::cin >> num;

	num = "7";
	LN res(num);
	LN one("1");

	++res;
	std::cout << res--.toString() << "\n";
	std::cout << res.toString() << "\n";

	LN pre = ++res + one;
	std::cout <<"res is: " << res.toString() << "\n";
	LN post = res++ + one;
	std::cout <<"res is: " << res.toString() << "\n";
	LN premin = res-- + one;
	std::cout <<"res is: " << res.toString() << "\n";
	LN postmin = --res + one;
	std::cout <<"res is: " << res.toString() << "\n";

	std::cout << pre.toString() << "\n";
	std::cout << post.toString() << "\n";
	std::cout << premin.toString() << "\n";
	std::cout << postmin.toString() << "\n";

	int erty = int(res);
	std::cout << erty << "\n";

	res = 32567_ln;
	int qwerty = int(res);
	std::cout << qwerty << "\n";

	return ERROR_SUCCESS;
}
