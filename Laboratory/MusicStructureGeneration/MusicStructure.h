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
	//�Ȃ̃p�[�g�̎�ނ�\�����邽�߂̗񋓎q
	enum class Part : unsigned char {
		None,					//���݂��Ȃ����Ƃ�\��(�����l�p)

		Intro,					//�C���g��
		Interlude,				//�ԑt�̂���
		Outro,					//�A�E�g��
		A,						//A�p�[�g
		B,						//B�p�[�g
		Climax,					//�T�r
		C,						//C�p�[�g

		Special					//None�ł͂Ȃ�(���݂͂���)���A���ނɊ܂܂�Ȃ��p�[�g��\��
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

	//�����f�B�����A���S���Y���̎�ނ̗�
	enum class MelodyAlgorithm : unsigned char {
		None,

		Standard				//��{�I�Ȑ����A���S���Y��
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
		CloseHiHatCymbal,
		OpenHiHatCymbal,
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

	enum class Base : unsigned char {
		None,

		Simple,					//�R�[�h�̊�ꉹ���Ђ����痬������
		UpDown,					//�R�[�h�̊�ꉹ����I�N�^�[�u��̉��ƌ��݂ɖ炷
		Melo					//�����f�B���ɑJ�ڂ���x�[�X���C�������
	};


	//�Ȃ̃p�[�g��\������\��
	typedef struct PartStruct {
		Part type;							//�p�[�g�̎�ނ��i�[����
		uint32_t startTimes;				//�J�n�n�_�̎��Ԃ��i�[����(4����������̒�����256�ɂ���)
		uint32_t length;					//�p�[�g�̒���
	} PartStruct;

	//����h�����y��ɂ��Ẵ��Y���p�^�[�����i�[����\��
	typedef struct DrumPattern {
		Drum drum;							//�炷�h�����̎��
		vector<uint32_t> startTimes;		//�e���̊J�n�n�_���i�[����
		//�e����32�������̒����̉��Ƃ��ďo�͂��邱�Ƃɂ���
	};

	//����p�[�g�̃��Y���p�^�[����\������\��
	typedef struct RhythmStruct {
		Rhythm type;						//���Y���̎��
		bool fill_in;						//�Ō�Ƀt�B���C���������Ă邩�ǂ���
		vector<DrumPattern> drumPatterns;	//�e�h�����y��̃��Y���p�^�[�����i�[����\��
	}RhythmStruct;

	//����p�[�g�̃R�[�h��ɂ��Ă̏��
	typedef struct ChordStruct {
		ChordAlgorithm type;				//�R�[�h�����A���S���Y���̎��
		vector<Chords::Chord> chords;		//�R�[�h��
		vector<uint32_t> lengths;			//�e�R�[�h�̒���
	} ChordStruct;

	//����p�[�g�̃����f�B���C���ɂ��Ă̏��
	typedef struct MelodyStruct {
		MelodyAlgorithm type;				//�����f�B�����A���S���Y���̎��
		vector<uint8_t> noteNums;			//�m�[�g�̔ԍ��̔z��(�����f�B�̊e���̍����̑J�ڂ�\������)
		vector<uint32_t> noteLengths;		//�m�[�g�̒����̔z��(noteNums�ƃT�C�Y�͈�v����)
		vector<uint32_t> noteTime;			//�m�[�g�̊J�n���_�̔z��
	}MelodyStruct;

	typedef struct BaseStruct {
		Base type;							//�x�[�X���C���̎��
		vector<uint8_t> noteNums;			//�m�[�g�ԍ��̔z��
		vector<uint32_t> noteLength;		//�m�[�g�̒����̔z��
		vector<uint32_t> noteTime;			//�m�[�g�̊J�n���_�̔z��
	};

	//�I�u���K�[�h�̍\��
	typedef struct ObbligatoStruct {
		Obbligato type;						//�I�u���K�[�h�̎�ނ�\��
		Instrument instrument;				//�y��̎��
		uint32_t startTime;						//�J�n�n�_
		vector<uint8_t> noteNums;			//�m�[�g�̔ԍ��̔z��
		vector<uint32_t> noteLengths;		//�m�[�g�̒����̔z��(noteNums�ƃT�C�Y�͈�v����)
		vector<uint32_t> noteTime;			//�m�[�g�̊J�n���_�̔z��
	}ObbligatoStruct;

	//�Ȃ̍\���������݌v�}�̂悤�ȍ\��
	typedef struct MusicStruct {
		int key;							//�ȑS�̂̃L�[(0�`11�ŕ\�������B0��C)
		uint32_t length;					//�ȑS�̂̒���
		vector<PartStruct> part;			//�p�[�g���̗���i�[(�\���������Ɏ��ԏ��Ń\�[�g����Ă邱�Ƃ��O��ƂȂ��Ă�)
		vector<RhythmStruct> rhythm;		//���Y�����̗�(�p�[�g�ɑΉ����ĕ���)
		vector<ChordStruct> chord;			//�R�[�h���̗�(�p�[�g�ɑΉ����ĕ���)
		vector<MelodyStruct> melody;		//�����f�B�\���̔z��(�p�[�g�ɑΉ����ĕ���)
		vector<BaseStruct> base;			//�x�[�X�\���̔z��(�p�[�g�ɑΉ����ĕ���)
		vector<ObbligatoStruct> obbligatos;	//�I�u���K�[�h�̗�(�p�[�g�Ɉˑ����Ȃ��A�����C��)
	};

	//�K�v�ȃf�[�^�F�L�[�A���y�̃p�[�g�i�s�A��Ԃ̒����A�R�[�h�p�^�[��(�M�^�[�A�s�A�m�ȂǊy��ɑΉ�������\��)�A���Y���p�^�[���A�����f�B�p�^�[���A�x�[�X���C��
	MusicStructure(int key, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<Rhythm> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<Base> bases);
	MusicStructure(const MusicStruct& mus);			//�݌v�}���󂯎���Ă�������̂܂܎g���ď���������R���X�g���N�^�B�����g��Ȃ�
	MusicStructure();								//��̗v�f��錾���邽�߂̃R���X�g���N�^�B�������s���ۂɂ�prepare�����o�֐���ʂŌĂяo���K�v������

	~MusicStructure();

	void prepare(int key, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<Rhythm> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<Base> bases);

	void create();							//�Ȃ𐧍삷��(���Y���A�R�[�h�A�����f�B�A�x�[�X)
	void addObbligato();					//���݂̋Ȃ��Q�Ƃ��Ȃ���A�w�肳�ꂽ�I�u���K�[�h�̒ǉ����s������(����������\��)

private:
	MusicStruct* mMusicStruct;				//�Ȃ̐݌v�}
};

#endif