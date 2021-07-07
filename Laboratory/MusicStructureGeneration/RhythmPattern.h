#ifndef RHYTHM_PATTERN_H_2021_H_
#define RHYTHM_PATTERN_H_2021_H_
#include <cstdint>
#include <vector>
using namespace std;

//メロディやコードのリズムパターンを生成してくれるクラス
class RhythmPattern
{
public:
	//ある一つのリズム要素を表す構造(メロディやドラムのパターンなどに使う予定)
	typedef struct Rhythm {
		uint32_t length;			//音の長さ(256で四分音符一つ分を表現する)
		uint32_t startTime;			//音の開始地点(256で四分音符一つ分)
		bool on;					//休符か音符かの判定
	} Rhythm;

	//コンストラクタ
	RhythmPattern();
	~RhythmPattern();

	//分割アルゴリズムによって生成するリズムパターン
	void create(int size, int mostSmall);
	void createSplitRhythmPattern(int size, int mostSmall);		//第一引数：生成する音の長さのサイズ、第二引数：生成するパターンの最小音符

	vector<Rhythm> getRhythmPattern() {
		return mRhythmPattern;
	}

private:
	vector<Rhythm> mRhythmPattern;								//各Rhythm構造のlengthが一列に並んでいるイメージでリズムパターンを表現する
};

#endif