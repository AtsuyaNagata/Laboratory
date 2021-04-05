#ifndef MIDIWINDOW_H_2020_1_09_
#define MIDIWINDOW_H_2020_1_09_

#include "DxLib.h"
#include "resource.h"

//�V���O���g���ɂ���
class MIDIWindow
{
public:
	/*�V���O���g���Ɩ����Ȓ��Ԃ���*/
	static void create();
	static void destroy();
	static MIDIWindow* instance() {
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
	MIDIWindow();
	~MIDIWindow();

	static WNDPROC dxWndProc;		//�f�t�H���g�̃E�C���h�E�v���V�[�W���̊֐��|�C���^
	HWND hMainWnd;					//���C���E�B���h�E�̃n���h��

	static MIDIWindow *mWindow;
};

#endif
