//
// Created by 1 on 24.04.2022.
//

#include "phonebook.h"
#include <string>

phonebook::phonebook(string surname, string name, string patronymic, long long int number)
{
	this->surname = surname;
	this->name = name;
	this->patronymic = patronymic;
	this->number = number;
}

string phonebook::show()
{
	string a = surname + " " + name + " " + patronymic + " " + to_string(number) + '\n';
	return a;
}

phonebook::phonebook()
{
	this->surname = "";
	this->name = "";
	this->patronymic = "";
	this->number = 0;
}

phonebook phonebook::create()
{
	phonebook a = phonebook();
	return a;
}

bool operator<(phonebook& left, phonebook& right)
{
	if (left.surname < right.surname)
	{
		return true;
	}
	else if (left.surname > right.surname)
	{
		return false;
	}
	if (left.name < right.name)
	{
		return true;
	}
	else if (left.name > right.name)
	{
		return false;
	}
	if (left.patronymic < right.patronymic)
	{
		return true;
	}
	else if (left.patronymic > right.patronymic)
	{
		return false;
	}
	if (left.number < right.number)
	{
		return true;
	}
	else if (left.number > right.number)
	{
		return false;
	}
	return false;
}

bool operator==(phonebook& left, phonebook& right)
{
	return left.surname == right.surname & left.name == right.name & left.patronymic == right.patronymic &
		   left.number == right.number;
}

bool operator>(phonebook& left, phonebook& right)
{
	return !(left < right) & !(left == right);
}
