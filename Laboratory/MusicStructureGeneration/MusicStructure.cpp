#include "MusicStructure.h"
#include <iostream>
#include <string>
#include <fstream>
#include "MIDI.h"
#include <algorithm>
using namespace std;

//必要なデータ：キー、音楽のパート進行、区間の長さ、コードパターン(ギター、ピアノなど楽器に対応させる予定)、リズムパターン、メロディパターン、ベースライン
MusicStructure::MusicStructure(int key, Scale scale, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<RhythmType> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<BaseAlgorithm> baseAls) :
	mMusicStruct(nullptr),
	mRhythmPattern(nullptr),
	mChords(nullptr),
	mMelody(nullptr),
	mMIDI(nullptr)
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
	mMelody(nullptr),
	mMIDI(nullptr)
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
	mMelody(nullptr),
	mMIDI(nullptr)
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
		mMelody->createDrum(mRhythmPattern);
		vector<Melody::Note> drumNotes = mMelody->getMelodyNotes();

		//もし異常なサイズ入力なら落とす
		if (drumPattern.size() != 7) {
			return;
		}
		for (int j = 0; j < drumNotes.size(); ++j) {
			Note note;
			note.length = drumNotes[j].length;
			note.num = drumNotes[j].num;
			note.startTime = drumNotes[j].startTime;
			note.velocity = 100;

			rs.notes.push_back(note);
		}

		//cout << "end of drum" << endl;

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
		mChords->create(parts[i].length / (256 * 4), parts[i].length, mMusicStruct->key, scale, Chords::Algorithm::Fashionable);
		//生成したコードを用いて曲の設計図のデータを埋める
		cs.chords = mChords->getChords();
		//cout << "end of chord" << endl;


		//==========メロディを生成する==========
		MelodyStruct& ms = mMusicStruct->melodys[i];
		mRhythmPattern->createMelodyRhythm(parts[i].length, 128);		//メロディ用のリズムパターンを生成。8分音符を最小単位としてる(要修正)
		mMelody->createMelody(mRhythmPattern, mChords);
		vector<Melody::Note> melodyNotes = mMelody->getMelodyNotes();
		for (int j = 0; j < melodyNotes.size(); ++j) {
			Note note;
			note.length = melodyNotes[j].length;
			note.num = melodyNotes[j].num;
			note.startTime = melodyNotes[j].startTime;
			note.velocity = 100;

			ms.notes.push_back(note);
		}
		//cout << "end of melody" << endl;

		//==========ベースラインを生成する==========
		BaseStruct& bs = mMusicStruct->bases[i];
		mRhythmPattern->createMelodyRhythm(parts[i].length, 128);						//メロディ用のリズムパターンを流用してる。8分音符を最小単位としてる(要修正)
		mMelody->createBase(mRhythmPattern, mChords, Melody::BaseAlgorithm::Melo);		//ベースラインを生成(現在はメロディ風に作るアルゴリズムのみ動作してる)
		vector<Melody::Note> baseNote = mMelody->getMelodyNotes();
		for (int j = 0; j < baseNote.size(); ++j) {
			Note note;
			note.length = baseNote[j].length;
			note.num = baseNote[j].num;
			note.startTime = baseNote[j].startTime;
			note.velocity = 90;							//ちょっと小さめの音にしてる

			bs.notes.push_back(note);
		}
		//cout << "end of base" << endl;

	}
}

