#ifndef MUSIC_STRUCTURE_H_2021_07_06_
#define MUSIC_STRUCTURE_H_2021_07_06_
#include <vector>
#include <cstdint>
#include "Chords.h"
#include "RhythmPattern.h"
#include "Melody.h"
using namespace std;

//音楽構造を取り扱うクラス。生成処理も含む
class MusicStructure
{
public:
	enum class Scale : unsigned char {
		Major,
		Minor,
		HarmonicMinor,
		MelodicMinor
	};

	//曲のパートの種類を表現するための列挙子
	enum class Part : unsigned char {
		None,					//存在しないことを表す(初期値用)

		Intro,					//イントロ
		Interlude,				//間奏のこと
		Outro,					//アウトロ
		A,						//Aパート
		B,						//Bパート
		Climax,					//サビ
		C,						//Cパート

		Special					//Noneではない(存在はする)が、分類に含まれないパートを表す
	};

	//リズムパターンの種類を表現するための列挙
	enum class RhythmType : unsigned char {
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

	//メロディ生成アルゴリズムの種類の列挙
	enum class MelodyAlgorithm : unsigned char {
		None,

		Standard				//基本的な生成アルゴリズム
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
	enum class DrumType : unsigned char {
		None,

		BassDrum,				//インデックス値：0
		CloseHiHatCymbal,		//インデックス値：1
		OpenHiHatCymbal,		//インデックス値：2
		SnareDrum,				//インデックス値：3
		Tom,					//インデックス値：4
		FloorTom,				//インデックス値：5
		CrashCymbal,			//インデックス値：6
		RideCymbal,				//インデックス値：7

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

	enum class BaseAlgorithm : unsigned char {
		None,

		Simple,					//コードの基底音をひたすら流すだけ
		UpDown,					//コードの基底音を一オクターブ上の音と交互に鳴らす
		Melo					//メロディ風に遷移するベースラインを作る
	};


	typedef struct Note {
		uint8_t num;			//ノートの番号の配列
		uint32_t length;		//ノートの長さの配列(noteNumsとサイズは一致する)
		uint32_t startTime;			//ノートの開始時点の配列
	}Note;

	//曲のパートを表現する構造
	typedef struct PartStruct {
		Part type;							//パートの種類を格納する
		uint32_t startTimes;				//開始地点の時間を格納する(4分音符一つ分の長さを256にする)
		uint32_t length;					//パートの長さ
	} PartStruct;

	//あるドラム楽器についてのリズムパターンを格納する構造
	typedef struct DrumPattern {
		DrumType type;						//鳴らすドラムの種類
		vector<uint32_t> startTimes;		//各音の開始地点を格納する
		//各音は32分音符の長さの音として出力することにした
	};

	//あるパートのリズムパターンを表現する構造
	typedef struct RhythmStruct {
		RhythmType type;					//リズムの種類
		bool fill_in;						//最後にフィルインが入ってるかどうか
		vector<DrumPattern> drumPatterns;	//各ドラム楽器のリズムパターンを格納する構造
	}RhythmStruct;

	//あるパートのコード列についての情報
	typedef struct ChordStruct {
		ChordAlgorithm type;				//コード生成アルゴリズムの種類
		vector<Chords::Chord> chords;		//コード列
	} ChordStruct;

	//あるパートのメロディラインについての情報
	typedef struct MelodyStruct {
		MelodyAlgorithm type;				//メロディ生成アルゴリズムの種類
		vector<Note> notes;					//ノート情報
	}MelodyStruct;

	typedef struct BaseStruct {
		BaseAlgorithm type;							//ベースラインの種類
		vector<Note> notes;					//ノート情報
	};

	//オブリガートの構造
	typedef struct ObbligatoStruct {
		Obbligato type;						//オブリガードの種類を表す
		Instrument instrument;				//楽器の種類
		uint32_t startTime;					//開始地点
		vector<Note> notes;					//ノート情報
	}ObbligatoStruct;

	//曲の構造を示す設計図のような構造
	typedef struct MusicStruct {
		int key;							//曲全体のキー(0～11で表現される。0はC)
		Scale scale;						//スケールを表現する
		uint32_t length;					//曲全体の長さ
		vector<PartStruct> parts;			//パート情報の列を格納(構造完成時に時間順でソートされてることが前提となってる)
		vector<RhythmStruct> rhythms;		//リズム情報の列(パートに対応して並ぶ)
		vector<ChordStruct> chords;			//コード情報の列(パートに対応して並ぶ)
		vector<MelodyStruct> melodys;		//メロディ構造の配列(パートに対応して並ぶ)
		vector<BaseStruct> bases;			//ベース構造の配列(パートに対応して並ぶ)
		vector<ObbligatoStruct> obbligatos;	//オブリガードの列(パートに依存しない、数も任意)
	};

	//必要なデータ：キー、音楽のパート進行、区間の長さ、コードパターン(ギター、ピアノなど楽器に対応させる予定)、リズムパターン、メロディパターン、ベースライン
	MusicStructure(int key, Scale scale, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<RhythmType> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<BaseAlgorithm> baseAls);
	//設計図を受け取ってそれをそのまま使って初期化するコンストラクタ。多分使わない
	MusicStructure(const MusicStruct& mus);
	//空の要素を宣言するためのコンストラクタ。生成を行う際にはprepareメンバ関数を別で呼び出す必要がある
	MusicStructure();
	
	~MusicStructure();						//デストラクタ

	//引数から構造を構築する処理を行う
	void prepare(int key, Scale scale, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<RhythmType> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<BaseAlgorithm> baseAls);

	void create();							//曲を制作する(リズム、コード、メロディ、ベース)
	void addObbligato();					//現在の曲を参照しながら、指定されたオブリガートの追加を行う処理(を実装する予定)

	void printMusicStruct();				//曲構造を表示する関数

private:
	MusicStruct* mMusicStruct;				//曲の設計図
	RhythmPattern* mRhythmPattern;			//リズムパターンを生成、管理するオブジェクト
	Chords* mChords;						//コード列を生成、管理するオブジェクト
	Melody* mMelody;						//メロディを生成、管理するオブジェクト
};

#endif