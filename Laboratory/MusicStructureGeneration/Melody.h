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
		uint8_t num;			//�m�[�g�ԍ�
		uint8_t velocity;		//���̑傫��
		uint32_t startTime;		//�J�n���_
		uint32_t length;		//����
	}Note;

	enum class BaseAlgorithm : unsigned char {
		None,

		Simple,					//�R�[�h�̊�ꉹ���Ђ����痬������
		UpDown,					//�R�[�h�̊�ꉹ����I�N�^�[�u��̉��ƌ��݂ɖ炷
		Melo					//�����f�B���ɑJ�ڂ���x�[�X���C�������
	};

	Melody();
	~Melody();

	//���Y���Ő��������m�[�g���A�R�[�h�̏���J�ڃ^�C�~���O���K�v�Ȃ̂ň����Ŏ󂯎��
	void createMelody(const RhythmPattern* rhythmPattern, const Chords* chords);
	//�x�[�X���C���𐶐����郁���o�֐�
	void createBase(const RhythmPattern* rhythmPattern, const Chords* chords, BaseAlgorithm baseAlgorithm);
	//�h�����̃m�[�g�ԍ�������U��
	void createDrum(const RhythmPattern* rhythmPattern);

	vector<Note> getMelodyNotes() {
		return mNotes;
	}

private:
	void createBaseMelody(const RhythmPattern* rhythmPattern, const Chords* chords);	//�����f�B���Ƀx�[�X���C���𐶐����鏈��

	vector<Note> mNotes;			//�m�[�g�ԍ����z��ŕ���
};

#endif