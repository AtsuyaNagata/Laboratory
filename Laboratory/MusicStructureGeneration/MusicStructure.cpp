#include "MusicStructure.h"
#include <iostream>
using namespace std;

//必要なデータ：キー、音楽のパート進行、区間の長さ、コードパターン(ギター、ピアノなど楽器に対応させる予定)、リズムパターン、メロディパターン、ベースライン
MusicStructure::MusicStructure(int key, Scale scale, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<RhythmType> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<BaseAlgorithm> baseAls) :
	mMusicStruct(nullptr),
	mRhythmPattern(nullptr),
	mChords(nullptr),
	mMelody(nullptr)
{
	mRhythmPattern = new RhythmPattern;
	mChords = new Chords;
	mMelody = new Melody;
	prepare(key, scale, parts, lengths, rhythms, chordAls, meloAls, baseAls);
}

//曲の設計図を受け取って初期化するコンストラクタ。これを使うことは多分ない
MusicStructure::MusicStructure(const MusicStruct& mus) :
	mMusicStruct(nullptr),
	mRhythmPattern(nullptr),
	mChords(nullptr),
	mMelody(nullptr)
{
	mRhythmPattern = new RhythmPattern;
	mChords = new Chords;
	mMelody = new Melody;

	mMusicStruct = new MusicStruct;
	mMusicStruct->key = mus.key;
	mMusicStruct->scale = mus.scale;
	mMusicStruct->length = mus.length;
	mMusicStruct->parts = mus.parts;
	mMusicStruct->rhythms = mus.rhythms;
	mMusicStruct->chords = mus.chords;
	mMusicStruct->melodys = mus.melodys;
	mMusicStruct->bases = mus.bases;
	mMusicStruct->obbligatos = mus.obbligatos;
}

//引数なしのコンストラクタ
MusicStructure::MusicStructure() :
	mMusicStruct(nullptr),
	mRhythmPattern(nullptr),
	mChords(nullptr),
	mMelody(nullptr)
{
	mRhythmPattern = new RhythmPattern;
	mChords = new Chords;
	mMelody = new Melody;
}

//デストラクタ
MusicStructure::~MusicStructure() {
	delete mMusicStruct;
	mMusicStruct = nullptr;
}

//構造を初期化するための処理
void MusicStructure::prepare(int key, Scale scale, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<RhythmType> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<BaseAlgorithm> baseAls)
{
	//要素が既に存在してる時、一度全て削除してしまう(要素が変に残ってバグに気付きにくくなるのを防ぐための仕様)
	if (mMusicStruct != nullptr) {
		delete mMusicStruct;
		mMusicStruct = nullptr;
	}
	//各構造数がパート数と一致しない異常な入力だった時
	if (parts.size() != lengths.size() || parts.size() != rhythms.size() || parts.size() != chordAls.size() || parts.size() != meloAls.size() || parts.size() != baseAls.size()) {
		return;			//構造を作る前に落とすことで、次の参照時にnullptrを返すようにする
	}
	mMusicStruct = new MusicStruct;

	//初期値の設定。具体的なノートの生成等の処理は各オブジェクトクラスで行うので、分けるためにここでは抽象的な値しか格納しない。
	mMusicStruct->key = key;
	mMusicStruct->scale = scale;

	uint32_t length = 0;
	for (int i = 0; i < parts.size(); ++i) {
		//パートの構造を引数の値から構築する(Obbligato構造は特殊な物なので、ここで格納処理は行わない)
		PartStruct ps;
		ps.type = parts[i];
		ps.length = lengths[i];
		ps.startTimes = length;
		mMusicStruct->parts.push_back(ps);			//初期化したパート構造をプッシュ(引数から全ての要素が格納可能となっている)

		RhythmStruct rs;
		rs.type = rhythms[i];
		rs.fill_in = false;
		mMusicStruct->rhythms.push_back(rs);		//初期化したリズム構造をプッシュ(具体的な値は格納されていない)

		ChordStruct cs;
		cs.type = chordAls[i];
		mMusicStruct->chords.push_back(cs);			//初期化したコード構造をプッシュ(具体的な値は格納されていない)

		MelodyStruct ms;
		ms.type = meloAls[i];
		mMusicStruct->melodys.push_back(ms);		//初期化したメロディ構造をプッシュ(具体的な値は格納されていない)

		BaseStruct bs;
		bs.type = baseAls[i];
		mMusicStruct->bases.push_back(bs);			//初期化したベース構造をプッシュ(具体的な値は格納されていない)

		length += lengths[i];						//現在参照したパートの最後尾の位置の値になる
	}
	mMusicStruct->length = length;
}

