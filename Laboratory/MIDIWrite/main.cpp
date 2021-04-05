#include "MIDIWindow.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	MIDIWindow* win = MIDIWindow::instance();
	win->create();
	win->updateWindow();

	win->destroy();
	return 0;
}


//�����ɃE�B���h�E�{�̂̏���������
void MIDIWindow::updateWindow() {
	if (ProcessMessage()) {		//���b�Z�[�W�̊m�F
		//�G���[���f����Ă����痎�Ƃ�
		destroy();
		exit(1);
	}
	int i = 0;
	while (!ProcessMessage() && !ScreenFlip() && !ClearDrawScreen()) {
		DrawBox(100, 100, i, i, GetColor(255, 255, 255), TRUE);	// �_��ł�
		i++;
		//ScreenFlip();
		//WaitKey();				// �L�[���͑҂�
	}
	//�E�B���h�E������ꂽ���̏���
}