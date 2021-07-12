#include "Chords.h"
#include <algorithm>
#include <ctime>

Chords::Chords()
	: mFloor(nullptr),
	mBaseNoteNum(36)		//音階の開始地点をドの音に設定する
{
	mFloor = new int[7];
	std::srand(std::time(nullptr));						//乱数をリセット
}

Chords::~Chords()
{
	mChords.clear();
	delete[] mFloor;
}

//音楽理論からコードを生成するアルゴリズム
void Chords::create(int chordNum, uint32_t length, int key, Scale scale)
{
	//初期化処理
	mChords.clear();

	//入力で受け取った小節数が最低数以下なら、なにもせず落とす
	if (chordNum < 1) {
		return;
	}

	mLength = length;
	mChordNum = chordNum;

	//スケールからメインで使用する7音を設定する処理
	for (int i = 0; i < 7; ++i) { 
		mFloor[i] = 0;
	}			//メモリの初期化をしておく
	if (scale == Scale::Major) {
		//使用するインデックスは[全全半全全全半]なので、0、2、4、5、7、9、11番となる
		mFloor[0] = 0; mFloor[1] = 2; mFloor[2] = 4; mFloor[3] = 5; mFloor[4] = 7; mFloor[5] = 9; mFloor[6] = 11;
	}
	//マイナースケールの時
	else if (scale == Scale::Minor) {
		//使用するインデックスは[全半全全半全全]なので、0、2、3、5、7、8、10番となる
		mFloor[0] = 0; mFloor[1] = 2; mFloor[2] = 3; mFloor[3] = 5; mFloor[4] = 7; mFloor[5] = 8; mFloor[6] = 10;
	}
	//ハーモニック・マイナースケールの時
	else if (scale == Scale::HarmonicMinor) {
		//使用するインデックスは、0、2、3、5、7、8、11番となる
		mFloor[0] = 0; mFloor[1] = 2; mFloor[2] = 3; mFloor[3] = 5; mFloor[4] = 7; mFloor[5] = 9; mFloor[6] = 11;
	}
	//メロディックマイナースケールの時
	else if (scale == Scale::MelodicMinor) {
		//使用するインデックスは[全全半全全全半]なので、0、2、4、5、7、9、11番となる
		mFloor[0] = 0; mFloor[1] = 2; mFloor[2] = 3; mFloor[3] = 5; mFloor[4] = 7; mFloor[5] = 9; mFloor[6] = 11;
	}
	//キーの高さを考慮する
	for (int i = 0; i < 7; ++i) {
		mFloor[i] = (mFloor[i] + key)/* % 12*/;		//オクターブを超える表現も考慮する
	}

	Chord chord;										//コードを格納する構造のインスタンス化
	//コードの開始地点は個別に処理
	//ⅤとⅦから開始するコードはあんまりない
	//Ⅰが一番多い
	//次にⅣが多い
	//さらに次にⅥ度が多く、Ⅱ、Ⅲはレア
	int randNum = (rand() - 1) * 100 / RAND_MAX;		//乱数の値の範囲は0～100
	//始まりのコードを生成する処理
	if (randNum < 30) {									//生成するコードがⅠの時の処理
		chord = makeSimpleChord(0);						//ベースのインデックス値を引数に与える
	}
	else if (randNum < 60) {							//生成するコードがⅣの時の処理
		chord = makeSimpleChord(3);
	}
	else if (randNum < 90) {							//生成するコードがⅥの時の処理
		chord = makeSimpleChord(5);
	}
	else if (randNum < 95) {							//生成するコードがⅥの時の処理

		chord = makeSimpleChord(1);
	}
	else{												//生成するコードがⅢの時の処理
		chord = makeSimpleChord(2);
	}
	//最初のコードをコード列にプッシュする
	mChords.push_back(chord);

	for(int i = 0; i < chordNum - 1; ++i) {				//開始時点に一つ追加したコード分を引いて生成ループを行う
		chord = makeChordFromPrev(chord);				//前回のコードの値から次のコードを生成する
		mChords.push_back(chord);						//生成したコードをコード列の最後尾に追加
	}
}

//7CM理論でコードを生成するアルゴリズム
void Chords::create7CM(int barNum)
{

}

Chords::Chord Chords::makeSimpleChord(int baseIndex)
{
	Chord chord;										//コードについての情報を格納する構造
	//異常な入力が来たら異常値で返す
	if (baseIndex < 0 || baseIndex >= 7) {
		chord.baseNoteNum = -1;
		chord.baseIndex = -1;
		chord.omitIndex = -1;
		chord.onIndex = -1;
		return chord;
	}

	//isはInternal sound(内音)の略
	int is1 = mFloor[baseIndex % 7];
	int is2 = mFloor[(baseIndex + 2) % 7];
	int is3 = mFloor[(baseIndex + 4) % 7];
	//もし下から2番目の内音が1番下の内音より小さければ、オクターブを一つ上げる
	if (is1 > is2) {
		is2 += 12;
	}
	//もし下から3番目の内音が2番下の内音より小さければ、オクターブを一つ上げる
	if (is2 > is3) {
		is3 += 12;
	}

	//3和音の音に必要なパラメータを設定する
	chord.baseNoteNum = mBaseNoteNum + is1;
	chord.baseIndex = mFloor[baseIndex];
	chord.onIndex = mFloor[baseIndex];					//baseIndexと値が一致していたらonIndexは考えないという事
	chord.omitIndex = 12;								//12はOmitを無効化する値
	chord.type = specificChord(is1, is2, is3);			//内音からコードのタイプを判別する
	chord.plus = ChordPlus::None;						//付加情報は一旦何も入れない
	chord.tension = ChordTension::None;					//上に同じ

	chord.length = mLength / mChordNum;					//長さは均等に配分する形で決定している

	return chord;
}