//曲構造で生成した音楽情報をMIDIファイルに書き込む処理
void MusicStructure::outputMIDI()
{
	//もしMIDIクラスのインスタンスが存在していなかったら
	if (mMIDI != nullptr) {
		delete mMIDI;
		mMIDI = nullptr;
	}

	mMIDI = new MIDI{};		//MIDI構造を生成しておく

	vector<Note> notes;
	string filename;
	//各パートごとにMIDIファイルを生成する
	for (int i = 0; i < mMusicStruct->parts.size(); ++i) {
		//始めにドラムのMIDIファイルを生成する
		notes = mMusicStruct->rhythms[i].notes;
		filename = "./data/drum/drum_part" + to_string(i);
		filename += ".mid";
		writeMidiFile(filename.c_str(), notes);

		//次にコードのMIDIファイルを生成する
		notes = chordToNote(mMusicStruct->chords[i].chords);			//コードからノートデータを構築して格納
		filename = "./data/chord/Chord_part" + to_string(i);
		filename += ".mid";
		writeMidiFile(filename.c_str(), notes);

		//さらに次にメロディのMIDIファイルを生成する
		notes = mMusicStruct->melodys[i].notes;
		filename = "./data/melody/melody_part" + to_string(i);
		filename += ".mid";
		writeMidiFile(filename.c_str(), notes);

		//最後にベースのMIDIファイルを生成する
		notes = mMusicStruct->bases[i].notes;
		filename = "./data/base/base_part" + to_string(i);
		filename += ".mid";
		writeMidiFile(filename.c_str(), notes);
		
	}//mMIDIの構造をMIDIファイルに書き込み

	//オブリガートの出力
	for (int i = 0; i < mMusicStruct->obbligatos.size(); ++i) {
		notes = mMusicStruct->obbligatos[i].notes;
		filename = "Obbligato" + to_string(i);
		filename += ".mid";
		writeMidiFile(filename.c_str(), notes);
	}

	return;
}

void MusicStructure::writeMidiFile(const char* filename, vector<Note> notes)
{
	//MIDI構造が構築されていなかったら異常なので落とす
	if (mMIDI == nullptr) {
		return;
	}

	vector<MIDI::MIDINoteEvent> noteEvents;

	vector<Note> wNote;			//書き込み用のデータを入れる領域
	wNote = notes;				//ノート・オンの要素をコピー

	//wNoteにノート・オフのイベントとなる要素を入れていく（既にノートオンの情報はwNoteに入ってる）
	Note noteBuff;
	int noteSize = wNote.size();
	for (int i = 0; i < noteSize; ++i) {
		//ノートオフの要素を格納していく
		noteBuff.startTime = notes[i].startTime + notes[i].length;		//ノートオフイベントの開始時点は「対応したノートオンイベントの開始時点 + その長さ」で表すことが出来る
		noteBuff.num = notes[i].num;									//ノートの値は一致させておく
		noteBuff.length = 0;											//長さ情報は必要ない事を明示するために0を格納してる
		noteBuff.velocity = 0;											//ベロシティを0として入力する事で、ノートオンイベントでノートオフを表現可能となる

		wNote.push_back(noteBuff);
	}

	//ソートした順番で処理することでノート同士が一部重なった状態は表現可能だが、ノートinノートの状態は表せなくなってる。これはSMFの仕様上しょうがない
	std::sort(wNote.begin(), wNote.end(), [](const Note& a, const Note& b) {
		return a.startTime < b.startTime;								//ノートの開始地点が早い順でソートする
		});

	size_t time = 0;													//デルタタイムを表現するためにループ内の進んだ時間値を格納する
	for (int i = 0; i < wNote.size(); ++i) {
		MIDI::MIDINoteEvent noteEvent;
		noteEvent.time = (wNote[i].startTime * 960 / 256) - time;		//相対的な時間を割り出し、デルタタイムとする
		time += noteEvent.time;											//現在時点を更新する

		noteEvent.noteNum = wNote[i].num;								//時間以外の必要な値をそのまま格納する
		noteEvent.velocity = wNote[i].velocity;							//上に同じ

		noteEvents.push_back(noteEvent);
	}
	mMIDI->createMIDI_oneTrack(noteEvents, wNote.size(), 120);			//1トラックかつ音の列を表現するのに最低限の要素を格納したMIDIオブジェクトを生成してmMIDIに適応させる
	mMIDI->MIDIWrite(filename);

	return;
}

