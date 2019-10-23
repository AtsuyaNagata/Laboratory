#ifndef MYWINDOW_H_2019_10_03_
#define MYWINDOW_H_2019_10_03_

#include "DxLib.h"
#include "resource.h"

//�V���O���g���ɂ���
class myWindow
{
public:
	/*�V���O���g���Ɩ����Ȓ��Ԃ���*/
	static void create();
	static void destroy();
	static myWindow* instance() {
		return mWindow;
	}

	//��������{�i�I�ȃN���X�̎���
	void SampleWindow() {
		DrawPixel(320, 240, GetColor(255, 255, 255));	// �_��ł�

		WaitKey();				// �L�[���͑҂�
	}

	void updateWindow();

private:
	//�����őg�ݍ��ރE�B���h�E�v���V�[�W��
	static LRESULT CALLBACK MyProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	//�������ɕK�v�ȏ������
	int SettingBeforeInit(bool);
	int SettingAfterInit();
	//�E�B���h�E�̐���
	myWindow();
	~myWindow();

	static WNDPROC dxWndProc;		//�f�t�H���g�̃E�C���h�E�v���V�[�W���̊֐��|�C���^
	HWND hMainWnd;					//���C���E�B���h�E�̃n���h��

	static myWindow *mWindow;
};

#endif
