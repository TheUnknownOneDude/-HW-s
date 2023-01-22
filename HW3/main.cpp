#include "phonebook.h"
#include "quicksort.h"
#include "return_codes.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// template <typename T>
// int pivot(T& args, const T& size) {     // тут будет (а может и не будет)
//     if (size != 1) {
//         std::vector<T> lesser;
//         std::vector<T> greater;
//         T pivot = args[(std::begin(args) + std::end(args)) / 2];
//         for (int i = 0; i < size; i++) {
//             if (args[i] <= pivot) {
//                 lesser.push_back(args[i]);
//             } else {
//                 greater.push_back(args[i]);
//             }
//         }
//     }
// };
template< typename T >
void closer(T *a, ifstream &in, ofstream &out)
{
	delete[] a;
	in.close();
	out.close();
}


struct backlog
{
	float *f;
	char *c;
	long l;

	bool operator>(backlog &rhs)
	{
		if (*f > *rhs.f)
		{
			return true;
		}
		else if (*f < *rhs.f)
		{
			return false;
		}
		if (*c > *rhs.c)
		{
			return true;
		}
		else if (*c < *rhs.c)
		{
			return false;
		}
		if (l > rhs.l)
		{
			return true;
		}
		else if (l < rhs.l)
		{
			return false;
		}
		return false;
	}

	bool operator<(backlog &rhs) { return !(*this > rhs) && !(*this == rhs); }

	bool operator==(backlog &rhs) { return *f == *rhs.f && *c == *rhs.c && l == rhs.l; }

	string show()
	{
		string a;
		a += to_string(*f) + " ";
		for (int i = 0; i < strlen(c); i++)
		{
			a += c[i];
		}
		a += " " + to_string(l) + '\n';
		return a;
	}
};

int main(int argc, char *args[])
{
	// phonebook a = phonebook("Prasolov", "Denis", "Vitalievich", 79132681237);
	if (argc != 3)
	{
		cout << "The parameter (count of parameters) is incorrect";
		return ERROR_INVALID_PARAMETER;
	}

	std::ifstream in(args[1]);

	if (!in.is_open())
	{
		cout << "No file with such name";
		return ERROR_FILE_NOT_FOUND;
	}

	std::ofstream out(args[2]);
	int n;
	std::string type, order;
	bool descending;
	in >> type >> order >> n;

	if (order == "descending")
	{
		descending = true;
	}
	else if (order == "ascending")
	{
		descending = false;
	}
	else
	{
		cout << "Unsupported functional";
		return ERROR_NOT_IMPLEMENTED;
	}

	if (type == "int")
	{
		int *numbers = new (nothrow) int[n];

		if (numbers == nullptr)
		{
			closer(numbers, in, out);
			cout << "Not enough memory to allocate";
			return ERROR_NOT_ENOUGH_MEMORY;
		}

		for (int i = 0; i < n; i++)
		{
			in >> numbers[i];
		}

		if (descending)
		{
			quicksort< int, true >(numbers, 0, n - 1);
		}
		else
		{
			quicksort< int, false >(numbers, 0, n - 1);
		}

		for (int i = 0; i < n; i++)
		{
			out << numbers[i] << '\n';
		}
		closer(numbers, in, out);
		return ERROR_SUCCESS;
	}
	else if (type == "float")
	{
		auto *floats = new (nothrow) float[n];

		if (floats == nullptr)
		{
			closer(floats, in, out);
			cout << "Not enough memory to allocate";
			return ERROR_NOT_ENOUGH_MEMORY;
		}

		for (int i = 0; i < n; i++)
		{
			in >> floats[i];
		}

		if (descending)
		{
			quicksort< float, true >(floats, 0, n - 1);
		}
		else
		{
			quicksort< float, false >(floats, 0, n - 1);
		}

		for (int i = 0; i < n; i++)
		{
			out << floats[i] << '\n';
		}
		closer(floats, in, out);
		return ERROR_SUCCESS;
	}
	else if (type == "phonebook")
	{
		auto *words = new (nothrow) phonebook[n];

		if (words == nullptr)
		{
			closer(words, in, out);
			cout << "Not enough memory to allocate";
			return ERROR_NOT_ENOUGH_MEMORY;
		}
		string surname;
		string name;
		string patronymic;
		long long int number;
		int i = 0;

		while (in >> surname >> name >> patronymic >> number)
		{
			words[i] = phonebook(surname, name, patronymic, number);
			i++;
		}

		if (descending)
		{
			quicksort< phonebook, true >(words, 0, n - 1);
		}
		else
		{
			quicksort< phonebook, false >(words, 0, n - 1);
		}

		for (int i = 0; i < n; i++)
		{
			out << words[i].show();
		}
		closer(words, in, out);
		return ERROR_SUCCESS;
	}
	else if (type == "backlog")
	{
		auto *logs = new (nothrow) backlog[n];
		auto *floatArray = new (nothrow) float[n];
		auto *words = new (nothrow) string[n];

		if (logs == nullptr || floatArray == nullptr || words == nullptr)
		{
			closer(logs, in, out);
			delete[] floatArray;
			delete[] words;
			cout << "Not enough memory to allocate";
			return ERROR_NOT_ENOUGH_MEMORY;
		}
		string f;
		string c;
		string l;
		int i = 0;

		//ввод
		while (in >> f >> c >> l)
		{
			floatArray[i] = stof(f);
			logs[i].f = &floatArray[i];
			words[i] = c;
			logs[i].c = &words[i][0];
			logs[i].l = stol(l);
			i++;
		}

		if (descending)
		{
			quicksort< backlog, true >(logs, 0, n - 1);
		}
		else
		{
			quicksort< backlog, false >(logs, 0, n - 1);
		}

		for (int i = 0; i < n; i++)
		{
			out << logs[i].show();
		}
		delete[] floatArray;
		delete[] words;
		closer(logs, in, out);
		return ERROR_SUCCESS;
	}
	else
	{
		in.close();
		out.close();
		cout << "Unsupported functional";
		return ERROR_NOT_IMPLEMENTED;
	}
}
