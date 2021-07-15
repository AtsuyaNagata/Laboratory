#include "RhythmPattern.h"
#include <algorithm>
#include <ctime>

RhythmPattern::RhythmPattern():
	mRhythm(0)
{
	std::srand(std::time(nullptr));						//乱数をリセット
}

RhythmPattern::~RhythmPattern() {
	for (int i = 0; i < mRhythm.size(); ++i) {
		mRhythm[i].position.clear();
	}
	mRhythm.clear();
}

//メロディのリズムパターンの生成なので作るリズムは一つだけ
void RhythmPattern::createMelodyRhythm(int size, int mostSmall) {
	//すでにある要素を削除
	for (int i = 0; i < mRhythm.size(); ++i) {
		mRhythm[i].position.clear();
	}
	mRhythm.clear();

	Rhythm rhythm;
	rhythm.type = RhythmType::Melody;					//メロディのリズムであることを示しておく
	mRhythm.push_back(rhythm);
	createSplitRhythmPattern_melody(size, mostSmall);
	vector<Position>& positions = mRhythm[0].position;
	for (int i = 0; i < positions.size(); ++i) {
		//適当に削除処理を行って、休符を作る
		if ((rand() - 1) * 100 / RAND_MAX < 20) {
			//最後尾の要素と交換してポップする(そっちの方が早いから)
			std::swap(positions[i], positions.back());
			positions.pop_back();
		}
	}
	//開始時点の比較でリズムパターンのソートを行う
	std::sort(positions.begin(), positions.end(), [](const Position& a, const Position& b) {
		return a.startTime < b.startTime;
	});
}

//再帰的にリズムを生成する処理
void RhythmPattern::createSplitRhythmPattern_melody(int size, int mostSmall){
	Position position;
	//sizeが最小サイズ or 確率以内ならプッシュ処理を行う
	if (size <= mostSmall || (rand() - 1) * 100 / RAND_MAX < 100 * mostSmall * mostSmall / (size * size)) {
		const vector<Position>& rPosition = mRhythm[0].position;			//一つ前の位置情報
		position.length = size;
		//サイズが0ならば
		if (rPosition.size() == 0) {
			position.startTime = 0;
		}
		else {
			position.startTime = rPosition[rPosition.size() - 1].startTime + rPosition[rPosition.size() - 1].length;
		}
		mRhythm[0].position.push_back(position);
		return;
	}

	//半分に分割してそれぞれ実行！
	createSplitRhythmPattern_melody(size / 2, mostSmall);
	createSplitRhythmPattern_melody(size / 2, mostSmall);
}

