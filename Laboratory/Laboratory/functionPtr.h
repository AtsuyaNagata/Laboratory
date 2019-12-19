#ifndef FUNCTION_POINTER_H_2019_8_25_
#define FUNCTION_POINTER_H_2019_8_25_

#include<iostream>

void funcptr() { printf("funcptr is called\n"); return; }
void const void_const_func() { printf("void const function called\n"); return; }
void const const_void_func() { printf("const void function called\n"); return; }

void usefuncptr() {
	void const (* const func)() = void_const_func;
	func();
	//func = const_void_func;	エラー！！変更できないことを確認！
}

void* return_funcptr() {
	return (void*)funcptr;
}

void check_return_funcptr() {
	void (*func)() = (void (*)())return_funcptr();
	func();

	return;
}

#endif