#ifndef CHORDS_H_2021_05_06_
#define CHORDS_H_2021_05_06_

#include <cstdint>
#include <vector>
using namespace std;

//コード列を生成するオブジェクトクラス
class Chords
{
public:
	enum class Algorithm : unsigned char {
		Simple,
		Fashionable,
		modulation
	};

	enum class Scale : unsigned char {
		None,

		Major,
		Minor,
		HarmonicMinor,
		MelodicMinor
	};

	//----------コードの種類の識別子----------
	//基本的な3和音によるコードの種類
	enum class ChordType : unsigned char {
		None,							//「コードなし」を表現する(現在は発生しないはず)

		Major,							//メジャーコードを表す
		Minor,							//マイナーコードを表す
		Sus4,							//3度の音を上げて完全四度(P4)にする(基底音から5つ上)
		Sus2,							//3度の音を下げて長二度(M2)にする(基底音から2つ上)
		Aug,							//メジャーコードの完全5度(P5)を半音上げる(基底音から8つ上)->クリシェ(一音だけ変えていく進行)に使う
		mb5,							//マイナーフラットファイブを表す(マイナーコードの完全5度を半音下げた音)
	};

	//コードの種類のうち、4音目を用いる時の付加情報を表す
	enum class ChordPlus : unsigned char {
		None,							//付加なし

		Seventh,						//セブンス(短7度)
		MajorSeventh,					//メジャーセブンス(長7度)
		Six,							//長6度の音を付加する(マイナーっぽい雰囲気をだせる)
		Add9,							//9度(2度)の音を加える
		Add11,							//11度(4度)の音を加える
		Add13,							//13度(6度)の音を加える
		Dim7,							//m♭5のときのみ付加可能、7thを半音下げた音を加える
	};

	//5つ目の音を付加する時のパターン
	enum class ChordTension : unsigned char {
		None,							//付加なし

		Ninth,							//9th	(長9度)
		FlatNinth,						//♭9th	(短9度)
		SharpNinth,						//#9th	(増9度[短10度])
		Eleventh,						//11th	(完全11度)
		SharpEleventh,					//#11th	(増11度)
		Thirteenth,						//13th	(長13度)
		FlatThirteenth,					//♭13th(短13度)
	};
	//---------------------------------------

	//================7CM理論================
	/*
	//束を表現した列挙子
	enum class Bundle : unsigned char {

	};
	*/
	/*typedef struct SevenCM {
	* uint8_t simpleFrequencies;		//簡易度数類(0～6で7つの音を表現する)
	}SevenCM;
	*/

	typedef struct Chord {
		ChordType type;					//3和音のコードの種類
		ChordPlus plus;					//4つの音を用いるコードの付加情報を表す
		ChordTension tension;			//5つの音を用いるときの付加情報を表す
		uint8_t baseNoteNum;			//基底の音(オンコードは一旦考えず、元のコードの基底音を指す 例：キーがCの時のC/G -> 60 + 4 = 64でなく 60 + 0 = 60[GでなくC])
		uint8_t baseIndex;				//キーから何番目が基底となっているかを表す(値の範囲：0～11)
		uint8_t onIndex;				//オンコード (基底音が違うコード 例:C/G ベースラインを常に上昇させたい時などに使う) の場合に基底となる音を指定するためのインデックス値(値の範囲：0～11)
		uint8_t omitIndex;				//Omitコード (コード内の特定の音を一音消したコード) (0～12 12以上の値は無効な値を表すことにする)
		uint32_t length;				//コードの長さ(四分音符一つ分：256)
	}Chord;

	//コンストラクタ
	Chords();
	//デストラクタ
	~Chords();

	//コードを生成する関数
	void create(int chordNum, uint32_t length, int key, Scale scale, Algorithm algorithm);			//生成するコードの数、生成するコードの長さ(256で四分音符一つ分の長さ)、キー、スケールを引数に取る
	void create7CM(int barNum);										//7CM理論を用いてコードを生成するアルゴリズム

	const vector<Chord> getChords() const {
		return mChords;
	}
	const int* getFloor() const {
		return mFloor;
	}
	const int getBaseNoteNum() const {
		return mBaseNoteNum;
	}
	const uint32_t getLength() const {
		return mLength;
	}
	

private:
	//生成の際に必要なメンバ
	vector<Chord> mChords;				//コード列を表現する配列
	Scale mScale;
	int* mFloor;						//階名を表現する配列が格納される(サイズは7で固定)
	int mBaseNoteNum;					//生成するコードの基準となるオクターブのCの高さが格納される
	int mChordNum;						//生成するコードの数を格納する(mChordsが完成する前に必要なものとして宣言されたメンバ)
	uint32_t mLength;					//コード列全体の長さを格納する
	Algorithm mAlgorithm;				//コードの生成アルゴリズムを決定するための列挙子

	//コードの生成に正と負の傾きを表す値を設けてみるとどうなる？

	//アルゴリズムに用いる便利な関数群
	Chord makeSimpleChord(int baseIndex);									//コードの構造にシンプルな値を格納する処理
	Chord makeChordFromPrev(Chord chord);									//前のコードから次のコードを生成する関数
	vector<Chord> changeChordFashionable(vector<Chord> chords);				//コード列をおしゃれなコード進行に変換して返す(メンバなので引数としてmChordsを受け取る必要はないが、一応の拡張性を意識して引数として受け取る構文にしてる)
	ChordType specificChord(int is1, int is2, int is3);						//内音3つからコードの種類を特定する関数
	vector<Chord> shiftChordRight(vector<Chord> chords, int idx, int n);		//指定したインデックス以降の要素をn分シフトした要素を返す
};

#endif