//コードからノート列を生成する。アルペジオなどは考えず、シンプルなコード進行のノートになる。
vector<MusicStructure::Note> MusicStructure::chordToNote(vector<Chords::Chord> chords)
{
	uint32_t time = 0;			//ループ内の現在時間を表す
	vector<Note> notes;			//全体的なノートデータの入れ物。今回の関数で返すもの
	Note note;					//ノートの配列にプッシュバックするためのバッファ
	vector<Note> chordNotes;	//コードを構成するノート群を表現
	note.velocity = 100;		//音量は100で固定
	for (int i = 0; i < chords.size(); ++i) {
		//毎回コードのノート群は初期化しとく
		chordNotes.clear();
		//最初に、ある一つのコードを構成するのに必要な基本情報をバッファに格納する
		note.startTime = time;
		note.length = chords[i].length;
		note.num = chords[i].baseNoteNum + 0;			//基底音のノート値
		chordNotes.push_back(note);						//基底音は共通の要素なのでコードのノート群に入れておく

		//ここからコードごとに表現が違う要素を格納する
		//基本のコード構成音の格納
		if (chords[i].type == Chords::ChordType::Major) {
			//メジャーコードの構成音のノート番号を格納(開始時点や長さは基底音と一緒)
			note.num = chords[i].baseNoteNum + 4;		//コードの2つ目のノート値を格納
			chordNotes.push_back(note);					//コードのノート群に追加
			note.num = chords[i].baseNoteNum + 7;		//コードの3つ目のノート値を格納
			chordNotes.push_back(note);					//コードのノート群に追加
		}
		else if (chords[i].type == Chords::ChordType::Minor) {
			//マイナーコードの構成音のノート番号を格納(開始時点や長さは基底音と一緒)
			note.num = chords[i].baseNoteNum + 3;
			chordNotes.push_back(note);
			note.num = chords[i].baseNoteNum + 7;
			chordNotes.push_back(note);
		}
		else if (chords[i].type == Chords::ChordType::Aug) {
			//Augコードの構成音のノート番号を格納(開始時点や長さは基底音と一緒)
			note.num = chords[i].baseNoteNum + 4;
			chordNotes.push_back(note);
			note.num = chords[i].baseNoteNum + 8;
			chordNotes.push_back(note);
		}
		else if (chords[i].type == Chords::ChordType::mb5) {
			//mb5コードの構成音のノート番号を格納(開始時点や長さは基底音と一緒)
			note.num = chords[i].baseNoteNum + 3;
			chordNotes.push_back(note);
			note.num = chords[i].baseNoteNum + 6;
			chordNotes.push_back(note);
		}
		else if (chords[i].type == Chords::ChordType::Sus2) {
			//sus2コードの構成音のノート番号を格納(開始時点や長さは基底音と一緒)
			note.num = chords[i].baseNoteNum + 2;
			chordNotes.push_back(note);
			note.num = chords[i].baseNoteNum + 7;
			chordNotes.push_back(note);
		}
		else if (chords[i].type == Chords::ChordType::Sus4) {
			//sus4コードの構成音のノート番号を格納(開始時点や長さは基底音と一緒)
			note.num = chords[i].baseNoteNum + 5;
			chordNotes.push_back(note);
			note.num = chords[i].baseNoteNum + 7;
			chordNotes.push_back(note);
		}

		//4音目のノートが存在するコードだったら、コードのノート群に4音目をさらに追加する
		if (chords[i].plus == Chords::ChordPlus::Add9) {
			for (int j = 0; j < 7; ++j) {
				if (chords[i].baseIndex == mChords->getFloor()[j]) {
					uint8_t floorBuff = mChords->getFloor()[(j + 1) % 7] + 12 - mChords->getFloor()[j];		//相対的に見て2度の位置にノートを追加するために差を計算する
					if (floorBuff > 12) {
						floorBuff -= 12;
					}
					note.num = chords[i].baseNoteNum + floorBuff;							//9度を入れたいが高すぎるので、オクターブ下の2度を入れる
					chordNotes.push_back(note);
					break;
				}
			}
		}
		else if (chords[i].plus == Chords::ChordPlus::Add11) {
			for (int j = 0; j < 7; ++j) {
				if (chords[i].baseIndex == mChords->getFloor()[j]) {
					uint8_t floorBuff = mChords->getFloor()[(j + 1) % 7] + 12 - mChords->getFloor()[j];		//相対的に見て4度の位置にノートを追加するために差を計算する
					if (floorBuff > 12) {
						floorBuff -= 12;
					}
					note.num = chords[i].baseNoteNum + floorBuff;			//11度を入れたいが高すぎるので、オクターブ下の4度を入れる
					chordNotes.push_back(note);
					break;
				}
			}
		}
		else if (chords[i].plus == Chords::ChordPlus::Add13) {
			for (int j = 0; j < 7; ++j) {
				if (chords[i].baseIndex == mChords->getFloor()[j]) {
					uint8_t floorBuff = mChords->getFloor()[(j + 1) % 7] + 12 - mChords->getFloor()[j];		//相対的に見て6度の位置にノートを追加するために差を計算する
					if (floorBuff > 12) {
						floorBuff -= 12;
					}
					note.num = chords[i].baseNoteNum + floorBuff;			//13度を入れたいが高すぎるので、オクターブ下の6度を入れる
					chordNotes.push_back(note);
					break;
				}
			}
		}
		else if (chords[i].plus == Chords::ChordPlus::Dim7) {
			if (chords[i].type == Chords::ChordType::mb5) {						//3和音がマイナー♭ファイブでなければDimは無効化される
				note.num = chords[i].baseNoteNum + 9;							//9半音上の音を追加
				chordNotes.push_back(note);
			}
		}
		else if (chords[i].plus == Chords::ChordPlus::MajorSeventh) {
			note.num = chords[i].baseNoteNum + 11;								//長7度の音を追加
			chordNotes.push_back(note);
		}
		else if (chords[i].plus == Chords::ChordPlus::Seventh) {
			note.num = chords[i].baseNoteNum + 10;								//短7度の音を追加
			chordNotes.push_back(note);
		}
		else if (chords[i].plus == Chords::ChordPlus::Six) {
			note.num = chords[i].baseNoteNum + 9;								//長6度の音を追加
			chordNotes.push_back(note);
		}

		//5つ目のノートが存在するコードだったら、5音目を追加する
		else if (chords[i].tension == Chords::ChordTension::FlatNinth) {
			note.num = chords[i].baseNoteNum + 13;						//短9度を追加
			chordNotes.push_back(note);
		}
		else if (chords[i].tension == Chords::ChordTension::Ninth) {
			note.num = chords[i].baseNoteNum + 14;						//長9度を追加
			chordNotes.push_back(note);
		}
		else if (chords[i].tension == Chords::ChordTension::SharpNinth) {
			note.num = chords[i].baseNoteNum + 15;						//増9度を追加
			chordNotes.push_back(note);
		}
		else if (chords[i].tension == Chords::ChordTension::Eleventh) {
			note.num = chords[i].baseNoteNum + 17;						//完全11度を追加
			chordNotes.push_back(note);
		}
		else if (chords[i].tension == Chords::ChordTension::SharpEleventh) {
			note.num = chords[i].baseNoteNum + 18;						//増11度を追加
			chordNotes.push_back(note);
		}
		else if (chords[i].tension == Chords::ChordTension::FlatThirteenth) {
			note.num = chords[i].baseNoteNum + 20;						//短13度を追加
			chordNotes.push_back(note);
		}
		else if (chords[i].tension == Chords::ChordTension::FlatThirteenth) {
			note.num = chords[i].baseNoteNum + 21;						//長13度を追加
			chordNotes.push_back(note);
		}

		if (chords[i].onIndex < 12) {									//オンコードが存在するなら(12未満の数字だったら)
			note.num = chords[i].baseNoteNum - 12 + chords[i].onIndex;	//オンコードのノートを通常の一オクターブ下の位置に追加する
			chordNotes.push_back(note);
		}
		if (chords[i].omitIndex < 12) {									//omitコードならば(12未満の数ならば)
			//omitに書かれたノートを探して削除する
			for (int j = 0; j < chordNotes.size(); ++j) {
				if (chordNotes[j].num == chords[i].omitIndex) {
					swap(chordNotes[j], chordNotes.back());
					chordNotes.pop_back();
				}
			}
		}

		//コードのノート群の開始時点は全て同じなので、特別ソートを行う必要はない

		//ノート配列に今回のコードのノート群を追加
		for (int j = 0; j < chordNotes.size(); ++j) {
			notes.push_back(chordNotes[j]);
		}
		
		//最後に時間を更新する
		time += chords[i].length;
	}

	return notes;
}

