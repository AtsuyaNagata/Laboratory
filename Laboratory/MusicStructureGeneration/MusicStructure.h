#ifndef MUSIC_STRUCTURE_H_2021_07_06_
#define MUSIC_STRUCTURE_H_2021_07_06_
#include <vector>
#include <cstdint>
#include "Chords.h"
#include "RhythmPattern.h"
using namespace std;

class Melody;

//���y�\������舵���N���X�B�����������܂�
class MusicStructure
{
public:
	//�K�v�ȃf�[�^�F���y�̃p�[�g�i�s�A��Ԃ̒����A�R�[�h�p�^�[��(�M�^�[�A�s�A�m�ȂǂɑΉ�������)�A���Y���p�^�[���A�����f�B�p�^�[���A�x�[�X���C��
	MusicStructure();
	~MusicStructure();

	//�Ȃ̃p�[�g�̎�ނ�\�����邽�߂̗񋓎q
	enum class Part : unsigned char {
		None,			//���݂��Ȃ����Ƃ�\��(�����l�p)

		Intro,			//�C���g��
		Interlude,		//�ԑt�̂���
		Outro,			//�A�E�g��
		A,				//A�p�[�g
		B,				//B�p�[�g
		Climax,			//�T�r
		C,				//C�p�[�g

		Special			//None�ł͂Ȃ�(���݂͂���)���A���ނɊ܂܂�Ȃ��p�[�g��\��
	};

	//���Y���p�^�[���̎�ނ�\�����邽�߂̗�
	enum class Rhythm : unsigned char {
		None,

		BasicDownBeat8,			//��{8�r�[�g
		BasicUpBeat8,			//��{8�r�[�g���ł�(�悭�T�r�ɗp������)
		SlipBeat8,				//�X���b�v8�r�[�g(�X�l�A��8���������ɂ������肷�郊�Y���p�^�[��)
		ShuffleBeat,			//�V���b�t���r�[�g(4���~3��3���~4�̂悤�ɍl���郊�Y���p�^�[���B2���ڂ�4���ڂɃo�b�N�r�[�g(�X�l�A�̋�)������)
		HalfTimeShuffle,		//�n�[�t�^�C���V���b�t���r�[�g(�`�^�` �`�^�` �^�^�` �`�^�` �̃��Y���p�^�[��)[�n�[�t�^�C���^�C����3���ڂɃo�b�N�r�[�g������Ӗ�����]
	};

	//�R�[�h�����A���S���Y���̎�ނ̗�
	enum class ChordAlgorithm : unsigned char {
		None,
		
		Simple,					//�V���v���Ȑ���
		Fashionable,			//�I�V�����ɐ���
		Modulation				//�]�����s���悤�ɐ���
	};

	//�I�u���K�[�h�̎�ނ̗񋓁B�����f�B�������������邽�߂̃����f�B�A�X�ȃp�[�g�̂���
	enum class Obbligato : unsigned char{
		None,

		Fake,					//�����f�B�E���C���̕��͋C��ۂ��Ȃ���鑕��
		FillIn,					//�����f�B�̌p�ڂɏo�����鑦���I�ȃ���(�h�����ő�̍ς�)�������Ƃ��Ă΂��
		Harmony,				//�����f�B�E���C���Ƀn���郁��
		Riff					//���`�[�t���X�ɒZ�������悤�ȗv�f�A�I�u���K�[�h�Ƃ��Ď����Ă���
	};
	
	//�h�����Ɋ܂܂��y��̎�ނ̗�
	enum class Drum : unsigned char {
		None,

		BassDrum,
		HiHatCymbal,
		SnareDrum,
		Tom,
		FloorTom,
		CrashCymbal,
		RideCymbal,

		Other					//����ȉ���������ۂɗp����(���݂͍l���Ȃ�)
	};

	//�y��̎�ނ̗�
	enum class Instrument : unsigned char {
		None,

		Piano,
		Guiter,
		Synthesizer,
		AudioMaterial			//�����f�ނƂ������Ӗ�����
	};

	//�Ȃ̃p�[�g��\������\��
	typedef struct PartStruct {
		Part part;							//�p�[�g�̎�ނ��i�[����
		uint32_t startTimes;				//�J�n�n�_�̎��Ԃ��i�[����(4����������̒�����256�ɂ���)
		uint32_t length;					//�p�[�g�̒���
	} PartStruct;

	//����h�����y��ɂ��Ẵ��Y���p�^�[�����i�[����\��
	typedef struct DrumPattern {
		Drum drum;							//�炷�h�����̎��
		vector<uint32_t> startTimes;		//�e���̊J�n�n�_���i�[����
	};

	//����p�[�g�̃��Y���p�^�[����\������\��
	typedef struct RhythmStruct {
		Rhythm rhythm;						//���Y���̎��
		bool fill_in;						//�Ō�Ƀt�B���C���������Ă邩�ǂ���
		vector<DrumPattern> drumPatterns;	//�e�h�����y��̃��Y���p�^�[�����i�[����\��
	}RhythmStruct;

	//����p�[�g�̃R�[�h��ɂ��Ă̏��
	typedef struct ChordStruct {
		ChordAlgorithm chordAlgorithm;		//�R�[�h�����A���S���Y��
		vector<Chords::Chord> chords;		//�R�[�h��
		vector<uint32_t> lengths;			//�e�R�[�h�̒���
	} ChordStruct;

	//����p�[�g�̃����f�B���C���ɂ��Ă̏��
	typedef struct MelodyStruct {
		bool breakIn;						//�u���C�N(�����f�B�������������邽�߂Ɉꕔ�������镔��)�����Ă邩�ǂ���
		vector<uint8_t> noteNums;			//�m�[�g�̔ԍ��̗�
		vector<uint32_t> noteLengths;		//�m�[�g�̒����̗�(noteNums�ƃT�C�Y�͈�v����)
	}MelodyStruct;

	//�I�u���K�[�h�̍\��
	typedef struct ObbligatoStruct {
		Obbligato type;						//�I�u���K�[�h�̎�ނ�\��
		Instrument instrument;				//�y��̎��
		int startTime;						//�J�n�n�_
		vector<uint8_t> noteNums;			//�m�[�g�̔ԍ��̗�
		vector<uint32_t> noteLengths;		//�m�[�g�̒����̗�(noteNums�ƃT�C�Y�͈�v����)
	}ObbligatoStruct;

	//�Ȃ̍\���������݌v�}�̂悤�ȍ\��
	typedef struct MusicStruct {
		int key;							//�ȑS�̂̃L�[(0�`11�ŕ\�������B0��C)
		uint32_t length;					//�ȑS�̂̒���
		vector<PartStruct> part;			//�p�[�g���̗���i�[(�\���������Ɏ��ԏ��Ń\�[�g����Ă邱�Ƃ��O��ƂȂ��Ă�)
		vector<RhythmStruct> rhythm;		//���Y�����̗�(�p�[�g�ɑΉ����ĕ���)
		vector<ChordStruct> chord;			//�R�[�h���̗�(�p�[�g�ɑΉ����ĕ���)
		vector<ObbligatoStruct> obbligatos;	//�I�u���K�[�h�̗�(�p�[�g�Ɉˑ����Ȃ��A�����C��)
	};

	void create();

private:

};

#endif