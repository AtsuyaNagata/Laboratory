#include "Melody.h"
#include "Chords.h"
#include "RhythmPattern.h"
#include <iostream>
#include <algorithm>

Melody::Melody() {

}

void Melody::createMelody(const RhythmPattern* rhythmPattern, const Chords* chordString) {
	//最初に削除処理をしておく
	mNotes.clear();

	//メロディの状態を保持するための構造
	enum class MelodyState : unsigned char {
		Inner,			//内音
		Embroidery,		//刺繍音
		Progress,		//経過音
		Lean,			//倚音
		None			//その他
	} melodyState = MelodyState::None;					//前回のメロディの状態

	RhythmPattern::Rhythm rhythm = rhythmPattern->getRhythmPattern()[0];	//今回の周回においてのリズム構造を抽出(メロディのリズムは0番目のインデックスに一つだけ存在するはず)
	vector<Chords::Chord> chords = chordString->getChords();				//コード列を獲得しておく

	//もし、コードの合計長さ < メロディリズムの最大開始地点だったら異常値なので生成せずに落とす
	if (chordString->getLength() < rhythm.position[rhythm.position.size() - 1].startTime) {
		return;
	}

	uint32_t nowCreatePosition;								//現在生成しようとしている位置(四分音符一つ分の長さ：256)
	uint32_t nowChordLength = chords[0].length;				//現ループのコードの長さ(初期値は一番最初のコードの長さ)
	int nowChordIndex = 0;									//現在生成しようとしているノートの参照位置に対応するコードの種類を示すためのインデックス
	int index = 0;											//今回のループのノートのインデックス値が入る。
	int prevChordIndex;										//前回のループのインデックス値を保存しておくバッファ
	int baseNoteNum = chordString->getBaseNoteNum() + 24;	//Chordsクラスの所有する基底音の値から2オクターブ上の位置をメロディ生成の基底音とする
	const int* floor = chordString->getFloor();				//今回使用する階名を獲得しておく(7個の配列で固定されてる)

	//メロディのリズムパターンを抽出する処理
	if (rhythmPattern->getRhythmPattern().size() > 1 || rhythmPattern->getRhythmPattern()[0].type != RhythmPattern::RhythmType::Melody) {
		//リズムパターンクラスの構造が一致しないのでエラーとして何も生成せずに落とす
		return;
	}

	//メロディの生成処理メインループ
	for (int i = 0; i < rhythm.position.size(); ++i) {
		//==========ノート生成処理実行前の準備処理==========
		nowCreatePosition = rhythm.position[i].startTime;				//今回の周回のためにlengthを更新しておく
		//コード遷移が行われたかどうかの判定
		for (int j = nowChordIndex; j < chords.size(); ++j) {
			//現時点のコードの長さが現在の生成位置を超えるまで処理を行う
			if (nowCreatePosition < nowChordLength) {					//常に「現在のコードの合計長 > 現在の生成位置」の関係を保つ様にする
				break;
			}
			//次のコードの長さを加算しておく
			nowChordIndex++;											//コードのインデックスを加算する(コードの長さの情報は加算のみで算出されるので、インデックスの扱いはここだけで行う様にする)
			nowChordLength += chords[nowChordIndex].length;				//更新したインデックス値のコードの長さを現在のコード長に加算する
		}

		Chords::Chord chord = chords[nowChordIndex];					//今回の周回においてのコード構造を抽出

		Note note;														//ノートの値を一時的においておくバッファ
		note.velocity = 100;											//複雑な音量調整は今は考えない
		note.startTime = rhythm.position[i].startTime;					//リズムデータから開始地点と長さを格納する
		note.length = rhythm.position[i].length;						//上に同じ

		//コードの基底のインデックス値を獲得しておく
		int baseIndex = -1;
		for (int j = 0; j < 7; ++j) {
			if (floor[j] == chord.baseIndex) {
				baseIndex = j;
				break;
			}
		}
		//もし階数に含まれない音がコードの基底音だった場合、そのコードの内音を適当に選んで決定する(仮アルゴリズムとして実装してる)
		if (baseIndex == -1) {
			int randBuff = (std::rand() - 1) * 3 / RAND_MAX;				//一時的なバッファ
			index = (baseIndex + ((randBuff == 0) ? 0 : ((randBuff == 1) ? 2 : 4))) % 7;	//内音を選択するための式
			note.num = baseNoteNum + chord.baseIndex + floor[index];
			mNotes.push_back(note);
			continue;
		}

		//==========メインのメロディ生成処理==========
		int randState = (std::rand() - 1) * 100 / RAND_MAX;						//メロディの状態遷移を決定するための乱数値(0から99までの値が出る)

		if (melodyState == MelodyState::Inner) {
			//20％の確率
			if (randState < 20) {
				//内音：コードを構成する音
				melodyState = MelodyState::Inner;
				int randBuff = (std::rand() - 1) * 3 / RAND_MAX;				//一時的なバッファ
				index = (baseIndex + ((randBuff == 0) ? 0 : ((randBuff == 1) ? 2 : 4))) % 7;	//内音を選択するための式
				note.num = baseNoteNum + floor[index];						//ノート値の設定
				prevChordIndex = index;
			}
			//30％の確率
			else if (randState < 50) {
				//刺繍音：同じ音程の内音で挟まれる音。内音の一音上か半音下か一音下の音を使う
				melodyState = MelodyState::Embroidery;
				int randBuff = (std::rand() - 1) * 2 / RAND_MAX;		//一時的なバッファ
				index = (prevChordIndex + ((randBuff == 0) ? -1 : 1)) % 7;		//刺繍音を選択するための式
				if (index <= -1) { index = 1; }		//インデックスが負の方向に行き過ぎてたら戻す
				note.num = baseNoteNum + floor[index];	//ノート値の設定
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
				note.num = baseNoteNum + floor[index];	//ノート値の設定
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
				note.num = baseNoteNum + floor[index];
				//長さも特別に変更をかける
			}
		}
		//刺繍音の続き
		else if (melodyState == MelodyState::Embroidery) {
			//90％の確率で内音を選択する
			if (randState < 90) {
				melodyState = MelodyState::Inner;
				index = prevChordIndex;			//前回の内音をそのまま使用する
				note.num = baseNoteNum + floor[index];
			}
			else {
				//先取音：刺繍音を内音に戻さず連続させること(あんまり出ないようにする)
				//逸音：刺繍音から内音に戻さずに刺繍音も続けずに別の音へ展開させる(あんまり出ないようにする)
				//掛留音：倚音の代わりに同じ音程の内音をタイでつなげる（あんまでないように）
				//要はテキトーな音を入れる
				melodyState = MelodyState::None;				//未定義状態にしておく
				index = (std::rand() - 1) * 7 / RAND_MAX;		//一時的なバッファ
				note.num = baseNoteNum + floor[index];
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
			note.num = baseNoteNum + octaveBuff + floor[index];
			prevChordIndex = index;
		}
		//経過音の続き
		else if (melodyState == MelodyState::Progress) {
			//絶対に内音に続くの内音が来る
			melodyState = MelodyState::Inner;
			index = (index + ((prevChordIndex - index > 0) ? -1 : 1)) % 7;
			if (index <= -1) { index = 6; }		//インデックスが負の方向に行き過ぎてたら戻す
			note.num = baseNoteNum + floor[index];
		}
		//前回が未定義状態だったら
		else if (melodyState == MelodyState::None) {
			//1/2の確率で内音、1/2の確率で倚音
			if ((std::rand() - 1) * 2 / RAND_MAX == 0) {
				//内音
				melodyState = MelodyState::Inner;
				int randBuff = (std::rand() - 1) * 3 / RAND_MAX;		//一時的なバッファ
				index = (baseIndex + ((randBuff == 0) ? 0 : ((randBuff == 1) ? 2 : 4))) % 7;	//内音を選択するための式
				note.num = baseNoteNum + floor[index];	//ノート値の設定
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
				note.num = baseNoteNum + floor[index];
			}
		}
		//確定したノートの高さをMelodyのノートとしてプッシュする
		mNotes.push_back(note);
		//============================================
	}

	return;
}

void Melody::createBase(const RhythmPattern* rhythmPattern, const Chords* chords, BaseAlgorithm baseAlgorithm) {
	//現在はこれだけ
	if (baseAlgorithm == BaseAlgorithm::Melo) {
		createBaseMelody(rhythmPattern, chords);
	}

	return;
}

//とりあえず内音のみでシンプルなベースラインを作る事を考える
void Melody::createBaseMelody(const RhythmPattern* rhythmPattern, const Chords* chordString) {
	//最初に削除処理をしておく
	mNotes.clear();

	RhythmPattern::Rhythm rhythm = rhythmPattern->getRhythmPattern()[0];	//今回の周回においてのリズム構造を抽出(メロディのリズムは0番目のインデックスに一つだけ存在するはず)
	vector<Chords::Chord> chords = chordString->getChords();				//コード列を獲得しておく

	//もし、コードの合計長さ < メロディリズムの最大開始地点だったら異常値なので生成せずに落とす
	if (chordString->getLength() < rhythm.position[rhythm.position.size() - 1].startTime) {
		return;
	}

	uint32_t nowCreatePosition;								//現在生成しようとしている位置(四分音符一つ分の長さ：256)
	uint32_t nowChordLength = chords[0].length;				//現ループのコードの長さ(初期値は一番最初のコードの長さ)
	int nowChordIndex = 0;									//現在生成しようとしているノートの参照位置に対応するコードの種類を示すためのインデックス
	int index = 0;											//今回のループのノートのインデックス値が入る。
	int prevChordIndex;										//前回のループのインデックス値を保存しておくバッファ
	int baseNoteNum = chordString->getBaseNoteNum() - 12;	//Chordsクラスの所有する基底音の値から1オクターブ下の位置をベースライン生成の基底音とする
	const int* floor = chordString->getFloor();				//今回使用する階名を獲得しておく(7個の配列で固定されてる)

	//メロディの生成処理メインループ
	for (int i = 0; i < rhythm.position.size(); ++i) {
		//==========ノート生成処理実行前の準備処理==========
		nowCreatePosition = rhythm.position[i].startTime;				//今回の周回のためにlengthを更新しておく
		//コード遷移が行われたかどうかの判定
		for (int j = nowChordIndex; j < chords.size(); ++j) {
			//現時点のコードの長さが現在の生成位置を超えるまで処理を行う
			if (nowCreatePosition < nowChordLength) {					//常に「現在のコードの合計長 > 現在の生成位置」の関係を保つ様にする
				break;
			}
			//次のコードの長さを加算しておく
			nowChordIndex++;											//コードのインデックスを加算する(コードの長さの情報は加算のみで算出されるので、インデックスの扱いはここだけで行う様にする)
			nowChordLength += chords[nowChordIndex].length;				//更新したインデックス値のコードの長さを現在のコード長に加算する
		}

		Chords::Chord chord = chords[nowChordIndex];					//今回の周回においてのコード構造を抽出

		Note note;														//ノートの値を一時的においておくバッファ
		note.velocity = 100;			//複雑な音量調整は今は考えない
		note.startTime = rhythm.position[i].startTime;					//リズムデータから開始地点と長さを格納する
		note.length = rhythm.position[i].length;						//上に同じ

		int baseIndex = - 1;								//コードの基底のノート値を獲得しておく
		for (int j = 0; j < 7; ++j) {
			if (floor[j] == chord.baseIndex) {
				baseIndex = j;
				break;
			}
		}
		//コードの基底音が階数似ない時の仮アルゴリズム
		if (baseIndex == -1) {
			note.num = baseIndex + chord.baseIndex;
			mNotes.push_back(note); continue;
		}

		//==========メインのメロディ生成処理==========
		//80％の確率で前回のノート値を参照して値を出す
		if ((std::rand() - 1) * 100 / RAND_MAX < 80) {
			//何もしない
		}
		//残りの確率で遷移を考える
		else {
			//適当な確率で内音を選択する
			int randNum = (std::rand() - 1) * 100 / RAND_MAX;
			if (randNum < 30) {
				index = 2;
			}
			else if (randNum < 60) {
				index = 4;
			}
			else {
				index = 0;
			}
		}
		note.num = baseNoteNum + floor[(baseIndex + index) % 7];

		//確定したノートの高さをMelodyのノートとしてプッシュする
		
		mNotes.push_back(note);
	}

	return;
}

void Melody::createDrum(const RhythmPattern* rhythmPattern)
{
	//最初に削除処理をしておく
	mNotes.clear();

	vector<RhythmPattern::Rhythm> rhythms = rhythmPattern->getRhythmPattern();
	//渡されたデータがドラムセットのサイズと一致していなければ
	if (!(rhythms.size() >= 7 && rhythmPattern->getRhythmPattern().size() <= 9)) {
		return;
	}
	for (int i = 0; i < rhythms.size(); ++i) {
		Note note;
		if (rhythms[i].type == RhythmPattern::RhythmType::BassDrum) {
			note.num = 36;					//GM配列に従ってバスドラムのノート番号を設定
		}
		else if (rhythms[i].type == RhythmPattern::RhythmType::SnareDrum) {
			note.num = 40;					//GM配列に従ってエレクトリック・スネアのノート番号を設定
		}
		else if (rhythms[i].type == RhythmPattern::RhythmType::FloorTom) {
			note.num = 43;					//GM配列に従ってフロアタムのノート番号を設定
		}
		else if (rhythms[i].type == RhythmPattern::RhythmType::Tom) {
			note.num = 50;					//GM配列に従ってハイタムのノート番号を設定
		}
		else if (rhythms[i].type == RhythmPattern::RhythmType::CloseHiHatCymbal) {
			note.num = 42;					//GM配列に従ってクローズハイハットのノート番号を設定
		}
		else if (rhythms[i].type == RhythmPattern::RhythmType::OpenHiHatCymbal) {
			note.num = 46;					//GM配列に従ってオープンハイハットのノート番号を設定
		}
		else if (rhythms[i].type == RhythmPattern::RhythmType::Cymbal) {
			note.num = 49;					//GM配列に従ってクラッシュシンバルのノート番号を設定
		}

		vector<RhythmPattern::Position> positions = rhythms[i].position;
		for (int j = 0; j < positions.size(); ++j) {
			note.startTime = positions[j].startTime;
			note.length = positions[j].length;
			note.velocity = 100;			//複雑な音量調整は今は考えない
			mNotes.push_back(note);
		}
	}
	//最後にソート処理で昇順に並べる
	std::sort(mNotes.begin(),mNotes.end(), [](const Note& a, const Note& b) {
		return a.startTime < b.startTime;
		});
}