#ifndef CLASS_H_2019_8_24_
#define CLASS_H_2019_8_24_
#include<iostream>

//確認したいことがあれば、その都度defineしないとダメ
#ifdef CLASS_SAMPLE_CHECK	//クラスのコンストラクタのチェック
class Sample {
public:
	Sample() :element(3) { printf("Sample() called\n"); }
	Sample(int n) :element(n) { printf("Sample(int n) called\n"); }
	~Sample() {}

	int element = 100;	//この100はスルーされる
};

void SampleClassCheck() {
	Sample a;
	//Sample b(); <- この書き方では関数と見なされるから注意な（n敗）
	Sample c = Sample();
	printf("a.element = %d\n", a.element);
}
#endif

#define STRUCT_IN_CLASS_CHECK
#ifdef STRUCT_IN_CLASS_CHECK	//クラスの中にStructを宣言した時の挙動のチェック
class initializeStruct {
public:
	initializeStruct():
		sampleStruct{ 
			{10,"aaa"}, 
			{1, "bbb"} 
		}
	{
		printf("initializeStruc is initialized\n");
	}

	void print() {
		//printf("sizeof(sampleStruct) is %d\n", sizeof(sampleStruct));
		for (int i = 0; i < 2; ++i) {
			printf("element[%d] = %d\n", i, sampleStruct[i].element);
			printf("name[%d] = %s\n", i, sampleStruct[i].name);
		}
	}

private:
	struct{
		int element;
		const char* name;
	} sampleStruct[2];	//sampleStruct[] と書くと、なぞのばしょが参照される

};

//aを初期化して要素を出力するだけの関数
void InitializeStructCheck() {
	initializeStruct a;
	a.print();
}

#endif

#endif