void MusicStructure::create()
{
	//パート分だけ同様な処理を行う
	vector<PartStruct> parts = mMusicStruct->parts;
	for (int i = 0; i < parts.size(); ++i) {
		//==========ドラムのリズムパターンを生成する==========
		RhythmStruct& rs = mMusicStruct->rhythms[i];
		rs.type = RhythmType::BasicDownBeat8;
		rs.fill_in = false;
		//引数はドラムパターンタイプ、生成するドラムパターンの長さ、フィルインを入れるかのフラグ
		mRhythmPattern->createDrumRhythm(RhythmPattern::DrumPattern::BasicDownBeat8, parts[i].length, rs.fill_in);		//まだ基本的な8ビートしか実装していない
		vector<RhythmPattern::Rhythm> drumPattern = mRhythmPattern->getRhythmPattern();

		//もし異常なサイズ入力なら落とす
		if (drumPattern.size() != 8) {
			return;
		}

		for (int j = 0; j < drumPattern.size(); ++j) {
			DrumPattern dp;
			//ドラムパターン配列のインデックス値ごとに分類分けする処理
			if (j == 0) {
				dp.type = DrumType::BassDrum;
			}
			else if (j == 1) {
				dp.type = DrumType::SnareDrum;
			}
			else if (j == 2) {
				dp.type = DrumType::CloseHiHatCymbal;
			}
			else if (j == 3) {
				dp.type = DrumType::OpenHiHatCymbal;
			}
			else if (j == 4) {
				dp.type = DrumType::Tom;
			}
			else if (j == 5) {
				dp.type = DrumType::FloorTom;
			}
			else if (j == 6) {
				dp.type = DrumType::CrashCymbal;
			}
			else if (j == 7) {
				dp.type = DrumType::RideCymbal;
			}
			for (int k = 0; k < drumPattern[j].position.size(); ++k) {
				dp.startTimes.push_back(drumPattern[j].position[k].startTime);
			}
			rs.drumPatterns.push_back(dp);
		}

		//==========コード進行を生成する==========
		ChordStruct& cs = mMusicStruct->chords[i];
		Chords::Scale scale = Chords::Scale::None;
		if (mMusicStruct->scale == Scale::Major) {
			scale = Chords::Scale::Major;
		}
		else if (mMusicStruct->scale == Scale::Minor) {
			scale = Chords::Scale::Minor;
		}
		else if (mMusicStruct->scale == Scale::HarmonicMinor) {
			scale = Chords::Scale::HarmonicMinor;
		}
		else if (mMusicStruct->scale == Scale::MelodicMinor) {
			scale = Chords::Scale::MelodicMinor;
		}
		//コードの生成関数を呼び出す
		mChords->create(parts[i].length / (256 * 4), parts[i].length, mMusicStruct->key, scale);
		//生成したコードを用いて曲の設計図のデータを埋める
		cs.chords = mChords->getChords();

		//==========メロディを生成する==========
		//TODO：エラーが出てる
		MelodyStruct& ms = mMusicStruct->melodys[i];
		mRhythmPattern->createMelodyRhythm(parts[i].length, 128);		//メロディ用のリズムパターンを生成。8分音符を最小単位としてる(要修正)
		mMelody->createMelody(mRhythmPattern, mChords);
		vector<Melody::Note> melodyNotes = mMelody->getMelodyNotes();
		for (int j = 0; j < melodyNotes.size(); ++j) {
			Note note;
			note.length = melodyNotes[i].length;
			note.num = melodyNotes[i].num;
			note.startTime = melodyNotes[i].startTime;

			ms.notes.push_back(note);
		}

		//==========ベースラインを生成する==========
		BaseStruct& bs = mMusicStruct->bases[i];
		mRhythmPattern->createMelodyRhythm(parts[i].length, 128);						//メロディ用のリズムパターンを流用してる。8分音符を最小単位としてる(要修正)
		mMelody->createBase(mRhythmPattern, mChords, Melody::BaseAlgorithm::Melo);		//ベースラインを生成(現在はメロディ風に作るアルゴリズムのみ動作してる)
		vector<Melody::Note> baseNote = mMelody->getMelodyNotes();
		for (int j = 0; j < baseNote.size(); ++j) {
			Note note;
			note.length = melodyNotes[i].length;
			note.num = melodyNotes[i].num;
			note.startTime = melodyNotes[i].startTime;

			bs.notes.push_back(note);
		}
	}
}

