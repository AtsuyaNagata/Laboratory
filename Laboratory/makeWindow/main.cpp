#include "myWindow.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	myWindow* win = myWindow::instance();
	win->create();
	win->updateWindow();

	win->destroy();
	return 0;
}