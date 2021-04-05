#ifndef LAMBDA_H_2020_1_24_
#define LAMBDA_H_2020_1_24_
#include<iostream>
using namespace std;

void lambdaIndex() {
	int a[3]{0, 1, 2};
	//cout << a[[]{return 1; }()] << std::endl;	//エラー！
	cout << []{return 1;}()[a] << std::endl;	//1が出力される
}

#endif // !LAMBDA_H_2020_1_24_
