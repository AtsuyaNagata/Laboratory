#ifndef RHYTHM_PATTERN_H_2021_H_
#define RHYTHM_PATTERN_H_2021_H_
#include <cstdint>
#include <vector>
using namespace std;

//メロディやコードのリズムパターンを生成してくれるクラス
class RhythmPattern
{
public:
	enum class DrumPattern : unsigned char {
		None,

		BasicDownBeat8,			//基本8ビート
		BasicUpBeat8,			//基本8ビート裏打ち(よくサビに用いられる)
		SlipBeat8,				//スリップ8ビート(スネアが8分音符裏にあったりするリズムパターン)
		ShuffleBeat,			//シャッフルビート(4拍×3を3拍×4のように考えるリズムパターン。2拍目と4拍目にバックビート(スネアの強)が入る)
		HalfTimeShuffle,		//ハーフタイムシャッフルビート(チタチ チタチ タタチ チタチ のリズムパターン)[ハーフタイムタイムは3拍目にバックビートが入る意味合い]
	};

	enum class RhythmType : unsigned char {
		None,

		Melody,
		BassDrum,
		CloseHiHatCymbal,
		OpenHiHatCymbal,
		SnareDrum,
		Tom,
		FloorTom,
		CrashCymbal,
		RideCymbal
	};

	//ある一つのリズム要素の位置を表す構造(メロディやドラムのパターンなどに使う予定)
	typedef struct Position {
		uint32_t length;			//音の長さ(256で四分音符一つ分を表現する)
		uint32_t startTime;			//音の開始地点(256で四分音符一つ分)
	}Position;

	typedef struct Rhythm {
		RhythmType type;			//リズムの種類
		vector<Position> position;		//リズムの位置の集合
	} Rhythm;

	//コンストラクタ
	RhythmPattern();
	~RhythmPattern();

	
	void createMelodyRhythm(int size, int mostSmall);									//現在は分割アルゴリズムによって生成するメロディリズムパターンが動作してる
	void createDrumRhythm(DrumPattern drumPattern, uint32_t length, bool fillIn);		//ドラムのパターンを生成する関数

	vector<Rhythm> getRhythmPattern() const {
		return mRhythm;
	}

private:
	void createSplitRhythmPattern_melody(int size, int mostSmall);		//第一引数：生成する音の長さのサイズ、第二引数：生成するパターンの最小音符

	vector<Rhythm> mRhythm;								//各Rhythm構造のlengthが一列に並んでいるイメージでリズムパターンを表現する
};

#endif