#ifndef NULL_PTR_H_2019_8_23_
#define NULL_PTR_H_2019_8_23_

class NULLptr {
public:
	NULLptr(int n) : np(0), element(n) {}

	NULLptr* np; //npにアクセスすると、例外nullptrを吐く
	int element;
};

#endif