#ifndef OPENAL_H_2020_1_10_
#define OPENAL_H_2020_1_10_

/*"https://hatakenoko.hateblo.jp/entry/2018/05/24/220046"にあるコード*/
#define SAMPLINGRATE 44100
//ここでπを宣言
#define _USE_MATH_DEFINES

#include <al.h>
#include <alc.h>
#include <math.h>
#include <stdlib.h>

#pragma comment(lib, "OpenAL32.lib")

int playAL(void) {
	//OpenALの下準備　おまじない的な
	ALCdevice* device = alcOpenDevice(NULL);					//出力デバイスを指定(スピーカーなど)引数にNULLを入れるをデフォルトのスピーカーが出力先になる
	ALCcontext* context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);								//複数のソースから発生した音を一つにまとめて出力する宣言

	//ALuintは、OpenALの規格に合わせたデータをやり取りするのに用いられる変数
	ALuint buffer;		//バッファ(保存領域とか周波数とかサンプル当たりのビット数とかを管理してる)を宣言
	ALuint source;		//ソース(PLAYINGとかPAUSEDとかを管理してる)を宣言
	//それを生成
	alGenBuffers(1, &buffer);
	alGenSources(1, &source);

	//new演算子で音源を入れる配列を動的確保
	signed short* wav_data = new signed short[SAMPLINGRATE];
	//ここで440Hzのsin波を1秒間分生成
	for (int i = 0; i < SAMPLINGRATE; i++) {
		wav_data[i] = 32767 * sin(2 * M_PI * i * 440 / SAMPLINGRATE);		//(2π x i/SAMPLINGLATE) x 440、32767は16bitwavの最大サイズ
	}
	//バッファに音源データを入れる
	//第一引数：データを入れるバッファ、第二引数：データフォーマット（モノラル、ステレオ、8、16ビット）、第三引数：入れるデータ、第四引数：入れるデータのサイズ、第五引数：サンプリングレート
	alBufferData(buffer, AL_FORMAT_MONO16, wav_data, SAMPLINGRATE * sizeof(signed short), SAMPLINGRATE);
	//ソースにバッファを適用
	alSourcei(source, AL_BUFFER, buffer);
	//ループ再生をON
	alSourcei(source, AL_LOOPING, AL_TRUE);
	//ソースを再生！
	alSourcePlay(source);

	//ここで一時停止
	system("PAUSE");

	//動的確保したメモリを破棄
	delete[] wav_data;
	// バッファの破棄
	alDeleteBuffers(1, &buffer);
	// ソースの破棄
	alDeleteSources(1, &source);

	//OpenALの後始末
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
	return 0;
}

#endif // !OPENAL_H_2020_1_10
