#ifndef AUTO_H_2019_11_18_
#define AUTO_H_2019_11_18_

#include<iostream>

template<class T>
class dummy;

template<typename T, typename U>
auto add(const T& lhs, const U* rhs)
	-> decltype(lhs + rhs);

void printType()
{
	auto intType = 10;
	const auto intType2 = intType;
	auto&& intType3 = 100; 
	//エラーを吐かせて型情報をとる
	/*dummy<decltype(intType)> x;
	dummy<decltype(intType2)> y;
	dummy<decltype(intType3)> z;*/

	std::cout << typeid(intType).name() << std::endl;
	std::cout << typeid(intType2).name() << std::endl;
	std::cout << typeid(intType3).name() << std::endl;

}

#endif