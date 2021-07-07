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
	Melody();
	~Melody();

	//リズムで生成したノート数、コードの情報や遷移タイミングが必要なので引数で受け取る
	void create(RhythmPattern* rhythmPattern, Chords* chords, vector<uint32_t> chordLengths);
	vector<uint8_t> getMelodyNote() {
		return mMelody;
	}

private:
	vector<uint8_t> mMelody;			//ノート番号が配列で並ぶ
	vector<uint32_t> mChordLengths;		//コード遷移のタイミング情報が長さの羅列として格納しておく
};

#endif