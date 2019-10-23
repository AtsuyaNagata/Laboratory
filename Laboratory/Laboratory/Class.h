#ifndef CLASS_H_2019_8_24_
#define CLASS_H_2019_8_24_
#include<iostream>

//�m�F���������Ƃ�����΁A���̓s�xdefine���Ȃ��ƃ_��
#ifdef CLASS_SAMPLE_CHECK	//�N���X�̃R���X�g���N�^�̃`�F�b�N
class Sample {
public:
	Sample() :element(3) { printf("Sample() called\n"); }
	Sample(int n) :element(n) { printf("Sample(int n) called\n"); }
	~Sample() {}

	int element = 100;	//����100�̓X���[�����
};

void SampleClassCheck() {
	Sample a;
	//Sample b(); <- ���̏������ł͊֐��ƌ��Ȃ���邩�璍�ӂȁin�s�j
	Sample c = Sample();
	printf("a.element = %d\n", a.element);
}
#endif

#define STRUCT_IN_CLASS_CHECK
#ifdef STRUCT_IN_CLASS_CHECK	//�N���X�̒���Struct��錾�������̋����̃`�F�b�N
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
	} sampleStruct[2];	//sampleStruct[] �Ə����ƁA�Ȃ��̂΂��傪�Q�Ƃ����

};

//a�����������ėv�f���o�͂��邾���̊֐�
void InitializeStructCheck() {
	initializeStruct a;
	a.print();
}

#endif

#endif