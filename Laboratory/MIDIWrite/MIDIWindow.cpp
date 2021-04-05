#include <DxLib.h>
#include <iostream>
#include <windows.h>
#include "MIDIWindow.h"

MIDIWindow* MIDIWindow::mWindow = nullptr;
WNDPROC MIDIWindow::dxWndProc = nullptr;

void MIDIWindow::create() {
	if (mWindow) {
		fprintf(stderr, "Create called two times!!\n");
		exit(1);
	}

	mWindow = new MIDIWindow();
}

void MIDIWindow::destroy() {
	DxLib_End();
	delete mWindow;
	mWindow = 0;
}


//�R���X�g���N�^
MIDIWindow::MIDIWindow() {		
	SettingBeforeInit(true);
	if (DxLib_Init()) {		//0�̎�����
		exit(1);
	}
	SettingAfterInit();

}

MIDIWindow::~MIDIWindow() {
	DxLib_End();
}


LRESULT CALLBACK MIDIWindow::MyProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
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

int MIDIWindow::SettingBeforeInit(bool wndMode) {
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

int MIDIWindow::SettingAfterInit() {
	SetDrawScreen(DX_SCREEN_BACK);									//�`��𗠉�ʂɂ���悤�ݒ�
	SetGraphMode(960, 640, 32);										//�E�B���h�E�T�C�Y�̕ύX
	hMainWnd = GetMainWindowHandle();								//main�E�B���h�E�n���h�����󂯎���Ă�
	dxWndProc = (WNDPROC)GetWindowLong(hMainWnd, GWLP_WNDPROC);		//��U�ʏ�̃v���Z�X��ϐ��ɓ���Ă�...�H
	SetWindowLong(hMainWnd, GWLP_WNDPROC, (LONG)MyProc);			//�v���Z�X�̂���ւ�

	return 0;
}