void MusicStructure::printMusicStruct()
{
	cout << "曲のキー：" << mMusicStruct->key << endl;
	cout << "曲のスケール：";
	if (mMusicStruct->scale == Scale::Major) {
		cout << "メジャー";
	}
	else if (mMusicStruct->scale == Scale::Minor) {
		cout << "マイナー";
	}
	else if (mMusicStruct->scale == Scale::HarmonicMinor) {
		cout << "ハーモニックマイナー";
	}
	else if (mMusicStruct->scale == Scale::MelodicMinor) {
		cout << "メロディックマイナー";
	}
	cout << endl << endl;

	//各パートについて表示する処理
	for (int i = 0; i < mMusicStruct->parts.size(); ++i) {
		//==========パートの情報の描画==========
		cout << "パート" << i << "の種類：";
		if (mMusicStruct->parts[i].type == Part::A) {
			cout << "A";
		}
		else if (mMusicStruct->parts[i].type == Part::B) {
			cout << "B";
		}
		else if (mMusicStruct->parts[i].type == Part::C) {
			cout << "C";
		}
		else if (mMusicStruct->parts[i].type == Part::Climax) {
			cout << "Climax";
		}
		else if (mMusicStruct->parts[i].type == Part::Intro) {
			cout << "Intro";
		}
		else if (mMusicStruct->parts[i].type == Part::Outro) {
			cout << "Outro";
		}
		else if (mMusicStruct->parts[i].type == Part::Interlude) {
			cout << "Interlude";
		}
		else if (mMusicStruct->parts[i].type == Part::Special) {
			cout << "Special";
		}
		cout << endl;
		cout << "パートの開始時間：" << mMusicStruct->parts[i].startTimes << "　パートの長さ：" << mMusicStruct->parts[i].length << endl << endl;

		//==========ドラムのリズムパターンについての描画==========
		cout << "ドラムのパターンの種類：";
		if (mMusicStruct->rhythms[i].type == RhythmType::BasicDownBeat8) {
			cout << "基本8ビート";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::BasicUpBeat8) {
			cout << "基本8ビート 裏打ち";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::HalfTimeShuffle) {
			cout << "ハーフタイムシャッフル";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::ShuffleBeat) {
			cout << "シャッフルビート";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::SlipBeat8) {
			cout << "スリップ8ビート";
		}
		cout << endl << endl;

		//==========コード列についての描画==========
		cout << "コード生成アルゴリズム：";
		if (mMusicStruct->chords[i].type == ChordAlgorithm::Simple) {
			cout << "シンプル";
		}
		else if (mMusicStruct->chords[i].type == ChordAlgorithm::Fashionable) {
			cout << "オシャレ";
		}
		else if (mMusicStruct->chords[i].type == ChordAlgorithm::Modulation) {
			cout << "転調";
		}
		vector<Chords::Chord> chords = mMusicStruct->chords[i].chords;
		cout << "　コード列：";
		for (int j = 0; j < chords.size(); ++j) {
			//Cのようなコードにおいて階名部分の描画
			if (chords[j].baseNoteNum % 12 == 0) {
				cout << "C";
			}
			else if (chords[j].baseNoteNum % 12 == 1) {
				cout << "C#";
			}
			else if (chords[j].baseNoteNum % 12 == 2) {
				cout << "D";
			}
			else if (chords[j].baseNoteNum % 12 == 3) {
				cout << "D#";

			}
			else if (chords[j].baseNoteNum % 12 == 4) {
				cout << "E";
			}
			else if (chords[j].baseNoteNum % 12 == 5) {
				cout << "F";
			}
			else if (chords[j].baseNoteNum % 12 == 6) {
				cout << "F#";
			}
			else if (chords[j].baseNoteNum % 12 == 7) {
				cout << "G";
			}
			else if (chords[j].baseNoteNum % 12 == 8) {
				cout << "A♭";
			}
			else if (chords[j].baseNoteNum % 12 == 9) {
				cout << "A";
			}
			else if (chords[j].baseNoteNum % 12 == 10) {
				cout << "B♭";
			}
			else if (chords[j].baseNoteNum % 12 == 11) {
				cout << "B";
			}

			//和音の種類について記述する
			if (chords[j].type == Chords::ChordType::Major) {
				//何も記述しない
			}
			else if (chords[j].type == Chords::ChordType::Minor) {
				cout << "m";
			}
			else if (chords[j].type == Chords::ChordType::Aug) {
				cout << "aug";
			}
			//マイナー♭は書き方が特殊なので2個描画処理がある
			else if (chords[j].type == Chords::ChordType::mb5) {
				if (chords[j].plus != Chords::ChordPlus::Dim7) {
					cout << "m";
				}
			}

			//4つ目の音についての付加情報を記述する
			if (chords[j].plus == Chords::ChordPlus::Seventh) {
				cout << "7";
			}
			else if (chords[j].plus == Chords::ChordPlus::MajorSeventh) {
				cout << "M7";
			}
			else if (chords[j].plus == Chords::ChordPlus::Six) {
				cout << "6";
			}
			else if (chords[j].plus == Chords::ChordPlus::Add9) {
				cout << "Add9";
			}
			else if (chords[j].plus == Chords::ChordPlus::Add11) {
				cout << "Add11";
			}
			else if (chords[j].plus == Chords::ChordPlus::Add13) {
				cout << "Add13";
			}
			else if (chords[j].plus == Chords::ChordPlus::Dim7) {
				cout << "Dim7";
			}

			//3和音の構成に属するが、4和音の情報が必要になる部分の描画
			if (chords[j].type == Chords::ChordType::Sus2) {
				cout << "sus2";
			}
			else if (chords[j].type == Chords::ChordType::Sus4) {
				cout << "sus4";
			}
			else if (chords[j].type == Chords::ChordType::mb5) {
				if (chords[j].plus != Chords::ChordPlus::Dim7) {
					cout << "-5";
				}
			}
			//5つ目の音の付加情報を記述する
			if (chords[j].tension == Chords::ChordTension::FlatNinth) {
				cout << "♭9";
			}
			else if (chords[j].tension == Chords::ChordTension::Ninth) {
				cout << "9";
			}
			else if (chords[j].tension == Chords::ChordTension::SharpNinth) {
				cout << "#9";
			}
			else if (chords[j].tension == Chords::ChordTension::Eleventh) {
				cout << "11";
			}
			else if (chords[j].tension == Chords::ChordTension::SharpEleventh) {
				cout << "#11";
			}
			else if (chords[j].tension == Chords::ChordTension::FlatThirteenth) {
				cout << "♭13";
			}
			else if (chords[j].tension == Chords::ChordTension::Thirteenth) {
				cout << "13";
			}

			//オンコードの記述
			if (chords[j].onIndex != chords[j].baseIndex) {					//ノートの基底音とオンコードの値が一致していない時のみオンコードの描画を行う
				cout << "/";
				if (chords[j].onIndex == 0) {
					cout << "C";
				}
				else if (chords[j].onIndex == 1) {
					cout << "C#";
				}
				else if (chords[j].onIndex == 2) {
					cout << "D";
				}
				else if (chords[j].onIndex == 3) {
					cout << "D#";
				}
				else if (chords[j].onIndex == 4) {
					cout << "E";
				}
				else if (chords[j].onIndex == 5) {
					cout << "F";
				}
				else if (chords[j].onIndex == 6) {
					cout << "F#";
				}
				else if (chords[j].onIndex == 7) {
					cout << "G";
				}
				else if (chords[j].onIndex == 8) {
					cout << "A♭";
				}
				else if (chords[j].onIndex == 9) {
					cout << "A";
				}
				else if (chords[j].onIndex == 10) {
					cout << "B♭";
				}
				else if (chords[j].onIndex == 11) {
					cout << "B";
				}
			}
			//オミットコードは今は考えない
			cout << "　";
		}
		cout << endl << endl;

		//==========メロディについての描画==========
		cout << "メロディ生成アルゴリズム：";
		if (mMusicStruct->melodys[i].type == MelodyAlgorithm::Standard) {
			cout << "Standard";
		}
		cout << endl;
		vector<Note> notes = mMusicStruct->melodys[i].notes;
		cout << "メロディのノート列：(パートから見て相対的な開始時間、長さ、ノート番号)" << endl;
		for (int j = 0; j < notes.size(); ++j) {
			cout << notes[j].startTime << ", " << notes[i].length << ", " << notes[i].num << "　";
		}
		cout << endl << endl;

		//==========ベースについての描画==========
		cout << "ベース生成アルゴリズム：";
		if (mMusicStruct->bases[i].type == BaseAlgorithm::Simple) {
			cout << "Simple";
		}
		else if (mMusicStruct->bases[i].type == BaseAlgorithm::UpDown) {
			cout << "Updown";
		}
		else if (mMusicStruct->bases[i].type == BaseAlgorithm::Melo) {
			cout << "Melo";
		}
		cout << endl;
		notes = mMusicStruct->bases[i].notes;
		cout << "ベースのノート列：(パートから見て相対的な開始時間、長さ、ノート番号)" << endl;
		for (int j = 0; j < notes.size(); ++j) {
			cout << notes[j].startTime << ", " << notes[i].length << ", " << notes[i].num << "　";
		}
		cout << endl << endl;
	}
}