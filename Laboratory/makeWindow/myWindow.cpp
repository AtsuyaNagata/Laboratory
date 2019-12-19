#include <DxLib.h>
#include <iostream>
#include <windows.h>
#include "myWindow.h"

myWindow* myWindow::mWindow = nullptr;
WNDPROC myWindow::dxWndProc = nullptr;

void myWindow::create() {
	if (mWindow) {
		fprintf(stderr, "Create called two times!!\n");
		exit(1);
	}

	mWindow = new myWindow();
}

void myWindow::destroy() {
	delete mWindow;
	mWindow = 0;
}


//�R���X�g���N�^
myWindow::myWindow() {		
	SettingBeforeInit(true);
	if (DxLib_Init()) {		//0�̎�����
		exit(1);
	}
	SettingAfterInit();

}

myWindow::~myWindow() {
	DxLib_End();
}

void myWindow::updateWindow() {
	if (ProcessMessage()) {
		destroy();
		exit(1);
	}
	while (!ProcessMessage() && !ScreenFlip() && !ClearDrawScreen()) {
		DrawPixel(320, 240, GetColor(255, 255, 255));	// �_��ł�

		WaitKey();				// �L�[���͑҂�

		ScreenFlip();			//����ʂƓ���ւ�
	}
}


LRESULT CALLBACK myWindow::MyProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDM_END:
			SendMessage(hWnd, WM_CLOSE, NULL, NULL);
			break;
		}
		break;
	}

	//�����ړI�̃��b�Z�[�W�������ĂȂ������猳�̃v���V�[�W�������s
	return CallWindowProc((WNDPROC)dxWndProc, hWnd, msg, wp, lp);
}

int myWindow::SettingBeforeInit(bool wndMode) {
	//TRUE�F�E�B���h�E���[�h�ŋN��		FALSE�F�S��ʃ��[�h�ŋN��
	ChangeWindowMode(wndMode);
	if (wndMode) {
		//���j���[ID�̓ǂݍ���
		LoadMenuResource(IDR_MENU1);
	}
	//�E�B���h�E�̑傫�����ςɂ���
	if (SetWindowSizeChangeEnableFlag(true)) {
		//�����ɓ�������G���[�I
		exit(1);
	}

	return 0;
}

int myWindow::SettingAfterInit() {
	SetDrawScreen(DX_SCREEN_BACK);									//�`��𗠉�ʂɂ���悤�ݒ�
	SetGraphMode(640, 640, 32);										//�E�B���h�E�T�C�Y�̕ύX
	hMainWnd = GetMainWindowHandle();								//main�E�B���h�E�n���h�����󂯎���Ă�
	dxWndProc = (WNDPROC)GetWindowLong(hMainWnd, GWLP_WNDPROC);		//��U�ʏ�̃v���Z�X��ϐ��ɓ���Ă�...�H
	SetWindowLong(hMainWnd, GWLP_WNDPROC, (LONG)MyProc);			//�v���Z�X�̂���ւ�

	return 0;
}
