#include "MIDIWindow.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	MIDIWindow* win = MIDIWindow::instance();
	win->create();
	win->updateWindow();

	win->destroy();
	return 0;
}


//ここにウィンドウ本体の処理を書く
void MIDIWindow::updateWindow() {
	if (ProcessMessage()) {		//メッセージの確認
		//エラーが吐かれていたら落とす
		destroy();
		exit(1);
	}
	int i = 0;
	while (!ProcessMessage() && !ScreenFlip() && !ClearDrawScreen()) {
		DrawBox(100, 100, i, i, GetColor(255, 255, 255), TRUE);	// 点を打つ
		i++;
		//ScreenFlip();
		//WaitKey();				// キー入力待ち
	}
	//ウィンドウが閉じられた時の処理
}