//--------------------ここから曲構造の描画、出力処理(長ったらしいので後ろに移動した)--------------------
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
			cout << "「" << notes[j].startTime << ", " << notes[j].length << ", " << (int)notes[j].num << "」　";
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
			cout << "「" << notes[j].startTime << ", " << notes[j].length << ", " << (int)notes[j].num << "」　";
		}
		cout << endl << endl;
	}

	return;
}

//マークダウンに曲構造を記述する処理
void MusicStructure::outputMarkDown(const char* filename)
{
	ofstream ofs(filename);
	if (!ofs.is_open()) {
		cout << "MusicStruct：outputMarkDown error" << endl;
		return;
	}

	ofs << "# 生成した曲の構造" << endl << endl;

	ofs << "## 曲のキー：";
	if (mMusicStruct->key % 12 == 0) {
		ofs << "C";
	}
	else if (mMusicStruct->key % 12 == 1) {
		ofs << "C#";
	}
	else if (mMusicStruct->key % 12 == 2) {
		ofs << "D";
	}
	else if (mMusicStruct->key % 12 == 3) {
		ofs << "D#";

	}
	else if (mMusicStruct->key % 12 == 4) {
		ofs << "E";
	}
	else if (mMusicStruct->key % 12 == 5) {
		ofs << "F";
	}
	else if (mMusicStruct->key % 12 == 6) {
		ofs << "F#";
	}
	else if (mMusicStruct->key % 12 == 7) {
		ofs << "G";
	}
	else if (mMusicStruct->key % 12 == 8) {
		ofs << "A♭";
	}
	else if (mMusicStruct->key % 12 == 9) {
		ofs << "A";
	}
	else if (mMusicStruct->key % 12 == 10) {
		ofs << "B♭";
	}
	else if (mMusicStruct->key % 12 == 11) {
		ofs << "B";
	}
	ofs << endl << endl;

	ofs << "## 曲のスケール：";
	if (mMusicStruct->scale == Scale::Major) {
		ofs << "メジャー";
	}
	else if (mMusicStruct->scale == Scale::Minor) {
		ofs << "マイナー";
	}
	else if (mMusicStruct->scale == Scale::HarmonicMinor) {
		ofs << "ハーモニックマイナー";
	}
	else if (mMusicStruct->scale == Scale::MelodicMinor) {
		ofs << "メロディックマイナー";
	}
	ofs << endl << "<br>" << endl << endl;

	//各パートについて表示する処理
	for (int i = 0; i < mMusicStruct->parts.size(); ++i) {
		//==========パートの情報の描画==========
		ofs << "# パート" << i + 1 << "の種類：";
		if (mMusicStruct->parts[i].type == Part::A) {
			ofs << "Aメロ";
		}
		else if (mMusicStruct->parts[i].type == Part::B) {
			ofs << "Bメロ";
		}
		else if (mMusicStruct->parts[i].type == Part::C) {
			ofs << "Cメロ";
		}
		else if (mMusicStruct->parts[i].type == Part::Climax) {
			ofs << "サビ";
		}
		else if (mMusicStruct->parts[i].type == Part::Intro) {
			ofs << "イントロ";
		}
		else if (mMusicStruct->parts[i].type == Part::Outro) {
			ofs << "アウトロ";
		}
		else if (mMusicStruct->parts[i].type == Part::Interlude) {
			ofs << "間奏";
		}
		else if (mMusicStruct->parts[i].type == Part::Special) {
			ofs << "特別なパート";
		}
		ofs << endl << endl;

		ofs << "### パートの開始時間：" << mMusicStruct->parts[i].startTimes << "　パートの長さ：" << mMusicStruct->parts[i].length << endl << endl;

		//==========ドラムのリズムパターンについての描画==========
		ofs << "### ドラムのパターンの種類：";
		if (mMusicStruct->rhythms[i].type == RhythmType::BasicDownBeat8) {
			ofs << "基本8ビート";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::BasicUpBeat8) {
			ofs << "基本8ビート 裏打ち";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::HalfTimeShuffle) {
			ofs << "ハーフタイムシャッフル";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::ShuffleBeat) {
			ofs << "シャッフルビート";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::SlipBeat8) {
			ofs << "スリップ8ビート";
		}
		ofs << endl << endl;

		//==========コード列についての描画==========
		ofs << "### コード生成アルゴリズム：";
		if (mMusicStruct->chords[i].type == ChordAlgorithm::Simple) {
			ofs << "シンプル";
		}
		else if (mMusicStruct->chords[i].type == ChordAlgorithm::Fashionable) {
			ofs << "オシャレ";
		}
		else if (mMusicStruct->chords[i].type == ChordAlgorithm::Modulation) {
			ofs << "転調";
		}
		ofs << endl << endl;
		vector<Chords::Chord> chords = mMusicStruct->chords[i].chords;
		ofs << "### コード列：";
		for (int j = 0; j < chords.size(); ++j) {
			//Cのようなコードにおいて階名部分の描画
			if (chords[j].baseNoteNum % 12 == 0) {
				ofs << "C";
			}
			else if (chords[j].baseNoteNum % 12 == 1) {
				ofs << "C#";
			}
			else if (chords[j].baseNoteNum % 12 == 2) {
				ofs << "D";
			}
			else if (chords[j].baseNoteNum % 12 == 3) {
				ofs << "D#";

			}
			else if (chords[j].baseNoteNum % 12 == 4) {
				ofs << "E";
			}
			else if (chords[j].baseNoteNum % 12 == 5) {
				ofs << "F";
			}
			else if (chords[j].baseNoteNum % 12 == 6) {
				ofs << "F#";
			}
			else if (chords[j].baseNoteNum % 12 == 7) {
				ofs << "G";
			}
			else if (chords[j].baseNoteNum % 12 == 8) {
				ofs << "A♭";
			}
			else if (chords[j].baseNoteNum % 12 == 9) {
				ofs << "A";
			}
			else if (chords[j].baseNoteNum % 12 == 10) {
				ofs << "B♭";
			}
			else if (chords[j].baseNoteNum % 12 == 11) {
				ofs << "B";
			}

			//和音の種類について記述する
			if (chords[j].type == Chords::ChordType::Major) {
				//何も記述しない
			}
			else if (chords[j].type == Chords::ChordType::Minor) {
				ofs << "m";
			}
			else if (chords[j].type == Chords::ChordType::Aug) {
				ofs << "aug";
			}
			//マイナー♭は書き方が特殊なので2個描画処理がある
			else if (chords[j].type == Chords::ChordType::mb5) {
				if (chords[j].plus != Chords::ChordPlus::Dim7) {
					ofs << "m";
				}
			}

			//4つ目の音についての付加情報を記述する
			if (chords[j].plus == Chords::ChordPlus::Seventh) {
				ofs << "7";
			}
			else if (chords[j].plus == Chords::ChordPlus::MajorSeventh) {
				ofs << "M7";
			}
			else if (chords[j].plus == Chords::ChordPlus::Six) {
				ofs << "6";
			}
			else if (chords[j].plus == Chords::ChordPlus::Add9) {
				ofs << "Add9";
			}
			else if (chords[j].plus == Chords::ChordPlus::Add11) {
				ofs << "Add11";
			}
			else if (chords[j].plus == Chords::ChordPlus::Add13) {
				ofs << "Add13";
			}
			else if (chords[j].plus == Chords::ChordPlus::Dim7) {
				ofs << "Dim7";
			}

			//3和音の構成に属するが、4和音の情報が必要になる部分の描画
			if (chords[j].type == Chords::ChordType::Sus2) {
				ofs << "sus2";
			}
			else if (chords[j].type == Chords::ChordType::Sus4) {
				ofs << "sus4";
			}
			else if (chords[j].type == Chords::ChordType::mb5) {
				if (chords[j].plus != Chords::ChordPlus::Dim7) {
					ofs << "-5";
				}
			}
			//5つ目の音の付加情報を記述する
			if (chords[j].tension == Chords::ChordTension::FlatNinth) {
				ofs << "♭9";
			}
			else if (chords[j].tension == Chords::ChordTension::Ninth) {
				ofs << "9";
			}
			else if (chords[j].tension == Chords::ChordTension::SharpNinth) {
				ofs << "#9";
			}
			else if (chords[j].tension == Chords::ChordTension::Eleventh) {
				ofs << "11";
			}
			else if (chords[j].tension == Chords::ChordTension::SharpEleventh) {
				ofs << "#11";
			}
			else if (chords[j].tension == Chords::ChordTension::FlatThirteenth) {
				ofs << "♭13";
			}
			else if (chords[j].tension == Chords::ChordTension::Thirteenth) {
				ofs << "13";
			}

			//オンコードの記述
			if (chords[j].onIndex != chords[j].baseIndex) {					//ノートの基底音とオンコードの値が一致していない時のみオンコードの描画を行う
				ofs << "/";
				if (chords[j].onIndex == 0) {
					ofs << "C";
				}
				else if (chords[j].onIndex == 1) {
					ofs << "C#";
				}
				else if (chords[j].onIndex == 2) {
					ofs << "D";
				}
				else if (chords[j].onIndex == 3) {
					ofs << "D#";
				}
				else if (chords[j].onIndex == 4) {
					ofs << "E";
				}
				else if (chords[j].onIndex == 5) {
					ofs << "F";
				}
				else if (chords[j].onIndex == 6) {
					ofs << "F#";
				}
				else if (chords[j].onIndex == 7) {
					ofs << "G";
				}
				else if (chords[j].onIndex == 8) {
					ofs << "A♭";
				}
				else if (chords[j].onIndex == 9) {
					ofs << "A";
				}
				else if (chords[j].onIndex == 10) {
					ofs << "B♭";
				}
				else if (chords[j].onIndex == 11) {
					ofs << "B";
				}
			}
			//オミットコードは今は考えない
			ofs << "　";
		}
		ofs << endl << endl;

		//==========メロディについての描画==========
		ofs << "### メロディ生成アルゴリズム：";
		if (mMusicStruct->melodys[i].type == MelodyAlgorithm::Standard) {
			ofs << "Standard";
		}
		ofs << endl << endl;
		vector<Note> notes = mMusicStruct->melodys[i].notes;
		ofs << "メロディのノート列：(パートから見て相対的な開始時間、長さ、ノートの高さ)<br>" << endl;
		for (int j = 0; j < notes.size(); ++j) {
			ofs << "「" << notes[j].startTime << ", " << notes[j].length << ", ";
			//ノート番号を文字に変換して出力
			if (notes[j].num % 12 == 0) {
				ofs << "C";
			}
			else if (notes[j].num % 12 == 1) {
				ofs << "C#";
			}
			else if (notes[j].num % 12 == 2) {
				ofs << "D";
			}
			else if (notes[j].num % 12 == 3) {
				ofs << "D#";

			}
			else if (notes[j].num % 12 == 4) {
				ofs << "E";
			}
			else if (notes[j].num % 12 == 5) {
				ofs << "F";
			}
			else if (notes[j].num % 12 == 6) {
				ofs << "F#";
			}
			else if (notes[j].num % 12 == 7) {
				ofs << "G";
			}
			else if (notes[j].num % 12 == 8) {
				ofs << "A♭";
			}
			else if (notes[j].num % 12 == 9) {
				ofs << "A";
			}
			else if (notes[j].num % 12 == 10) {
				ofs << "B♭";
			}
			else if (notes[j].num % 12 == 11) {
				ofs << "B";
			}
			ofs << "」　";
		}
		ofs << endl << endl;

		//==========ベースについての描画==========
		ofs << "### ベース生成アルゴリズム：";
		if (mMusicStruct->bases[i].type == BaseAlgorithm::Simple) {
			ofs << "Simple";
		}
		else if (mMusicStruct->bases[i].type == BaseAlgorithm::UpDown) {
			ofs << "Updown";
		}
		else if (mMusicStruct->bases[i].type == BaseAlgorithm::Melo) {
			ofs << "Melo";
		}
		ofs << endl << endl;
		notes = mMusicStruct->bases[i].notes;
		ofs << "ベースのノート列：(パートから見て相対的な開始時間、長さ、ノート番号)<br>" << endl;
		for (int j = 0; j < notes.size(); ++j) {
			ofs << "「" << notes[j].startTime << ", " << notes[j].length << ", " << (int)notes[j].num << "」　";
		}
		ofs << endl << endl;
	}

	return;
}
