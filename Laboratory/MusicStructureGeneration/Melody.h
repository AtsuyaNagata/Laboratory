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

	//���Y���Ő��������m�[�g���A�R�[�h�̏���J�ڃ^�C�~���O���K�v�Ȃ̂ň����Ŏ󂯎��
	void create(RhythmPattern* rhythmPattern, Chords* chords, vector<uint32_t> chordLengths);
	vector<uint8_t> getMelodyNote() {
		return mMelody;
	}

private:
	vector<uint8_t> mMelody;			//�m�[�g�ԍ����z��ŕ���
	vector<uint32_t> mChordLengths;		//�R�[�h�J�ڂ̃^�C�~���O��񂪒����̗���Ƃ��Ċi�[���Ă���
};

#endif