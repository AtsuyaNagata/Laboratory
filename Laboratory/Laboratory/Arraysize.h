#ifndef ARRAY_SIZE_H_2019_8_24_
#define ARRAY_SIZE_H_2019_8_24_

#include<iostream>
using namespace std;

void checkArraySize() {
	char p[10];
	char np[] = { '1', '2', '3' };
	//char *p = { '1', '2', '3' }; �Ə����ƃG���[��f��
	struct {
		int element;		//4�r�b�g
		const char* name;	//12�r�b�g
	} sp[] = { {10, "aaa"},{11, "bbb"} };	// *2 ��32�r�b�g

	//�X�g���N�g�̓��e�������Ă邩�̃`�F�b�N
	printf("%d\n", sp[0].element);
	printf("%s\n", sp[1].name);

	//���ꂼ��̃T�C�Y�̃`�F�b�N
	cout << "size char p[10] " << sizeof(p) << endl;
	cout << "size char np[10] " << sizeof(np) << endl;
	cout << "size char sp[10] " << sizeof(sp) << endl;

	char* sp_c = reinterpret_cast<char*>(sp);
	//�X�g���N�g�̓��e����{�Ƃ��m�F
	for (int i = 0; i < sizeof(sp); ++i) {
		printf("%d : ", sp_c[i]);
	}
}

#endif