void RhythmPattern::createDrumRhythm(DrumPattern drumPattern, uint32_t length, bool fillIn) {
	//すでにある要素を削除
	for (int i = 0; i < mRhythm.size(); ++i) {
		mRhythm[i].position.clear();
	}
	mRhythm.clear();

	int quarterNoteNum = length / 256;			//長さを四分音符何個かで表す

	if (drumPattern == DrumPattern::None) {
		return;
	}

	//ドラムセットを表現する様に、リズムパターンの配列に対応する楽器を入れる
	Rhythm rhythm;
	//インデックス0：バスドラム
	rhythm.type = RhythmType::BassDrum;
	mRhythm.push_back(rhythm);
	//インデックス1：スネアドラム
	rhythm.type = RhythmType::SnareDrum;
	mRhythm.push_back(rhythm);
	//インデックス2：クローズドハイハット
	rhythm.type = RhythmType::CloseHiHatCymbal;
	mRhythm.push_back(rhythm);
	//インデックス3：オープンハイハット
	rhythm.type = RhythmType::OpenHiHatCymbal;
	mRhythm.push_back(rhythm);
	//インデックス4：タム
	rhythm.type = RhythmType::Tom;
	mRhythm.push_back(rhythm);
	//インデックス5：フロアタム
	rhythm.type = RhythmType::FloorTom;
	mRhythm.push_back(rhythm);
	//インデックス6：シンバル
	rhythm.type = RhythmType::Cymbal;
	mRhythm.push_back(rhythm);

	Position position;								//一時的に位置情報を保持するためのバッファ

	if (drumPattern == DrumPattern::BasicDownBeat8) {

		//8ビートの基本パターンを構築する
		for (int i = 0; i < quarterNoteNum * 2; ++i) {		//八分音符ごとに処理を行う
			//八分音符4個分ごとにバスドラムの音を設置する
			if (i % 4 == 0) {
				position.startTime = i * 128;
				position.length = 256 / 8;					//長さは32分音符で固定
				mRhythm[0].position.push_back(position);	//バスドラムに位置情報を追加
			}
			//八分音符4個分ごとにスネアドラムの音を設定する(バスドラとは2分音符分ズレる)
			if (i % 4 == 2) {
				position.startTime = i * 128;
				position.length = 256 / 8;					//長さは32分音符で固定
				mRhythm[1].position.push_back(position);	//スネアドラムに位置情報を追加
			}
			//毎ループクローズドハイハットを入れる
			position.startTime = i * 128;
			position.length = 256 / 8;						//長さは32分音符で固定
			mRhythm[2].position.push_back(position);		//クローズドハイハットに位置情報を追加
		}
		//----------ここからバリエーションを設ける----------
		//バスドラムのバリエーション
		for (int i = 0; i < quarterNoteNum * 2; ++i){
			int randNum = (rand() - 1) * 100 / RAND_MAX;
			//8分音符2つ目の位置にバスドラムの音を設置するかどうか
			if (i % 8 == 1) {
				if (randNum < 20) {
					position.startTime = i * 128;
					position.length = 256 / 8;
					mRhythm[0].position.push_back(position);
				}
			}
			//8分音符4つ目の位置に音を設置するかどうか
			if (i % 8 == 3) {
				//バスドラムの音を設置するかどうかを確率で判定する
				if (randNum < 20) {
					position.startTime = i * 128;
					position.length = 256 / 8;
					mRhythm[0].position.push_back(position);
					//更に半分の確率で8分音符5番目のバスドラを削除し、代わりに6番目の8分音符にバスドラを入れる
					if (randNum < 10) {
						for (int i = 0; i < mRhythm[0].position.size(); ++i) {					//全てのスネアの要素を走査する
							if (mRhythm[0].position[i].startTime == (i + 1) * 128) {			//8分音符3つ目と位置が一致してたら(i%8=3なので、iから1引いて位置を計算)
								//最後尾の要素と交換してポップする(そっちの方が早いから)
								std::swap(mRhythm[0].position[i], mRhythm[0].position.back());
								mRhythm[0].position.pop_back();
							}
						}
						position.startTime = (i + 2) * 128;
						position.length = 256 / 8;
						mRhythm[0].position.push_back(position);
					}
				}
				//スネアを8分音符4つ目に設置するかどうかを確率で判定する
				else if (randNum < 40) {
					position.startTime = i * 128;
					position.length = 256 / 8;
					mRhythm[1].position.push_back(position);
					//更に半分の確率で8分音符3つ目のスネア音を消す
					if (randNum < 30) {
						for (int i = 0; i < mRhythm[1].position.size(); ++i) {					//全てのスネアの要素を走査する
							if (mRhythm[1].position[i].startTime == (i - 1) * 128) {			//8分音符3つ目と位置が一致してたら(i%8=3なので、iから1引いて位置を計算)
								//最後尾の要素と交換してポップする(そっちの方が早いから)
								std::swap(mRhythm[1].position[i], mRhythm[1].position.back());
								mRhythm[1].position.pop_back();
							}
						}
					}
				}
			}

			//8分音符6つ目の位置に音を設置するかどうか
			if (i % 8 == 5) {
				//スネアを8分音符4つ目に設置するかどうかを確率で判定する
				if (randNum < 20) {
					position.startTime = i * 128;
					position.length = 256 / 8;
					mRhythm[1].position.push_back(position);
				}
			}

			//クラッシュシンバルは8分音符16個ごとに判定を設ける(2小節ごとに判定する)
			if (i % 16 == 15) {
				if (randNum < 80) {
					position.startTime = i * 128;
					position.length = 256 / 8;
					mRhythm[6].position.push_back(position);
				}
			}
		}

		//最後に纏めてソート処理を行う(多分かなり重い処理)
		for (int i = 0; i < mRhythm.size(); ++i) {
			std::sort(mRhythm[i].position.begin(), mRhythm[i].position.end(), [](const Position& a, const Position& b) {
				return a.startTime < b.startTime;
			});
		}
	}

	//気が向いたら追加する
	else if (drumPattern == DrumPattern::BasicUpBeat8) {

	}
	else if (drumPattern == DrumPattern::SlipBeat8) {

	}
	else if (drumPattern == DrumPattern::ShuffleBeat) {

	}
	else if (drumPattern == DrumPattern::HalfTimeShuffle) {

	}
}