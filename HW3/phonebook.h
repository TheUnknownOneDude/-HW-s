//
// Created by 1 on 24.04.2022.
//

#include <iostream>

using namespace std;

#ifndef MAIN_CPP_PHONEBOOK_H
	#define MAIN_CPP_PHONEBOOK_H

class phonebook
{
  public:
	phonebook();

	string surname;
	string name;
	string patronymic;
	long long int number;

	phonebook(string surname, string name, string patronymic, long long int number);

	string show();

	phonebook create();
};

bool operator<(phonebook& left, phonebook& right);

bool operator>(phonebook& left, phonebook& right);

bool operator==(phonebook& left, phonebook& right);

#endif	  // MAIN_CPP_PHONEBOOK_H
