#include "MusicStructure.h"

//必要なデータ：キー、音楽のパート進行、区間の長さ、コードパターン(ギター、ピアノなど楽器に対応させる予定)、リズムパターン、メロディパターン、ベースライン
MusicStructure::MusicStructure(int key, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<Rhythm> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<Base> bases):
	mMusicStruct(nullptr)
{
	prepare(key, parts, lengths, rhythms, chordAls, meloAls, bases);
}

//曲の設計図を受け取って初期化するコンストラクタ。これを使うことは多分ない
MusicStructure::MusicStructure(const MusicStruct& mus) : mMusicStruct(nullptr) {
	mMusicStruct = new MusicStruct;
	mMusicStruct->key = mus.key;
	mMusicStruct->length = mus.length;
	mMusicStruct->part = mus.part;
	mMusicStruct->rhythm = mus.rhythm;
	mMusicStruct->chord = mus.chord;
	mMusicStruct->melody = mus.melody;
	mMusicStruct->base = mus.base;
	mMusicStruct->obbligatos = mus.obbligatos;
}

//引数なしのコンストラクタ。曲の設計図をヌルポで初期化するだけ
MusicStructure::MusicStructure() : mMusicStruct(nullptr) {}

//デストラクタ
MusicStructure::~MusicStructure() {
	delete mMusicStruct;
	mMusicStruct = nullptr;
}

void MusicStructure::prepare(int key, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<Rhythm> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<Base> bases)
{
	//要素が既に存在してる時、一度全て削除してしまう(要素が変に残ってバグに気付きにくくなるのを防ぐための仕様)
	if (mMusicStruct != nullptr) {
		delete mMusicStruct;
		mMusicStruct = nullptr;
	}
	//各構造数がパート数と一致しない異常な入力だった時
	if (parts.size() != lengths.size() || parts.size() != rhythms.size() || parts.size() != chordAls.size() || parts.size() != meloAls.size() || parts.size() != bases.size()) {
		return;			//構造を作る前に落とすことで、次の参照時にnullptrを返すようにする
	}
	mMusicStruct = new MusicStruct;
	//初期値の設定。具体的なノートの生成等の処理は各クラスで行うので、ここでは抽象的な値しか格納しない。
	mMusicStruct->key = key;
	uint32_t length = 0;
	for (int i = 0; i < parts.size(); ++i) {
		//パートの構造を引数の値から構築する(Obbligato構造は特殊な物なので、ここで格納処理は行わない)
		PartStruct ps;
		ps.type = parts[i];
		ps.length = lengths[i];
		ps.startTimes = length;
		mMusicStruct->part.push_back(ps);		//初期化したパート構造をプッシュ(引数から全ての要素が格納可能となっている)

		RhythmStruct rs;
		rs.type = rhythms[i];
		rs.fill_in = false;
		mMusicStruct->rhythm.push_back(rs);		//初期化したリズム構造をプッシュ(具体的な値は格納されていない)

		ChordStruct cs;
		cs.type = chordAls[i];
		mMusicStruct->chord.push_back(cs);		//初期化したコード構造をプッシュ(具体的な値は格納されていない)

		MelodyStruct ms;
		ms.type = meloAls[i];
		mMusicStruct->melody.push_back(ms);		//初期化したメロディ構造をプッシュ(具体的な値は格納されていない)

		BaseStruct bs;
		bs.type = bases[i];
		mMusicStruct->base.push_back(bs);		//初期化したベース構造をプッシュ(具体的な値は格納されていない)

		length += lengths[i];					//現在参照したパートの最後尾の位置の値になる
	}
	mMusicStruct->length = length;
}

void MusicStructure::create()
{
	//パート分だけ同様な処理を行う
	vector<PartStruct> part = mMusicStruct->part;
	for (int i = 0; i < part.size(); ++i) {
		//リズムを生成する
		RhythmStruct &rs = mMusicStruct->rhythm[i];


		//コード進行を生成する
		ChordStruct& cs = mMusicStruct->chord[i];


		//メロディを生成する
		MelodyStruct& ms = mMusicStruct->melody[i];


		//ベースラインを生成する
		BaseStruct& bs = mMusicStruct->base[i];


	}
}