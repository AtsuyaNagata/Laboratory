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


//コンストラクタ
myWindow::myWindow() {		
	SettingBeforeInit(true);
	if (DxLib_Init()) {		//0の時成功
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
		DrawPixel(320, 240, GetColor(255, 255, 255));	// 点を打つ

		WaitKey();				// キー入力待ち

		ScreenFlip();			//裏画面と入れ替え
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

	//もし目的のメッセージが入ってなかったら元のプロシージャを実行
	return CallWindowProc((WNDPROC)dxWndProc, hWnd, msg, wp, lp);
}

int myWindow::SettingBeforeInit(bool wndMode) {
	//TRUE：ウィンドウモードで起動		FALSE：全画面モードで起動
	ChangeWindowMode(wndMode);
	if (wndMode) {
		//メニューIDの読み込み
		LoadMenuResource(IDR_MENU1);
	}
	//ウィンドウの大きさを可変にする
	if (SetWindowSizeChangeEnableFlag(true)) {
		//ここに入ったらエラー！
		exit(1);
	}

	return 0;
}

int myWindow::SettingAfterInit() {
	SetDrawScreen(DX_SCREEN_BACK);									//描画を裏画面にするよう設定
	SetGraphMode(640, 640, 32);										//ウィンドウサイズの変更
	hMainWnd = GetMainWindowHandle();								//mainウィンドウハンドルを受け取ってる
	dxWndProc = (WNDPROC)GetWindowLong(hMainWnd, GWLP_WNDPROC);		//一旦通常のプロセスを変数に入れてる...？
	SetWindowLong(hMainWnd, GWLP_WNDPROC, (LONG)MyProc);			//プロセスのすり替え

	return 0;
}
