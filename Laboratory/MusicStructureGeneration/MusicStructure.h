#ifndef MUSIC_STRUCTURE_H_2021_07_06_
#define MUSIC_STRUCTURE_H_2021_07_06_
#include <vector>
#include <cstdint>
#include "Chords.h"
#include "RhythmPattern.h"
using namespace std;

class Melody;

//音楽構造を取り扱うクラス。生成処理も含む
class MusicStructure
{
public:
	//必要なデータ：音楽のパート進行、区間の長さ、コードパターン(ギター、ピアノなどに対応させる)、リズムパターン、メロディパターン、ベースライン
	MusicStructure();
	~MusicStructure();

	//曲のパートの種類を表現するための列挙子
	enum class Part : unsigned char {
		None,			//存在しないことを表す(初期値用)

		Intro,			//イントロ
		Interlude,		//間奏のこと
		Outro,			//アウトロ
		A,				//Aパート
		B,				//Bパート
		Climax,			//サビ
		C,				//Cパート

		Special			//Noneではない(存在はする)が、分類に含まれないパートを表す
	};

	//リズムパターンの種類を表現するための列挙
	enum class Rhythm : unsigned char {
		None,

		BasicDownBeat8,			//基本8ビート
		BasicUpBeat8,			//基本8ビート裏打ち(よくサビに用いられる)
		SlipBeat8,				//スリップ8ビート(スネアが8分音符裏にあったりするリズムパターン)
		ShuffleBeat,			//シャッフルビート(4拍×3を3拍×4のように考えるリズムパターン。2拍目と4拍目にバックビート(スネアの強)が入る)
		HalfTimeShuffle,		//ハーフタイムシャッフルビート(チタチ チタチ タタチ チタチ のリズムパターン)[ハーフタイムタイムは3拍目にバックビートが入る意味合い]
	};

	//コード生成アルゴリズムの種類の列挙
	enum class ChordAlgorithm : unsigned char {
		None,
		
		Simple,					//シンプルな生成
		Fashionable,			//オシャレに生成
		Modulation				//転調を行うように生成
	};

	//オブリガードの種類の列挙。メロディを引き立たせるためのメロディアスなパートのこと
	enum class Obbligato : unsigned char{
		None,

		Fake,					//メロディ・ラインの雰囲気を保ちながら鳴る装飾
		FillIn,					//メロディの継目に出現する即興的なメロ(ドラムで大体済む)おかずとも呼ばれる
		Harmony,				//メロディ・ラインにハモるメロ
		Riff					//モチーフを更に短くしたような要素、オブリガードとして持っておく
	};
	
	//ドラムに含まれる楽器の種類の列挙
	enum class Drum : unsigned char {
		None,

		BassDrum,
		HiHatCymbal,
		SnareDrum,
		Tom,
		FloorTom,
		CrashCymbal,
		RideCymbal,

		Other					//特殊な音等を入れる際に用いる(現在は考えない)
	};

	//楽器の種類の列挙
	enum class Instrument : unsigned char {
		None,

		Piano,
		Guiter,
		Synthesizer,
		AudioMaterial			//音声素材といった意味合い
	};

	//曲のパートを表現する構造
	typedef struct PartStruct {
		Part part;							//パートの種類を格納する
		uint32_t startTimes;				//開始地点の時間を格納する(4分音符一つ分の長さを256にする)
		uint32_t length;					//パートの長さ
	} PartStruct;

	//あるドラム楽器についてのリズムパターンを格納する構造
	typedef struct DrumPattern {
		Drum drum;							//鳴らすドラムの種類
		vector<uint32_t> startTimes;		//各音の開始地点を格納する
	};

	//あるパートのリズムパターンを表現する構造
	typedef struct RhythmStruct {
		Rhythm rhythm;						//リズムの種類
		bool fill_in;						//最後にフィルインが入ってるかどうか
		vector<DrumPattern> drumPatterns;	//各ドラム楽器のリズムパターンを格納する構造
	}RhythmStruct;

	//あるパートのコード列についての情報
	typedef struct ChordStruct {
		ChordAlgorithm chordAlgorithm;		//コード生成アルゴリズム
		vector<Chords::Chord> chords;		//コード列
		vector<uint32_t> lengths;			//各コードの長さ
	} ChordStruct;

	//あるパートのメロディラインについての情報
	typedef struct MelodyStruct {
		bool breakIn;						//ブレイク(メロディを引き立たせるために一部消音する部分)を入れてるかどうか
		vector<uint8_t> noteNums;			//ノートの番号の列
		vector<uint32_t> noteLengths;		//ノートの長さの列(noteNumsとサイズは一致する)
	}MelodyStruct;

	//オブリガードの構造
	typedef struct ObbligatoStruct {
		Obbligato type;						//オブリガードの種類を表す
		Instrument instrument;				//楽器の種類
		int startTime;						//開始地点
		vector<uint8_t> noteNums;			//ノートの番号の列
		vector<uint32_t> noteLengths;		//ノートの長さの列(noteNumsとサイズは一致する)
	}ObbligatoStruct;

	//曲の構造を示す設計図のような構造
	typedef struct MusicStruct {
		int key;							//曲全体のキー(0～11で表現される。0はC)
		uint32_t length;					//曲全体の長さ
		vector<PartStruct> part;			//パート情報の列を格納(構造完成時に時間順でソートされてることが前提となってる)
		vector<RhythmStruct> rhythm;		//リズム情報の列(パートに対応して並ぶ)
		vector<ChordStruct> chord;			//コード情報の列(パートに対応して並ぶ)
		vector<ObbligatoStruct> obbligatos;	//オブリガードの列(パートに依存しない、数も任意)
	};

	void create();

private:

};

#endif