#ifndef MYWINDOW_H_2019_10_03_
#define MYWINDOW_H_2019_10_03_

#include "DxLib.h"
#include "resource.h"

//シングルトンにする
class myWindow
{
public:
	/*シングルトンと愉快な仲間たち*/
	static void create();
	static void destroy();
	static myWindow* instance() {
		return mWindow;
	}

	//ここから本格的なクラスの実装
	void SampleWindow() {
		DrawPixel(320, 240, GetColor(255, 255, 255));	// 点を打つ

		WaitKey();				// キー入力待ち
	}

	void updateWindow();

private:
	//自分で組み込むウィンドウプロシージャ
	static LRESULT CALLBACK MyProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	//初期化に必要な処理二つ
	int SettingBeforeInit(bool);
	int SettingAfterInit();
	//ウィンドウの生成
	myWindow();
	~myWindow();

	static WNDPROC dxWndProc;		//デフォルトのウインドウプロシージャの関数ポインタ
	HWND hMainWnd;					//メインウィンドウのハンドル

	static myWindow *mWindow;
};

#endif
