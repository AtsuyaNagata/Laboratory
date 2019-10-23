#ifndef ASCII_H_2019_8_23_
#define ASCII_H_2019_8_23_

#include<iostream>
using namespace std;

void printAscii() {
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			printf("%c ", 16 * i + j);
		}
		printf("\n");
	}
}

void print_zero() {
	printf("%d\n", '\0');
}

#endif