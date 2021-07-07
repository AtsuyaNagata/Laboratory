#include "Melody.h"
#include "Chords.h"
#include "RhythmPattern.h"

Melody::Melody() {

}

void Melody::create(RhythmPattern* rhythmPattern, Chords* chords, vector<uint32_t> chordLengths) {
	//コードの数とコードの長さの配列の数が一致していなかったらおかしいので落とす
	if (chords->getChords().size() != chordLengths.size()) {
		return;
	}
	//メロディの状態を保持するための構造
	enum class MelodyState : unsigned char {
		Inner,			//内音
		Embroidery,		//刺繍音
		Progress,		//経過音
		Lean,			//倚音
		None			//その他
	} melodyState = MelodyState::None;					//前回のメロディの状態

	int nowCreatePosition = 0;							//現在生成しようとしているノートの生成位置の時間のポジションを表現する
	int nowChordIndex = 0;								//現在生成しようとしているノートの参照位置に対応するコードの種類を示すためのインデックス
	int index = 0;										//今回のノートのインデックス値が入る。
	int prevChordIndex;									//前回のループのインデックス値を保存しておくバッファ
	int baseNoteNum = chords->getBaseNoteNum() + 24;	//Chordsクラスの所有する基底音の値から2オクターブ上の位置をメロディ生成の基底音とする
	const int* floor = chords->getFloor();				//今回使用する階名を獲得しておく(7個の配列で固定されてる)
	for (int i = 0; i < rhythmPattern->getRhythmPattern().size(); ++i) {
		//==========ノート生成処理実行前の準備処理==========
		RhythmPattern::Rhythm rhythm = rhythmPattern->getRhythmPattern()[i];		//今回の周回においてのリズム構造を抽出
		Chords::Chord chord = chords->getChords()[nowChordIndex];				//今回の周回においてのコード構造を抽出
		uint8_t note;															//ノートの値を一時的においておくバッファ
		int baseIndex = chord.baseIndex;
		//==========メインのメロディ生成処理==========
		if (rhythm.on == true) {													//今回のリズムパターンが休符の時は飛ばす
			int randState = (std::rand() - 1) * 100 / RAND_MAX;						//メロディの状態遷移を決定するための乱数値(0から99までの値が出る)

			if (melodyState == MelodyState::Inner) {
				//20％の確率
				if (randState < 20) {
					//内音：コードを構成する音
					melodyState = MelodyState::Inner;
					int randBuff = (std::rand() - 1) * 3 / RAND_MAX;				//一時的なバッファ
					index = (baseIndex + ((randBuff == 0) ? 0 : ((randBuff == 1) ? 2 : 4))) % 7;	//内音を選択するための式
					note = baseNoteNum + floor[index];						//ノート値の設定
					prevChordIndex = index;
				}
				//30％の確率
				else if (randState < 50) {
					//刺繍音：同じ音程の内音で挟まれる音。内音の一音上か半音下か一音下の音を使う
					melodyState = MelodyState::Embroidery;
					int randBuff = (std::rand() - 1) * 2 / RAND_MAX;		//一時的なバッファ
					index = (prevChordIndex + ((randBuff == 0) ? -1 : 1)) % 7;		//刺繍音を選択するための式
					if (index <= -1) { index = 1; }		//インデックスが負の方向に行き過ぎてたら戻す
					note = baseNoteNum + floor[index];	//ノート値の設定
				}
				//30％の確率
				else if (randState < 80) {
					//経過音：違う2つの内音をつなぐ音を使う
					melodyState = MelodyState::Progress;
					if ((prevChordIndex - baseIndex) % 7 == 0) {							//マイナスだと異常動作するので+7しておく
						index = (prevChordIndex + 1) % 7;
					}
					else if (((prevChordIndex - baseIndex) + 7) % 7 == 2) {				//マイナスだと異常動作するので+7しておく
						int randBuff = (std::rand() - 1) * 2 / RAND_MAX;				//0or1の値を格納する
						index = (prevChordIndex + ((randBuff == 0) ? -1 : 1)) % 7;		//経過音を選択するための式
					}
					else {
						index = (prevChordIndex - 1) % 7;
					}
					if (index <= -1) { index = 6; }		//インデックスが負の方向に行き過ぎてたら戻す
					note = baseNoteNum + floor[index];	//ノート値の設定
				}
				//20％の確率
				else {
					//倚音：内音の前に長めに一つ上か一つ下か半音下の音を使う
					melodyState = MelodyState::Lean;
					int randBuff = (std::rand() - 1) * 3 / RAND_MAX;		//一時的なバッファ
					//内音以外の何かしらを選択する
					switch (randBuff) {
					case 0:
						index = (baseIndex + 1) % 7;
						break;
					case 1:
						index = (baseIndex + 3) % 7;
						break;
					case 2:
						index = (baseIndex + 5) % 7;
						break;
						/*case 3:
							index = (baseIndex + 6) % 7;
							break;*/
					}
					note = baseNoteNum + floor[index];
					//長さも特別に変更をかける
				}
			}
			//刺繍音の続き
			else if (melodyState == MelodyState::Embroidery) {
				//90％の確率で内音を選択する
				if (randState < 90) {
					melodyState = MelodyState::Inner;
					index = prevChordIndex;			//前回の内音をそのまま使用する
					note = baseNoteNum + floor[index];
				}
				else {
					//先取音：刺繍音を内音に戻さず連続させること(あんまり出ないようにする)
					//逸音：刺繍音から内音に戻さずに刺繍音も続けずに別の音へ展開させる(あんまり出ないようにする)
					//掛留音：倚音の代わりに同じ音程の内音をタイでつなげる（あんまでないように）
					//要はテキトーな音を入れる
					melodyState = MelodyState::None;				//未定義状態にしておく
					index = (std::rand() - 1) * 7 / RAND_MAX;		//一時的なバッファ
					note = baseNoteNum + floor[index];
				}
			}
			//倚音の続き
			else if (melodyState == MelodyState::Lean) {
				//絶対に内音が来る
				melodyState = MelodyState::Inner;
				int octaveBuff = 0;
				//必ず内音のうち一番小さい値が来る条件
				if (index == (baseIndex + 6) % 7) {
					index = baseIndex;
					//Aメロとサビの時、オクターブを上げる
					/*if (mMelody == Melody::A || mMelody == Melody::Climax) {
						octaveBuff = 12;
					}*/
				}
				//必ず内音のうち一番大きい値が来る条件
				else if (index == (baseIndex + 5) % 7) {
					index = (baseIndex + 4) % 7;
				}
				else {
					//上でも下でも移動していい時
					if ((std::rand() - 1) * 2 / RAND_MAX == 0) {
						index = (index - 1) % 7;
					}

				}
				if (index <= -1) { index = 6; octaveBuff = 0; }		//インデックスが負の方向に行き過ぎてたら戻す(呼ばれない事が前提ではある)
				note = baseNoteNum + octaveBuff + floor[index];
				prevChordIndex = index;
			}
			//経過音の続き
			else if (melodyState == MelodyState::Progress) {
				//絶対に内音に続くの内音が来る
				melodyState = MelodyState::Inner;
				index = (index + ((prevChordIndex - index > 0) ? -1 : 1)) % 7;
				if (index <= -1) { index = 6; }		//インデックスが負の方向に行き過ぎてたら戻す
				note = baseNoteNum + floor[index];
			}
			//前回が未定義状態だったら
			else if (melodyState == MelodyState::None) {
				//1/2の確率で内音、1/2の確率で倚音
				if ((std::rand() - 1) * 2 / RAND_MAX == 0) {
					//内音
					melodyState = MelodyState::Inner;
					int randBuff = (std::rand() - 1) * 3 / RAND_MAX;		//一時的なバッファ
					index = (baseIndex + ((randBuff == 0) ? 0 : ((randBuff == 1) ? 2 : 4))) % 7;	//内音を選択するための式
					note = baseNoteNum + floor[index];	//ノート値の設定
					prevChordIndex = index;
				}
				else {
					//倚音
					melodyState = MelodyState::Lean;
					int randBuff = (std::rand() - 1) * 3 / RAND_MAX;		//一時的なバッファ
					//内音以外の何かしらを選択する4番目は音が飛ぶ事が多いので削除
					switch (randBuff) {
					case 0:
						index = (baseIndex + 1) % 7;
						break;
					case 1:
						index = (baseIndex + 3) % 7;
						break;
					case 2:
						index = (baseIndex + 5) % 7;
						break;
						/*case 3:
							index = (baseIndex + 6) % 7;
							break;*/
					}
					note = baseNoteNum + floor[index];
				}
			}
			//確定したノートの高さをMelodyとしてプッシュする
			mMelody.push_back(note);
		}
		//============================================

		nowCreatePosition += rhythm.length;		//次の周回のためにlengthを加算しておく
		//コード遷移が行われたかどうかの判定
		if (nowCreatePosition > chordLengths[nowChordIndex]) {
			nowCreatePosition -= chordLengths[nowChordIndex];
			nowChordIndex++;
		}
	}
}