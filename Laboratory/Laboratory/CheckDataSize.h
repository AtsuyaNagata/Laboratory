#ifndef CHECK_DATA_SIZE_H_2019_09_09_
#define CHECK_DATA_SIZE_H_2019_09_09_

#include<iostream>

void checkDataSize() {
	int int_size = 0x80000000;
	size_t size_t_size = 0xffffffff;
	double double_size = 0xffffffffffffffff;
	float float_size = 0xffffffff;
	long long_size = 0xffffffff;
	long long long_long_size = 0xffffffffffffffff;

	printf("int:: sizeof : %d  fullNumber : %d\n", sizeof(int_size), int_size);
	printf("size_t_size:: sizeof : %d  fullNumber : %d\n", sizeof(size_t_size), size_t_size);
	printf("double_size:: sizeof : %lf  fullNumber : %lf\n", sizeof(double_size), double_size);
	printf("float_size:: sizeof : %f  fullNumber : %f\n", sizeof(float_size), float_size);
	printf("long_size:: sizeof : %d  fullNumber : %d\n", sizeof(long_size), long_size);
	printf("long_long_size:: sizeof : %d  fullNumber : %d\n", sizeof(long_long_size), long_long_size);


	unsigned int unsigned_int_size = 0xffffffff;
	unsigned long unsigned_long_size = 0xffffffff;
	unsigned long long unsigned_long_long_size = 0xffffffffffffffff;

	printf("unsigned_int_size:: sizeof : %u  fullNumber : %u\n", sizeof(unsigned_int_size), unsigned_int_size);
	printf("unsigned_long_size:: sizeof : %u  fullNumber : %u\n", sizeof(unsigned_long_size), unsigned_long_size);
	printf("unsigned_long_long_size:: sizeof : %llu  fullNumber : %llu\n", sizeof(unsigned_long_long_size), unsigned_long_long_size);
}

#endif