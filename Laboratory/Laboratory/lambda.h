#ifndef LAMBDA_H_2020_1_24_
#define LAMBDA_H_2020_1_24_
#include<iostream>
using namespace std;

void lambdaIndex() {
	int a[3]{0, 1, 2};
	//cout << a[[]{return 1; }()] << std::endl;	//�G���[�I
	cout << []{return 1;}()[a] << std::endl;	//1���o�͂����
}

#endif // !LAMBDA_H_2020_1_24_
