#ifndef ENUM_SIZE_H_2019_11_21_
#define ENUM_SIZE_H_2019_11_21_
#include<iostream>
using namespace std;

void enumSize() {
	enum class tekito : unsigned char {
		aaa,
		bbb,
		ccc
	};
	enum class tekito2 : unsigned int {
		aaa,
		bbb,
		ccc
	};

	cout << sizeof(tekito) << endl;
	cout << sizeof(tekito2) << endl;
}

#endif