Chords::Chord Chords::makeChordFromPrev(Chord chord) {
	Chord createChord;									//生成するコードが格納される場所
	int randNum = (rand() - 1) * 100 / RAND_MAX;
	//TODO:前回のコードがドミナントモーションだった時の処理
	
	//前のコードがⅠだった時の処理
	if (chord.baseIndex == mFloor[0]) {
		//Ⅳに進行
		if (randNum < 50) {
			createChord = makeSimpleChord(3);
		}
		//Ⅴに進行
		else if (randNum < 80) {
			createChord = makeSimpleChord(4);
		}
		else if (randNum < 90) {
			createChord = makeSimpleChord(1);
		}
		//Ⅲ(Ⅲb)に進行
		else {
			createChord = makeSimpleChord(2);
			//TODO:メジャーコードの時、特殊にⅢ♭に進行するパターンも考慮する
		}
	}
	//前のコードがⅡだった時の処理
	else if (chord.baseIndex == mFloor[1]) {
		//Ⅲに進行
		if (randNum < 20) {
			createChord = makeSimpleChord(2);
		}
		//Ⅴに進行
		else {
			createChord = makeSimpleChord(4);
		}
	}
	//前のコードがⅢだった時の処理
	else if (chord.baseIndex == mFloor[2]) {
		//Ⅵに進行
		if (randNum < 60) {
			createChord = makeSimpleChord(5);
		}
		//Ⅳに進行
		else {
			createChord = makeSimpleChord(3);
		}
	}
	//前のコードがⅣだった時の処理
	else if (chord.baseIndex == mFloor[3]) {
		//Ⅴに進行
		if (randNum < 50) {
			createChord = makeSimpleChord(4);
		}
		//Ⅰに進行
		else {
			createChord = makeSimpleChord(0);
		}
	}
	//前のコードがⅤだった時の処理
	else if (chord.baseIndex == mFloor[4]) {
		//Ⅰに進行
		if (randNum < 80) {
			createChord = makeSimpleChord(0);
		}
		//Ⅲに進行
		else {
			createChord = makeSimpleChord(2);
			if (randNum < 90) {
				createChord.type = ChordType::Major;
			}
			else {
				createChord.type = ChordType::Minor;
			}
		}
	}
	//前のコードがⅥだった時の処理
	else if (chord.baseIndex == mFloor[5]) {
		//Ⅳに進行
		if (randNum < 30) {
			createChord = makeSimpleChord(3);
		}
		//Ⅴに進行
		else if (randNum < 50) {
			createChord = makeSimpleChord(4);
		}
		//Ⅱに進行
		else if (randNum < 70) {
			createChord = makeSimpleChord(1);
		}
		//Ⅲに進行
		else {
			createChord = makeSimpleChord(2);
		}
	}
	//前のコードがⅦだった時の処理
	else if (chord.baseIndex == mFloor[6]) {
		createChord = makeSimpleChord((rand() - 1) * 7 / RAND_MAX);		//Ⅶの進行はよくわからんから乱数で決めてる
	}
	else {
		//バグなのでなにもしないでおく
	}

	return createChord;
}

//コードを内音から特定する処理
Chords::ChordType Chords::specificChord(int is1, int is2, int is3)		//isはInternal soundの略
{
	ChordType type = ChordType::None;
	if (is2 - is1 == 5) {					//基底音とその次の内音が5度差なら
		if (is3 - is2 == 2) {				//さらに2つ目と3つ目の内音が2度差なら
			type == ChordType::Sus4;		//sus4のコードで確定する
		}
	}
	else if (is2 - is1 == 4) {				//基底音とその次の内音が4度差なら
		if (is3 - is2 == 3) {				//さらに2つ目と3つ目の内音が3度差なら
			type = ChordType::Major;		//メジャーコードで確定する
		}
		else if (is3 - is2 == 4) {			//2つ目と3つ目の内音が4度差なら
			type = ChordType::Aug;			//Augコードで確定する
		}
	}
	else if (is2 - is1 == 3) {
		if (is3 - is2 == 4) {
			type = ChordType::Minor;		//マイナーコード
		}
		if (is3 - is2 == 3) {
			type = ChordType::mb5;			//マイナー♭ファイブ
		}
	}
	else if (is2 - is1 == 2) {
		if (is3 - is2 == 5) {
			type = ChordType::Sus2;			//sus2のコード
		}
	}

	return type;
}