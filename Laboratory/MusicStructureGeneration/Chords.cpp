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
void Chords::create(int chordNum, uint32_t length, int key, Scale scale, Algorithm algorithm)
{
	//初期化処理
	mChords.clear();

	//入力で受け取った小節数が最低数以下なら、なにもせず落とす
	if (chordNum < 1) {
		return;
	}

	mLength = length;
	mChordNum = chordNum;
	mAlgorithm = algorithm;

	//スケールからメインで使用する7音を設定する処理
	for (int i = 0; i < 7; ++i) { 
		mFloor[i] = 0;
	}			//メモリの初期化をしておく
	if (scale == Scale::Major) {
		//使用するインデックスは[全全半全全全半]なので、0、2、4、5、7、9、11番となる
		mFloor[0] = 0; mFloor[1] = 2; mFloor[2] = 4; mFloor[3] = 5; mFloor[4] = 7; mFloor[5] = 9; mFloor[6] = 11;
		mScale = Scale::Major;
	}
	//マイナースケールの時
	else if (scale == Scale::Minor) {
		//使用するインデックスは[全半全全半全全]なので、0、2、3、5、7、8、10番となる
		mFloor[0] = 0; mFloor[1] = 2; mFloor[2] = 3; mFloor[3] = 5; mFloor[4] = 7; mFloor[5] = 8; mFloor[6] = 10;
		mScale = Scale::Minor;
	}
	//ハーモニック・マイナースケールの時
	else if (scale == Scale::HarmonicMinor) {
		//使用するインデックスは、0、2、3、5、7、8、11番となる
		mFloor[0] = 0; mFloor[1] = 2; mFloor[2] = 3; mFloor[3] = 5; mFloor[4] = 7; mFloor[5] = 9; mFloor[6] = 11;
		mScale = Scale::HarmonicMinor;
	}
	//メロディックマイナースケールの時
	else if (scale == Scale::MelodicMinor) {
		//使用するインデックスは[全全半全全全半]なので、0、2、4、5、7、9、11番となる
		mFloor[0] = 0; mFloor[1] = 2; mFloor[2] = 3; mFloor[3] = 5; mFloor[4] = 7; mFloor[5] = 9; mFloor[6] = 11;
		mScale = Scale::MelodicMinor;
	}
	//キーの高さを考慮する
	for (int i = 0; i < 7; ++i) {
		mFloor[i] = (mFloor[i] + key)/* % 12*/;		//オクターブを超える表現も考慮する
	}

	//シンプルなコード生成アルゴリズムを実行する
	if (mAlgorithm == Algorithm::Simple) {
		Chord chord;										//コードを格納する構造のインスタンス化
		//コードの開始地点は個別に処理
		//ⅤとⅦから開始するコードはあんまりない
		//Ⅰが一番多い
		//次にⅣが多い
		//さらに次にⅥ度が多く、Ⅱ、Ⅲはレア
		int randNum = (rand() - 1) * 100 / RAND_MAX;		//乱数の値の範囲は0～100
		int perNum = 0;
		//始まりのコードを生成する処理
		if (randNum < (perNum += 35)) {									//生成するコードがⅠの時の処理
			chord = makeSimpleChord(0);									//ベースのインデックス値を引数に与える
		}
		else if (randNum < (perNum += 4)) {								//生成するコードがⅡの時の処理
			chord = makeSimpleChord(1);
		}
		else if (randNum < (perNum += 1)) {								//生成するコードがⅢの時の処理
			chord = makeSimpleChord(2);
		}
		else if (randNum < (perNum += 30)) {							//生成するコードがⅣの時の処理
			chord = makeSimpleChord(3);
		}
		else if (randNum < (perNum += 5)) {								//生成するコードがⅤの時の処理
			chord = makeSimpleChord(4);
		}
		else if (randNum < (perNum += 25)) {							//生成するコードがⅥの時の処理
			chord = makeSimpleChord(5);
		}																//Ⅶから生成するのはなし
		else {															//余剰の確率はⅠにするようにしておく
			chord = makeSimpleChord(0);
		}
		//最初のコードをコード列にプッシュする
		mChords.push_back(chord);
		for (int i = 0; i < mChordNum - 1; ++i) {						//開始時点に一つ追加したコード分を引いて生成ループを行う
			chord = makeChordFromPrev(chord);							//前回のコードの値から次のコードを生成する
			mChords.push_back(chord);									//生成したコードをコード列の最後尾に追加
		}
	}
	//おしゃれなコードを生成するアルゴリズムを実行する
	else if (mAlgorithm == Algorithm::Fashionable) {
		//はじめの処理は同様シンプルなコード生成と同様な処理を行う
		Chord chord;										//コードを格納する構造のインスタンス化
		int randNum = (rand() - 1) * 100 / RAND_MAX;		//乱数の値の範囲は0～100
		int perNum = 0;
		//始まりのコードを生成する処理
		if (randNum < (perNum += 35)) {									//生成するコードがⅠの時の処理
			chord = makeSimpleChord(0);									//ベースのインデックス値を引数に与える
		}
		else if (randNum < (perNum += 4)) {								//生成するコードがⅡの時の処理
			chord = makeSimpleChord(1);
		}
		else if (randNum < (perNum += 1)) {								//生成するコードがⅢの時の処理
			chord = makeSimpleChord(2);
		}
		else if (randNum < (perNum += 30)) {							//生成するコードがⅣの時の処理
			chord = makeSimpleChord(3);
		}
		else if (randNum < (perNum += 5)) {								//生成するコードがⅤの時の処理
			chord = makeSimpleChord(4);
		}
		else if (randNum < (perNum += 25)) {							//生成するコードがⅥの時の処理
			chord = makeSimpleChord(5);
		}																//Ⅶから生成するのはなし
		else {															//余剰の確率はⅠにするようにしておく
			chord = makeSimpleChord(0);
		}
		//最初のコードをコード列にプッシュする
		mChords.push_back(chord);
		for (int i = 0; i < mChordNum - 1; ++i) {						//開始時点に一つ追加したコード分を引いて生成ループを行う
			chord = makeChordFromPrev(chord);							//前回のコードの値から次のコードを生成する
			mChords.push_back(chord);									//生成したコードをコード列の最後尾に追加
		}
		mChords = changeChordFashionable(mChords);
	}
	//転調を行う様な生成アルゴリズムを
	else if (mAlgorithm == Algorithm::modulation) {

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
	chord.baseIndex = mFloor[baseIndex];				//指定されたインデックスの階数値をコードの基底音として設定する
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

//指定したインデックス以降の要素をn分シフトした要素を返す(サイズは不変とし、溢れた要素は削除する)
vector<Chords::Chord> Chords::shiftChordRight(vector<Chord> chords, int idx, int n)
{
	vector<Chord> shiftChords = chords;

	for (int i = idx + n; i < shiftChords.size(); ++i) {
		shiftChords[i] = chords[i - n];
	}
	return shiftChords;
}

vector<Chords::Chord> Chords::changeChordFashionable(vector<Chord> chords) {
	vector<Chord> fashionableChords = chords;
	//単純な置き換え
	for (int i = 0; i < chords.size(); ++i) {
		//ここで付加されたセブンスコード等の付加情報は後述のコードの並びに応じた変換によって消えることはない。変更されることはある
		int randNum = (rand() - 1) * 100 / RAND_MAX;
		//50％の確率でセブンスを付加
		if (randNum < 50) {
			fashionableChords[i].plus = ChordPlus::Seventh;
			//ⅠとⅣは特別にM7にする
			if (chords[i].baseIndex == mFloor[0] || chords[i].baseIndex == mFloor[3]) {
				fashionableChords[i].plus = ChordPlus::MajorSeventh;
			}
		}
		else if (randNum < 70) {
			//ⅠとⅣをadd9にする処理
			if (chords[i].baseIndex == mFloor[0] || chords[i].baseIndex == mFloor[3]) {
				fashionableChords[i].plus = ChordPlus::Add9;
			}
		}
	}
	//複数のコードの並びによって変更する(メジャーコードの時の処理)
	if (mScale == Scale::Major) {
		for (int i = 0; i < chords.size(); ++i) {
			int randNum = (rand() - 1) * 100 / RAND_MAX;

			//3つ連続したコードに変更するので、最後のインデックスから2つ以上小さい値しか受け付けない
			if (i < chords.size() - 2) {
				//[Ⅴ->Ⅰ]を[Ⅴ->Ⅰsus4->Ⅰ]にする
				if (chords[i].baseIndex == mFloor[4] && chords[i + 1].baseIndex == mFloor[0]) {
					// 変更確率50％
					if (randNum < 50) {
						chords = shiftChordRight(chords, i + 1, 1);			//シフト処理と溢れた要素の削除処理
						fashionableChords[i] = makeSimpleChord(4);			//Ⅴのコードに変更する
						fashionableChords[i + 1] = makeSimpleChord(0);
						fashionableChords[i + 1].type = ChordType::Sus4;	//次のコードはⅠsus4
						fashionableChords[i + 2] = makeSimpleChord(0);		//さらに次はⅠ

						//変更した場所を飛ばして処理を続けるようにする
						i += 2;
						continue;
					}
				}
				//コード生成で階数にないコードを生成する時は、baseIndexは-1(255)としておく
				//[Ⅰ->Ⅱm]を[Ⅰ->Ⅰ#dim->Ⅱm]に変更する
				if (chords[i].baseIndex == mFloor[0] && chords[i + 1].baseIndex == mFloor[1]) {
					if (randNum < 50) {
						chords = shiftChordRight(chords, i + 1, 1);			//シフト処理と溢れた要素の削除処理
						fashionableChords[i] = makeSimpleChord(0);			//Ⅴのコードに変更する
						fashionableChords[i + 1] = makeSimpleChord(0);		//一旦Ⅰのコードを作っておく
						fashionableChords[i + 1].baseIndex = -1;			//階数にない基底音のインデックス値を表現するために異常値を格納しておく
						fashionableChords[i + 1].baseNoteNum += 1;			//Ⅰの基底音から一音高い音を基底音とするため、1加算してる
						fashionableChords[i + 1].type = ChordType::mb5;		//次のコードはⅠDim7なのでmb5を用意しておく
						fashionableChords[i + 1].plus = ChordPlus::Dim7;	//Dim7を付加
						fashionableChords[i + 2] = makeSimpleChord(1);		//さらに次はⅡm

						//変更した場所を飛ばして処理を続けるようにする
						i += 2;
						continue;
					}
				}
			}

			//2つ連続したコードを変更するので、最後のインデックスから1つ以上小さい値しか受け付けない
			if (i < chords.size() - 1) {
				//[Ⅴ->Ⅰ]を変更する処理(変更確率20％)
				if (chords[i].baseIndex == mFloor[4] && chords[i + 1].baseIndex == mFloor[0]) {
					if (randNum < 20) {
						//[Ⅴ->Ⅰ]を[Ⅱm7/Ⅴ->Ⅰ]に変更する処理(変更確率33％)
						if (randNum < 10) {
							fashionableChords[i] = makeSimpleChord(1);						//Ⅱm/Ⅴのコードに変更する
							fashionableChords[i].plus = ChordPlus::Seventh;					//セブンスを付加する
							fashionableChords[i].onIndex = 4;								//Ⅴをオンコードとする
							//fashionableChords[i + 1] = makeSimpleChord(0);				//次のコードをⅠに変更する(fashionableChordsのⅠ7をそのまま残すため、現在はコメントアウトしてる)
							//変更した場所を飛ばして処理を続けるようにする
							i += 1;
							continue;
						}
						//[Ⅴ->Ⅰ]を[Ⅳ->Ⅰ]に変更する処理(変更確率66％)
						else {
							fashionableChords[i] = makeSimpleChord(3);						//Ⅳのコードに変更する
							//fashionableChords[i + 1] = makeSimpleChord(0);				//次のコードをⅠに変更する(Ⅰ7をそのまま残すため、現在はコメントアウトしてる)
							if (randNum < 15) {
								fashionableChords[i].type = ChordType::Minor;				//メジャーコードからマイナーコードにする
								if (randNum < 13) {
									fashionableChords[i].plus = ChordPlus::Seventh;			//セブンスをさらに追加する
								}
							}
							//変更した場所を飛ばして処理を続けるようにする
							i += 1;
							continue;
						}
					}
				}

				//[Ⅳ->Ⅴ]を[Ⅳm->Ⅴ]、[Ⅳm7->Ⅴ7]、[Ⅳm6->Ⅴ7]のいずれかに変更する処理
				if (chords[i].baseIndex == mFloor[3] && chords[i + 1].baseIndex == mFloor[4]) {				//コードがⅣの時
					if (randNum < 30) {
						fashionableChords[i] = makeSimpleChord(3);							//Ⅳのコードに変更する
						fashionableChords[i].type = ChordType::Minor;						//メジャーコードからマイナーコードに変更する
						if (randNum < 10) {
							fashionableChords[i].plus = ChordPlus::Seventh;					//セブンスを付加する
							//この時、次のコードをⅤ7にしておく
							fashionableChords[i + 1] = makeSimpleChord(4);
							fashionableChords[i + 1].plus = ChordPlus::Seventh;
						}
						else if (randNum < 20) {
							fashionableChords[i].plus = ChordPlus::Six;						//シックスを付加する
							//この時、次のコードをⅤ7にしておく
							fashionableChords[i + 1] = makeSimpleChord(4);
							fashionableChords[i + 1].plus = ChordPlus::Seventh;
						}
						//変更した場所を飛ばして処理を続けるようにする
						i += 1;
						continue;
					}
				}

				//[Ⅲm->Ⅵm]を[Ⅲ7->Ⅵm7]にする
				if (chords[i].baseIndex == mFloor[2] && chords[i + 1].baseIndex == mFloor[5]) {				//コードがⅣの時
					if (randNum < 30) {
						fashionableChords[i] = makeSimpleChord(2);							//Ⅳのコードに変更する
						fashionableChords[i].type = ChordType::Major;						//マイナーコードからメジャーコードに変更する
						fashionableChords[i + 1] = makeSimpleChord(5);
						fashionableChords[i + 1].plus = ChordPlus::Seventh;
						//変更した場所を飛ばして処理を続けるようにする
						i += 1;
						continue;
					}
				}
				//[Ⅵm->Ⅱm]を[Ⅵ7->Ⅱm7]にする
				if (chords[i].baseIndex == mFloor[5] && chords[i + 1].baseIndex == mFloor[1]) {				//コードがⅣの時
					if (randNum < 30) {
						fashionableChords[i] = makeSimpleChord(5);							//Ⅵのコードに変更する
						fashionableChords[i].type = ChordType::Major;						//メジャーコードからマイナーコードに変更する
						fashionableChords[i + 1] = makeSimpleChord(1);						//次のコードを設定
						fashionableChords[i + 1].plus = ChordPlus::Seventh;					//セブンスを付加
						//変更した場所を飛ばして処理を続けるようにする
						i += 1;
						continue;
					}
				}
			}
		}
	}
	return fashionableChords;
}
