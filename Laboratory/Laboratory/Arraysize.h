#ifndef ARRAY_SIZE_H_2019_8_24_
#define ARRAY_SIZE_H_2019_8_24_

#include<iostream>
using namespace std;

void checkArraySize() {
	char p[10];
	char np[] = { '1', '2', '3' };
	//char *p = { '1', '2', '3' }; と書くとエラーを吐く
	struct {
		int element;		//4ビット
		const char* name;	//12ビット
	} sp[] = { {10, "aaa"},{11, "bbb"} };	// *2 で32ビット

	//ストラクトの内容が入ってるかのチェック
	printf("%d\n", sp[0].element);
	printf("%s\n", sp[1].name);

	//それぞれのサイズのチェック
	cout << "size char p[10] " << sizeof(p) << endl;
	cout << "size char np[10] " << sizeof(np) << endl;
	cout << "size char sp[10] " << sizeof(sp) << endl;

	char* sp_c = reinterpret_cast<char*>(sp);
	//ストラクトの内容を一倍とずつ確認
	for (int i = 0; i < sizeof(sp); ++i) {
		printf("%d : ", sp_c[i]);
	}
}

#endif