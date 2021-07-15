#ifndef Melody_H_2021_05_06_
#define Melody_H_2021_05_06_
#include <cstdint>
#include <vector>
using namespace std;

class Chords;
class RhythmPattern;

class Melody
{
public:
	typedef struct Note {
		uint8_t num;			//ノート番号
		uint8_t velocity;		//音の大きさ
		uint32_t startTime;		//開始時点
		uint32_t length;		//長さ
	}Note;

	enum class BaseAlgorithm : unsigned char {
		None,

		Simple,					//コードの基底音をひたすら流すだけ
		UpDown,					//コードの基底音を一オクターブ上の音と交互に鳴らす
		Melo					//メロディ風に遷移するベースラインを作る
	};

	Melody();
	~Melody();

	//リズムで生成したノート数、コードの情報や遷移タイミングが必要なので引数で受け取る
	void createMelody(const RhythmPattern* rhythmPattern, const Chords* chords);
	//ベースラインを生成するメンバ関数
	void createBase(const RhythmPattern* rhythmPattern, const Chords* chords, BaseAlgorithm baseAlgorithm);
	//ドラムのノート番号を割り振る
	void createDrum(const RhythmPattern* rhythmPattern);

	vector<Note> getMelodyNotes() {
		return mNotes;
	}

private:
	void createBaseMelody(const RhythmPattern* rhythmPattern, const Chords* chords);	//メロディ風にベースラインを生成する処理

	vector<Note> mNotes;			//ノート番号が配列で並ぶ
};

#endif